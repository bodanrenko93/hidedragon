#define _WIN32_WINNT 0x0400
#include "windows.h"
#include "mmsystem.h"
#include "stdio.h"
#include "resource.h"
#include "commctrl.h"


#define HOOKLIBAPI extern "C" __declspec(dllexport)

#include "../hooklib.h"


HINSTANCE hInst;
int iTest,iBaSW_dll;
HHOOK hMouseHook;
char dd [50];
WndParam sWP2;
WndAndHandle *sW_dll,*sForBK;
OptionFile sOP;
HWND hEdit_Password,hStatic_Result,hDlg_Validate;
HWND hTrayWnd_dll;
 


#pragma data_seg("DllShared")
HWND hAll = NULL;
HWND hLV = NULL;
int iLButton = 0;
int iRButton = 0;
int iWin     = 0,iWinNum = 0;
int iReserveCount = 0, iTemp = 0;
HWND hCurWnd = NULL;
int bHideWindows = 1;
DWORD dwOldVolume = 0;

#pragma data_seg()

__declspec(allocate("DllShared")) HWND WindowsArray [100];
__declspec(allocate("DllShared")) char cReservedWnds [80][80];
__declspec(allocate("DllShared")) char cPIs[80][80];

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
		//RestoreAllWindows();
		UnhookWindowsHookEx(hMouseHook);
		hMouseHook = NULL;
		//ClearAll();
	}
	return 1;

	 
	
}

LRESULT CALLBACK MouseHookProc(int nCode,WPARAM wParam ,LPARAM lParam)
{	
	if (nCode >= 0)
	{
		if (wParam == WM_LBUTTONDOWN)
		{
			iLButton = 1;
		}
		if (wParam == WM_RBUTTONDOWN)
		{
			iRButton = 1;
		}
		if (wParam == WM_LBUTTONUP)
		{
			iLButton = 0;
		}
		if (wParam == WM_RBUTTONUP)
		{
			iRButton = 0;
		}
		if (iLButton==1 && iRButton == 1)
		{
			iLButton = iRButton = 0;
			MyHideWindows();		
		}


	}
	else
		return CallNextHookEx(hMouseHook,nCode,wParam,lParam);


	
	return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
}

	
BOOL CALLBACK EnumWindowsFunc(HWND hwnd,LPARAM lParam)
{
	unsigned long lPID;
	char cPID[10];
	int i1 = 0;

	if (GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)
	{
		if (hwnd == hCurWnd || GetParent(hwnd) == hCurWnd)
			return 1;
		GetWindowText(hwnd,dd,30);
		if (!strcmp(dd,"Program Manager"))
			return 1;
		iTemp = iReserveCount;
		GetWindowThreadProcessId(hwnd,&lPID);
		//_ultoa(lPID,cPID,10);
		sprintf(cPID,"%X",lPID);
   
		//2.2 method 
		for(i1 = 0;i1<iBaSW_dll;i1++)
		{
			if (sW_dll[i1].bReserved != 1)
			{
				//ShowWindow(sW_dll[i1].hWnd,SW_HIDE);
				PostMessage(sW_dll[i1].hWnd,WM_SHOWWINDOW,FALSE,NULL);
				sW_dll[i1].bHide = 1;
			}
		}   
		GetClassName(hwnd,dd,50);
		if (!strcmp(dd,"Shell_TrayWnd")) 
			return 1;
	}
	//pFT1 ->psNID ->hIcon = pFT1->hIcon2;
	//strcpy(pFT1 ->psNID ->szTip,"隐身专家(已隐藏窗口)");
	//Shell_NotifyIcon(NIM_MODIFY, pFT1->psNID);
	return 1;

}

  
int MyHideWindows()
{
	NOTIFYICONDATA nid; 
	/*if(sOP.bUseBKMouse == 1)
	{
		MoveWindow2BK();
		return 1;;
	}*/

	if (bHideWindows == 1)	
	{
		if (sOP.bDisableSound == 1)
		{
			waveOutGetVolume(0,&dwOldVolume);
			waveOutSetVolume(0,0);
		}
		EnableWindow(hAll,FALSE);
		EnableWindow(hLV,FALSE);


		//EnumWindows((WNDENUMPROC)EnumWindowsFunc,0);
		for(int i1 = 0;i1<iBaSW_dll;i1++)
		{
			if (sW_dll[i1].bReserved != 1)
			{

				ShowWindow(sW_dll[i1].hWnd,SW_HIDE);
				sW_dll[i1].bHide = 1;
				if(sW_dll[i1].iHasTrayIcon == 1)
				{
					nid.cbSize = sizeof(NOTIFYICONDATA);
					nid.hWnd = sW_dll[i1].trayicon.hwnd;
					nid.uID = sW_dll[i1].trayicon.uID;
					Shell_NotifyIcon(NIM_DELETE,&nid);

				}
			}
		}   

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

		if (dwOldVolume != 0)
		{
			waveOutSetVolume(0,dwOldVolume);
			dwOldVolume = 0;
		}
		//2.2 method ///
		for (int i = 0;i < iBaSW_dll;i++)
		{
			ShowWindow(sW_dll[i].hWnd,SW_SHOW);
			if(sW_dll[i].iHasTrayIcon == 1 && sW_dll[i].bHide == 1)
			{
				nid.cbSize           = sizeof(NOTIFYICONDATA);
				nid.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
				nid.uID              = sW_dll[i].trayicon.uID;
				nid.hIcon            = sW_dll[i].trayicon.hIcon;
				nid.hWnd             = sW_dll[i].trayicon.hwnd;
				strcpy(nid.szTip,(const char *)sW_dll[i].cIconTip);
				//wcstombs(nid.szTip,sW_dll[i].cIconTip,1022);
				nid.uCallbackMessage = sW_dll[i].trayicon.uCallbackMessage;
				Shell_NotifyIcon(NIM_ADD,&nid);				
			}
			sW_dll[i].bHide = 0;
		}
		bHideWindows = 1;


 
		/*iWin --;
		while(iWin >= 0)
		{
			ShowWindow(WindowsArray[iWin],SW_SHOW);
			iWin --;
		}
		iWin = 0;
		//pFT1->psNID ->hIcon = pFT1->hIcon1;
		//strcpy(pFT1->psNID ->szTip,"隐身专家(未隐藏窗口)");
		//Shell_NotifyIcon(NIM_MODIFY,pFT1->psNID);*/
	}
	return 1;
}


int ReserveWnds(WndAndHandle * sWP ,int iCount)
{
	//2.2 method///
	sW_dll = sWP;
	iBaSW_dll = iCount;



	/*iReserveCount = iCount;
	while(iCount > 0)
	{
		iCount --;
		strcpy (cReservedWnds[iCount],sWP.cT[iCount]);
		strcpy (cPIs[iCount],sWP.cP[iCount] );

	}*/
	return 1;
}

int ClearAll()
{
	int i;
	for ( i = 0;i < 100;i++)
	{
		WindowsArray [i] = NULL;
	}
	for(i = 0;i < 80;i++)
	{
		strcpy(cReservedWnds[i],"");
	}
	return 1;
}

int RestoreAllWindows(int iKorS)
{
	if(iKorS == 'S')   
	{
		for (int i = 0;i < iBaSW_dll;i++)
		{
			ShowWindow(sW_dll[i].hWnd,SW_SHOW);
			sW_dll[i].bHide = 0;
		}
	}
	else
	{
		//2.2 method
		for (int i = 0;i<iBaSW_dll;i++)
		{
			//GetWindowThreadProcessId(sW[i].,&dPID);
			if (sW_dll[i].bHide == 1)
			{
				TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,FALSE,sW_dll[i].lPID),0);
			}
		}
	}
	return 1;
}

int SetParameter(OptionFile sOP_temp)
{
	sOP = sOP_temp;
	return 1;
}

INT_PTR WINAPI ValidateProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char cPass[15];
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hDlg_Validate = hwndDlg;
		hEdit_Password = GetDlgItem(hwndDlg,IDC_EDIT_PASSWORD);
		hStatic_Result = GetDlgItem(hwndDlg,IDC_STATIC_RESULT);
		SendMessage(hEdit_Password,EM_LIMITTEXT,10,0);

		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			memset(cPass,0,15);
			GetWindowText(hEdit_Password,cPass,11);
			if(strcmp(cPass,sOP.cPassword)==0)
			{
				EndDialog(hwndDlg,1);
			}
			else
			{
				SetWindowText(hEdit_Password,"");
				ShowWindow(hStatic_Result,SW_SHOW);

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



/*int MoveWindow2BK()
{
	HWND hForeWnd;
	DWORD dwPID;
	int i = 0,j=0;
	WndAndHandle * pTemp = NULL, * pNext = NULL;

	sForBK = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	for(i=0;i<100;i++)
	{
		sForBK[i].cWndTitle = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		sForBK[i].cIconTip = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	}

	hForeWnd = GetForegroundWindow();
	GetWindowThreadProcessId(hForeWnd,&dwPID);

	EnumWindows(EnumWindowsForBK,0);

	for(i = 0;i<iAllWnd;i++)
	{
		if (sForBK[i].lPID == dwPID)
		{
			if(sForBK[i].iWindowType == 1)
		}
	}

	for (i = 0;i<iAll-1;i++)
	{
		if(sForBK[i].lPID == dwPID)
		{
			for(j = i+1;j <= iAllWnd-1;j++)
			{
				if(sForBK[i].lPID == sForBK[j].lPID	&& strcmp(sForBK[i].cProcessName,"explorer.exe") != 0)
				{
					if(sForBK[i].bReserved == 1)
					{
						strcpy(sForBK[j].cWndTitle,sForBK[i].cWndTitle);
						sForBK[j].bReserved = 1;
					}
					pTemp = sForBK + j;
					while(pTemp->Next!=NULL)
					{
						pNext = pTemp->Next;
						pTemp = pNext;
					}
					pTemp->Next = sForBK+i;
					sForBK[i].bExist = 1;
					sForBK[i].Next = NULL;
					pTemp->iLink ++;
				}
			}
		}
	}


	return 1;
}*/
































  
