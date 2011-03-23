#define _WIN32_WINNT 0x501
#include "stdafx.h"
#include ".\cleardlg.h"
#include "shlobj.h"
#include "shellapi.h"
#include "shlwapi.h"
#include "stdio.h"
#include "urlhist.h"
#include "resource.h"
#include "hd.h"
#include "HyperLink.h"



#include <commctrl.h>




//#include "TCHAR.H"






#pragma   comment(lib,"shlwapi.lib") 
#define TVM_GETITEMSTATE (TV_FIRST + 39)
#define IDC_HAND            MAKEINTRESOURCE(32649)

HANDLE CClearDlg::hThread=0;
HWND CClearDlg::hThreadWnd=0;
HWND CClearDlg::hMainWnd=0;
HWND CClearDlg::hTreeWnd=0;
HCURSOR CClearDlg::g_hCursorLink=0;
HCURSOR CClearDlg::g_hCursorLink_org=0;
HINSTANCE CClearDlg::hInst=0;								// current instance




extern HWND g_TabWnd[10];
extern int g_iUseLanguage;
extern OptionFile sOP;
extern HWND g_hCleanStatus;

extern int g_UseIcon;


int g_ExitDoWork = 0;

#define CheckThreadExit\
	if(g_ExitDoWork == 1)\
	{\
		CloseHandle(hThread);\
		hThread = NULL;\
		MessageBox(NULL,L"DDDD",L"DDD",MB_OK);\
		InitMainDlg((HWND)hMainWnd);\
		return 0;\
	}




CClearDlg::CClearDlg(void)
{
//hThread=0;
//hThreadWnd=0;
//hMainWnd=0;
//hTreeWnd=0;
//g_hCursorLink=0;
}

CClearDlg::~CClearDlg(void)
{
}

LRESULT CALLBACK CClearDlg::ThreadDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD wmId,wmEvent;
	DWORD code=0;
	switch (message)
	{
		case WM_INITDIALOG:
			if(g_iUseLanguage == 1)
			{
				SetDialogTextW(&hDlg,IDD_DIALOG_CLEANPROGRESS);
			}

			return TRUE;
			break;
		case WM_DESTROY:
			DestroyWindow(hThreadWnd);
			break;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			switch (wmId)
			{
				case IDC_PROGRESS_CANCEL:
					EndDialog(hDlg, IDCANCEL);
					if(hThread)
					{
						g_ExitDoWork = 1;
//						GetExitCodeThread((HANDLE)hThread,&code);
//						::TerminateThread((HANDLE)hThread,code);
						CloseHandle(hThread);
						hThread = NULL;
					}
					return TRUE;
			break;
			}
		break;
	}
    return FALSE;
}


HBRUSH g_brushCleanPage;

CHyperLink m_lnk_cleanpri;




 LRESULT CALLBACK CClearDlg::MainDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
			DWORD wmId,wmEvent;
			DWORD dwThreadId=0, dwThrdParam = 1; 
			hTreeWnd=::GetDlgItem((HWND)hMainWnd,IDC_CLEAN_TREE);
			HWND hCleanAds = NULL;
			wchar_t * cDefaultText = NULL;
			wchar_t * cDefaultLink = NULL;
	switch (message)
	{
		case WM_INITDIALOG:
			g_brushCleanPage = CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));

			InitMainDlg(hDlg);
			if(sOP.bCleanAfterHide == 1)
			{
				CheckDlgButton(hDlg,IDC_CLEAN_CLEANAFTER,BST_CHECKED);
			}
			if(g_iUseLanguage == 1)
			{
				SetDialogTextW(&hDlg,IDD_TAB_CLEANPRIV);
			}


			hCleanAds = GetDlgItem(hDlg,IDC_CLEANPRIV_ADS);

#ifndef _NOADS
			m_lnk_cleanpri.Attach(hCleanAds);
			cDefaultText = new wchar_t[1024];
			memset(cDefaultText,0,1024*sizeof(wchar_t));
			LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");

			cDefaultLink = new wchar_t[1024];
			memset(cDefaultLink,0,1024*sizeof(wchar_t));
			
			LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.hidedragon.com/cn/union.html");
			

			m_lnk_cleanpri.SetLink(cDefaultText,cDefaultLink, TRUE);


			delete [] cDefaultText;
			cDefaultText = NULL;
			delete [] cDefaultLink;
			cDefaultLink = NULL;
#endif			
#ifdef _NOADS
			ShowWindow(hCleanAds,SW_HIDE);
#endif			
			{
				HCURSOR hCursor = ::LoadCursor(NULL,IDC_HAND);
				g_hCursorLink=CopyCursor(hCursor);

				
				g_hCursorLink_org=CopyCursor(::LoadCursor(NULL,IDC_ARROW));
				if(!(DWORD)g_hCursorLink_org)
				{
					int i=::GetLastError();
					i=i;
				}
			}



			if(g_UseIcon == 1)
			{
				HWND hCleanNow = GetDlgItem(hDlg,IDC_CLEAN_CLEANNOW);
				
				
				HICON hI3 = (struct HICON__*)LoadImage(
					hInst,MAKEINTRESOURCE(IDI_CLEAN_CLEANNOW),IMAGE_ICON,16,16,NULL);
				
				
				
				SendMessage(hCleanNow,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
				
				DestroyIcon(hI3);
				
				
				
				RECT rtM;
				rtM.left = 0;
				rtM.right = 20;
				rtM.top = 0;
				rtM.bottom = 00;
				Button_SetTextMargin(hCleanNow,&rtM);
				
			}


			
			break;
	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)(wParam),GetSysColor(CTLCOLOR_DLG));
		return (LRESULT)g_brushCleanPage;
	case WM_CTLCOLORDLG:
		return (LRESULT)g_brushCleanPage;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			switch (wmId)
				{
					case IDOK:
					case IDCANCEL:
						EndDialog(hDlg, LOWORD(wParam));
						return TRUE;
						break;
					case IDC_CLEAN_CLEANNOW://前台处理
						if(hTreeWnd)
						{
							::ShowWindow((HWND)hThreadWnd,SW_SHOW);
							SendDlgItemMessage(GetDlgItem((HWND)hMainWnd,
								IDC_PROGRESS_PROGRESS),IDC_PROGRESS_PROGRESS,PBM_SETPOS,0,0);
							hThread =(void*) CreateThread(NULL,0,DoWork,&dwThrdParam,0,&dwThreadId);
						}
						break;
					case IDC_CLEAN_CLEANAFTER:
						if(IsDlgButtonChecked(hDlg,IDC_CLEAN_CLEANAFTER) == BST_CHECKED)
						{
							sOP.bCleanAfterHide = 1;
						}
						else
						{
							sOP.bCleanAfterHide = 0;
						}
						WriteOptionFile(sOP);

						break;
					case IDC_CLEAN_BK://后台处理
						if(hThread == NULL)
						{
							dwThrdParam = 0;
							hThread =(void*) CreateThread(NULL,0,DoWork,
								NULL,0,&dwThreadId);
						}
						break;
			}
			break;

		case WM_CLOSE: 
			DestroyWindow(hDlg); 
			return TRUE;
			break;
		case WM_DESTROY: 
			DestroyIcon(g_hCursorLink_org);
			DestroyIcon(g_hCursorLink);
			DeleteObject(g_brushCleanPage);
			DestroyWindow(hThreadWnd);
			DestroyWindow(hDlg); 
			return TRUE;
			break;
		case WM_SIZE:
			{
				HWND hTree = GetDlgItem(hDlg,IDC_CLEAN_TREE);
				HWND hSelAll = GetDlgItem(hDlg,IDC_CLEAN_SELALL);
				HWND hUnselAll = GetDlgItem(hDlg,IDC_CLEAN_UNSELALL);
				HWND hCleanNow = GetDlgItem(hDlg,IDC_CLEAN_CLEANNOW);
				HWND hCleanAfter = GetDlgItem(hDlg,IDC_CLEAN_CLEANAFTER);
				HWND hCleanTip = GetDlgItem(hDlg,IDC_CLEANPRIV_TIP);
				hCleanAds = GetDlgItem(hDlg,IDC_CLEANPRIV_ADS);
				
				RECT pageRect;
				GetWindowRect(hDlg,&pageRect);
				SetWindowPos(hTree,NULL,10,35,
					pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-99,SWP_NOZORDER);




				RECT selRect;
				GetWindowRect(hSelAll,&selRect);
				

				
				LONG selLen = selRect.right - selRect.left;
				SetWindowPos(hSelAll,NULL,10,pageRect.bottom-pageRect.top-45,
					0,0,SWP_NOZORDER|SWP_NOSIZE);
				SetWindowPos(hUnselAll,NULL,20 + selLen,pageRect.bottom-pageRect.top-45,
					0,0,SWP_NOZORDER|SWP_NOSIZE);

				SetWindowPos(hCleanTip,NULL,10,5,
				pageRect.right-pageRect.left - 20,30,SWP_NOZORDER);

				

				





				LONG lWidth = pageRect.right-pageRect.left;



				RECT refreshRect;
				GetWindowRect(hCleanNow,&refreshRect);
				LONG refreshLen = refreshRect.right - refreshRect.left;
				SetWindowPos(hCleanNow,NULL,pageRect.right-pageRect.left-refreshLen-9,
					pageRect.bottom-pageRect.top-50,0,0,SWP_NOZORDER|SWP_NOSIZE);

				SetWindowPos(hCleanAfter,NULL,10,
					pageRect.bottom-pageRect.top-23,0,0,SWP_NOZORDER|SWP_NOSIZE);




				RECT adlinkRect;
				GetWindowRect(hCleanAds,&adlinkRect);
				
				LONG adlinkLen = adlinkRect.right - adlinkRect.left;
				SetWindowPos(hCleanAds,NULL,pageRect.right-pageRect.left-refreshLen-139,
					pageRect.bottom-pageRect.top-23,pageRect.right-pageRect.left - (30 + 50),
					20,SWP_NOZORDER);
			}



			break;
		case WM_MOUSEMOVE:
			{
				RECT rc,rc2;
				::GetWindowRect(::GetDlgItem((HWND)hMainWnd,IDC_CLEAN_SELALL),&rc);
				::GetWindowRect(::GetDlgItem((HWND)hMainWnd,IDC_CLEAN_UNSELALL),&rc2);
				POINT pt;
				pt.x   =   LOWORD(lParam);     //   horizontal   position   of   cursor     
				pt.y   =   HIWORD(lParam);     //   vertical   position   of   cursor  
				::ClientToScreen((HWND)hMainWnd,&pt);
				if(PtInRect(&rc,pt)||PtInRect(&rc2,pt))
				{
					if(!g_hCursorLink)
					{
						int i=::GetLastError();
						i=i;
					}
					::SetCursor((HCURSOR)g_hCursorLink);
				}
				else
				{
					::SetCursor((HCURSOR)g_hCursorLink_org);
				}
			}
			break;
//		case WM_SETCURSOR:
//				if(g_hCursorLink)
//				{
//				::SetCursor((HCURSOR)g_hCursorLink);
//				}
//				return true;
//			break;
		case WM_LBUTTONDOWN:
			{
				RECT rc,rc2;
				::GetWindowRect(::GetDlgItem((HWND)hMainWnd,IDC_CLEAN_SELALL),&rc);
				::GetWindowRect(::GetDlgItem((HWND)hMainWnd,IDC_CLEAN_UNSELALL),&rc2);
				POINT pt;
				pt.x   =   LOWORD(lParam);     //   horizontal   position   of   cursor     
				pt.y   =   HIWORD(lParam);     //   vertical   position   of   cursor  
				::ClientToScreen((HWND)hMainWnd,&pt);
				if(PtInRect(&rc,pt))
				{//全选
					HTREEITEM h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,
						IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_ROOT,0);
					if(!h) return false;

					do
					{
						TVITEM item;
						item.mask = TVIF_HANDLE | TVIF_STATE;
						item.hItem = h;
						item.stateMask = TVIS_STATEIMAGEMASK;
						item.state = INDEXTOSTATEIMAGEMASK(2);
						SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_SETITEM,0,
							(LPARAM)&item);
						h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,
							TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
					}while(h);
				}
				if(PtInRect(&rc2,pt))
				{//全不选
					HTREEITEM h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,
						TVM_GETNEXTITEM,TVGN_ROOT,0);
					if(!h) return false;

					do
					{
						TVITEM item;
						item.mask = TVIF_HANDLE | TVIF_STATE;
						item.hItem = h;
						item.stateMask = TVIS_STATEIMAGEMASK;
						item.state = INDEXTOSTATEIMAGEMASK(1);
						SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_SETITEM,0,
							(LPARAM)&item);
						h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,
							TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
					}while(h);
				}
			}
		case WM_NOTIFY:			
			{
				DWORD idCtrl=wParam;
				if(IDC_CLEAN_TREE==idCtrl)
				{
				NMHDR  nItem=*(LPNMHDR)lParam;
					if(NM_CLICK == nItem.code)
					{
						POINT pt;   
						::GetCursorPos(&pt);   
						UINT   uFlags=0;   
						RECT   rect;   
						GetWindowRect((HWND)hTreeWnd,&rect);   
						TVHITTESTINFO info;
						info.pt.x   =   pt.x   -   rect.left   ;   
						info.pt.y   =   pt.y   -   rect.top   ;
						
						HTREEITEM   hItem=(HTREEITEM)::SendMessage((HWND)hTreeWnd,TVM_HITTEST,
							0,(LPARAM)(LPTVHITTESTINFO)&info);
						if(info.flags&TVHT_ONITEMSTATEICON)
						{
							if(hItem)
							{
								wchar_t buf[255];
								::ZeroMemory(buf,sizeof(buf));

								GetTreeItemText(::GetDlgItem((HWND)hMainWnd,IDC_CLEAN_TREE),
									buf,sizeof(buf),hItem);
								wchar_t pText[1024] = L"";
								
								if(0==wcscmp(buf,LoadItemStringW(L"IDS_CLEANPRIV_IEADDRESSEDIT",
									pText,L"IE地址栏下拉列表（IE重启后生效）")))
								{//子项目全选或全不选
									BOOL IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,
										IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)hItem,
										TVIS_STATEIMAGEMASK)>>12)-1);
									if(!IsDo)
									{//子项目全选
					HTREEITEM ch=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,
						TVM_GETNEXTITEM,TVGN_CHILD,(LPARAM)hItem);
					while(ch)
					{
						TVITEM item;
						item.mask = TVIF_HANDLE | TVIF_STATE;
						item.hItem = ch;
						item.stateMask = TVIS_STATEIMAGEMASK;
						item.state = INDEXTOSTATEIMAGEMASK(2);
						SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_SETITEM,0,
							(LPARAM)&item);
					ch=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,
						TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)ch);
					}
									}
									else
									{//子项目全不选
					HTREEITEM ch=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,
						IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_CHILD,(LPARAM)hItem);
					while(ch)
					{
						TVITEM item;
						item.mask = TVIF_HANDLE | TVIF_STATE;
						item.hItem = ch;
						item.stateMask = TVIS_STATEIMAGEMASK;
						item.state = INDEXTOSTATEIMAGEMASK(1);
						SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,
							TVM_SETITEM,0,(LPARAM)&item);
					ch=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,
						IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)ch);
					}

									}
								}
							}
						}
					}
				}
				break;
			}
	}
	return false;
}

 void CClearDlg::InitMainDlg(HWND	hWnd)
{
	//建立进度对话框，但不显示出来
	 g_ExitDoWork = 0;
	if(!(DWORD)hThreadWnd)		hThreadWnd=::CreateDialog(hInst,(LPCTSTR)IDD_DIALOG_CLEANPROGRESS
		,(HWND)hWnd,(DLGPROC)ThreadDlgProc);

	g_hCleanStatus = hThreadWnd;

/*******************初始化Tree控件***************/

	HWND hTreeWnd = GetDlgItem(hWnd,IDC_CLEAN_TREE);
	DWORD dwStyle = ::GetWindowLong(hTreeWnd, GWL_STYLE);
	DWORD dwNewStyle = (dwStyle & ~TVS_CHECKBOXES);
	::SetWindowLong(hTreeWnd,GWL_STYLE,dwNewStyle);
	 dwNewStyle =dwNewStyle |  TVS_CHECKBOXES;
	::SetWindowLong(hTreeWnd,GWL_STYLE,dwNewStyle);
	
	
	
	
	wchar_t * pText = new wchar_t[1024];
	memset(pText,0,1024 * sizeof(wchar_t));

	::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_DELETEITEM,0,0xffff0000);
	TVITEM item;
	::ZeroMemory(&item,sizeof(TVITEM));
	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_CACHEWEBFILE",pText,L"上网保存在缓冲中的网页文件");
	item.mask = TVIF_TEXT|TVIF_HANDLE|TVIF_STATE;
	item.stateMask = TVIS_STATEIMAGEMASK;
	item.state = INDEXTOSTATEIMAGEMASK(2);
 	TVINSERTSTRUCT lvi;
	::ZeroMemory(&lvi,sizeof(LPTVINSERTSTRUCT));
	lvi.hInsertAfter=TVI_LAST;
	lvi.hParent=NULL;
	lvi.item=item;
	LRESULT parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_WEBHISTORY",pText,L"上网产生的已访问过的网页历史记录");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_IEPASSWORD",pText,L"IE自动保存的密码");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_IEFORM",pText,L"IE自动完成表单资料");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_MYIE2",pText,L"MyIE2自动使用痕迹");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);
	
	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_WINDOWSTEMPDIR",pText,L"Windows临时文件夹");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_STARTMENUDOC",pText,L"开始菜单中的文档记录");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_NETWORKPLACE",pText,L"访问网上邻居的记录（对XP有效）");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_RUNCOMMAND",pText,L"运行框里的命令（重启后有效）");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_SYSTEMCLIPBOARD",pText,L"系统剪贴板");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_WINDOWSSEARCHHISTORY",pText,L"Windows搜索记录");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_OPENFILEHISTORY",pText,L"打开文件跟踪记录");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_CLEANPREREADFILE",pText,L"清理预读文件");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);


	
	::ZeroMemory(&item,sizeof(TVITEM));
	item.mask = TVIF_TEXT;
	
	
	
	
	
	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_IEADDRESSEDIT",
		pText,L"IE地址栏下拉列表（IE重启后生效）");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);
	//读注册表,加子项目
	HKEY MyKey;
	RegOpenKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Internet Explorer\\TypedURLs",&MyKey);
	wchar_t buf[255],UrlBuf[255];
	if(MyKey)
	{
		DWORD size_temp=sizeof(buf);
		DWORD i=0;
		LONG result;
		do
		{
		::ZeroMemory(buf,size_temp);
		::ZeroMemory(UrlBuf,size_temp);
		DWORD regsz=REG_SZ;
		size_temp=sizeof(buf);
		result=::RegEnumValue(MyKey,i,buf,&size_temp,NULL,NULL,NULL,NULL);
		//result=::RegEnumValue(MyKey,i,buf,&size_temp,NULL,&regsz,(LPBYTE)UrlBuf,&size_temp);
		size_temp=sizeof(buf);
		::RegQueryValueEx(MyKey,buf,NULL,&regsz,(LPBYTE)UrlBuf,&size_temp);
		if(wcslen(UrlBuf)>0)
		{
			item.pszText=UrlBuf;
			lvi.item=item;
			lvi.hParent=(HTREEITEM)parent;
			SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);
		}
		i++;
		}while(ERROR_SUCCESS==result);
	}

	::RegCloseKey(MyKey);
	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_WINDOWSMEDIAPLAYER",
		pText,L"Windows媒体播放器");
	lvi.item=item;
	lvi.hParent=NULL;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_REALONE",pText,L"Realone/Realplayer");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_POWERDVD",pText,L"PowerDVD");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_COOKIES",pText,
		L"上网产生的Cookies");L"上网产生的Cookies";
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_WORD",pText,L"Microsoft Word");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_EXCEL",pText,L"Microsoft Excel");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_POWERPOINT",pText,L"Microsoft Powpoint");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_ACCESS",pText,
		L"Microsoft Access");L"Microsoft Access";
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_WORDPAD",pText,L"Windows 写字板");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_OUTLOOKTEMPDIR",
		pText,L"Microsoft Outlook 临时文件夹");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_OFFICE",pText,
		L"Microsoft Office");L"Microsoft Office";
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_ULTRAEDIT",pText,L"UltraEdit");L"UltraEdit";
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_WINZIP",pText,L"WinZip");L"WinZip";
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_EDITPLUS",pText,L"EditPlus");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	item.pszText=LoadItemStringW(L"IDS_CLEANPRIV_WINRAR",pText,L"WinRar");
	lvi.item=item;
	parent=::SendDlgItemMessage(hWnd,IDC_CLEAN_TREE,TVM_INSERTITEM,0,(LPARAM)&lvi);

	delete [] pText;
	pText = NULL;

/*************初始化TREE控件结束********************/
//				{//全选
//					HTREEITEM h=(HTREEITEM)SendDlgItemMessage((HWND)hWnd,
//						IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_ROOT,0);
//					if(!h) return ;
//				int i = 0;
//				do
//				{
//						TVITEM item1;
//						item1.mask = TVIF_HANDLE | TVIF_STATE;
//						item1.hItem = h;
//						item1.stateMask = TVIS_STATEIMAGEMASK;
//						item1.state = INDEXTOSTATEIMAGEMASK(2);
//						if(IsInCheckedArray(i))
//						{
//							SendDlgItemMessage((HWND)hWnd,IDC_CLEAN_TREE,TVM_SETITEM,0,
//								(LPARAM)&item1);
//						}
//						h=(HTREEITEM)SendDlgItemMessage((HWND)hWnd,IDC_CLEAN_TREE,
//							TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
//						i ++;
//					}while(h);
//				}

}




int g_TreeCheckedArray[] = {0,1,3,4,5,6,7};
int iArrayCount = 7;






int IsInCheckedArray(int idx)
{
	int i = 0;
	for(i = 0;i < iArrayCount;i ++)
	{
		if(g_TreeCheckedArray[i] == idx)
		{
			return 1;
		}
	}



	return 0;
}


extern HWND g_hGenWnd;
extern HWND g_hAdvWnd;


BOOL CClearDlg::NotSearchIP()
{
	if(g_hGenWnd == NULL || g_hAdvWnd == NULL)
	{
		return FALSE;
	}
	HWND hBtn_Search = GetDlgItem(g_hGenWnd,IDC_HIDEIP_HIDENOW);
	HWND hBtn_Adv_Search = GetDlgItem(g_hAdvWnd,IDC_HIDEIP_SEARCHNOW);

	if(IsWindowEnabled(hBtn_Search) == FALSE || 
		IsWindowEnabled(hBtn_Adv_Search) == FALSE)
	{
		return FALSE;
	}


	return TRUE;
}



DWORD WINAPI CClearDlg::DoWork(LPVOID pParam)
{
	CClearDlg * pDlg = (CClearDlg * )pParam;
	HWND TreeWnd=::GetDlgItem((HWND)hMainWnd,IDC_CLEAN_TREE);
	wchar_t buf[255];
	wchar_t buf_temp[255];
	wchar_t pText[1024];

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	HTREEITEM h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_ROOT,0);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	BOOL IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//正在清除上网保存在缓冲中的网页文件...
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_TEMPWEB",
			pText,L"IDS_CLEANING_TEMPWEB"));
		::ZeroMemory(buf,sizeof(buf));
		::SHGetSpecialFolderPath((HWND)hMainWnd,buf,32,false);
		EmptyDirectory(buf);
	}

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText((HWND)TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//上网产生的已访问过的网页历史记录
		memset(pText,0,1024*sizeof(wchar_t));

		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_WEBHISTORY",
			pText,L"CLEANINGWEBHISTORY"));

	HRESULT hr;
	IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
	if(CoInitialize(NULL)!=0)
	{
		HDMessageBoxW(L"IDS_CLEANPRIV_INITCOMFAILURE",L"INITCOMFAILURE");
		return 0;
		
	}

		hr = CoCreateInstance(CLSID_CUrlHistory, NULL, 
			CLSCTX_INPROC_SERVER, IID_IUrlHistoryStg2, 
			(void**)&pUrlHistoryStg2);
		int i=::GetLastError();
		if (SUCCEEDED(hr))
		{
			hr = pUrlHistoryStg2->ClearHistory(); 
			pUrlHistoryStg2->Release();
		}
		::ZeroMemory(buf,sizeof(buf));
		::SHGetSpecialFolderPath((HWND)hMainWnd,buf,34,false);
		EmptyDirectory(buf);
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//IE自动保存的密码
		memset(pText,0,1024*sizeof(wchar_t));

		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_IEPASSWORD",
			pText,L"CLEANINGIEPASSWORD"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Internet Explorer",L"IntelliForms");
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//IE自动完成表单资料
		//待做
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_IEFORM",
			pText,L"CLEANINGIEFORM"));
		
		DWORD result=::SHDeleteKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Internet Explorer\\IntelliForms");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Internet Explorer",L"IntelliForms");
		ClearForm();
	}
	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//MyIE2自动使用痕迹
		//待做
		int i=0;
	}
	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo && NotSearchIP())
	{//Windows临时文件夹
		//待启用

		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_WINTEMPFOLDER",
			pText,L"CLEANING_WINTEMPFOLDER"));
		
		::ZeroMemory(buf,255 * sizeof(wchar_t));
		GetTempPath(255,buf);
		if('\\'==buf[wcslen(buf)-1]) buf[wcslen(buf)-1]=0;//最后一个"\\"改为0
		EmptyDirectory(buf);
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//开始菜单中的文档记录
		//待启用
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_STARTMENUDOC",
			pText,L"IDS_CLEANING_STARTMENUDOC"));
		::ZeroMemory(buf,sizeof(buf));
		::SHGetSpecialFolderPath((HWND)hMainWnd,buf,8,false);
		EmptyDirectory(buf);
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//访问网上邻居的记录（对XP有效）
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_NETWORKPLACE",
			pText,L"IDS_CLEANING_NETWORKPLACE"));
		::ZeroMemory(buf,sizeof(buf));
		::SHGetSpecialFolderPath((HWND)hMainWnd,buf,19,false);
		EmptyDirectory(buf);
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//运行框里的命令（重启后有效）
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_RUNDIALOG",
			pText,L"IDS_CLEANING_RUNDIALOG"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer",L"RunMRU");
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//系统剪贴板
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_CLIPBOARD",
			pText,L"IDS_CLEANING_CLIPBOARD"));
	
		::OpenClipboard((HWND)hMainWnd);
		::EmptyClipboard();
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Windows搜索记录
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_WINSEARCH",
			pText,L"IDS_CLEANING_WINSEARCH"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Search Assistant\\ACMru",L"5603");
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//打开文件跟踪记录
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_OPENFILE",
			pText,L"IDS_CLEANING_OPENFILE"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell",L"MuiCache");
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);

	if(IsDo)
	{//清理预读文件
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_PREREADFILE",
			pText,L"IDS_CLEANING_PREREADFILE"));
		
		::ZeroMemory(buf,sizeof(buf));
		::GetWindowsDirectory(buf,sizeof(buf));
		wcscat(buf,L"\\Prefetch");
		EmptyDirectory(buf);
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//IE地址栏下拉列表（IE重启后生效）
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_IEADDRESSBAR",
			pText,L"IDS_CLEANING_IEADDRESSBAR"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Internet Explorer",L"TypedURLs");
	}
	else
	{
		HKEY MyKey;
		RegOpenKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Internet Explorer\\TypedURLs",&MyKey);
		HTREEITEM ch=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_CHILD,(LPARAM)h);
		int i=0;
		LONG result=0;
		do
		{
		wchar_t buf[255];
		DWORD size_temp=sizeof(buf);
		::ZeroMemory(buf,size_temp);
		DWORD regsz=REG_SZ;
		size_temp=sizeof(buf);
		result=::RegEnumValue(MyKey,i++,buf,&size_temp,NULL,NULL,NULL,NULL);
		if(wcslen(buf)<=0) continue;
		IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)ch,TVIS_STATEIMAGEMASK)>>12)-1);
		if(IsDo)
		{
			memset(pText,0,1024*sizeof(wchar_t));
			::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_IEADDRESSBAR",
				pText,L"IDS_CLEANING_IEADDRESSBAR"));
			
			DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Internet Explorer\\TypedURLs",buf);
		}
		ch=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)ch);
		}while(ERROR_SUCCESS==result);
		::RegCloseKey(MyKey);
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Windows媒体播放器
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_MEDIALPLAYER",
			pText,L"IDS_CLEANING_MEDIALPLAYER"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\MediaPlayer\\Player",L"RecentFileList");
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Realone/Realplayer
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_REALPLAYER",
			pText,L"IDS_CLEANING_REALPLAYER"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\RealNetworks\\RealPlayer\\6.0\\Preferences",L"MostRecentClips1");
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//PowerDVD
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_POWERDVD",
			pText,L"IDS_CLEANING_POWERDVD"));
		
		::ZeroMemory(buf,sizeof(buf));
		::SHGetSpecialFolderPath((HWND)hMainWnd,buf,5,false);
		::wcscat(buf,L"\\CyberLink\\PowerDVD\\Default.PLS");
		DeleteRegValue(HKEY_CURRENT_USER,L"SOFTWARE\\CyberLink\\PowerDVD",L"UIInitDir");
		::DeleteFile(buf);
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//上网产生的Cookies
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_COOKIES",
			pText,L"IDS_CLEANING_COOKIES"));
		
		::ZeroMemory(buf,sizeof(buf));
		::SHGetSpecialFolderPath((HWND)hMainWnd,buf,33,false);
		EmptyDirectory(buf);
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_COOKIES",
			pText,L"IDS_CLEANING_COOKIES"));
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Microsoft Word
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_WORD",
			pText,L"IDS_CLEANING_WORD"));
		
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\9.0\\Word\\Data",L"Settings");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\8.0\\Word\\Data",L"Settings");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\10.0\\Word\\Data",L"Settings");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\11.0\\Word\\Data",L"Settings");
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Word",L"File MRU");
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Microsoft Excel
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_EXCEL",
			pText,L"IDS_CLEANING_EXCEL"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\8.0\\Excel",L"Recent Files");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\9.0\\Excel",L"Recent Files");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\10.0\\Excel",L"Recent Files");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\11.0\\Excel",L"Recent Files");

		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Excel",L"File MRU");
	}


	CheckThreadExit


	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Microsoft Powpoint
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_POWERPOINT",
			pText,L"IDS_CLEANING_POWERPOINT"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\8.0\\PowerPoint",L"Recent File List");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\9.0\\PowerPoint",L"Recent File List");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\10.0\\PowerPoint",L"Recent File List");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\11.0\\PowerPoint",L"Recent File List");


		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\PowerPoint",L"File MRU");
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Microsoft Access
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_ACCESS",
			pText,L"IDS_CLEANING_ACCESS"));
		
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\8.0\\Access\\Settings",L"MRU1");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\8.0\\Access\\Settings",L"MRU2");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\8.0\\Access\\Settings",L"MRU3");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\8.0\\Access\\Settings",L"MRU4");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\9.0\\Access\\Settings",L"MRU1");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\9.0\\Access\\Settings",L"MRU2");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\9.0\\Access\\Settings",L"MRU3");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\9.0\\Access\\Settings",L"MRU4");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\10.0\\Access\\Settings",L"MRU1");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\10.0\\Access\\Settings",L"MRU2");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\10.0\\Access\\Settings",L"MRU3");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\10.0\\Access\\Settings",L"MRU4");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\11.0\\Access\\Settings",L"MRU1");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\11.0\\Access\\Settings",L"MRU2");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\11.0\\Access\\Settings",L"MRU3");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\11.0\\Access\\Settings",L"MRU4");
		
		


		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU1");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU2");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU3");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU4");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU4");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU5");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU6");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU7");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU8");
		DeleteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\12.0\\Access\\Settings",L"MRU9");


	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Windows 写字板
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_WORDPAD",
			pText,L"IDS_CLEANING_WORDPAD"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Wordpad",L"Recent File List");
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Microsoft Outlook 临时文件夹
	//待做
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_OUTLOOKTEMPFOLDER",
			pText,L"IDS_CLEANING_OUTLOOKTEMPFOLDER"));
		
		//EmptyOutlookTempFolder();

		HKEY MyKey;
		RegOpenKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Office\\11.0\\Outlook\\Security",&MyKey);
		wchar_t buf[255] = L"",UrlBuf[255] = L"";
		if(MyKey)
		{
			DWORD size_temp=sizeof(buf);
			
			::ZeroMemory(buf,size_temp);
			::ZeroMemory(UrlBuf,size_temp);
			DWORD regsz=REG_SZ;
			size_temp=sizeof(buf);

			wcscpy(buf,L"OutlookSecureTempFolder");
			::RegQueryValueEx(MyKey,buf,NULL,&regsz,(LPBYTE)UrlBuf,&size_temp);
			if(wcslen(UrlBuf)>0)
			{
				EmptyDirectory(UrlBuf);
			}
		}
		
		::RegCloseKey(MyKey);
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//Microsoft Office
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_OFFICETEMPFOLDER",
			pText,L"IDS_CLEANING_OFFICETEMPFOLDER"));
		
		::ZeroMemory(buf,sizeof(buf));
		::SHGetSpecialFolderPath((HWND)hMainWnd,buf,26,false);
		::wcscat(buf,L"\\Microsoft\\Office\\Recent");
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_OFFICEHISTORY",
			pText,L"IDS_CLEANING_OFFICEHISTORY"));
		
		EmptyDirectory(buf);
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//UltraEdit
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_ULTRAEDIT",
			pText,L"IDS_CLEANING_ULTRAEDIT"));
	::ZeroMemory(buf,sizeof(buf));
	::SHGetSpecialFolderPath((HWND)hMainWnd,buf,26,false);
	::wcscat(buf,L"\\IDMComp\\UltraEdit\\Uedit32.INI");
	WritePrivateProfileString(L"Recent File List",L"File1",NULL,buf);
	WritePrivateProfileString(L"Recent File List",L"File2",NULL,buf);
	WritePrivateProfileString(L"Recent File List",L"File3",NULL,buf);
	WritePrivateProfileString(L"Recent File List",L"File4",NULL,buf);
	WritePrivateProfileString(L"Settings",L"Last File",NULL,buf);
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//WinZip
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_WINZIP",
			pText,L"IDS_CLEANING_WINZIP"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Nico Mak Computing\\WinZip",L"filemenu");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\Nico Mak Computing\\WinZip",L"extract");
	}

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//EditPlus
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_EDITPLUS",
			pText,L"IDS_CLEANING_EDITPLUS"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\ES-Computing\\EditPlus 2",L"Recent File List");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\ES-Computing\\EditPlus 2",L"Recent Dir List");
	}



	CheckThreadExit

	::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	h=(HTREEITEM)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)h);
	GetTreeItemText(TreeWnd,buf_temp,255,h);
	IsDo=(BOOL)(((UINT)SendDlgItemMessage((HWND)hMainWnd,IDC_CLEAN_TREE,TVM_GETITEMSTATE,(WPARAM)h,TVIS_STATEIMAGEMASK)>>12)-1);
	if(IsDo)
	{//WinRar
		memset(pText,0,1024*sizeof(wchar_t));
		::SetDlgItemText((HWND)hThreadWnd,IDC_PROGRESS_INFO,LoadItemStringW(L"IDS_CLEANING_WINRAR",
			pText,L"IDS_CLEANING_WINRAR"));
		
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\WinRAR",L"DialogEditHistory");
		EmptyRegKey(HKEY_CURRENT_USER,L"Software\\WinRAR",L"ArcHistory");
	}



	CheckThreadExit

	//::SendDlgItemMessage((HWND)hThreadWnd,IDC_PROGRESS_PROGRESS,PBM_STEPIT,0,0);
	SendDlgItemMessage(GetDlgItem((HWND)hMainWnd,IDC_PROGRESS_PROGRESS),IDC_PROGRESS_PROGRESS,PBM_SETPOS,0,0);
	InitMainDlg((HWND)hMainWnd);
	::ShowWindow((HWND)hThreadWnd,SW_HIDE);
	CloseHandle(hThread);
	hThread = NULL;
	

	//::MessageBox(NULL,L"恭喜您，清理完成！",L"完成",MB_OK);

	if(pParam != NULL)
	{
		HDMessageBoxW(L"IDS_CLEANPRIV_CLEANSUCESS",L"CLEANFINISH");
	}
	
return 1;
}



//清除文件夹下所有内容
void CClearDlg::EmptyDirectory(const wchar_t * dir)
{
	wchar_t path[1024];
	::ZeroMemory(path,sizeof(path));
	wcscpy(path,dir);
	if(path[wcslen(path)-1]!='\\')
	{
	wcscat(path,L"\\*.*");
	}
	else
	{
	wcscat(path,L"*.*");
	}

	wchar_t buf[1024];
	WIN32_FIND_DATA dataFind;
	BOOL bMoreFiles=TRUE;
	HANDLE hFind=FindFirstFile(path,&dataFind);
	HANDLE hFind_bk = hFind;
	
	while(hFind!=INVALID_HANDLE_VALUE&&bMoreFiles==TRUE)
	{
		if (wcscmp(dataFind.cFileName, L".") == 0 ||wcscmp(dataFind.cFileName, L"..") == 0 ||
			_wcsicmp(dataFind.cFileName, L"desktop.ini") == 0||_wcsicmp(dataFind.cFileName, L"index.dat") == 0)
		{
			bMoreFiles=FindNextFile(hFind,&dataFind);
			continue;
		}
		::ZeroMemory(buf,sizeof(buf));
		wcscpy(buf,dir);
		wcscat(buf,L"\\");
		wcscat(buf,dataFind.cFileName);

		DWORD dwAttributes = GetFileAttributes(buf);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(buf, dwAttributes);
		}

		if (dataFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{//是目录则递归
			EmptyDirectory(buf);
			RemoveDirectory(buf);
		}
		else
		{//是文件
			DeleteFile(buf);
		}

		bMoreFiles=FindNextFile(hFind,&dataFind);
	}

	FindClose(hFind_bk);
		//SHFILEOPSTRUCT FileOp;
		//FileOp.fFlags = FOF_NOCONFIRMATION;
		//FileOp.hNameMappings = NULL;
		//FileOp.hwnd = NULL;
		//FileOp.lpszProgressTitle = NULL;
		//FileOp.pFrom = dir;//
		//FileOp.pTo = NULL;//
		//FileOp.wFunc = FO_DELETE;
		//SHFileOperation(&FileOp);
		//::CreateDirectory(TEXT(dir),NULL);

}
//清除表单的相关函数
 BOOL CClearDlg::IsWindows2k()
{
	BOOL bRet = FALSE;
	BOOL bOsVersionInfoEx;
	OSVERSIONINFOEX osvi;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure,
	// If that fails, try using the OSVERSIONINFO structure.
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return bRet;
	}

	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 5)
	{
		bRet = TRUE;
	}

	return bRet;
}

 BOOL CClearDlg::IsWindowsNT()
{
	BOOL bRet = FALSE;
	BOOL bOsVersionInfoEx;
	OSVERSIONINFOEX osvi;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure,
	// If that fails, try using the OSVERSIONINFO structure.
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return bRet;
	}

	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion <= 4)
	{
		bRet = TRUE;
	}

	return bRet;
}

void CClearDlg::ClearForm()
{
		wchar_t sKey[255];
		DWORD dwRet;

		if (IsWindows2k() || IsWindowsNT())//先判断系统
		{
			wchar_t sBaseKey[255];
			SECURITY_DESCRIPTOR NewSD;
			BYTE* pOldSD;
			PACL pDacl = NULL;
			PSID pSid = NULL;
			TCHAR szSid[256];
			if (GetUserSid(&pSid))
			{
				//get the hiden key name
				GetSidString(pSid, szSid);

				wcscpy(sKey,L"Software\\Microsoft\\Protected Storage System Provider\\");
				wcscat(sKey,szSid);

				//get old SD
				wcscpy(sBaseKey,sKey);
				GetOldSD(HKEY_CURRENT_USER, sBaseKey, &pOldSD);

				//set new SD and then clear
				if (CreateNewSD(pSid, &NewSD, &pDacl))
				{
					RegSetPrivilege(HKEY_CURRENT_USER, sKey, &NewSD, FALSE);

					wcscat(sKey,L"\\Data");
					RegSetPrivilege(HKEY_CURRENT_USER, sKey, &NewSD, FALSE);

					wcscat(sKey,L"\\e161255a-37c3-11d2-bcaa-00c04fd929db");
					RegSetPrivilege(HKEY_CURRENT_USER, sKey, &NewSD, TRUE);

					dwRet = SHDeleteKey(HKEY_CURRENT_USER, sKey);
				}

				if (pDacl != NULL)
					HeapFree(GetProcessHeap(), 0, pDacl);

				//restore old SD
				if (pOldSD)
				{
					RegSetPrivilege(HKEY_CURRENT_USER, sBaseKey, 
						(SECURITY_DESCRIPTOR*)pOldSD, FALSE);
					delete pOldSD;
				}
			}
			if (pSid)
				HeapFree(GetProcessHeap(), 0, pSid);
		}

		//win9x
		DWORD dwSize = MAX_PATH;
		TCHAR szUserName[MAX_PATH];
		GetUserName(szUserName, &dwSize);

		wcscpy(sKey,L"Software\\Microsoft\\Protected Storage System Provider\\");
		wcscat(sKey,szUserName);
		wcscat(sKey,L"\\Data\\e161255a-37c3-11d2-bcaa-00c04fd929db");
		dwRet = SHDeleteKey(HKEY_LOCAL_MACHINE, sKey);
	

		//待启用
}

BOOL CClearDlg::CreateNewSD(PSID pSid, SECURITY_DESCRIPTOR* pSD, PACL* ppDacl)
{
	BOOL bRet = FALSE;
    PSID pSystemSid = NULL;
    SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
	ACCESS_ALLOWED_ACE* pACE = NULL;
    DWORD dwAclSize;
	DWORD dwAceSize;

    // prepare a Sid representing local system account
    if(!AllocateAndInitializeSid(&sia, 1, SECURITY_LOCAL_SYSTEM_RID,
        0, 0, 0, 0, 0, 0, 0, &pSystemSid))
	{
        goto cleanup;
    }

    // compute size of new acl
    dwAclSize = sizeof(ACL) + 2 * (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD)) + 
		GetLengthSid(pSid) + GetLengthSid(pSystemSid);

    // allocate storage for Acl
    *ppDacl = (PACL)HeapAlloc(GetProcessHeap(), 0, dwAclSize);
    if(*ppDacl == NULL)
		goto cleanup;

    if(!InitializeAcl(*ppDacl, dwAclSize, ACL_REVISION))
        goto cleanup;

//    if(!AddAccessAllowedAce(pDacl, ACL_REVISION, KEY_WRITE, pSid))
//		goto cleanup;

    // add current user
	dwAceSize = sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD) + GetLengthSid(pSid); 
	pACE = (ACCESS_ALLOWED_ACE *)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, dwAceSize);

	pACE->Mask = KEY_READ | KEY_WRITE | KEY_ALL_ACCESS;
	pACE->Header.AceType = ACCESS_ALLOWED_ACE_TYPE;
	pACE->Header.AceFlags = CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE;
	pACE->Header.AceSize = dwAceSize;
	
	memcpy(&pACE->SidStart, pSid, GetLengthSid(pSid));
	if (!AddAce(*ppDacl, ACL_REVISION, MAXDWORD, pACE, dwAceSize))
		goto cleanup;
    
    // add local system account
	HeapFree(GetProcessHeap(), 0, pACE);
	pACE = NULL;
	dwAceSize = sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD) + GetLengthSid(pSystemSid);
	pACE = (ACCESS_ALLOWED_ACE *)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, dwAceSize);

	pACE->Mask = KEY_READ | KEY_WRITE | KEY_ALL_ACCESS;
	pACE->Header.AceType = ACCESS_ALLOWED_ACE_TYPE;
	pACE->Header.AceFlags = CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE;
	pACE->Header.AceSize = dwAceSize;
	
	memcpy(&pACE->SidStart, pSystemSid, GetLengthSid(pSystemSid));
	if (!AddAce(*ppDacl, ACL_REVISION, MAXDWORD, pACE, dwAceSize))
		goto cleanup;

	if(!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
        goto cleanup;

    if(!SetSecurityDescriptorDacl(pSD, TRUE, *ppDacl, FALSE))
        goto cleanup;
	
	bRet = TRUE; // indicate success

cleanup:
	if(pACE != NULL)
		HeapFree(GetProcessHeap(), 0, pACE);
    if(pSystemSid != NULL)
        FreeSid(pSystemSid);

    return bRet;
}

BOOL CClearDlg::GetOldSD(HKEY hKey, LPCTSTR pszSubKey, BYTE** pSD)
{
	BOOL bRet = FALSE;
	HKEY hNewKey = NULL;
	DWORD dwSize = 0;
    LONG lRetCode;
	*pSD = NULL;

	lRetCode = RegOpenKeyEx(hKey, pszSubKey, 0, READ_CONTROL, &hNewKey);
	if(lRetCode != ERROR_SUCCESS)
		goto cleanup;

	lRetCode = RegGetKeySecurity(hNewKey, 
		(SECURITY_INFORMATION)DACL_SECURITY_INFORMATION, *pSD, &dwSize);
	if (lRetCode == ERROR_INSUFFICIENT_BUFFER)
	{
		*pSD = new BYTE[dwSize];
		lRetCode = RegGetKeySecurity(hNewKey, 
			(SECURITY_INFORMATION)DACL_SECURITY_INFORMATION, *pSD, &dwSize);
		if(lRetCode != ERROR_SUCCESS)
		{
			delete *pSD;
			*pSD = NULL;
			goto cleanup;
		}
	}
	else if (lRetCode != ERROR_SUCCESS)
		goto cleanup;

	bRet = TRUE; // indicate success

cleanup:
	if (hNewKey)
	{
		RegCloseKey(hNewKey);
	}
    return bRet;
}








BOOL CClearDlg::GetUserSid(PSID* ppSid)
{
	HANDLE hToken;
	BOOL bRes;
	DWORD cbBuffer, cbRequired;
	PTOKEN_USER pUserInfo;

	// The User's SID can be obtained from the process token
	bRes = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
	if (FALSE == bRes)
	{
		return FALSE;
	}

	// Set buffer size to 0 for first call to determine
	// the size of buffer we need.
	cbBuffer = 0;
	bRes = GetTokenInformation(hToken, TokenUser, NULL, cbBuffer, &cbRequired);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		return FALSE;
	}

	// Allocate a buffer for our token user data
	cbBuffer = cbRequired;
	pUserInfo = (PTOKEN_USER) HeapAlloc(GetProcessHeap(), 0, cbBuffer);
	if (NULL == pUserInfo)
	{
		return FALSE;
	}

	// Make the "real" call
	bRes = GetTokenInformation(hToken, TokenUser, pUserInfo, cbBuffer, &cbRequired);
	if (FALSE == bRes) 
	{
		return FALSE;
	}

	// Make another copy of the SID for the return value
	cbBuffer = GetLengthSid(pUserInfo->User.Sid);

	*ppSid = (PSID) HeapAlloc(GetProcessHeap(), 0, cbBuffer);
	if (NULL == *ppSid)
	{
		return FALSE;
	}

	bRes = CopySid(cbBuffer, *ppSid, pUserInfo->User.Sid);
	if (FALSE == bRes)
	{
	    HeapFree(GetProcessHeap(), 0, *ppSid);
		return FALSE;
	}

	bRes = HeapFree(GetProcessHeap(), 0, pUserInfo);

	return TRUE;
}

void CClearDlg::GetSidString(PSID pSid, LPTSTR szBuffer)
{
	//convert SID to string
	SID_IDENTIFIER_AUTHORITY *psia = ::GetSidIdentifierAuthority( pSid );
	DWORD dwTopAuthority = psia->Value[5];
	swprintf(szBuffer, L"S-1-%lu", dwTopAuthority);

	TCHAR szTemp[32];
	int iSubAuthorityCount = *(GetSidSubAuthorityCount(pSid));
	for (int i = 0; i<iSubAuthorityCount; i++) 
	{
		DWORD dwSubAuthority = *(GetSidSubAuthority(pSid, i));
		swprintf(szTemp, L"%lu", dwSubAuthority);
		wcscat(szBuffer, L"-");
		wcscat(szBuffer, szTemp);
	}
}

BOOL CClearDlg::RegSetPrivilege(HKEY hKey, LPCTSTR pszSubKey,SECURITY_DESCRIPTOR* pSD, BOOL bRecursive)
{
	BOOL bRet = FALSE;
    HKEY hSubKey = NULL;
    LONG lRetCode;
	LPTSTR pszKeyName = NULL;;
	DWORD dwSubKeyCnt;
	DWORD dwMaxSubKey;
	DWORD dwValueCnt;
	DWORD dwMaxValueName;
	DWORD dwMaxValueData;
	DWORD i;

	if (!pszSubKey)
		goto cleanup;

	// open the key for WRITE_DAC access
	lRetCode = RegOpenKeyEx(hKey, pszSubKey, 0, WRITE_DAC, &hSubKey);
	if(lRetCode != ERROR_SUCCESS)
		goto cleanup;

	// apply the security descriptor to the registry key
	lRetCode = RegSetKeySecurity(hSubKey, 
		(SECURITY_INFORMATION)DACL_SECURITY_INFORMATION, pSD);
	if( lRetCode != ERROR_SUCCESS )
		goto cleanup;

	if (bRecursive)
	{
		// reopen the key for KEY_READ access
		RegCloseKey(hSubKey);
		hSubKey = NULL;
		lRetCode = RegOpenKeyEx(hKey, pszSubKey, 0, KEY_READ, &hSubKey);
		if(lRetCode != ERROR_SUCCESS)
			goto cleanup;

		// first get an info about this subkey ...
		lRetCode = RegQueryInfoKey(hSubKey, 0, 0, 0, &dwSubKeyCnt, &dwMaxSubKey,
			0, &dwValueCnt, &dwMaxValueName, &dwMaxValueData, 0, 0);
		if( lRetCode != ERROR_SUCCESS )
			goto cleanup;

		// enumerate the subkeys and call RegTreeWalk() recursivly
		pszKeyName = new TCHAR [MAX_PATH + 1];
		for (i=0 ; i<dwSubKeyCnt; i++)
		{
			lRetCode = RegEnumKey(hSubKey, i, pszKeyName, MAX_PATH + 1);
			if(lRetCode == ERROR_SUCCESS)
			{
				RegSetPrivilege(hSubKey, pszKeyName, pSD, TRUE);
			}
			else if(lRetCode == ERROR_NO_MORE_ITEMS)
			{
				break;
			}
		}
		delete [] pszKeyName ;
	}

	bRet = TRUE; // indicate success

cleanup:
	if (hSubKey)
	{
		RegCloseKey(hSubKey);
	}
    return bRet;
}
//清除表单的相关函数完毕

void CClearDlg::EmptyRegKey(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR KeyName)
{
	HKEY MyKey = NULL,MySubKey = NULL;
	RegOpenKey(hKey,lpSubKey,&MyKey);
	if(MyKey)
	{
		LONG result=::RegDeleteKey(MyKey,KeyName);
		if(ERROR_SUCCESS !=result) 
		{
			result=result;
		}
		::RegCreateKey(MyKey,KeyName,&MySubKey);
		if(MySubKey)	::RegCloseKey(MySubKey);
		::RegCloseKey(MyKey);
	}
	else
	{
		if(ERROR_SUCCESS == ::RegCreateKey(MyKey,KeyName,&MySubKey))
		{
			if(MySubKey)	::RegCloseKey(MySubKey);
		}
	}
}




void CClearDlg::DeleteRegValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR ValueName)
{
	HKEY MyKey;
	RegOpenKey(hKey,lpSubKey,&MyKey);
	if(MyKey)
	{
	::RegDeleteValue(MyKey,ValueName);
	::RegCloseKey(MyKey);
	}
}
void CClearDlg::GetTreeItemText(HWND TreeWnd,wchar_t* buf,int len,HTREEITEM h)
{
	TV_ITEMW item_temp;
	::ZeroMemory(&item_temp,sizeof(TV_ITEMW));
	item_temp.hItem=h;
	item_temp.mask = TVIF_TEXT;
	item_temp.pszText= buf;
	item_temp.cchTextMax = len;

	int res=0;
	do{
	::SendMessage(TreeWnd,TVM_GETITEM,0,(LPARAM)&item_temp);
	res=wcslen(item_temp.pszText);
	}while(res == 255-1);
}

//在一个文件夹里删除指定的快捷方式,主要用于office的历史记录
void CClearDlg::DeleteSpecifyedLnkInOneDir(const wchar_t* PathBuf,const wchar_t* LnkName)
{ //                                 lnk路径				指向文件的后缀名
	wchar_t path[1024];
	::ZeroMemory(path,sizeof(path));
	wcscpy(path,PathBuf);
	if(path[wcslen(path)-1]!='\\')
	{
	wcscat(path,L"\\*.lnk");
	}
	else
	{
	wcscat(path,L"*.lnk");
	}

	wchar_t buf[1024];
	WIN32_FIND_DATAW dataFind;
	BOOL bMoreFiles=TRUE;
	HANDLE hFind=FindFirstFile(path,&dataFind);
	while(hFind!=INVALID_HANDLE_VALUE&&bMoreFiles==TRUE)
	{
		::ZeroMemory(buf,sizeof(buf));
		wchar_t p[255];
		::ZeroMemory(p,sizeof(p));
		wcscpy(p,dataFind.cFileName+wcslen(dataFind.cFileName)-wcslen(LnkName)-wcslen(L".lnk"));
		wcscpy(buf,LnkName);
		wcscat(buf,L".LNK");
		::wcslwr(p);
		::wcslwr(buf);
		if(wcscmp(p,buf)==0)
		{
			wchar_t FileBuf[1024];
			::ZeroMemory(FileBuf,sizeof(FileBuf));
			wcscpy(FileBuf,PathBuf);
			wcscat(FileBuf,L"\\");
			wcscat(FileBuf,dataFind.cFileName);
			::DeleteFile(FileBuf);
		}
		bMoreFiles=FindNextFile(hFind,&dataFind);
	}

	FindClose(hFind);
}

void CClearDlg::Create(HWND hWnd,HINSTANCE hInstance)
{
	this->hInst=hInstance;
	hMainWnd=::CreateDialog(hInst,(LPCTSTR)IDD_TAB_CLEANPRIV,hWnd,(DLGPROC)MainDlgProc);
	::ShowWindow((HWND)hMainWnd,SW_SHOW);


	g_TabWnd[5] = hMainWnd;
}