#include <windows.h>


#include "hd.h"
#include "ProcessData.h"
#include <TlHelp32.h>
#include <ShlObj.h> 
#include "PictureEx.h"
#include "WebDB.h"






WndAndHandle * g_QuickHideAllTray_au = NULL;
WndAndHandle * sBKSel_con = NULL;



extern HWND hTrayWnd;
extern HICON hIcon_Unknown;
extern WndAndHandle * sW;
extern int iBaSW;

extern CPictureEx m_flash;
extern HWND g_TabWnd[10];


URL_Array urlArray;



int AllocResourceForTrayList_au()
{
	g_QuickHideAllTray_au = (WndAndHandle*)VirtualAlloc(NULL,
		sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);


	int i = 0;
	for(i = 0;i<100;i++)
	{
		g_QuickHideAllTray_au[i].cWndTitle = NULL;//(wchar_t *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		g_QuickHideAllTray_au[i].cIconTip = (wchar_t*)new wchar_t [1024];
		memset(g_QuickHideAllTray_au[i].cIconTip,0,1024 * sizeof(wchar_t));
//		memset(g_QuickHideAllTray_au[i].cWndTitle,0,2048 * sizeof(wchar_t));
		memset(g_QuickHideAllTray_au[i].cProcessName,0,30 * sizeof(wchar_t));
		g_QuickHideAllTray_au[i].Next = NULL;
	}

	

	return 1;
}

extern HBRUSH g_brushAllPage;
extern HBRUSH g_brushTrayPage;
extern HBRUSH g_brushBKPage;

extern HFONT hFont_New2;

int ReleaseResourceForTrayList_au()
{
	int i = 0;
	for(i = 0;i<100;i++)
	{
		delete [] g_QuickHideAllTray_au[i].cIconTip;
	}
	VirtualFree(g_QuickHideAllTray_au,0,MEM_RELEASE);
	return 1;
}



int EnumAllVisibleTray_au()
{
	wchar_t * pszText = NULL;
	DWORD lPID;
	int iconIndex = 0,iconNum = 0;
	wchar_t cPID[20] = {0};
	int i = 0,j = 0;
	ICONINFO iconInfo;
	wchar_t cProcessName[1024] = L"";




	for(i = 0;i<100;i++)
	{
		if(g_QuickHideAllTray_au[i].trayicon.Reserved[0] == 22
			&& g_QuickHideAllTray_au[i].trayicon.Reserved[1] == 33)
		{
			DestroyIcon(g_QuickHideAllTray_au[i].trayicon.hIcon);
		}
		g_QuickHideAllTray_au[i].iHasTrayIcon = 0;
		g_QuickHideAllTray_au[i].iIconIndex = 0;
		g_QuickHideAllTray_au[i].iCommandId = 0;
		g_QuickHideAllTray_au[i].hWnd = NULL;
		g_QuickHideAllTray_au[i].trayicon.hIcon = NULL;
		g_QuickHideAllTray_au[i].trayicon.hwnd = NULL;
		g_QuickHideAllTray_au[i].trayicon.uID = 0;
		g_QuickHideAllTray_au[i].trayicon.uCallbackMessage = 0;
		g_QuickHideAllTray_au[i].bToBk = 0;
		g_QuickHideAllTray_au[i].lPID = 0;
		g_QuickHideAllTray_au[i].bHide = 0;
		g_QuickHideAllTray_au[i].bExist = 0;
		memset(g_QuickHideAllTray_au[i].cProcessName,0,30 * sizeof(wchar_t));
		memset(g_QuickHideAllTray_au[i].cIconTip,0,1024 * sizeof(wchar_t));
		//memset(g_QuickHideAllTray_au[i].cWndTitle,0,2048 * sizeof(wchar_t));
	}




	GetWindowThreadProcessId(hTrayWnd,&lPID);
	CProcessData<TBBUTTON> data(lPID);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo = {0};
	wchar_t Tipwchar_t = 0;
	wchar_t sTip[1024] = L"\0";
	wchar_t* pTip = NULL;
	int x = 0,iRet = 0;
	
	j = 0;
	int iBtnCount = SendMessage(hTrayWnd,TB_BUTTONCOUNT,0,0);

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
		SendMessage(hTrayWnd,TB_GETBUTTON,i,(LPARAM)data.GetData());
		data.ReadData(&tb);
		data.ReadData(&tray,(void * )tb.dwData);

		
		
		if (!(tb.fsState & TBSTATE_HIDDEN))
		{
			GetWindowThreadProcessId(tray.hwnd,&lPID);
			
			if(lPID == GetCurrentProcessId())
			{
				continue;
			}
			memset(&iconInfo,0,sizeof(iconInfo));
			if(GetIconInfo(tray.hIcon,&iconInfo) == 0)
			{
				DWORD dwErr = GetLastError();
				tray.hIcon = hIcon_Unknown;
				//GetProcessTrayIcon(lPID,&tb,&tray);
			}
			if(iconInfo.hbmColor != NULL)
			{
				DeleteObject(iconInfo.hbmColor);
			}
			if(iconInfo.hbmMask != NULL)
			{
				DeleteObject(iconInfo.hbmMask);
			}
			g_QuickHideAllTray_au[j].bHide = tb.idCommand;//Use this member save idcommand for hide or show
			g_QuickHideAllTray_au[j].cWndTitle = NULL;
			//g_QuickHideAllTray_au[j].cIconTip = new wchar_t[1024];
			//memset(g_QuickHideAllTray_au[j].cIconTip,0,1024 * sizeof(wchar_t));
			memset(g_QuickHideAllTray_au[j].cProcessName,0,30 * sizeof(wchar_t));
			GetWindowThreadProcessId(hTrayWnd,&lExID);
			if(lExID == lPID)
			{
				wcscpy(g_QuickHideAllTray_au[j].cProcessName,L"explorer.exe");
			}


			//g_QuickHideAllTray_au[j].trayicon = tray;
			g_QuickHideAllTray_au[j].trayicon.hIcon = tray.hIcon;
			g_QuickHideAllTray_au[j].trayicon.hwnd = tray.hwnd;
			g_QuickHideAllTray_au[j].trayicon.Reserved[0] = tray.Reserved[0];
			g_QuickHideAllTray_au[j].trayicon.Reserved[1] = tray.Reserved[1];
			g_QuickHideAllTray_au[j].trayicon.uCallbackMessage = tray.uCallbackMessage;
			g_QuickHideAllTray_au[j].trayicon.uID = tray.uID;
			g_QuickHideAllTray_au[j].lPID = lPID;
			g_QuickHideAllTray_au[j].hWnd = NULL;
			g_QuickHideAllTray_au[j].Next = NULL;





			memset(sTip,0,1024*(sizeof(wchar_t)));
			x = 0;
			pTip = (wchar_t*)tb.iString;
			do 
			{	
				if(x >= 1000)
				{
					wcscpy(sTip,L"[ToolTip was either too long or not set]");
					break;
				}
				
				data.ReadData_Tip((wchar_t*)&Tipwchar_t, (void *)pTip++);
			}while( x < 1000 && (sTip[x++] = Tipwchar_t));

			iRet = 0;

			wcscpy(g_QuickHideAllTray_au[j].cIconTip,sTip);

			{
				memset(cProcessName,0,1024*sizeof(wchar_t));
				GetProcessName(lPID,cProcessName);
				if(wcscmp(cProcessName,L"flashget.exe") == 0)
				{
					int j = 0;
				}
				int bInList = IsInAutoHideList(cProcessName);
				if(bInList == 1)
				{
					BKThisIcon(g_QuickHideAllTray_au + j);
				}
			}


			j ++;
		}
	}


	return 1;
}

int iW_Sel_au = 0;
wchar_t * cBKWndText_au = NULL;
PROCESSENTRY32 pe322_au;
extern HWND hMainDialog;
WndAndHandle * sBKSel_au = NULL;

extern WndAndHandle * sBKSel;


int EnumAllVisibleWindow_au()
{
	EnumWindows(EnumWindowsForAU,(LPARAM)sBKSel_au);

	iW_Sel_au = 0;


	return 1;
}








int AllocAutoHideWindow()
{
	int i = 0;
	sBKSel_au = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,
		MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);


	memset(sBKSel_au,0,sizeof(WndAndHandle)*100);
	
	for(i = 0;i<100;i++)
	{
		sBKSel_au[i].cWndTitle = (wchar_t *) VirtualAlloc(NULL,2048,
			MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		sBKSel_au[i].cIconTip = (wchar_t*)new wchar_t [1024];
		memset(sBKSel_au[i].cIconTip,0,1024 * sizeof(wchar_t));
		memset(sBKSel_au[i].cWndTitle,0,2048 * sizeof(wchar_t));
		memset(sBKSel_au[i].cProcessName,0,30 * sizeof(wchar_t));
		sBKSel_au[i].Next = NULL;
	}
	

	return 1;
}


int ResetAutoHideWindow()
{
	iW_Sel_au = 0;
	
	int i = 0;
	for(i = 0;i<100;i++)
	{
		sBKSel_au[i].iHasTrayIcon = 0;
		sBKSel_au[i].iIconIndex = 0;
		sBKSel_au[i].iCommandId = 0;
		sBKSel_au[i].hWnd = NULL;
		sBKSel_au[i].trayicon.hIcon = NULL;
		sBKSel_au[i].lPID = 0;
		sBKSel_au[i].bExist = 0;
		memset(sBKSel_au[i].cProcessName,0,30 * sizeof(wchar_t));
		memset(sBKSel_au[i].cIconTip,0,1024 * sizeof(wchar_t));
		memset(sBKSel_au[i].cWndTitle,0,2048 * sizeof(wchar_t));
	}






	return 1;
}


int ReleaseAutoHideWindow()
{
	int i = 0;
	for(i = 0;i<100;i++)
	{
		VirtualFree(sBKSel_au[i].cWndTitle,0,MEM_RELEASE);
		delete [] sBKSel_au[i].cIconTip;
		VirtualFree(sBKSel_au[i].cWndTitle,0,MEM_RELEASE);
	}

	VirtualFree(sBKSel_au,0,MEM_RELEASE);



	return 1;
}



BOOL CALLBACK EnumWindowsForAU(HWND hwnd,LPARAM lParam)
{
	DWORD lPID;
	WndAndHandle * sBKSel_au = (WndAndHandle*)lParam;
	HANDLE hProcessSnap2_bk = NULL,hProcessSnap2 = NULL;

	if (::GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)
	{
		cBKWndText_au = (wchar_t*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		memset(cBKWndText_au,0,2048 * sizeof(wchar_t));
		GetWindowText(hwnd,cBKWndText_au,2048);
		GetWindowThreadProcessId(hwnd,&lPID);
		if (/*!wcscmp(cBKWndText_au,"")||*/!wcscmp(cBKWndText_au,L"Program Manager"))
		{
			VirtualFree(cBKWndText_au,0,MEM_RELEASE);
			return 1;
		}
		if (hwnd == hMainDialog||::GetParent(hwnd) == hMainDialog)
		{
			VirtualFree(cBKWndText_au,0,MEM_RELEASE);
			return 1;
		}


		hProcessSnap2 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		hProcessSnap2_bk = hProcessSnap2;
		pe322_au.dwSize = sizeof(PROCESSENTRY32);
		Process32First((void*)hProcessSnap2,&pe322_au);
		int iFoundFlag_BK = 0;
		while(Process32Next((void*)hProcessSnap2,&pe322_au))
		{
			if(pe322_au.th32ProcessID == lPID)
			{
				iFoundFlag_BK = 1;
				if(wcscmp(wcslwr(pe322_au.szExeFile),L"explorer.exe")==0)
				{
					if (wcscmp(cBKWndText_au,L"") != 0)
					{
						wcscpy(sBKSel_au[iW_Sel_au].cProcessName,pe322_au.szExeFile);
						sBKSel_au[iW_Sel_au].hWnd = hwnd;
						sBKSel_au[iW_Sel_au].lPID = lPID;
						sBKSel_au[iW_Sel_au].Next = NULL;
						GetWindowText(hwnd,sBKSel_au[iW_Sel_au].cWndTitle,2040);

					}
				}
				else
				{
					wcscpy(sBKSel_au[iW_Sel_au].cProcessName,pe322_au.szExeFile);
					sBKSel_au[iW_Sel_au].hWnd = hwnd;
					sBKSel_au[iW_Sel_au].lPID = lPID;
					sBKSel_au[iW_Sel_au].Next = NULL;
					if (wcscmp(cBKWndText_au,L"") == 0)
					{
						wcscpy(sBKSel_au[iW_Sel_au].cWndTitle,pe322_au.szExeFile);
						sBKSel_au[iW_Sel_au].bReserved = 1;
					}
					else
					{
						GetWindowText(hwnd,sBKSel_au[iW_Sel_au].cWndTitle,2040);
					}
					if (IsWindow(GetParent(hwnd)))
					{
						sBKSel_au[iW_Sel_au].iWindowType = 1;
					}
				}
			}
		}
		
		CloseHandle((HANDLE)hProcessSnap2);
		

		VirtualFree(cBKWndText_au,0,MEM_RELEASE);
		cBKWndText_au = NULL;
		if(wcscmp(sBKSel_au[iW_Sel_au].cProcessName,L"itunes.exe") == 0)
		{
			int j = 0;
		}
		if(IsInAutoHideList(sBKSel_au[iW_Sel_au].cProcessName,1) == 1)
		{
			BKCurrentWindow_au(sBKSel_au+iW_Sel_au);
		}
		iW_Sel_au ++;
	}
	return 1;
}


HidedProcessList g_HidedWndList;


int BKCurrentWindow_au(WndAndHandle * pItem)
{
	if(IsInHidedWndList(pItem))
	{
		return 0;
	}


	wchar_t * cTitle = new wchar_t[2048],cPID[20];
	memset(cTitle,0,2048 * sizeof(wchar_t));
	memset(cPID,0,20 * sizeof(wchar_t));
	

	FillList();
	swprintf(cPID,L"%X",pItem->lPID);
	wcscpy(cTitle,pItem->cWndTitle);
	ConstructBKSW_au(cPID,cTitle);
	delete [] cTitle;
	HideSelected(&sBKSel_con,1,1);
		
	WndAndHandle * pHidedItem = new WndAndHandle;
	memset(pHidedItem,0,sizeof(WndAndHandle));
	pHidedItem->lPID = pItem->lPID;
	g_HidedWndList.push_back(pHidedItem);



	Switch2BKTab();
	FillList();




	return 1;
}




int ConstructBKSW_au(wchar_t * cPID,wchar_t * cTitle, HWND hWnd /*= NULL*/)
{
	wchar_t cCurrentPID[20];
	int i = 0,j = 0;
	WndAndHandle *pNext = NULL,*pTemp = NULL,*pBKSel = NULL,* pTemp2 = NULL;
	
	sBKSel_con = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	pTemp2 = sBKSel_con;
	for(i = 0;i<100;i++)
	{
		sBKSel_con[i].cWndTitle = (wchar_t *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		sBKSel_con[i].cIconTip = (wchar_t*)new wchar_t [1024];
		memset(sBKSel_con[i].cIconTip,0,1024 * sizeof(wchar_t));
		memset(sBKSel_con[i].cWndTitle,0,2048 * sizeof(wchar_t));
		memset(sBKSel_con[i].cProcessName,0,30 * sizeof(wchar_t));
		sBKSel_con[i].Next = NULL;
	}
	
	j = 0;
	for(i = 0;i < iBaSW;i ++)
	{
		memset(cCurrentPID,0,20 * sizeof(wchar_t));
		swprintf(cCurrentPID,L"%X",sW[i].lPID);
		if(wcscmp(cCurrentPID,cPID) == 0 && sW[i].bHide == 0 &&
			wcscmp(sW[i].cProcessName,L"explorer.exe") != 0 &&
			wcscmp(sW[i].cProcessName,L"iexplore.exe") != 0)
		{
			if(wcscmp(sW[i].cWndTitle,L"") == 0)
			{
				wcscpy(sBKSel_con[j].cWndTitle,sW[i].cProcessName);
			}
			else
			{
				wcscpy(sBKSel_con[j].cWndTitle,sW[i].cWndTitle);
			}
			wcscpy(sBKSel_con[j].cProcessName,sW[i].cProcessName);
			sBKSel_con[j].hWnd = sW[i].hWnd;
			sBKSel_con[j].lPID = sW[i].lPID;
			sBKSel_con[j].iWindowType = sW[i].iWindowType;
			sW[i].bToBk = 1;
			if(sW[i].iHasTrayIcon == 1)
			{
				sBKSel_con[j].trayicon = sW[i].trayicon;
				sBKSel_con[j].iCommandId = sW[i].iCommandId;
				wcscpy(sBKSel_con[j].cIconTip,sW[i].cIconTip);
			}
			sBKSel_con[j].Next = NULL;
			j ++;
			if(j > 1)
			{
				sBKSel_con[j-2].Next = sBKSel_con + (j-1);
			}
		}
		else if(wcscmp(sW[i].cProcessName,L"explorer.exe") == 0 && 
			wcscmp(sW[i].cWndTitle,cTitle) == 0)
		{
			wcscpy(sBKSel_con[j].cWndTitle,sW[i].cWndTitle);
			wcscpy(sBKSel_con[j].cProcessName,sW[i].cProcessName);
			sBKSel_con[j].hWnd = sW[i].hWnd;
			sBKSel_con[j].lPID = sW[i].lPID;
			sBKSel_con[j].iWindowType = sW[i].iWindowType;
			sW[i].bToBk = 1;
			sBKSel_con[j].Next = NULL;
			j ++;
			break;
		}
		else if(wcscmp(sW[i].cProcessName,L"iexplore.exe") == 0 && 
			(hWnd?(sW[i].hWnd == hWnd):(wcscmp(sW[i].cWndTitle,cTitle) == 0)))
		{
			wcscpy(sBKSel_con[j].cWndTitle,sW[i].cWndTitle);
			wcscpy(sBKSel_con[j].cProcessName,sW[i].cProcessName);
			sBKSel_con[j].hWnd = sW[i].hWnd;
			sBKSel_con[j].lPID = sW[i].lPID;
			sBKSel_con[j].iWindowType = sW[i].iWindowType;
			sW[i].bToBk = 1;
			sBKSel_con[j].Next = NULL;
			j ++;
			break;
		}
	}
	
	return 1;
}



HidedProcessList g_HidedTrayList;

int BKThisIcon(WndAndHandle * pItem)
{
	WndAndHandle * pTemp = NULL;
	
	if(IsInHidedList(pItem) == 1)//Hided
	{
		return 0;
	}

	pItem->bToBk = 1;
	HWND hWnd = (HWND)1;
	pItem->hWnd = hWnd;
	
	HideSelected(&pItem,1);

	WndAndHandle * pHidedItem = NULL;
	pHidedItem = new WndAndHandle;
	memset(pHidedItem,0,sizeof(WndAndHandle));
	pHidedItem->lPID = pItem->lPID;
	pHidedItem->iWindowType = 2;//在这里这个成员用来标记是否为托盘图标2是托盘，1是窗口




	g_HidedTrayList.push_back(pHidedItem);

	FillTrayList();





	return 1;
}


int ReleaseTrayHidedList()
{
	HidedProcessList::iterator iNode;
	
	int iCount = g_HidedTrayList.size();
	if(iCount <= 0)
	{
		return 0;
		
	}
	iNode = g_HidedTrayList.begin();
	
	while(iNode != g_HidedTrayList.end())
	{
		WndAndHandle * pI = *iNode;
		
		
		g_HidedTrayList.erase(iNode++);
		//iNode --;
		
		delete  pI;
		
		//iNode ++;
	}	



	return 1;
}

int IsInHidedList(WndAndHandle * pItem)
{
	HidedProcessList::iterator iNode;
	
	int iCount = g_HidedTrayList.size();
	if(iCount <= 0)
	{
		return 0;

	}
	iNode = g_HidedTrayList.begin();

	while(iNode != g_HidedTrayList.end())
	{
		WndAndHandle * pI = *iNode;
		
		if(pI->lPID == pItem->lPID)
		{
			return 1;
		}
		
		iNode ++;
	}



	return 0;
}





int IsInHidedWndList(WndAndHandle * pItem)
{
	HidedProcessList::iterator iNode;
	
	int iCount = g_HidedWndList.size();
	if(iCount <= 0)
	{
		return 0;
		
	}
	iNode = g_HidedWndList.begin();
	
	while(iNode != g_HidedWndList.end())
	{
		WndAndHandle * pI = *iNode;
		
		if(pI->lPID == pItem->lPID)
		{
			return 1;
		}
		
		iNode ++;
	}

	return 0;
}

int ReleaseHidedWndList()
{
	HidedProcessList::iterator iNode;
	
	int iCount = g_HidedWndList.size();
	if(iCount <= 0)
	{
		return 0;
		
	}
	iNode = g_HidedWndList.begin();
	
	while(iNode != g_HidedWndList.end())
	{
		WndAndHandle * pI = *iNode;



		g_HidedWndList.erase(iNode++);
		//iNode --;		

		
		delete pI;
		pI = NULL;
		//iNode ++;
	}


	return 1;
}








extern DBProcessList g_DBProcessList;




int IsInAutoHideList(wchar_t * pName,int iType /* == 2*/)
{
	DBProcessList::iterator iNode;
	
	iNode = g_DBProcessList.begin();
	while(iNode != g_DBProcessList.end())
	{
		AutoHideItem * pI = *iNode;
		
		int iRet = wcscmp(wcslwr(pI->cProcessName),wcslwr(pName));
		if(iType == 1 && pI->iType == iType && iRet == 0)
		{
			return 1;
		}
		if(iType == 2 && pI->iType == iType && iRet == 0)
		{
			return 1;
		}
		
		iNode ++;
	}

	return 0;
}



BOOL GetGifLink()
{
	wchar_t pLink_w[1024];// = new wchar_t;
	char pLink_a[1024];// = new char;
	
	
	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_LINK",pLink_w,L"/cn/gif/cn/giflink.db");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	

	ReadGifLinkToList(urlArray);

	
	int iRet = GetFileFromServer(pLink_a);


	
// 	delete [] pLink_w;
// 	pLink_w = NULL;
// 	delete [] pLink_a;
// 	pLink_a = NULL;





	ReadGifLinkToList(urlArray);





	return TRUE;
}






BOOL GetGifFile()
{
	wchar_t pLink_w[1024];// = new wchar_t;
	char pLink_a[1024];// = new char;

	
	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_ADV",pLink_w,L"/cn/gif/cn/gif_adv.gif");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	
	int iRet = GetFileFromServer(pLink_a);
	






	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_AH",pLink_w,L"/cn/gif/cn/gif_ah.gif");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	
	iRet = GetFileFromServer(pLink_a);
	

	



	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_BK",pLink_w,L"/cn/gif/cn/gif_bk.gif");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	
	iRet = GetFileFromServer(pLink_a);
	




	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_CLEAN",pLink_w,L"/cn/gif/cn/gif_clean.gif");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	
	iRet = GetFileFromServer(pLink_a);
	


	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_GEN",pLink_w,L"/cn/gif/cn/gif_gen.gif");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	
	iRet = GetFileFromServer(pLink_a);
	



	
	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_TRAY",pLink_w,L"/cn/gif/cn/gif_tray.gif");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	
	iRet = GetFileFromServer(pLink_a);
	
	

	
	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_WEB",pLink_w,L"/cn/gif/cn/gif_web.gif");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	
	iRet = GetFileFromServer(pLink_a);
	
	
	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_GIF_ALL",pLink_w,L"/cn/gif/cn/gif_all.gif");
	
	memset(pLink_a,0,1024);
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);
	
	iRet = GetFileFromServer(pLink_a);

	


	ReloadGif();







// 	delete [] pLink_w;
// 	pLink_w = NULL;
// 	delete [] pLink_a;
// 	pLink_a = NULL;




	return TRUE;
}



extern CPictureEx gif_adv;
extern CPictureEx gif_gen;
extern CPictureEx gif_tray;
extern CPictureEx gif_bk;
extern CPictureEx gif_web;
extern CPictureEx gif_clean;
extern CPictureEx gif_ah;








BOOL ReloadGif()
{
	wchar_t cPath[1024]; 
	memset(cPath,0,1024*sizeof(wchar_t));


	SHGetSpecialFolderPath(NULL, cPath, CSIDL_PERSONAL, FALSE);
	wcscat(cPath,L"\\HideDragon");
	wcscat(cPath,L"\\Downloads\\");
	wcscat(cPath,L"gif_all.gif");
	
	
	m_flash.Load(cPath);
	m_flash.Draw();
	m_flash.Invalidate();





	//Sleep(1000*1);


	memset(cPath,0,1024*sizeof(wchar_t));
	
	
	SHGetSpecialFolderPath(NULL, cPath, CSIDL_PERSONAL, FALSE);
	wcscat(cPath,L"\\HideDragon");
	wcscat(cPath,L"\\Downloads\\");
	wcscat(cPath,L"gif_adv.gif");
	
	
	gif_adv.Load(cPath);
	gif_adv.Draw();


// 	Sleep(1000*1);
	
	
	memset(cPath,0,1024*sizeof(wchar_t));
	
	
	SHGetSpecialFolderPath(NULL, cPath, CSIDL_PERSONAL, FALSE);
	wcscat(cPath,L"\\HideDragon");
	wcscat(cPath,L"\\Downloads\\");
	wcscat(cPath,L"gif_bk.gif");
	
	
	gif_bk.Load(cPath);
	gif_bk.Draw();


// 	Sleep(1000*2);


	memset(cPath,0,1024*sizeof(wchar_t));
	
	
	SHGetSpecialFolderPath(NULL, cPath, CSIDL_PERSONAL, FALSE);
	wcscat(cPath,L"\\HideDragon");
	wcscat(cPath,L"\\Downloads\\");
	wcscat(cPath,L"gif_clean.gif");
	
	
	gif_clean.Load(cPath);
	gif_clean.Draw();



// 	Sleep(1000*1);
	memset(cPath,0,1024*sizeof(wchar_t));
	
	
	SHGetSpecialFolderPath(NULL, cPath, CSIDL_PERSONAL, FALSE);
	wcscat(cPath,L"\\HideDragon");
	wcscat(cPath,L"\\Downloads\\");
	wcscat(cPath,L"gif_gen.gif");
	
	
	gif_gen.Load(cPath);
	gif_gen.Draw();



	
	memset(cPath,0,1024*sizeof(wchar_t));
	
	
	SHGetSpecialFolderPath(NULL, cPath, CSIDL_PERSONAL, FALSE);
	wcscat(cPath,L"\\HideDragon");
	wcscat(cPath,L"\\Downloads\\");
	wcscat(cPath,L"gif_tray.gif");
	
	
	gif_tray.Load(cPath);
	gif_tray.Draw();


// 	memset(cPath,0,1024*sizeof(wchar_t));
// 	
// 	
// 	SHGetSpecialFolderPath(NULL, cPath, CSIDL_PERSONAL, FALSE);
// 	wcscat(cPath,L"\\HideDragon");
// 	wcscat(cPath,L"\\Downloads\\");
// 	wcscat(cPath,L"gif_web.gif");
// 	
// 	
// 	gif_web.Load(cPath);
	//gif_web.Draw();





	memset(cPath,0,1024*sizeof(wchar_t));
	
	
	SHGetSpecialFolderPath(NULL, cPath, CSIDL_PERSONAL, FALSE);
	wcscat(cPath,L"\\HideDragon");
	wcscat(cPath,L"\\Downloads\\");
	wcscat(cPath,L"gif_ah.gif");
	
	
	gif_ah.Load(cPath);
	gif_ah.Draw();


	int i = 0;
	for(i = 0;i < 8;i ++)
	{
		if(i != 6)
		PostMessage(g_TabWnd[i],WM_SIZE,NULL,NULL);




		InvalidateRect(m_flash.m_hWnd,NULL,FALSE);
		InvalidateRect(gif_adv.m_hWnd,NULL,FALSE);
		InvalidateRect(gif_bk.m_hWnd,NULL,FALSE);
		InvalidateRect(gif_clean.m_hWnd,NULL,FALSE);
		InvalidateRect(gif_gen.m_hWnd,NULL,FALSE);
		InvalidateRect(gif_tray.m_hWnd,NULL,FALSE);
		InvalidateRect(gif_ah.m_hWnd,NULL,FALSE);
	}
	


	return TRUE;
}


BOOL IfNotExitDbCopy()
{
	wchar_t cDestDir[MAX_PATH] = L"";
	
	SHGetSpecialFolderPathW(NULL, cDestDir, CSIDL_PERSONAL, FALSE);
	wcscat(cDestDir,L"\\HideDragon");
	CreateDirectoryW(cDestDir,NULL);
	
	wcscat(cDestDir,L"\\downloads\\");
	wcscat(cDestDir,L"giflink.db");
	BOOL bRet = PathFileExistsW(cDestDir);

	if(bRet == TRUE)
	{
		return FALSE;
	}


	
	CopyFile(L"giflink.db",cDestDir,FALSE);



	return TRUE;
}



BOOL ReleaseUrlArray(URL_Array & urlArray)
{
	int i = 0;

	int iCount = urlArray.size();

	for(i = 0;i < iCount;i ++)
	{
		sGifUrl * pUrl = *(urlArray.begin()+(iCount-i-1));
		urlArray.erase(urlArray.begin()+(iCount-i-1));

		delete pUrl;
		pUrl = NULL;
	}
	

	return TRUE;
}


BOOL ReadGifLinkToList(URL_Array & urlArray)
{
	IfNotExitDbCopy();
	CWebDB db("giflink.db");
	
	
	if(db.Init() == 0)
	{
		return FALSE;
	}
	


	int iCount = db.GetCount();
	
	if(iCount > 0)
	{
		ReleaseUrlArray(urlArray);
	}
	int i = 0;
	for(i = 0;i < iCount;i ++)
	{
		sGifUrl * pUrl = new sGifUrl;
		memset(pUrl->cName,0,sizeof(wchar_t)*1024);
		
		memset(pUrl->cUrl,0,sizeof(wchar_t)*1024);


		db.GetWndTextLink(i,pUrl->cName,pUrl->cUrl);
		

		urlArray.push_back(pUrl);
	}
	
	

	return TRUE;
}











