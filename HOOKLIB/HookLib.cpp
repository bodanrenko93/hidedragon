#define _WIN32_WINNT 0x0600
#define  _WIN32_IE  0x0500
#include "windows.h"
#include "mmsystem.h"
#include "stdio.h"
#include "resource.h"
#include "commctrl.h"
#include "shlwapi.h"

#include "Volume/VolumeOutMaster.h"
#include "Volume/VolumeOutWave.h"

//C:\Program Files\Microsoft SDKs\Windows\v6.0A\include\shellapi.h
#include <shellapi.h>



#define HOOKLIBAPI extern "C" __declspec(dllexport)



//#define NTDDI_VERSION  NTDDI_WINXP





#include "../hooklib.h"
#include "ProcessData.h"
#include <process.h>



#include "Driver.h"



HINSTANCE hInst;
int iTest,g_iUseLanguage,* g_iTipCount;
HHOOK hMouseHook,hTrayHook;
wchar_t dd [50],*** g_pTipText;
WndParam sWP2;
WndAndHandle *sForBK;
OptionFile sOP;
HWND hEdit_Password,hStatic_Result,hDlg_Validate;
HWND hTrayWnd_dll,* pArr_dll = NULL;
HOOKLIBAPI WndAndHandle * sW_dll = NULL;
HANDLE hEvt_ReadWHFile;

WndAndHandle * g_QuickHideAllTray = NULL;


#ifdef _HIDEPROCESS
	CDriver * pDri = NULL;
#endif

HANDLE g_arrayHide[100];


EXEREFRESHFUNC * pFillList;
EXESETDLGTEXTFUNC * g_pSetDialogText;
EXESETTIPTEXTFUNC * g_pSetTipText;
EXECLEARTIPTEXT * g_pClearTipText;
EXEAPPENDTIPTEXT * g_pAppendTipText;
EXECLEANHISTORY * g_pCleanHistory;






#pragma data_seg("DllShared")
HWND hAll = NULL;
HWND hLV = NULL;
int iLButton = 0;
int iRButton = 0;
int iMButton = 0;
int iHideComplete = 1;
int iCbt_CW = 0;
int iCbt_DW = 0;
int iWin     = 0,iWinNum = 0;
int iReserveCount = 0, iTemp = 0;
HWND hCurWnd = NULL;
int bHideWindows = 1;
DWORD dwOldVolume = 0;
DWORD dwOldWave = 0;
int iBaSW_dll = 0;
int iBaSW_QH = 0;
int iHiding = 0;
UINT g_HD_Msg_GetIcon = 0;
HICON hTrayIcon_hook = NULL;
CRITICAL_SECTION csFillList;
#pragma data_seg()


__declspec(allocate("DllShared")) HWND WindowsArray [100];





#pragma comment (linker,"/SECTION:DllShared,RWS")








BOOL WINAPI DllMain(HINSTANCE hInstance,DWORD dwReason,LPVOID)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	hInst = hInstance;


	return TRUE;

}


int Add (int i ,int j)
{
	iTest = i + j;
	return iTest;
}

int SetMouseHook(int f)
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	DWORD dwID = GetCurrentThreadId();


	if (f)
	{
		GetVersionEx(&osvi);
		if(osvi.dwMajorVersion >= 5 && osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			hMouseHook = SetWindowsHookEx(WH_MOUSE_LL,MouseHookProc,hInst,0);
		}
		else
		{
			hMouseHook = SetWindowsHookEx(WH_MOUSE,MouseHookProc,hInst,0);
		}
	}
	else
	{
		UnhookWindowsHookEx(hMouseHook);
		hMouseHook = NULL;
	}
	return 1;
}

//int SetCbtHook(int iInstall)
//{
//	if(iInstall == 1)
//	{
//		hCbtHook = SetWindowsHookEx(WH_CALLWNDPROCRET,CbtHookProc,hInst,0);
//		if(hCbtHook == NULL)
//		{
//			OutputDebugString(L"Set CbtHook Failure!");
//		}
//		hEvt_ReadWHFile = CreateEvent(NULL,FALSE,FALSE,"EVENT_FOR_READ_WHFILE");
//		OutputDebugString(L"Create Event.");
//	}
//	else
//	{
//		UnhookWindowsHookEx(hCbtHook);
//		hCbtHook = NULL;
//		CloseHandle(hEvt_ReadWHFile);
//	}
//	return 1;
//}

HWND FindTrayWnd()
{
		HWND hTrayWnd = FindWindow(L"Shell_TrayWnd", NULL);
		hTrayWnd = FindWindowEx(hTrayWnd, 0, L"TrayNotifyWnd", NULL);
 		HWND hwndTmp;	
		hwndTmp = ::FindWindowEx(hTrayWnd, 0, L"SysPager", NULL);
		if(!hwndTmp)
			hTrayWnd=::FindWindowEx(hTrayWnd,0,L"ToolbarWindow32",NULL);
		else
			hTrayWnd=::FindWindowEx(hwndTmp,0,L"ToolbarWindow32",NULL);




		return hTrayWnd;
}

int SetTrayHook(int iInstall)
{
	if(iInstall == 1)
	{
		g_HD_Msg_GetIcon = RegisterWindowMessage(L"HD_Msg_GetIcon");


		HWND hTrayWnd = FindTrayWnd();

	
		DWORD dwExploreID = 0;
		dwExploreID = GetWindowThreadProcessId(hTrayWnd,NULL);
		hTrayHook = SetWindowsHookEx(WH_CALLWNDPROC,TrayHookProc,hInst,dwExploreID);
		if(hTrayHook == NULL)
		{
			OutputDebugString(L"Set SetTrayHook Failure!");
		}
#ifdef _HIDEPROCESS
		pDri = new CDriver;
#endif
	}
	else
	{
		UnhookWindowsHookEx(hTrayHook);
		hTrayHook = NULL;
#ifdef _HIDEPROCESS
		delete pDri;
#endif
	
	}
	return 1;
}





LRESULT CALLBACK TrayHookProc(int nCode,WPARAM wParam ,LPARAM lParam)
{
	CWPSTRUCT *pCWPS=(CWPSTRUCT*)lParam;
	{			
		if(pCWPS->message == g_HD_Msg_GetIcon)
		{
			HIMAGELIST hImgLst=(HIMAGELIST)::SendMessage(pCWPS->hwnd,TB_GETIMAGELIST,0,0);
			HICON hIcon=::ImageList_GetIcon(hImgLst,int(pCWPS->wParam),ILD_NORMAL);
			if(hIcon == NULL)
			{
				DWORD dwErr = GetLastError();
				//MessageBox(NULL,L"IS NULL",L"ISNULL",MB_OK);
				return 0;
			}
// 			ICONINFO IInfo;
// 			BOOL bRet = GetIconInfo(hIcon,&IInfo);

			DWORD HD_Pid=NULL;
			DWORD tid=::GetWindowThreadProcessId(hLV, &HD_Pid);
			HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS
				|PROCESS_VM_OPERATION
				|PROCESS_VM_READ
				|PROCESS_VM_WRITE,
				FALSE,
				HD_Pid);
		
			if(hProcess != NULL)
			{
// 				DWORD dwErr = GetLastError();
// 				MessageBox(NULL,L"ProIS NULL",L"proISNULL",MB_OK);
// 				return 0;
				int iRet = WriteProcessMemory(hProcess,PVOID(pCWPS->lParam),&hIcon,sizeof(HICON),NULL);
				if(iRet == 0)
				{
					//MessageBox(NULL,L"IS 0",L"0",MB_OK);

				}
				CloseHandle(hProcess);
			}
			else
			{
				hTrayIcon_hook = hIcon;
			}

			return 0;
		}
		else
		{

		}
		return CallNextHookEx(hTrayHook,nCode,wParam,lParam);
	}

	return CallNextHookEx(hTrayHook,nCode,wParam,lParam);
}


HICON ReadTrayIcon_dll()
{
	return hTrayIcon_hook;
}


LRESULT CALLBACK CbtHookProc(int nCode,WPARAM wParam ,LPARAM lParam)
{
//	HANDLE hEvt_CbtIn = NULL;
//	if(nCode >= 0)
//	{
//		CWPRETSTRUCT * pCS = (CWPRETSTRUCT*)lParam;
//		
//		{			
//			if(iCbt_CW == 0 &&  pCS->message == WM_CREATE)
//			{
//				iCbt_CW = 1;
////				SendMessage(hCurWnd,WM_COMMAND,1001,NULL);
////				SendMessage(hCurWnd,WM_COMMAND,IDC_REFRESH,NULL);
////				SendMessage(hCurWnd,WM_COMMAND,IDC_REFRESH,NULL);
////				SendMessage(hCurWnd,WM_COMMAND,IDC_REFRESH,NULL);
////				SendMessage(hCurWnd,WM_COMMAND,IDC_REFRESH,NULL);
//				iCbt_CW = 0;
//			}
//			return CallNextHookEx(hCbtHook,nCode,wParam,lParam);
//		}
//	}
//	else
//		return CallNextHookEx(hCbtHook,nCode,wParam,lParam);
//
//	return CallNextHookEx(hCbtHook,nCode,wParam,lParam);
	return 1;
}




LRESULT CALLBACK MouseHookProc(int nCode,WPARAM wParam ,LPARAM lParam)
{
	if (nCode >= HC_ACTION)
	{

		if(wParam == WM_MBUTTONDOWN)
		{
			iMButton = 1;
		}
		if (wParam == WM_LBUTTONDOWN)
		{
			iLButton = 1;
			if(sOP.bRightFirst == 1 && iRButton == 0)
			{
				iLButton = 0;
			}
		}
		if (wParam == WM_RBUTTONDOWN)
		{
			iRButton = 1;
		}
		if (wParam == WM_MBUTTONUP)
		{
			iMButton = 2;
		}

		if (wParam == WM_LBUTTONUP)
		{
			iLButton = 0;
		}
		if (wParam == WM_RBUTTONUP)
		{
			iRButton = 0;
		}
		if ((iLButton==1 && iRButton == 1)||iMButton == 1)
		{
			if(iMButton == 1 && sOP.bUseMiddle == 0)
			{
				iHiding = 0;
				return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
			}
			if(iLButton==1 && iRButton == 1 && sOP.bUseMouse == 0)
			{
				iHiding = 0;
				return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
			}
//			if(iHiding == 1)
//			{
//				return 1;
//			}
//			iHiding = 1;
			
			OutputDebugString(L"all equal 1");
			iLButton = iRButton = 0;
			iMButton = 2;
			if(bHideWindows == 0 && sOP.bUseResHK == 1)
			{
				iHiding = 0;
				return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
			}
			pFillList();

			iHideComplete = 0;
			MyHideWindows();
			dbview(L"leave MyHideWindows........................................");
			pFillList();

			iHiding = 0;
			dbview(L"leave mousehook........................................");

			//iHideComplete = 1;
			return 1;
		}
		return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
	}
	else
	{
		return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
	}

 	return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
}


	
BOOL CALLBACK EnumWindowsFunc(HWND hwnd,LPARAM lParam)
{
	unsigned long lPID;
	wchar_t cPID[10];
	int i1 = 0;

	if (GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)
	{
		if (hwnd == hCurWnd || GetParent(hwnd) == hCurWnd)
			return 1;
		PostMessage(hwnd,WM_GETTEXT,30,(LPARAM)dd);
		if (!wcscmp(dd,L"Program Manager"))
			return 1;
		iTemp = iReserveCount;
		GetWindowThreadProcessId(hwnd,&lPID);
		swprintf(cPID,L"%X",lPID);
   
		//2.2 method 
		for(i1 = 0;i1<iBaSW_dll;i1++)
		{
			if (sW_dll[i1].bReserved != 1)
			{
				ShowWindow(sW_dll[i1].hWnd,SW_HIDE);
				sW_dll[i1].bHide = 1;
			}
		}   
		GetClassName(hwnd,dd,50);
		if (!wcscmp(dd,L"Shell_TrayWnd"))
			return 1;
	}
	return 1;
}


 

unsigned __stdcall HideWindowProc(void * pParam)
{
	//dbview(L"enter HideWindowProc........................................");
	DWORD dwErr = 0;
	BOOL bRet = FALSE;
	NOTIFYICONDATA nid;
	
	
	WndAndHandle * pWH = (WndAndHandle*)pParam;
	

	wchar_t cDbgStr[256] = L"";
	wcscpy(cDbgStr,L"enter HideWindowProc......");
	wcscat(cDbgStr,pWH->cProcessName);
	wcscat(cDbgStr,L"..................................");

	dbview(cDbgStr);


	ShowWindow(pWH->hWnd,SW_HIDE);
	if(pWH->iHasTrayIcon == 1)
	{
		memset(&nid,0,sizeof(NOTIFYICONDATA));
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = pWH->trayicon.hwnd;
		nid.uID = pWH->trayicon.uID;
		{
			int iCommandId = pWH->iCommandId;
			HWND g_TrayWnd = FindTrayToolbarWindow();
			SendMessageW(g_TrayWnd,TB_HIDEBUTTON,WPARAM(iCommandId),
			(LPARAM)MAKELONG(TRUE,0));
			NOTIFYICONDATAW nid2;
			memset(&nid2,0,sizeof(nid2));
			nid2.cbSize=sizeof(nid2);
			nid2.hWnd=nid.hWnd;
			nid2.uID=nid.uID;
			nid2.uFlags=NIF_STATE;
			nid2.dwState=NIS_HIDDEN;
			::SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)FALSE,0);
			Shell_NotifyIcon(NIM_MODIFY,&nid2);
			SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)TRUE,0);
		}
	}
  
	CloseHandle(g_arrayHide[pWH->iLink]);
	g_arrayHide[pWH->iLink] = NULL;


	if(sOP.bExitProcess == 1)
	{
		HANDLE hHidedProcess;
		DWORD id_process = 0;
		if(wcscmp(pWH->cProcessName,L"explorer.exe") != 0)
		{
			GetWindowThreadProcessId(pWH->hWnd,&id_process);
			hHidedProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, id_process );
			TerminateProcess(hHidedProcess,0);
		}
		else
		{
			sOP.bExitProcess = sOP.bExitProcess;
		}
	}


	memset(cDbgStr,0,256*sizeof(wchar_t));
	wcscpy(cDbgStr,L"leave HideWindowProc......");
	wcscat(cDbgStr,pWH->cProcessName);
	wcscat(cDbgStr,L"..................................");
	
	dbview(cDbgStr);
	return 100;
}

 

unsigned __stdcall MonitorProc(void * pParam)
{
	dbview(L"enter MonitorProc........................................");
	DWORD dwErr = 0;
	BOOL bRet = FALSE;
	

	int i = 0;
	Sleep(1000*1);
	DWORD dwCode = 0;
	while(i < iBaSW_dll)
	{
		GetExitCodeThread(g_arrayHide[i],&dwCode);
		if(dwCode == STILL_ACTIVE)
		{
			dbview(L"start TerminateThread........................................");

			TerminateThread(g_arrayHide[i],101);
			CloseHandle(g_arrayHide[i]);

		}



		i++;
	}



	iHideComplete = 1;
	dbview(L"leave MonitorProc........................................");

	return 1;
}


 
int MyHideWindows()
{
	NOTIFYICONDATA nid;
	if (bHideWindows == 1)
	{
		if (sOP.bDisableSound == 1)
		{
			CVolumeOutMaster OM;
			dwOldVolume = OM.GetCurrentVolume();
			OM.SetCurrentVolume(0);
			
			CVolumeOutWave OW;
			dwOldWave = OW.GetCurrentVolume();
			OW.SetCurrentVolume(0);



			//waveOutGetVolume(0,&dwOldVolume);
			//waveOutSetVolume(0,0);
		}
		EnableWindow(hAll,FALSE);
		EnableWindow(hLV,FALSE);


		for(int i1 = 0;i1<iBaSW_dll;i1++)
		{
			if(sW_dll[i1].bReserved != sOP.bReserveSel)
			{
				//ShowWindow(sW_dll[i1].hWnd,SW_HIDE);
				if(sOP.bHideProcess == 1)
				{
					HideProcess(sW_dll[i1].lPID);
				}
				sW_dll[i1].bHide = 1;


				sW_dll[i1].iLink = i1; // Use this member save thread handle index

				g_arrayHide[i1] = (HANDLE)_beginthreadex(NULL,0,HideWindowProc,
					sW_dll + i1,0,NULL);
			
			}
		}

		HANDLE hMonitorHandle = (HANDLE)_beginthreadex(NULL,0,MonitorProc,
			g_arrayHide,0,NULL);
		if(hMonitorHandle == INVALID_HANDLE_VALUE)
		{
			hMonitorHandle = NULL;
		}
		else
		{
			CloseHandle(hMonitorHandle);
		}
		if(sOP.bCleanAfterHide == 1)
		{
			g_pCleanHistory();   
		}
		


		if(sOP.bHideAllTray == 1)
		{
			AllocResourceForTrayList();
			EnumAllVisibleTray_dll();
			HideAllTray();
		}
		dbview(L"enter sOP.bNeedStart == 1........................................");

		
		if(sOP.bNeedStart == 1)
		{
			ShellExecute(NULL,L"open",sOP.cTrailRun,NULL,NULL,SW_SHOWNORMAL);
		}
		
		g_pClearTipText();
		dbview(L"enter sOP.bUseMiddle........................................");
		if(sOP.bUseMiddle)
		{
			g_pAppendTipText(L"IDS_USAGETIP_RECLICKMIDDLESHOWWINDOW",
			L"再按鼠标中键可以重新显示窗口。");
		}
		else if(sOP.bUseMouse)
		{
			g_pAppendTipText(L"IDS_USAGETIP_RECLICKRIGHTLEFTSHOWWINDOW",
			L"再一齐按鼠标左右键可以重新显示窗口。");
		}
		else
		{
			g_pAppendTipText(L"IDS_USAGETIP_REPRESSHOTKEYTOSHOWWINDOW",
			L"再按下 隐藏窗口 快捷键可以重新显示窗口。");
		}

		if(sOP.bUseMiddle)
		{
			g_pSetTipText(L"IDS_USAGETIP_RECLICKMIDDLESHOWWINDOW",
			L"再按鼠标中键可以重新显示窗口。");
		}
		else if(sOP.bUseMouse)
		{
			g_pSetTipText(L"IDS_USAGETIP_RECLICKRIGHTLEFTSHOWWINDOW",
			L"再一齐按鼠标左右键可以重新显示窗口。");
		}
		else
		{
			g_pSetTipText(L"IDS_USAGETIP_REPRESSHOTKEYTOSHOWWINDOW",
			L"再按下 隐藏窗口 快捷键可以重新显示窗口。");
		}

		dbview(L"enter bHideWindows = 0;........................................");

		bHideWindows = 0;
	}
	else if (bHideWindows == 0)
	{
		if (sOP.bUsePassword == 1)
		{
			if(hDlg_Validate != NULL && IsWindow(hDlg_Validate))
			{
				return 1;
			}
			if (DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_VALIDATE),NULL,(DLGPROC)ValidateProc) == 0)
			{
				return 1;
			}
		}

		EnableWindow(hAll,TRUE);
		EnableWindow(hLV,TRUE);

		RestoreVolume();
		//2.2 method ///
		for (int i = 0;i < iBaSW_dll;i++)
		{
			if(sOP.bHideProcess == 1)
			{
				ShowProcess(sW_dll[i].lPID);
			}
			ShowWindow(sW_dll[i].hWnd,SW_SHOW);
			
			if(sW_dll[i].iHasTrayIcon == 1 && sW_dll[i].bHide == 1)
			{
				nid.cbSize           = sizeof(NOTIFYICONDATA);
				nid.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
				nid.uID              = sW_dll[i].trayicon.uID;
				nid.hIcon            = sW_dll[i].trayicon.hIcon;
				nid.hWnd             = sW_dll[i].trayicon.hwnd;
				wcscpy(nid.szTip,(const wchar_t *)sW_dll[i].cIconTip);
				nid.uCallbackMessage = sW_dll[i].trayicon.uCallbackMessage;
//				Shell_NotifyIcon(NIM_ADD,&nid);
				{
					int iCommandId = sW_dll[i].iCommandId;
					HWND g_TrayWnd = FindTrayToolbarWindow();
					SendMessageW(g_TrayWnd,TB_HIDEBUTTON,WPARAM(iCommandId),
					(LPARAM)MAKELONG(FALSE,0));
					NOTIFYICONDATA nid2;
					memset(&nid2,0,sizeof(nid2));
					nid2.cbSize=sizeof(nid2);
					nid2.hWnd=nid.hWnd;
					nid2.uID=nid.uID;
					nid2.uFlags=NIF_STATE;
					nid2.dwState=NIS_HIDDEN;
					::SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)FALSE,0);
					Shell_NotifyIcon(NIM_MODIFY,&nid2);
					SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)TRUE,0);
				}
//				HANDLE hHideWndHandle = (HANDLE)_beginthreadex(NULL,0,HideWindowProc,sW_dll + i1,0,NULL);
//				if(hHideWndHandle == INVALID_HANDLE_VALUE)
//				{
//					g_arrayHide[i1] = NULL;
//				}
//				else
//				{
//					g_arrayHide[i1] = hHideWndHandle;
//				}



			}
			sW_dll[i].bHide = 0;
		}
		

		if(sOP.bAutoHide_QH == 1 && iBaSW_QH > 0)
		{
			int i = 0;
			for(i = 0;i < iBaSW_QH;i++)
			{
				ShowWindow(pArr_dll[i],SW_SHOW);
			}
			iBaSW_QH = 0;
		}

		if(sOP.bHideAllTray == 1)
		{
			ShowAllTray();
			ReleaseResourceForTrayList_dll();

		}


		bHideWindows = 1;
		g_pClearTipText();
		if(sOP.bUseMiddle)
		{
			g_pAppendTipText(L"IDS_USAGETIP_CLICKMIDDLEHIDEWND",
			L"按鼠标中键可以隐藏窗口。");
		}
		else if(sOP.bUseMouse)
		{
			g_pAppendTipText(L"IDS_USAGETIP_CLICKLEFTRIGHTHIDEWND",
			L"一齐按下鼠标左右键可以隐藏窗口。");	
		}
		else
		{
			g_pAppendTipText(L"IDS_USAGETIP_PRESSHOTKEYTOHIDEWINDOW",
			L"按下 隐藏窗口 的快捷键，即可隐藏窗口。");	
		}
		if(sOP.bReserveSel)
		{
			g_pAppendTipText(L"IDS_USAGETIP_SELWNDLISTCANRESERVEWND",
			L"您还可以在下边窗口列表里选择不想隐藏的窗口。");
		}
		else
		{
			g_pAppendTipText(L"IDS_USAGETIP_HIDESELITEM",
			L"在 窗口列表 里被选择的窗口将被隐藏。");
		}
		if(sOP.bUseMiddle)
		{
			g_pSetTipText(L"IDS_USAGETIP_CLICKMIDDLEHIDEWND",
			L"按鼠标中键可以隐藏窗口。");
		}
		else if(sOP.bUseMouse)
		{
			g_pSetTipText(L"IDS_USAGETIP_CLICKLEFTRIGHTHIDEWND",
			L"一齐按下鼠标左右键可以隐藏窗口。");
		}
		else
		{
			g_pSetTipText(L"IDS_USAGETIP_PRESSHOTKEYTOHIDEWINDOW",
			L"按下 隐藏窗口 的快捷键，即可隐藏窗口。");
		}

		iHideComplete = 1;

	}
	return 1;
}


int dbview(wchar_t * pInfo)
{
	OutputDebugString(pInfo);
	
	
	return 0;
}


int AllocResourceForTrayList()
{
	int i = 0;

	g_QuickHideAllTray = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	for(i = 0;i<100;i++)
	{
		g_QuickHideAllTray[i].cWndTitle = (wchar_t *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		g_QuickHideAllTray[i].cIconTip = (wchar_t*)new wchar_t [1024];
		memset(g_QuickHideAllTray[i].cIconTip,0,1024 * sizeof(wchar_t));
		memset(g_QuickHideAllTray[i].cWndTitle,0,2048 * sizeof(wchar_t));
		memset(g_QuickHideAllTray[i].cProcessName,0,30 * sizeof(wchar_t));
		g_QuickHideAllTray[i].Next = NULL;
	}
	

	return 1;
}


int EnumAllVisibleTray_dll()
{
	wchar_t * pszText = NULL;
	DWORD lPID;
	int iconIndex = 0,iconNum = 0;
	wchar_t cPID[20] = {0};
	int i = 0,j = 0;
	ICONINFO iconInfo;

	for(i = 0;i<100;i++)
	{
		g_QuickHideAllTray[i].iHasTrayIcon = 0;
		g_QuickHideAllTray[i].iIconIndex = 0;
		g_QuickHideAllTray[i].iCommandId = 0;
		g_QuickHideAllTray[i].hWnd = NULL;
		g_QuickHideAllTray[i].trayicon.hIcon = NULL;
		g_QuickHideAllTray[i].lPID = 0;
		g_QuickHideAllTray[i].bExist = 0;
		memset(g_QuickHideAllTray[i].cProcessName,0,30 * sizeof(wchar_t));
		memset(g_QuickHideAllTray[i].cIconTip,0,1024 * sizeof(wchar_t));
		memset(g_QuickHideAllTray[i].cWndTitle,0,2048 * sizeof(wchar_t));
	}




	GetWindowThreadProcessId(hTrayWnd_dll,&lPID);
	CProcessData<TBBUTTON> data(lPID);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo = {0};
	wchar_t Tipwchar_t = 0;
	wchar_t sTip[1024] = {0};
	wchar_t* pTip = NULL;
	int x = 0,iRet = 0;
	
	j = 0;
	int iBtnCount = SendMessage(hTrayWnd_dll,TB_BUTTONCOUNT,0,0);

	DWORD lExID = 0;
	for(i=0; i < iBtnCount;i++)
	{
		if (i <= 100)
		{
		}
		else
		{
			break;
		}
		SendMessage(hTrayWnd_dll,TB_GETBUTTON,i,(LPARAM)data.GetData());
		data.ReadData(&tb);
		data.ReadData(&tray,(void * )tb.dwData);

		
		
		if (!(tb.fsState & TBSTATE_HIDDEN))
		{
			GetWindowThreadProcessId(tray.hwnd,&lPID);
			
			if(lPID == GetCurrentProcessId())
			{
				continue;
			}
			if(GetIconInfo(tray.hIcon,&iconInfo) == 0)
			{
				DWORD dwErr = GetLastError();
				GetProcessTrayIcon(lPID,&tb,&tray);
				DeleteObject((HGDIOBJ)iconInfo.hbmColor);
				DeleteObject((HGDIOBJ)iconInfo.hbmMask);
			}
			else
			{
				DeleteObject((HGDIOBJ)iconInfo.hbmColor);
				DeleteObject((HGDIOBJ)iconInfo.hbmMask);
			}
			g_QuickHideAllTray[j].bHide = tb.idCommand;//Use this member save idcommand for hide or show
			//g_QuickHideAllTray[j].cWndTitle = NULL;
			//g_QuickHideAllTray[j].cIconTip = (wchar_t*)new wchar_t[1024];
			memset(g_QuickHideAllTray[j].cIconTip,0,1024 * sizeof(wchar_t));
			memset(g_QuickHideAllTray[j].cProcessName,0,30 * sizeof(wchar_t));
			GetWindowThreadProcessId(hTrayWnd_dll,&lExID);
			if(lExID == lPID)
			{
				wcscpy(g_QuickHideAllTray[j].cProcessName,L"explorer.exe");
			}

			g_QuickHideAllTray[j].trayicon = tray;
			g_QuickHideAllTray[j].lPID = lPID;
			g_QuickHideAllTray[j].hWnd = NULL;
			g_QuickHideAllTray[j].Next = NULL;

			memset(sTip,0,1024*(sizeof(wchar_t)));
			x = 0;
			pTip = (wchar_t*)tb.iString;
			do 
			{	
				if(x >= 1023)
				{
					wcscpy(sTip,L"[ToolTip was either too long or not set]");
					break;
				}
				
				data.ReadData_Tip((wchar_t*)&Tipwchar_t, (void *)pTip++);
			}while(sTip[x++] = Tipwchar_t);

			iRet = 0;
			wcscpy(g_QuickHideAllTray[j].cIconTip,sTip);

			j ++;
		}
	}


	return 1;
}


int ReleaseResourceForTrayList_dll()
{
	int i = 0;
	for(i = 0;i<100;i++)
	{
		delete [] g_QuickHideAllTray[i].cIconTip;
		g_QuickHideAllTray[i].cIconTip = NULL;
	}
	for(i = 0;i < 100;i ++)
	{
		VirtualFree(g_QuickHideAllTray[i].cWndTitle,0,MEM_RELEASE);
		g_QuickHideAllTray[i].cWndTitle = NULL;
	}


	VirtualFree(g_QuickHideAllTray,0,MEM_RELEASE);
	g_QuickHideAllTray = NULL;

	return 1;
}


int HideAllTray()
{
	int i = 0;
	while(g_QuickHideAllTray[i].lPID != 0 && i < 100)
	{
		int iCommandId = g_QuickHideAllTray[i].bHide;
		SendMessageW(hTrayWnd_dll,TB_HIDEBUTTON,WPARAM(iCommandId),
			(LPARAM)MAKELONG(TRUE,0));
		{
			NOTIFYICONDATA nid2;
			memset(&nid2,0,sizeof(nid2));
			nid2.cbSize=sizeof(nid2);
			nid2.hWnd=g_QuickHideAllTray[i].trayicon.hwnd;
			nid2.uID=g_QuickHideAllTray[i].trayicon.uID;
			nid2.uFlags=NIF_STATE;
			nid2.dwState=NIS_HIDDEN;
			::SendMessage(hTrayWnd_dll,WM_SETREDRAW,(WPARAM)FALSE,0);
			Shell_NotifyIcon(NIM_MODIFY,&nid2);
			SendMessage(hTrayWnd_dll,WM_SETREDRAW,(WPARAM)TRUE,0);
		}
		i++;
	}
	


	return 1;
}



int ShowAllTray()
{
	NOTIFYICONDATA nid;
	int i = 0;
	if(g_QuickHideAllTray == NULL)
	{
		return 0;
	}
	while(g_QuickHideAllTray && g_QuickHideAllTray[i].lPID != 0 && i < 100)
	{
		if(g_QuickHideAllTray[i].bHide > 0)
		{
			memset(&nid,0,sizeof(nid));
			nid.cbSize           = sizeof(NOTIFYICONDATA);
			nid.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
			nid.uID              = g_QuickHideAllTray[i].trayicon.uID;
			nid.hIcon            = g_QuickHideAllTray[i].trayicon.hIcon;
			nid.hWnd             = g_QuickHideAllTray[i].trayicon.hwnd;
			nid.uCallbackMessage = g_QuickHideAllTray[i].trayicon.uCallbackMessage;
			{
				int iCommandId = g_QuickHideAllTray[i].bHide;
				if(iCommandId == 0)
				{
					iCommandId = g_QuickHideAllTray[i].iCommandId;
				}
				SendMessageW(hTrayWnd_dll,TB_HIDEBUTTON,WPARAM(iCommandId),(LPARAM)MAKELONG(FALSE,0));
			}
			{
				NOTIFYICONDATA nid2;
				memset(&nid2,0,sizeof(nid2));
				nid2.cbSize=sizeof(nid2);
				nid2.hWnd=nid.hWnd;
				nid2.uID=nid.uID;
				nid2.uFlags=NIF_STATE;
				nid2.dwState=NIS_HIDDEN;
				::SendMessage(hTrayWnd_dll,WM_SETREDRAW,(WPARAM)FALSE,0);
				Shell_NotifyIcon(NIM_MODIFY,&nid2);
				SendMessage(hTrayWnd_dll,WM_SETREDRAW,(WPARAM)TRUE,0);
			}
		}
		i ++;
	}
	


	return 1;
}

int GetProcessTrayIcon(DWORD lPID,TBBUTTON * pTB,TRAYDATA * pTrayData)
{
	HWND hTrayWnd = FindTrayToolbarWindow();

	DWORD dwPID = 0;
	GetWindowThreadProcessId(hTrayWnd,&dwPID);
	BOOL ret=FALSE;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS
		|PROCESS_VM_OPERATION
		|PROCESS_VM_READ
		|PROCESS_VM_WRITE,
		FALSE,
		dwPID);
	
	PVOID ptb=VirtualAllocEx(hProcess,NULL,sizeof(TBBUTTON),MEM_COMMIT,PAGE_READWRITE);

	memset(pTB,0,sizeof(TBBUTTON));
	HICON hIcon = NULL;
	
	ULONG lButtons=(ULONG)SendMessage(hTrayWnd,TB_BUTTONCOUNT,0,0);
	for(ULONG i=0;i<lButtons;++i)
	{
		SendMessage(hTrayWnd,TB_GETBUTTON,WPARAM(i),LPARAM(ptb));
        ReadProcessMemory(hProcess,ptb,pTB,sizeof(TBBUTTON),NULL);

		ReadProcessMemory(hProcess,(PVOID)(pTB->dwData),pTrayData,sizeof(TRAYDATA),NULL);
		
		HWND hWnd=pTrayData->hwnd;
		DWORD dwProcessID = 0;
		GetWindowThreadProcessId(hWnd,&dwProcessID);
		if(dwProcessID == lPID)
		{
			SendMessage(hTrayWnd,g_HD_Msg_GetIcon,WPARAM(pTB->iBitmap),LPARAM(&hIcon));
			pTrayData->hIcon = hIcon;
			break;
		}
    }
	
	VirtualFreeEx(hProcess,ptb,0/*sizeof(TBBUTTON) boundschecker 提示应该为0*/,MEM_RELEASE);
    CloseHandle(hProcess);

	

	return 1;
}






int ReserveWnds(ExeData * pExeData)
{
	//2.2 method///
	sW_dll = pExeData->sWP;
	iBaSW_dll = pExeData->iCount;

	pArr_dll = pExeData->pArr_QH;
	g_iUseLanguage = pExeData->iUseLanguage;
	g_iTipCount = pExeData->pTipCount;
	g_pTipText = pExeData->pTipText;
	


	//WriteWHFile(sW_dll);
	

	return 1;
}

int ClearAll()
{
	int i;
	for ( i = 0;i < 100;i++)
	{
		WindowsArray [i] = NULL;
	}
	return 1;
}

int RestoreAllWindows(int iKorS)
{
	if(iKorS == 'S')   
	{
// 		for (int i = 0;i < iBaSW_dll;i++)
// 		{
// 			if(sW_dll[i].bHide == 1)
// 			{
// 				if(sOP.bHideProcess == 1)
// 				{	
// 					ShowProcess(sW_dll[i].lPID);
// 				}
// 				ShowWindow(sW_dll[i].hWnd,SW_SHOW);
// 				sW_dll[i].bHide = 0;
// 			}
// 		}
		if(bHideWindows == 0)
		{
			MyHideWindows();
		}
	}
	else
	{
		//2.2 method
		for (int i = 0;i<iBaSW_dll;i++)
		{
			if (sW_dll[i].bHide == 1)
			{
				TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,FALSE,sW_dll[i].lPID),0);
			}
		}
	}
	return 1;
}


int SetParameter(OptionFile sOP_temp,ExeFunc pExeFunc)
{
	sOP = sOP_temp;
	/*pDeleteAll = pDel;*/
	pFillList = pExeFunc.pFillList;
	g_pSetDialogText = pExeFunc.pSetDialogText;
	g_pSetTipText = pExeFunc.pSetTipText;
	g_pClearTipText = pExeFunc.pClearTipText;
	g_pAppendTipText = pExeFunc.pAppendTipText;
	g_pCleanHistory  = pExeFunc.pCleanHistory;
	return 1;
}

INT_PTR WINAPI ValidateProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	wchar_t cPass[15];
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hDlg_Validate = hwndDlg;
		hEdit_Password = GetDlgItem(hwndDlg,IDC_EDIT_PASSWORD);
		hStatic_Result = GetDlgItem(hwndDlg,IDC_STATIC_RESULT);
		SendMessage(hEdit_Password,EM_LIMITTEXT,10,0);
		if(g_iUseLanguage == 1)
		{
			g_pSetDialogText(&hwndDlg,IDD_DIALOG_VALIDATE);
		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			memset(cPass,0,15 * sizeof(wchar_t));
			GetWindowText(hEdit_Password,cPass,11);
			if(wcscmp(cPass,sOP.cPassword)==0)
			{
				EndDialog(hwndDlg,1);
			}
			else
			{
				SetWindowTextW(hEdit_Password,L"");
				ShowWindow(hStatic_Result,SW_SHOW);
				SetFocus(hEdit_Password);
			}
			return TRUE;
		case IDCANCEL:
			EndDialog(hwndDlg,0);
			return FALSE;
		}
	default:
		return FALSE;
	}
	return FALSE;
}




/*int SetSW_Dll(DWORD dwPID,HWND hWnd)
{

	int i = 0,j= 0;
	char cBaSW[4],cPID[10];
	WndAndHandle * sW_QH = NULL;
	memset(cBaSW,0,4);
	itoa(iBaSW_dll,cBaSW,10);
	OutputDebugString(cBaSW);
	sW_QH = new WndAndHandle[iBaSW_dll];


	ReadWHFile(sW_QH);
	if(bHideWindows == 1)
	{
		OutputDebugString("HideWindow == 1");
		for(i = 0;i < iBaSW_dll;i++)
		{
			memset(cPID,0,10);
			ltoa(sW_QH[i].lPID,cPID,10);
			OutputDebugString(cPID);
			memset(cPID,0,10);
			itoa(sW_QH[i].bReserved,cPID,10);
			OutputDebugString(cPID);
			memset(cPID,0,10);
			ltoa(dwPID,cPID,10);
			OutputDebugString(cPID);
			if(sW_QH[i].lPID == dwPID && sW_QH[i].bReserved == 1)
			{
				//ShowWindow(hWnd,SW_HIDE);
				OutputDebugString("Have Hided.");
				//SendMessage(hCurWnd,WM_ADDNEWWINDOW,(WPARAM)hWnd,0);
			}
		}
	}
	delete [] sW_QH;

	

	return 1;
}*/






///*int WriteWHFile(WndAndHandle * pWH)
//{
//	ResetEvent(hEvt_ReadWHFile);
//	HANDLE hWHFile;
//	int i = 0;
//	char *cWHFileName = (char*) new char[1024];
//	memset(cWHFileName,0,1024);
//	GetModuleFileName(hInst,cWHFileName,1024);
//	PathRemoveFileSpec(cWHFileName);
//	strcat(cWHFileName,"\\WHSW.whf");
//
//	hWHFile = CreateFile(cWHFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,
//		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
//	delete [] cWHFileName;
//
//	char * cRecord = new char[512];
//	char * cTemp = cRecord;
//	DWORD dwWritten = 0;
//	for(i = 0;i < iBaSW_dll;i++)
//	{
//		memset(cRecord,0,512);
//		/**(int*)cRecord = sW_dll[i].bExist;
//		cRecord += sizeof(int);*/
//		*(int*)cRecord = sW_dll[i].bHide;
//		cRecord += sizeof(int);
//		*(int*)cRecord = sW_dll[i].bReserved;
//		cRecord += sizeof(int);
//		/*memcpy(cRecord,sW_dll[i].cIconTip,1024);
//		cRecord += 1024;
//		memcpy(cRecord,sW_dll[i].cProcessName,20);
//		cRecord += 20;
//		memcpy(cRecord,sW_dll[i].cWndTitle,2048);
//		cRecord += 2048;*/
//		*(HWND *)cRecord = sW_dll[i].hWnd;
//		cRecord += sizeof(HWND);
//		/**(int*)cRecord = sW_dll[i].iHasTrayIcon;
//		cRecord += sizeof(int);
//		*(int*)cRecord = sW_dll[i].iIconIndex;
//		cRecord += sizeof(int);
//		*(int*)cRecord = sW_dll[i].iLink;
//		cRecord += sizeof(int);
//		*(int*)cRecord = sW_dll[i].iWindowType;
//		cRecord += sizeof(int);*/
//		*(DWORD*)cRecord = sW_dll[i].lPID;
//		cRecord += sizeof(DWORD);
//		/**(HICON*)cRecord = sW_dll[i].trayicon.hIcon;
//		cRecord += sizeof(HICON);
//		*(HWND *)cRecord = sW_dll[i].trayicon.hwnd;
//		cRecord += sizeof(HWND);
//		memcpy(cRecord,sW_dll[i].trayicon.Reserved,sizeof(DWORD)*2);
//		cRecord += sizeof(DWORD)*2;
//		*(UINT*)cRecord = sW_dll[i].trayicon.uCallbackMessage;
//		cRecord += sizeof(UINT);
//		*(UINT*)cRecord = sW_dll[i].trayicon.uID;
//		cRecord += sizeof(UINT);*/
//
//
///*		WriteFile(hWHFile,cTemp,cRecord-cTemp,&dwWritten,NULL);
//		cRecord = cTemp;
//	}
//	delete [] cTemp;
//
//	CloseHandle(hWHFile);
//	SetEvent(hEvt_ReadWHFile);
//
//	return 1;
//}*/



//int ReadWHFile(WndAndHandle * pWH)
//{
//	hEvt_ReadWHFile = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_FOR_READ_WHFILE");
//	if(hEvt_ReadWHFile == NULL)
//	{
//		OutputDebugString("Open ReadWHFile Event Failure!");
//	}
//
//	/*if(WaitForSingleObject(hEvt_ReadWHFile,1000*1000*3) == WAIT_OBJECT_0)
//	{
//
//	}*/
////	OutputDebugString("In ReadWHFile.");
//	HANDLE hWHFile;
//	int i = 0;
//	char *cWHFileName = (char*) new char[1024];
//	memset(cWHFileName,0,1024);
//	GetModuleFileName(hInst,cWHFileName,1024);
//	PathRemoveFileSpec(cWHFileName);
//	strcat(cWHFileName,"\\WHSW.whf");
//
//	hWHFile = CreateFile(cWHFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,
//		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
//	delete [] cWHFileName;
//
//	DWORD dwSize = GetFileSize(hWHFile,NULL);
//	
//	char * cFileBuff = new char[dwSize];
//	DWORD dwReaded = 0;
//	ReadFile(hWHFile,cFileBuff,dwSize,&dwReaded,NULL);
//	char * cTemp = cFileBuff;
//
//	for(i = 0;i < iBaSW_dll;i++)
//	{
//		pWH[i].bHide = *(int*)cFileBuff;
//		cFileBuff += sizeof(int);
//		pWH[i].bReserved = *(int*)cFileBuff;
//		cFileBuff += sizeof(int);
//		pWH[i].hWnd = *(HWND*)cFileBuff;
//		cFileBuff += sizeof(HWND);
//		pWH[i].lPID = *(DWORD *)cFileBuff;
//		cFileBuff += sizeof(DWORD);
//	}
//	cFileBuff = cTemp;
//
//	delete [] cFileBuff;
//	CloseHandle(hWHFile);
//
//	return 1;
//}


int RestoreVolume()
{
	if (dwOldVolume != 0)
	{
		CVolumeOutMaster OM;
		OM.SetCurrentVolume(dwOldVolume);

		CVolumeOutWave OW;
		OW.SetCurrentVolume(dwOldWave);

	}

	return 1;
}






HWND FindTrayToolbarWindow()
{
    HWND hTrayWnd = ::FindWindow(L"Shell_TrayWnd", NULL);
    hTrayWnd = ::FindWindowEx(hTrayWnd, 0, L"TrayNotifyWnd", NULL);
 	HWND hwndTmp;	
    hwndTmp = ::FindWindowEx(hTrayWnd, 0, L"SysPager", NULL);
	if(!hwndTmp)
		hTrayWnd=::FindWindowEx(hTrayWnd,0,L"ToolbarWindow32",NULL);
	else
		hTrayWnd=::FindWindowEx(hwndTmp,0,L"ToolbarWindow32",NULL);

	return hTrayWnd;
}

int HideProcess(DWORD dwPID)
{
#ifdef _HIDEPROCESS
	if(pDri == NULL)
	{
		return 0;
	}
	pDri->HideProc(dwPID);
#endif

	return 1;
}





int ShowProcess(DWORD dwPID)
{
#ifdef _HIDEPROCESS
	if(pDri == NULL)
	{
		return 0;
	}
	pDri->ShowProc(dwPID);
#endif


	return 1;
}


BOOL GetHidedProcessName(DWORD dwPID,wchar_t * cProcessName)
{
#ifdef _HIDEPROCESS
	if(pDri == NULL)
	{
		return 0;
	}
	pDri->GetHidedProcessName(dwPID,cProcessName);
#endif


	return 1;
}


int GetHidedProcessCount()
{
#ifdef _HIDEPROCESS
	if(pDri == NULL)
	{
		return 0;
	}
	int iCount = pDri->GetHidedProcessCount();
	return iCount;
#endif
	return 0;

}

int GetHidedProcessByIdx(int idx,wchar_t * cProcessName)
{
#ifdef _HIDEPROCESS
	if(pDri == NULL)
	{
		return 0;
	}
	int iRet = pDri->GetHidedProcessByIdx(idx,cProcessName);
	return iRet;
#endif
	
	return 0;

}





