//DongDapeng8@sohu.com All rights reserved 




// Update.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "commctrl.h"
#include "winsock2.h"
#include "process.h"
#include "shlwapi.h"
#include "stdlib.h"
#include "update.h"
#include "shellapi.h"
#include "unrar.h"
#include "tlhelp32.h"
#include "Language.h"




#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
DWORD dwError=0;
VersionInfo viCF,viRF;
HWND hMainDlg,hProgress;
HWND hStatic_Connect,hStatic_Check,hStatic_Download,hStatic_Install;
HFONT hFont_Old,hFont_New;
char b ;
HICON hIcon;
HANDLE hUpdateThread = NULL;
int iRunFlag = 0;

char * cRequestHeader = NULL, * cResponseHeader = NULL,*cRecvBuff=NULL,*cArcName [30];
char * pTemp= NULL,*pTemp2= NULL,cFileSize[20],*pVerBuff = NULL,*pVerValue = NULL;
char * RARBuff = NULL;
int g_iUseLanguage = 0;

wchar_t * cFileName = NULL;

// Foward declarations of functions included in this code module:
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MainProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK IfUpdateProc(HWND,UINT,WPARAM,LPARAM);



DWORD WINAPI UpdateThread(LPVOID);

int CreateVersionInfoFile();
int ReadVersionInfoFile(VersionInfo*);
int TerminateHidedragonExe();
int GarbageRecycle();
int CALLBACK rarCallback(UINT,LONG,LONG,LONG);
int HD_GetState(char*);
int CopyUpdatedFile();
int AutoCopyFile();









int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	wchar_t * cCurrentDic = new wchar_t[1024],*cTemp = NULL;
	memset(cCurrentDic,0,1024 * sizeof(wchar_t));
	GetModuleFileName(NULL,cCurrentDic,1024);
	cTemp = wcsrchr(cCurrentDic,L'\\');
	memset(cTemp,0,wcslen(cTemp)*2);
	SetCurrentDirectory(cCurrentDic);
	delete [] cCurrentDic;

	OutputDebugString(L"dddddddhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh");
	DWORD dwError = 0;
	CreateMutex(NULL, FALSE,L"HideDragonOnlineUpdate");
	dwError = GetLastError();
	if(dwError == ERROR_ALREADY_EXISTS)
	{
		return 0;

	}

	MSG msg;
	HACCEL hAccelTable;

	hInst = hInstance;
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_UPDATE, szWindowClass, MAX_LOADSTRING);

	cRequestHeader = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cResponseHeader = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cRecvBuff = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	pVerBuff = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	RARBuff = (char*)VirtualAlloc(NULL,16*1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cFileName = (wchar_t*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);



	InitCommonControls();

	g_iUseLanguage = SetCurrentLanguage();


	
	wchar_t * pCmdLine = new wchar_t[512];
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,lpCmdLine,strlen("-D"),pCmdLine,512*sizeof(wchar_t));
	if(wcscmp(pCmdLine,L"") == 0)
		exit(0);

	if(_wcsnicmp(pCmdLine,L"-N",2) == 0)
	{
		AutoCopyFile();
	}
	else if(_wcsnicmp(pCmdLine,L"-Z",2) == 0)
	{
		DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),NULL,(DLGPROC)MainProc);
		//exit(0);
	}
	else if(_wcsnicmp(pCmdLine,L"-U",2) == 0)
	{
		DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_IFUPDATENOW),NULL,(DLGPROC)IfUpdateProc);
	}


	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_UPDATE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}






// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDC_START || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

INT_PTR CALLBACK MainProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//LOGFONT lf;
	HWND hStaticProcess;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hMainDlg = hwndDlg;
		hStatic_Connect = GetDlgItem(hwndDlg,IDC_STATIC_CONNECT);
		hStatic_Check = GetDlgItem(hwndDlg,IDC_STATIC_CHECK);
		hStatic_Download = GetDlgItem(hwndDlg,IDC_STATIC_DOWNLOAD);
		hStatic_Install = GetDlgItem(hwndDlg,IDC_STATIC_INSTALL);
		hProgress = GetDlgItem(hwndDlg,IDC_PROGRESS);

		hFont_Old = (HFONT)SendMessage(hStatic_Connect,WM_GETFONT,NULL,NULL);
//		lf.lfHeight = 12;
//		lf.lfWidth = 0;
//		lf.lfEscapement = 0;
//		lf.lfOrientation = 0;
//		lf.lfWeight = 800;
//		lf.lfItalic = FALSE;
//		lf.lfUnderline = FALSE;
//		lf.lfStrikeOut = FALSE;
//		lf.lfCharSet = DEFAULT_CHARSET;
//		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
//		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
//		lf.lfQuality = DEFAULT_QUALITY;
//		lf.lfPitchAndFamily = DEFAULT_PITCH;
//		strcpy(lf.lfFaceName,"宋体");
//		hFont_New = CreateFontIndirect(&lf);

		hIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_UPDATE));
		SendMessage(hwndDlg,WM_SETICON,(WPARAM)ICON_BIG,(LPARAM)hIcon);
		if(g_iUseLanguage == 1)
		{
			SetDialogText(&hwndDlg,IDD_DIALOG_MAIN);
		}
		hStaticProcess = GetDlgItem(hwndDlg,IDC_STATIC_UPDATEPROGRESS);
		SendMessage(hStaticProcess,WM_SETFONT,(WPARAM)hFont_New,TRUE);
		

		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_START:
//			if(hUpdateThread == NULL)
//			{
//				hUpdateThread = (HANDLE)_beginthreadex(NULL,0,
//					(unsigned int(__stdcall*)(void*))UpdateThread,NULL,0,NULL);
//				if(g_iUseLanguage == 0)
//				{
//					SetDlgItemText(hwndDlg,IDC_START,"停止");
//				}
//				else
//				{
//					char * pText = new char[1024];
//					SetDlgItemText(hwndDlg,IDC_START,
//						LoadItemString("IDS_BUTTON_STOP",pText,"停止"));
//					delete [] pText;
//				}
//				SendMessage(hStatic_Connect,WM_SETFONT,(WPARAM)hFont_New,TRUE);
//			}
//			else
//			{
//				TerminateThread(hUpdateThread,0);
//				if(g_iUseLanguage == 0)
//				{
//					SetDlgItemText(hwndDlg,IDC_START,"开始");
//				}
//				else
//				{
//					char * pText = new char[1024];
//					SetDlgItemText(hwndDlg,IDC_START,
//						LoadItemString("IDS_BUTTON_START",pText,"开始"));
//					delete [] pText;
//				}
//				hUpdateThread = NULL;
//			}
			break;
			return 0;
		case IDC_CREATEFILE:
			CreateVersionInfoFile();
			break;
		case IDCANCEL:
			WSACleanup();
			GarbageRecycle();
			PostQuitMessage(1);
//			if (iRunFlag == 1)
//			{
//				ShellExecute(NULL,"open","hidedragon.exe","-D",NULL,SW_SHOWNORMAL);
//			}
			break;
		}
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK IfUpdateProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			hMainDlg = hDlg;
			hIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_UPDATE));
			SendMessage(hDlg,WM_SETICON,(WPARAM)ICON_BIG,(LPARAM)hIcon);
			if(g_iUseLanguage == 1)
			{
				SetDialogText(&hDlg,IDD_DIALOG_IFUPDATENOW);
			}

			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDCANCEL:
				GarbageRecycle();
				PostQuitMessage(1);
				ShellExecute(NULL,L"open",L"hidedragon.exe",L"-D",NULL,SW_SHOWNORMAL);
			
				break;
			case IDC_UPDATENOW:
				CopyUpdatedFile();
				HDMessageBox(L"IDS_HDUPDATE_HIDEDRAGONUPDATESUCCESS",
					L"隐身专家更新完成!",hDlg);
				break;
			default:
				return FALSE;
			}
	}
    return FALSE;
}


DWORD WINAPI UpdateThread(LPVOID lp)
{
/*	WSADATA wd;
	SOCKET sock_downfile;
	hostent * pHostent;	
	sockaddr_in ServerAddr;
	char cDomain[30] = "http://kinghome.51.net",SubBlock[100]="";
	DWORD dwSize = 20,dwWritten = 0,dwVerInfoSize = 0;
	int iError,bEndRecv=0,index=0,iVerLen = 0,iRHLen = 0;
	float fCurrentVer=0;
	long lTime = 0,lFileSize = 0,lRecvSize=0,lDownloadSize=0,lWritten = 0,rarMode = 0;
	HANDLE hVersionFile = NULL,hProgramFile = NULL,hArcData = NULL;
	int RHCode= 0,PFCode = 0;
	RARHeaderData rarHeader;
	RAROpenArchiveDataEx rarArchiveData;
	enum { EXTRACT, TEST, PRINT };
	int i = 0;


	hVersionFile = CreateFile((const char *)"VersionInfo_U.hdv",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hVersionFile == INVALID_HANDLE_VALUE)
	{
		dwError = GetLastError();
	}


	WSAStartup(MAKEWORD(2,2),&wd);

	sock_downfile = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	memset(cDomain,0,30);
	UrlGetPart("http://kinghome.51.net",cDomain,&dwSize,URL_PART_HOSTNAME,NULL);
	

	pHostent = gethostbyname(cDomain);
	if(pHostent == NULL)
	{
		HDMessageBox("IDS_HDUPDATE_CANNOTRESOLVEADDR",
			"无法解析服务器地址，请检查网络稍后再试。");
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;


		return 0;
	}
	memset(&ServerAddr,0,sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(80);
	memcpy((void*)&(ServerAddr.sin_addr),pHostent->h_addr_list[0],pHostent->h_length);

	if(connect(sock_downfile,(sockaddr*)&ServerAddr,sizeof(ServerAddr))!=0)
	{
		HDMessageBox("IDS_HDUPDATE_CANNOTCONNECTSERVER",
			"无法连接服务器，请检查网络稍后再试。");
		iError = WSAGetLastError();
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;

		return 0;
	}

	SendMessage(hStatic_Connect,WM_SETFONT,(WPARAM)hFont_Old,TRUE);
	SendMessage(hStatic_Check,WM_SETFONT,(WPARAM)hFont_New,TRUE);

	memset(cRequestHeader,'\0',1024);
	memset(cResponseHeader,'\0',1204);
	//Format request header///////////////////////////////////////////////////
	strcat(cRequestHeader,"GET ");
	strcat(cRequestHeader,"/VersionInfo_U.hdv");
	strcat(cRequestHeader," HTTP/1.1");
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"Host:");
	strcat(cRequestHeader,cDomain);
	strcat(cRequestHeader,"\r\n");

//	strcat(cRequestHeader,"Accept:*//**");
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"Connection:Keep-Alive");
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"\r\n");
	///////////////////////////////////////////////////////////////////////////////////////////
	//lTime = 1000000;
	//setsockopt(sock_downfile,SOL_SOCKET,SO_RCVTIMEO,(char*)&lTime,sizeof(lTime));
	iRHLen = strlen(cRequestHeader);
	if (send(sock_downfile,cRequestHeader,iRHLen,0)== SOCKET_ERROR)
	{
		dwError = WSAGetLastError();
		MessageBox(hMainDlg,"发送请求头失败----versioninfo.hdv","隐身专家",MB_OK|MB_ICONINFORMATION);
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;

		return 0;
	}

	//recv response header /////////////////////////////////////////////////////////////////////
	bEndRecv = 0;
	while(!bEndRecv&&index < 1024)
	{
		if(recv(sock_downfile,&b,1,0)==SOCKET_ERROR)
		{
			dwError = WSAGetLastError();
			MessageBox(hMainDlg,"接收响应头失败----versioninfo.hdv","隐身专家",MB_OK|MB_ICONINFORMATION);
			if(g_iUseLanguage == 0)
			{
				SetDlgItemText(hMainDlg,IDC_START,"开始");
			}
			else
			{
				char * pText = new char[1024];
				SetDlgItemText(hMainDlg,IDC_START,
					LoadItemString("IDS_BUTTON_START",pText,"开始"));
				delete [] pText;
			}
			CloseHandle(hUpdateThread);
			hUpdateThread = NULL;

			return 0;
		}

		cResponseHeader[index]=b;
		if(index>=3)
		{
			if(cResponseHeader[index-3]=='\r'&&cResponseHeader[index-2]=='\n'&&
				cResponseHeader[index-1]=='\r'&& cResponseHeader[index-0]=='\n')
			{
				bEndRecv=1;
			}
		}
		index++;
	}
	lRecvSize = index-1;
	bEndRecv = 0;

	/*recv(sock_downfile,cResponseHeader,1024,0);
	pTemp =strstr(cResponseHeader,"\r\n\r\n");
	memset(pTemp+4,0,1024-(cResponseHeader-pTemp));*/
	
	/*if (HD_GetState(cResponseHeader) == 0)
	{
		HDMessageBox("IDS_HDUPDATE_REQUESTFILENOTFOUND",
			"请求的文件没有找到。");
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;
		return 0;
	}
	
	pTemp = NULL;
	pTemp = strstr(cResponseHeader,"Content-Length");
	if(pTemp == NULL)
	{
		HDMessageBox("IDS_HDUPDATE_SERVERBUSY","服务器忙，请重试。");
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;
		return 0; 
	}
	pTemp+=strlen("Content-Length");
	pTemp += 2;
	pTemp2 = strstr(pTemp,"\r\n");
	memset(cFileSize,0,20);
	memcpy(cFileSize,pTemp,pTemp2-pTemp);
	lFileSize = atol(cFileSize);	


	while(lDownloadSize < lFileSize)
	{
		lRecvSize = recv(sock_downfile,cRecvBuff,1024,0);
		if (lRecvSize == SOCKET_ERROR)
		{
			dwError = WSAGetLastError();
			MessageBox(hMainDlg,"接收文件失败-----versioninfo.hdv","隐身专家",MB_OK|MB_ICONINFORMATION);
			if(g_iUseLanguage == 0)
			{
				SetDlgItemText(hMainDlg,IDC_START,"开始");
			}
			else
			{
				char * pText = new char[1024];
				SetDlgItemText(hMainDlg,IDC_START,
					LoadItemString("IDS_BUTTON_START",pText,"开始"));
				delete [] pText;
			}
			CloseHandle(hUpdateThread);
			hUpdateThread = NULL;

			return 0;
		}
		lDownloadSize += lRecvSize;
		WriteFile(hVersionFile,cRecvBuff,lRecvSize,&dwWritten,NULL);
	}
	CloseHandle(hVersionFile);
	ReadVersionInfoFile(&viRF);
	dwVerInfoSize = GetFileVersionInfoSize((LPTSTR)"HideDragon.exe",0);
	if(dwVerInfoSize == 0)
	{
		dwError = GetLastError();
		HDMessageBox("IDS_HDUPDATE_FILENOTENOUGH",
			"当前目录程序不完整。");
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;

		return 0;
	}
	struct LANGANDCODEPAGE {
	  WORD wLanguage;
	  WORD wCodePage;
	} *lpTranslate;


	GetFileVersionInfo((LPTSTR)"HideDragon.exe",0,dwVerInfoSize,pVerBuff);
	VerQueryValue(pVerBuff,"\\VarFileInfo\\Translation",(void **)&lpTranslate,(unsigned int*)&iVerLen);

	wsprintf( SubBlock, 
				TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
				lpTranslate[0].wLanguage,
				lpTranslate[0].wCodePage);

	VerQueryValue(pVerBuff,SubBlock,(void **)&pVerValue,(unsigned int*)&iVerLen);
	fCurrentVer = (float)atof(pVerValue);
	if(fCurrentVer<viRF.fVersionNum)
	{
		SendMessage(hStatic_Check,WM_SETFONT,(WPARAM)hFont_Old,TRUE);
		if(g_iUseLanguage == 0)
		{
			SetWindowText(hStatic_Download,"3.已检查到更新版本，正在下载...");
		}
		else
		{
			char * pText = new char[1024];
			SetWindowText(hStatic_Download,LoadItemString("IDS_HDUPDATE_DOWNLOADINGNEW",
				pText,"3.已检查到更新版本，正在下载..."));
			delete [] pText;
		}
		SendMessage(hStatic_Download,WM_SETFONT,(WPARAM)hFont_New,TRUE);
		memset(cRequestHeader,'\0',1024);
		index = 0;

		//If have newer version,downlaod it////////////////////////////////////////////////////////////
		strcpy(cRequestHeader,"GET ");
		strcat(cRequestHeader,"/hidedragon.rar");
		strcat(cRequestHeader," HTTP/1.1");
		strcat(cRequestHeader,"\r\n");

		strcat(cRequestHeader,"Host:");
		strcat(cRequestHeader,cDomain);
		strcat(cRequestHeader,"\r\n");

		strcat(cRequestHeader,"Accept:*//*");
		strcat(cRequestHeader,"\r\n");

		strcat(cRequestHeader,"User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
		strcat(cRequestHeader,"\r\n");

		strcat(cRequestHeader,"Connection:Keep-Alive");
		strcat(cRequestHeader,"\r\n");

		strcat(cRequestHeader,"\r\n");

		iRHLen = strlen(cRequestHeader);
		closesocket(sock_downfile);
		sock_downfile = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

		if(connect(sock_downfile,(sockaddr*)&ServerAddr,sizeof(ServerAddr))!=0)
		{
			HDMessageBox("IDS_HDUPDATE_CANNOTCONNECTSERVER",
				"无法连接服务器，请检查网络稍后再试。");
			iError = WSAGetLastError();
			if(g_iUseLanguage == 0)
			{
				SetDlgItemText(hMainDlg,IDC_START,"开始");
			}
			else
			{
				char * pText = new char[1024];
				SetDlgItemText(hMainDlg,IDC_START,
					LoadItemString("IDS_BUTTON_START",pText,"开始"));
				delete [] pText;
			}
			CloseHandle(hUpdateThread);
			hUpdateThread = NULL;

			return 0;
		}


		if(send(sock_downfile,cRequestHeader,iRHLen,NULL) == SOCKET_ERROR)
		{
			dwError = WSAGetLastError();
			MessageBox(hMainDlg,"发送请求头失败------hidedragon.rar","隐身专家",MB_OK|MB_ICONINFORMATION);
			if(g_iUseLanguage == 0)
			{
				SetDlgItemText(hMainDlg,IDC_START,"开始");
			}
			else
			{
				char * pText = new char[1024];
				SetDlgItemText(hMainDlg,IDC_START,
					LoadItemString("IDS_BUTTON_START",pText,"开始"));
				delete [] pText;
			}
			CloseHandle(hUpdateThread);
			hUpdateThread = NULL;

			return 0;
		}
		//recv response header////////////////////////////////////////////////////////////////////////////
		bEndRecv = 0;
		index = 0;
		memset(cResponseHeader,'\0',1024);
		while(!bEndRecv&&index < 1024)
		{
			lRecvSize = recv(sock_downfile,&b,1,0);
			if (lRecvSize ==SOCKET_ERROR)
			{
				dwError = WSAGetLastError();
				MessageBox(hMainDlg,"接收响应头失败-----hidedragon.rar","隐身专家",MB_OK|MB_ICONINFORMATION);
				if(g_iUseLanguage == 0)
				{
					SetDlgItemText(hMainDlg,IDC_START,"开始");
				}
				else
				{
					char * pText = new char[1024];
					SetDlgItemText(hMainDlg,IDC_START,
						LoadItemString("IDS_BUTTON_START",pText,"开始"));
					delete [] pText;
				}
				CloseHandle(hUpdateThread);
				hUpdateThread = NULL;

				return 0;
			}
			else if (lRecvSize == 0)
			{
				continue;
			}
			cResponseHeader[index]=b;
			if(index>=3)
			{
				if(cResponseHeader[index-3]=='\r'&&cResponseHeader[index-2]=='\n'&&
					cResponseHeader[index-1]=='\r'&& cResponseHeader[index-0]=='\n')
				{
					bEndRecv=1;
				}
			}
			index++;
		}
		lRecvSize = index-1;
		bEndRecv = 0;
		
		hProgramFile = CreateFile("HideDragon.rar",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

		if(HD_GetState(cResponseHeader) == 0)
		{
			HDMessageBox("IDS_REQUESTFILENOTFOUND","请求的文件没有找到。");
			if(g_iUseLanguage == 0)
			{
				SetDlgItemText(hMainDlg,IDC_START,"开始");
			}
			else
			{
				char * pText = new char[1024];
				SetDlgItemText(hMainDlg,IDC_START,
					LoadItemString("IDS_BUTTON_START",pText,"开始"));
				delete [] pText;
			}
			CloseHandle(hUpdateThread);
			hUpdateThread = NULL;
			return 0;
		}
		
		pTemp = NULL;
		pTemp = strstr(cResponseHeader,"Content-Length");
		if(pTemp == NULL)
		{
			HDMessageBox("IDS_HDUPDATE_SERVERBUSY","服务器忙，请稍后重试。");
			if(g_iUseLanguage == 0)
			{
				SetDlgItemText(hMainDlg,IDC_START,"开始");
			}
			else
			{
				char * pText = new char[1024];
				SetDlgItemText(hMainDlg,IDC_START,
					LoadItemString("IDS_BUTTON_START",pText,"开始"));
				delete [] pText;
			}
			CloseHandle(hUpdateThread);
			hUpdateThread = NULL;

			return 0;
		}
		pTemp+=strlen("Content-Length");
		pTemp += 2;
		pTemp2 = strstr(pTemp,"\r\n");
		memset(cFileSize,0,20);
		memcpy(cFileSize,pTemp,pTemp2-pTemp);
		lFileSize = atol(cFileSize);	

		lDownloadSize = 0;
		lRecvSize = 0;
		memset(cRecvBuff,0,1024);
		SendMessage(hProgress,PBM_SETRANGE32,(WPARAM)0,(LPARAM)lFileSize);
		SendMessage(hProgress,PBM_SETSTEP,(WPARAM)1024,0);
		while(lDownloadSize < lFileSize)
		{
			lRecvSize = recv(sock_downfile,cRecvBuff,1024,0);
			if(lRecvSize == SOCKET_ERROR)
			{
				dwError = WSAGetLastError();
				MessageBox(hMainDlg,"接收文件失败---hidedragon.rar","隐身专家",MB_OK|MB_ICONINFORMATION);
				if(g_iUseLanguage == 0)
				{
					SetDlgItemText(hMainDlg,IDC_START,"开始");
				}
				else
				{
					char * pText = new char[1024];
					SetDlgItemText(hMainDlg,IDC_START,
						LoadItemString("IDS_BUTTON_START",pText,"开始"));
					delete [] pText;
				}
				CloseHandle(hUpdateThread);
				hUpdateThread = NULL;

				return 0;
			}
			lDownloadSize += lRecvSize;
			WriteFile(hProgramFile,cRecvBuff,lRecvSize,&dwWritten,NULL);
			memset(cRecvBuff,0,1024);
			//SendMessage(hProgress,PBM_STEPIT,NULL,NULL);
			SendMessage(hProgress,PBM_SETPOS,(WPARAM)lDownloadSize,0);
		}
		CloseHandle(hProgramFile);
		SendMessage(hProgress,PBM_SETPOS,(WPARAM)lFileSize,0);
		SendMessage(hStatic_Check,WM_SETFONT,(WPARAM)hFont_Old,TRUE);
		if(g_iUseLanguage == 0)
		{
			SetWindowText(hStatic_Download,"3.下载完毕");
		}
		else
		{
			char * pText = new char[1024];
			SetWindowText(hStatic_Download,LoadItemString("IDS_HDUPDATE_DOWNLOADSUCCESS",
				pText,"3.下载完毕"));
			delete [] pText;
		}
		SendMessage(hStatic_Download,WM_SETFONT,(WPARAM)hFont_Old,TRUE);
	}
	else
	{
		if(g_iUseLanguage == 0)
		{
			SetWindowText(hStatic_Check,"2.没有检查到更新版本，请以后再试");
		}
		else
		{
			char * pText = new char[1024];
			SetWindowText(hStatic_Check,LoadItemString("IDS_HDUPDATE_NOTFOUNDNEWER",
				pText,"2.没有检查到更新版本，请以后再试"));
			delete [] pText;
		}
		SendMessage(hStatic_Check,WM_SETFONT,(WPARAM)hFont_New,TRUE);
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;

		return 0;
	}
	SendMessage(hStatic_Install,WM_SETFONT,(WPARAM)hFont_New,TRUE);
	/*ShellExecute(hMainDlg,"open","unrar.exe","e hidedragon.rar temp\\ -o+",NULL,SW_HIDE);
	Sleep(10000);*/

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/*memset((void*)&rarArchiveData,0,sizeof(rarArchiveData));
	memset(cArcName,0,30);
	strcpy((char*)cArcName,"hidedragon.rar");
	rarArchiveData.ArcName = (char*)cArcName;
	rarArchiveData.CmtBuf = RARBuff;
	rarArchiveData.CmtBufSize = sizeof(16*1024);
	rarArchiveData.OpenMode = RAR_OM_EXTRACT;

	hArcData = RAROpenArchiveEx(&rarArchiveData);

	if (rarArchiveData.OpenResult != 0)
	{
		MessageBox(hMainDlg,"open rar file error","hd",MB_OK|MB_ICONINFORMATION);
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;
	}
	rarMode = EXTRACT;
	RARSetCallback(hArcData,rarCallback,(LONG)&rarMode);
	rarHeader.CmtBuf = NULL;
	while((RHCode = RARReadHeader(hArcData,&rarHeader))==0)
	{
		PFCode = RARProcessFile(hArcData,RAR_EXTRACT,"temp\\",NULL);
		if (PFCode != 0)
		{
			MessageBox(hMainDlg,"rarprocessfile error","hd",MB_OK|MB_ICONINFORMATION);
			if(g_iUseLanguage == 0)
			{
				SetDlgItemText(hMainDlg,IDC_START,"开始");
			}
			else
			{
				char * pText = new char[1024];
				SetDlgItemText(hMainDlg,IDC_START,
					LoadItemString("IDS_BUTTON_START",pText,"开始"));
				delete [] pText;
			}
			CloseHandle(hUpdateThread);
			hUpdateThread = NULL;
		}
	}
	if(RHCode == ERAR_BAD_DATA)
	{
		MessageBox(hMainDlg,"File header broken","hd",MB_OK|MB_ICONINFORMATION);
		if(g_iUseLanguage == 0)
		{
			SetDlgItemText(hMainDlg,IDC_START,"开始");
		}
		else
		{
			char * pText = new char[1024];
			SetDlgItemText(hMainDlg,IDC_START,
				LoadItemString("IDS_BUTTON_START",pText,"开始"));
			delete [] pText;
		}
		CloseHandle(hUpdateThread);
		hUpdateThread = NULL;

	}
	RARCloseArchive(hArcData);
	VirtualFree(RARBuff,NULL,MEM_RELEASE);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	TerminateHidedragonExe();
	Sleep(5*1000);

	for(i = 0;i < viRF.iSiteNum-1;i++)
	{
		memset(cFileName,0,1024);
		strcpy(cFileName,"temp\\");
		strcat(cFileName,viRF.sUI[i].cURLs);
		
		if(!CopyFile(cFileName,viRF.sUI[i].cURLs,FALSE))
		{
			dwError = GetLastError();
		}
		DeleteFile(cFileName);
	}
	 
 

	SendMessage(hStatic_Install,WM_SETFONT,(WPARAM)hFont_Old,TRUE);
	HDMessageBox("IDS_HDUPDATE_HIDEDRAGONUPDATESUCCESS",
		"隐身专家在线更新成功!");

	if(g_iUseLanguage == 0)
	{
		SetDlgItemText(hMainDlg,IDC_START,"开始");
	}
	else
	{
		char * pText = new char[1024];
		SetDlgItemText(hMainDlg,IDC_START,
			LoadItemString("IDS_BUTTON_START",pText,"开始"));
		delete [] pText;
	}
	CloseHandle(hUpdateThread);
	hUpdateThread = NULL;
	DeleteFile("versioninfo_U.hdv");
	WSACleanup();
	iRunFlag = 1;
	PostMessage(hMainDlg,WM_COMMAND,MAKEWPARAM(IDCANCEL,0),0);*/


	return 1;
}




int CreateVersionInfoFile()
{
	HANDLE hVerInfoFile = NULL;
	char * cFileContent = NULL,*cTemp = NULL;
	long lFileSize = 0;
	DWORD dwWritten = 0;


	viCF.fVersionNum = 3.0f;
	viCF.iSiteNum = 1;
	viCF.sUI = (URL_Info*)VirtualAlloc(NULL,viCF.iSiteNum*sizeof(URL_Info),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);

	for(int i = 0;i<viCF.iSiteNum;i++)
	{
		viCF.sUI[i].cURLs = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		viCF.sUI[i].lSize = 0;
	}  
	strcpy(viCF.sUI[0].cURLs,"http://kinghome.51.net/en/29/notbook/notbook.php");
//	strcpy(viCF.sUI[0].cURLs,"VerHistory.rtf");
//	strcpy(viCF.sUI[1].cURLs,"unrar.dll");
//	strcpy(viCF.sUI[2].cURLs,"readme.rtf"); 
//	strcpy(viCF.sUI[3].cURLs,"Misc.hdo");
//	strcpy(viCF.sUI[4].cURLs,"license.txt");
//	strcpy(viCF.sUI[5].cURLs,"HOOKLIB.dll");
//	strcpy(viCF.sUI[6].cURLs,"HideDragon.exe");
//	strcpy(viCF.sUI[7].cURLs,"hidedragon.exe.manifest");
//	strcpy(viCF.sUI[8].cURLs,"HDUpdate.exe.manifest");
//	strcpy(viCF.sUI[9].cURLs,"HDUpdate.exe");
//	strcpy(viCF.sUI[10].cURLs,"stlport_vc646.dll");
//	
//	strcpy(viCF.sUI[11].cURLs,"Language\\Multi_eng.ini");
//	strcpy(viCF.sUI[12].cURLs,"Language\\Multi_ch.ini");
//	strcpy(viCF.sUI[13].cURLs,"Language\\Multi_Jp.ini");
//	strcpy(viCF.sUI[14].cURLs,"Language\\LangSetting.ini");



	hVerInfoFile = CreateFile(L"VersionInfo_U.hdv",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	cFileContent = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);

	cTemp = cFileContent;
	*(float*)cFileContent = viCF.fVersionNum;
	cFileContent += sizeof(float);
	lFileSize += sizeof(float);
	*(long*)cFileContent = viCF.iSiteNum;
	cFileContent += sizeof(int);
	lFileSize += sizeof(int);
	for(i = 0;i<viCF.iSiteNum;i++)
	{
		viCF.sUI[i].lSize = strlen(viCF.sUI[i].cURLs);
		*(long*)cFileContent = viCF.sUI[i].lSize;
		cFileContent += sizeof(long);
		lFileSize += sizeof(long);

		strcpy(cFileContent,viCF.sUI[i].cURLs);
		cFileContent += strlen(viCF.sUI[i].cURLs);
		lFileSize += strlen(viCF.sUI[i].cURLs);
	}
	cFileContent = cTemp;
	WriteFile(hVerInfoFile,cFileContent,lFileSize,&dwWritten,NULL);
	CloseHandle(hVerInfoFile);
	//Recycle memory///////////////////////////////////////////////////////////////////
	for(i = 0;i < viCF.iSiteNum;i++)
	{
		VirtualFree(viCF.sUI[i].cURLs,0,MEM_RELEASE);
	}
	VirtualFree(viCF.sUI,0,MEM_RELEASE);
	VirtualFree(cFileContent,0,MEM_RELEASE);


	MessageBox(hMainDlg,L"OK",L"HideDragon",MB_OK|MB_ICONINFORMATION);

	return 1;
}


int CALLBACK rarCallback(UINT msg,LONG UserData,LONG P1,LONG P2)
{
	switch(msg)
	{
	case UCM_PROCESSDATA:
		return 0;
	}
	return 0;
}

int TerminateHidedragonExe()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = (HANDLE)CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First((void*)hProcessSnap,&pe32);
	while(Process32Next((void*)hProcessSnap,&pe32))
	{
		if(wcscmp(wcslwr(pe32.szExeFile),L"hidedragon.exe")==0)
		{
			TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,FALSE,
				pe32.th32ProcessID),0);
			return 1;
		}
		
	}
	return 0;
}


int GarbageRecycle()
{
	VirtualFree(cRequestHeader,0,MEM_RELEASE);
	VirtualFree(cResponseHeader,0,MEM_RELEASE);
	VirtualFree(cRecvBuff,0,MEM_RELEASE);
	VirtualFree(pVerBuff,0,MEM_RELEASE);
	VirtualFree(RARBuff,0,MEM_RELEASE);
	VirtualFree(cFileName,0,MEM_RELEASE);
	for(int i = 0;i < viCF.iSiteNum;i++)
	{
		VirtualFree(viCF.sUI[i].cURLs,0,MEM_RELEASE);
	}
	VirtualFree(viCF.sUI,0,MEM_RELEASE);
	

	return 1;
}


//int HD_GetState(char * cResponseHeader)
//{
//	char * temp1= NULL;
//	char cState[3];
//
//
//	temp1 = cResponseHeader;
//	cState[0] = (char)temp1[9];
//	cState[1] = (char)temp1[10];
//	cState[2] = (char)temp1[11];
//
//	if(atoi(cState)!=200)
//	{
//		return 0;
//	}
//	
//	return 1;
//}
//


int CopyUpdatedFile()
{
	ReadVersionInfoFile(&viRF);

	TerminateHidedragonExe();
	
	memset(cFileName,0,1024);
	GetModuleFileName(NULL,cFileName,1024);
	wchar_t * cTemp = wcsrchr(cFileName,L'\\'),* cDesDir = new wchar_t[1024],*Back1 = new wchar_t[1024],
		*Back2 = new wchar_t[1024];

	memset(cDesDir,0,1024);
	memset(Back1,0,1024);
	memset(Back2,0,1024);
	memset(cTemp+1,0,wcslen(cTemp)-1);
	wcscpy(cDesDir,cFileName);
	cTemp = wcsstr(cDesDir,L"Downloads\\temp");
	memset(cTemp,0,wcslen(L"Downloads\\temp\\"));
	wcscpy(Back1,cFileName);
	wcscpy(Back2,cDesDir);



	for(int i = 0;i < viRF.iSiteNum;i++)
	{
		wcscpy(cFileName,Back1);
		wcscpy(cDesDir,Back2);
		wchar_t * pURL = new wchar_t[512];
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,viRF.sUI[i].cURLs,200,pURL,512);

		wcscat(cFileName,pURL);
		wcscat(cDesDir,pURL);

				
		if(!CopyFile(cFileName,cDesDir,FALSE))
		{
			dwError = GetLastError();
		}
		DeleteFile(cFileName);		
	}

	wcscpy(cDesDir,Back2);
	wcscat(cDesDir,L"Downloads\\");
	wcscat(cDesDir,L"HideDragon.rar");
	DeleteFile(cDesDir);
	wcscpy(cDesDir,Back2);
	wcscat(cDesDir,L"HideDragon.exe");
	ShellExecute(NULL,L"Open",cDesDir,L"-D",NULL,SW_SHOWNORMAL);

	delete [] cDesDir;
	delete [] Back1;
	delete [] Back2;
	GarbageRecycle();
	PostQuitMessage(1);

	

	return 1;
}

int AutoCopyFile()
{
	CopyUpdatedFile();
	GarbageRecycle();
	PostQuitMessage(1);

	return 1;
}




