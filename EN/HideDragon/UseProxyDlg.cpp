// UseProxyDlg.cpp: implementation of the UseProxyDlg class.
//
//////////////////////////////////////////////////////////////////////



#define _WIN32_WINNT 0x501

#include "stdafx.h"
#include "UseProxyDlg.h"
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <wininet.h>
#include "resource.h"
#include "hd.h"
#include "HyperLink.h"
#include "Ras.h"
#include <commctrl.h>



#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "comctl32.lib")

//全局变量
#define FINISHANDUSEPROXY	6001
#define MAXTIME 10000
#define EXITSOCK(s) 		{closesocket(s);return 0;}
#define DELETESTR _T("PROXYCN")
#define SOCKHEAD1 "\5\1"
#define SOCKRESPOND1 "\5"
#define SOCKHEAD2 "\5\3\00\01\00\00\00\00\xc1\x1b"
#define SOCKRESPOND2 '\x05'
#define HTTPHEAD "CONNECT http.tencent.com:443 HTTP/1.1\r\nAccept: */*\r\nContent-Type: text/html\r\nProxy-Connection: Keep-Alive\r\nContent-length: 0\r\n\r\n"
#define HTTPRESPOND "200 Connection established"
#define HTTPRESPONDHEAD "HTTP/1.0 "
#define TD1 "<td"
#define TD2 "</td>"
#define TR1 "<tr"
#define TR2 "</tr>"
#define HtmlBeginCode '<'
#define HtmlEndCode ">"
#define DOC "document.write(\""
#define DOCEND "\");"
#define TABLEHEADER1 "<tr align=\"center\"><td class=\"list_title\">id</td><td class=\"list_title\">地址</td><td class=\"list_title\">端口</td><td class=\"list_title\">类型</td><td class=\"list_title\">地区</td><td class=\"list_title\">验证时间</td><td class=\"list_title\">响应</td><td class=\"list_title\">whois</td></tr>"
#define TABLEEND "</table>"
#define FILEPATH _T("html_proxy/")
#define FILENAME1 _T("socks5-1.html")
#define FILENAME2 _T("socks4-1.html")
#define FILENAME3 _T("30fastproxy-1.html")
#define FILENAME4 _T("http-1.html")
#define GETFILESTR1 "GET /"
#define GETFILESTR2 " HTTP/1.1\r\n\
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-powerpoint, application/vnd.ms-excel, application/msword, application/x-shockwave-flash, application/x-ms-application, application/vnd.ms-xpsdocument, application/xaml+xml, application/x-ms-xbap, */*\r\n\
Accept-Language: zh-cn\r\n\
UA-CPU: x86\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0; .NET CLR 1.1.4322; .NET CLR 3.0.04506)\r\n\
Host: www.proxycn.com\r\n\
Connection: Keep-Alive\r\n\r\n"
#define WAVERS 0x0101
#define HOST "www.proxycn.com"
#define SERVICE "80"
#define RN "\r\n"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HWND UseProxyDlg::hMainWnd=0;
HWND UseProxyDlg::hAdvWnd=0;
HWND UseProxyDlg::hGenWnd=0;
HWND UseProxyDlg::hUseInfoWnd=0;
ProxyInfo UseProxyDlg::ProxyInfoHead;
HWND UseProxyDlg::ProGressWnd=0;
HANDLE UseProxyDlg::hFile=0;
HANDLE UseProxyDlg::hMapping=0;
_TCHAR* UseProxyDlg::basepointer=0;
HANDLE UseProxyDlg::hThread=0;
HANDLE hThread_setproxylist = NULL;



HANDLE hThread_download = NULL;


HANDLE hThread1,hThread2,hThread3,hThread4;

HANDLE hThread_testpro;


int g_ExitDownloadQuery = 0 ,g_ExitDownloadThread = 0,g_ExitSetProxyThread = 0,g_ExitTestProxyThread = 0;



#define CHECK_DOWNLOAD_QUERY_EXIT\
	if(g_ExitDownloadQuery == 1)\
	{\
		CloseHandle(hThread);\
		hThread = NULL;\
		MessageBox(NULL,L"DDDD2",L"DDD2",MB_OK);\
		return 0;\
	}




#define CHECK_DOWNLOAD_EXIT\
	if(g_ExitDownloadThread == 1)\
	{\
		if(DllFile1)\
		{\
			::CloseHandle(DllFile1);\
			DllFile1=0;\
		}\
		closesocket(s);\
		delete (void*)ptemp;\
		MessageBox(NULL,L"DDDD2",L"DDD2",MB_OK);\
		return 0;\
	}


#define CHECK_SETPROXYLIST_EXIT\
	if(g_ExitSetProxyThread == 1)\
	{\
		::CloseHandle(hThread_setproxylist);\
		hThread_setproxylist=0;\
		MessageBox(NULL,L"DDDD2",L"DDD2",MB_OK);\
		return 0;\
	}



#define CHECK_TESTPROXY_EXIT\
	if(g_ExitTestProxyThread == 1)\
	{\
		::CloseHandle(hThread_testpro);\
		hThread_testpro=0;\
		MessageBox(NULL,L"DDDD2",L"DDD2",MB_OK);\
		return 0;\
	}






HINSTANCE UseProxyDlg::hInst=0;								// current instance
int g_CompleteFlag1 = 1;
int g_CompleteFlag2 = 1;
int g_CompleteFlag3 = 1;
int g_CompleteFlag4 = 1;


extern HWND g_TabWnd[10];
extern int g_iUseLanguage;
extern int g_UseIcon;



extern HWND g_hGenWnd;
extern HWND g_hAdvWnd;
extern HWND g_IPList;




HANDLE g_RestoreHandle = NULL;
HBRUSH g_brushGeneralPage;


UseProxyDlg::UseProxyDlg()
{
return;
}

UseProxyDlg::~UseProxyDlg()
{
return;
}

//自定义函数
LRESULT CALLBACK UseProxyDlg::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message) 
	{
		case WM_INITDIALOG:
			OnInitDlg(hInst,hWnd);
			g_TabWnd[6] = hWnd;
			return true;
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDOK:
				case IDCANCEL:
					EndDialog(hWnd, LOWORD(wParam));
					return TRUE;
					break;
				default:
					break;
				   //return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
			
		case WM_SIZE:
			{
				SetWindowPos(hGenWnd,NULL,0,0,LOWORD(lParam),HIWORD(lParam),
				SWP_NOZORDER|SWP_NOMOVE);

				SetWindowPos(hAdvWnd,NULL,0,0,LOWORD(lParam),HIWORD(lParam),
				SWP_NOZORDER|SWP_NOMOVE);

				

			}
			break;
		case WM_PAINT:
			break;
		case WM_DESTROY:
			{
			if(hThread)
			{
				DWORD ExitCode=0;
				GetExitCodeThread(hThread,&ExitCode);
				TerminateThread(hThread,ExitCode);
				hThread=0;
			}
			
			ClearStack();
	 		WSACleanup();
			DestroyWindow(g_TabWnd[6]);
			//PostQuitMessage(0);
			}
			break;
		case FINISHANDUSEPROXY:
			{
				if(ProxyInfoHead.next)
				{
				char buf1[100],buf2[100],buf3[100];
				::ZeroMemory(buf1,sizeof(buf1));
				::ZeroMemory(buf2,sizeof(buf2));
				::ZeroMemory(buf3,sizeof(buf3));
				WideCharToMultiByte(CP_ACP, 0,ProxyInfoHead.next->HostIp , -1, buf1, sizeof(buf1), NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0,ProxyInfoHead.next->port , -1, buf2, sizeof(buf2), NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0,ProxyInfoHead.next->kind , -1, buf3, sizeof(buf3), NULL, NULL);
				UseProxy(buf1,buf2,buf3);
				GetSystemProxy(GetDlgItem(hWnd,IDC_USE_INFO));
				}
				//ClearStack();
			}
			break;
		default:
			return false;
   }
   return 0;
}





CHyperLink m_lnk_adv;
HWND hAdv_ADLink;

LRESULT CALLBACK UseProxyDlg::AdvDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_INITDIALOG:
			{
				OnInitAdvDlg(hInst,hWnd);
				if(g_iUseLanguage == 1)
				{
					SetDialogTextW(&hWnd,IDD_TAB_HIDEIP_ADVANCE);
				}


				hAdv_ADLink = GetDlgItem(hWnd,IDC_HIDEIP_ADV_ADLINK);

#ifndef _NOADS
				m_lnk_adv.Attach(hAdv_ADLink);
// 				m_lnk_adv.SetLink(L"隐身专家广告联盟，让伙伴更强！", 
// 			L"http://www.hidedragon.com/cn/union.html", TRUE);

				{
					wchar_t * cDefaultText = new wchar_t[1024];
					memset(cDefaultText,0,1024*sizeof(wchar_t));
					LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");
					
					wchar_t * cDefaultLink = new wchar_t[1024];
					memset(cDefaultLink,0,1024*sizeof(wchar_t));
					
					LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.hidedragon.com/cn/union.html");
					
					
					m_lnk_adv.SetLink(cDefaultText,cDefaultLink, TRUE);
					
					
					delete [] cDefaultText;
					cDefaultText = NULL;
					delete [] cDefaultLink;
					cDefaultLink = NULL;
				}




#endif

#ifdef _NOADS
				ShowWindow(hAdv_ADLink,SW_HIDE);
#endif



				if(g_UseIcon == 1)
				{
					HWND hSearchIp = GetDlgItem(hWnd,IDC_HIDEIP_SEARCHNOW);
					
					
					HICON hI3 = (struct HICON__*)LoadImage(
						hInst,MAKEINTRESOURCE(IDI_HIDEIP_SEARCHIP),IMAGE_ICON,16,16,NULL);
					
					
					
					SendMessage(hSearchIp,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
					
					DestroyIcon(hI3);
					
					
					HWND hUseTip = GetDlgItem(hWnd,IDC_HIDEIP_USETHISPROXY);
					
					
					hI3 = (struct HICON__*)LoadImage(
						hInst,MAKEINTRESOURCE(IDI_HIDEIP_HIDENOW),IMAGE_ICON,16,16,NULL);
					
					
					
					SendMessage(hUseTip,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
					
					DestroyIcon(hI3);
					



					HWND hRestoreIp = GetDlgItem(hWnd,IDC_HIDEIP_ADV_RESTOREIP);
					
					
					hI3 = (struct HICON__*)LoadImage(
						hInst,MAKEINTRESOURCE(IDI_HIDEIP_RESTOREIP),IMAGE_ICON,16,16,NULL);
					
					
					
					SendMessage(hRestoreIp,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
					
					DestroyIcon(hI3);
					
					RECT rtM;
					rtM.left = 0;
					rtM.right = 20;
					rtM.top = 0;
					rtM.bottom = 00;
					Button_SetTextMargin(hSearchIp,&rtM);
					Button_SetTextMargin(hUseTip,&rtM);
					
					Button_SetTextMargin(hRestoreIp,&rtM);
					
				}



				return TRUE;
			}
			break;
		case WM_DESTROY:
			DestroyWindow(g_hAdvWnd);
			break;
		case WM_LBUTTONDOWN:
			break;
		
//		case WM_CTLCOLORSTATIC:
//			SetBkColor((HDC)(wParam),GetSysColor(CTLCOLOR_DLG));
//			return (LRESULT)g_brushGeneralPage;
//		case WM_CTLCOLORDLG:
//			return (LRESULT)g_brushGeneralPage;
		case WM_SIZE:
			{
				HWND hGroup_Advanced = GetDlgItem(hWnd,IDC_HIDEIP_SELSERVER);
				
				SetWindowPos(hGroup_Advanced,NULL,10,10,LOWORD(lParam) - 15,HIWORD(lParam) - 15,
				SWP_NOZORDER);
				
				
				
				
				HWND hAdv_List = GetDlgItem(hWnd,IDC_HIDEIP_IPLIST);

				SetWindowPos(hAdv_List,NULL,19,70,LOWORD(lParam) - 35,HIWORD(lParam) - 155,
				SWP_NOZORDER);


				HWND hBtn_General = GetDlgItem(hWnd,IDC_HIDEIP_BTN_GENERAL);
				RECT generalRect;
				GetWindowRect(hBtn_General,&generalRect);
				LONG generalLen = generalRect.right - generalRect.left;
				SetWindowPos(hBtn_General,NULL,LOWORD(lParam)-generalLen-15,
				HIWORD(lParam)-35,0,0,SWP_NOZORDER|SWP_NOSIZE);




				HWND hBtn_Search = GetDlgItem(hWnd,IDC_HIDEIP_SEARCHNOW);
				RECT searchRect;
				GetWindowRect(hBtn_Search,&searchRect);
				LONG searchLen = searchRect.right - searchRect.left;
				SetWindowPos(hBtn_Search,NULL,19,
				/*HIWORD(lParam)-*/35,0,0,SWP_NOZORDER|SWP_NOSIZE);

				HWND hStopHide = GetDlgItem(hWnd,IDC_HIDEIP_ADV_STOPSEARCH);
				SetWindowPos(hStopHide,NULL,160,
				/*HIWORD(lParam)-*/35,0,0,SWP_NOZORDER|SWP_NOSIZE);







				HWND hBtn_UseProxy = GetDlgItem(hWnd,IDC_HIDEIP_USETHISPROXY);
				RECT useproxyRect;
				GetWindowRect(hBtn_UseProxy,&useproxyRect);
				LONG useproxyLen = useproxyRect.right - useproxyRect.left;
				SetWindowPos(hBtn_UseProxy,NULL,19,
				HIWORD(lParam)-75,0,0,SWP_NOZORDER|SWP_NOSIZE);
				

				HWND hBtn_RestoreIP = GetDlgItem(hWnd,IDC_HIDEIP_ADV_RESTOREIP);
				RECT restoreRect;
				GetWindowRect(hBtn_RestoreIP,&restoreRect);
				LONG restoreLen = restoreRect.right - restoreRect.left;
				SetWindowPos(hBtn_RestoreIP,NULL,159,
				HIWORD(lParam)-75,0,0,SWP_NOZORDER|SWP_NOSIZE);


				
				RECT adlinkRect;
				GetWindowRect(hAdv_ADLink,&adlinkRect);
				RECT btnRect;
				GetWindowRect(hBtn_Search,&btnRect);
				LONG btnLen = btnRect.right - btnRect.left;
				LONG adlinkLen = adlinkRect.right - adlinkRect.left;
				SetWindowPos(hAdv_ADLink,NULL,(btnLen*2) + 50,30,LOWORD(lParam) - (btnLen + 50),
					20,SWP_NOZORDER);


				HWND hBtn_IPState = GetDlgItem(hWnd,IDC_HIDEIP_ADV_IPSTATE);

				GetWindowRect(hBtn_IPState,&restoreRect);
				SetWindowPos(hBtn_IPState,NULL,19,
				HIWORD(lParam)-35,0,0,SWP_NOZORDER|SWP_NOSIZE);
				
				
			}
			break;
		case WM_COMMAND:
			{
				DWORD wmId    = LOWORD(wParam); 
				DWORD wmEvent = HIWORD(wParam); 
				// Parse the menu selections:
				switch (wmId)
				{
				case IDC_HIDEIP_GENERAL://打开简易面板
					::ShowWindow(hGenWnd,SW_SHOW);
					::ShowWindow(hAdvWnd,SW_HIDE);
					break;
				case IDC_HIDEIP_SEARCHNOW://刷新
					{
						if(ProxyInfoHead.next) ClearStack();
						DWORD dwThreadId; 
						g_ExitDownloadQuery = 0;
						hThread = CreateThread(NULL,0,DownloadAndQueryFile,(void*)hWnd,0,&dwThreadId);  
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH),1);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_SEARCHNOW),0);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_BTN_GENERAL),0);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_USETHISPROXY),0);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_ADV_RESTOREIP),0);
						{
							HWND hStopHide = GetDlgItem(hWnd,IDC_HIDEIP_ADV_STOPSEARCH);
							ShowWindow(hStopHide,SW_SHOW);
						}

					}
					break;
				case IDC_HIDEIP_USETHISPROXY://使用选中的代理
					{
						HWND ListWnd=GetDlgItem(hWnd,IDC_HIDEIP_IPLIST);
						int pos=::SendMessage(ListWnd, LVM_GETNEXTITEM, -1, MAKELPARAM(LVIS_SELECTED, 0));
						if(pos == -1)
						{
							return FALSE;
						}
	
						LVITEM lvi;
						memset(&lvi, 0, sizeof(LVITEM));
						lvi.iSubItem = 1;
						_TCHAR HostIp[128],port[128],patrol[128],pArea[128];
						::ZeroMemory(HostIp,sizeof(HostIp));
						::ZeroMemory(port,sizeof(port));
						::ZeroMemory(patrol,sizeof(patrol));
						::ZeroMemory(pArea,sizeof(pArea));
						int nLen = sizeof(HostIp);
						int nRes;
						do
						{
							nLen *= 2;
							lvi.cchTextMax = nLen;
							lvi.pszText = HostIp;
							nRes  = (int)::SendMessage(ListWnd, LVM_GETITEMTEXT, (WPARAM)pos,
								(LPARAM)&lvi);
						} while (nRes == nLen-1);
						lvi.iSubItem = 2;
						do
						{
							nLen *= 2;
							lvi.cchTextMax = nLen;
							lvi.pszText = port;
							nRes  = (int)::SendMessage(ListWnd, LVM_GETITEMTEXT, (WPARAM)pos,
								(LPARAM)&lvi);
						} while (nRes == nLen-1);
						lvi.iSubItem = 3;
						do
						{
							nLen *= 2;
							lvi.cchTextMax = nLen;
							lvi.pszText = patrol;
							nRes  = (int)::SendMessage(ListWnd, LVM_GETITEMTEXT, (WPARAM)pos,
								(LPARAM)&lvi);
						} while (nRes == nLen-1);
						lvi.iSubItem = 4;

						do
						{
							nLen *= 2;
							lvi.cchTextMax = nLen;
							lvi.pszText = pArea;
							nRes  = (int)::SendMessage(ListWnd, LVM_GETITEMTEXT, (WPARAM)pos,
								(LPARAM)&lvi);
						} while (nRes == nLen-1);

						char buf1[100],buf2[100],buf3[100];
						::ZeroMemory(buf1,sizeof(buf1));
						::ZeroMemory(buf2,sizeof(buf2));
						::ZeroMemory(buf3,sizeof(buf3));
						WideCharToMultiByte(CP_ACP, 0,HostIp, -1, buf1, sizeof(buf1), NULL, NULL);
						WideCharToMultiByte(CP_ACP, 0,port, -1, buf2, sizeof(buf2), NULL, NULL);
						WideCharToMultiByte(CP_ACP, 0,patrol, -1, buf3, sizeof(buf3), NULL, NULL);
						UseProxy(buf1,buf2,buf3);
//						UseProxy(HostIp,port,patrol);
						GetSystemProxy(hUseInfoWnd);
						//::MessageBox(hWnd,_T("状态：已经将该服务器设置到系统中!"),_T("完成"),0);
						wchar_t * pText = new wchar_t[1024];
						memset(pText,0,1024);
						wcscpy(pText,L"IP: ");
						wcscat(pText,HostIp);
						wcscat(pText,L" : ");
						wcscat(pText,port);
						wcscat(pText,L" ");
						wchar_t * pAreaString = new wchar_t[1024];
						memset(pAreaString,0,1024);

						wcscat(pText,LoadItemStringW(L"IDS_HIDEIP_AREA",pAreaString,L" area:"));
						delete [] pAreaString;
						pAreaString = NULL;
						wcscat(pText,L" ");
						wcscat(pText,pArea);
						
						SendMessage(hWnd,WM_COMMAND,IDC_HIDEIP_ADV_STOPSEARCH,NULL);
						SetDlgItemText(hGenWnd,IDC_HIDEIP_IPSTATE,pText);
						//SetWindowText(hTip,pText);
						
						ShowWindow(GetDlgItem(hGenWnd,IDC_HIDEIP_RESTOREIP),SW_SHOW);
						ShowWindow(GetDlgItem(hGenWnd,IDC_HIDEIP_HIDENOW),SW_HIDE);
						SetDlgItemText(hAdvWnd,IDC_HIDEIP_ADV_IPSTATE,pText);
						delete [] pText;

						HDMessageBoxW(L"IDS_HIDEIP_SETPROXYSUCCESS",L"State:Set Proxy Sucess!");
					}
					break;
				case IDC_HIDEIP_ADV_RESTOREIP://恢复
					{
						if(g_RestoreHandle == NULL)
						{
							g_RestoreHandle = (HANDLE)_beginthreadex(NULL,
								0,RestoreIPProc,NULL,0,NULL);
							if(g_RestoreHandle == INVALID_HANDLE_VALUE)
							{
								g_RestoreHandle = NULL;
								return 0;
							}
						}
							
							
							
						//restore();
					}
					break;
				case IDC_HIDEIP_ADV_SEARCHFAIL:
					{
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH),0);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_SEARCHNOW),1);
						StopSearch();
						HWND StateWnd=::GetDlgItem(hWnd,IDC_HIDEIP_ADV_IPSTATE);
						HWND hGenStateWnd=::GetDlgItem(hGenWnd,IDC_HIDEIP_IPSTATE);

						HWND hStopHide = GetDlgItem(hWnd,IDC_HIDEIP_ADV_STOPSEARCH);
						ShowWindow(hStopHide,SW_HIDE);

						HWND hBtn_General = GetDlgItem(hWnd,IDC_HIDEIP_BTN_GENERAL);
						EnableWindow(hBtn_General,TRUE);
						
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_USETHISPROXY),1);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_ADV_RESTOREIP),1);
					}
					break;
				case IDC_HIDEIP_ADV_STOPSEARCH:
					{
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH),0);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_SEARCHNOW),1);
						StopSearch();
						HWND StateWnd=::GetDlgItem(hWnd,IDC_HIDEIP_ADV_IPSTATE);
						HWND hGenStateWnd=::GetDlgItem(hGenWnd,IDC_HIDEIP_IPSTATE);
						//::SetWindowText(StateWnd,_T("状态：停止搜索"));
						wchar_t * pText = new wchar_t[1024];
						memset(pText,0,1024*sizeof(wchar_t));
						::SetWindowText(StateWnd,
							LoadItemStringW(L"IDS_HIDEIP_STATESTOPSEARCH",pText,
							L"State:Stop Search"));

						::SetWindowText(hGenStateWnd,
							LoadItemStringW(L"IDS_HIDEIP_STATESTOPSEARCH",pText,
							L"State:Stop Search"));


						delete [] pText;
						::SendMessage(ProGressWnd,PBM_SETPOS,0,0);

						HWND hStopHide = GetDlgItem(hWnd,IDC_HIDEIP_ADV_STOPSEARCH);
						ShowWindow(hStopHide,SW_HIDE);

						HWND hBtn_General = GetDlgItem(hWnd,IDC_HIDEIP_BTN_GENERAL);
						EnableWindow(hBtn_General,TRUE);
						
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_USETHISPROXY),1);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_ADV_RESTOREIP),1);
			

					}
					break;
				}
			}
	return false;
	}
	return false;
}




bool UseProxyDlg::RestoreIP()
{
//	if(g_RestoreHandle == NULL)
//	{
//		int iShowDlg = 0;
//		g_RestoreHandle = (HANDLE)_beginthreadex(NULL,
//			0,RestoreIPProc,&iShowDlg
//			,0,NULL);
//		if(g_RestoreHandle == INVALID_HANDLE_VALUE)
//		{
//			g_RestoreHandle = NULL;
//			return 0;
//		}
//	}

	restore(0);


	return false;
}



HWND hHideIP_ADLink;
CHyperLink m_lnk_hideip;



LRESULT CALLBACK UseProxyDlg::GenDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_INITDIALOG:
			{
				::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH),0);



				if(g_iUseLanguage == 1)
				{
					SetDialogTextW(&hWnd,IDD_TAB_HIDEIP_GENERAL);
				}

				hHideIP_ADLink = GetDlgItem(hWnd,IDC_HIDEIP_ADLINK);
#ifndef _NOADS
				m_lnk_hideip.Attach(hHideIP_ADLink);
// 				m_lnk_hideip.SetLink(L"隐身专家广告联盟，让伙伴更强！", 
// 			L"http://www.hidedragon.com/cn/union.html", TRUE);
				{
					wchar_t * cDefaultText = new wchar_t[1024];
					memset(cDefaultText,0,1024*sizeof(wchar_t));
					LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");
					
					wchar_t * cDefaultLink = new wchar_t[1024];
					memset(cDefaultLink,0,1024*sizeof(wchar_t));
					
					LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.hidedragon.com/cn/union.html");
					
					
					m_lnk_hideip.SetLink(cDefaultText,cDefaultLink, TRUE);
					
					
					delete [] cDefaultText;
					cDefaultText = NULL;
					delete [] cDefaultLink;
					cDefaultLink = NULL;
				}





#endif

#ifdef _NOADS
				ShowWindow(hHideIP_ADLink,SW_HIDE);
#endif
			}




			if(g_UseIcon == 1)
			{
				HWND hHideIp = GetDlgItem(hWnd,IDC_HIDEIP_HIDENOW);
				
				
				HICON hI3 = (struct HICON__*)LoadImage(
					hInst,MAKEINTRESOURCE(IDI_HIDEIP_HIDENOW),IMAGE_ICON,16,16,NULL);
				
				
				
				SendMessage(hHideIp,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
				
				DestroyIcon(hI3);
				

				
				HWND hRestoreIp = GetDlgItem(hWnd,IDC_HIDEIP_RESTOREIP);


				hI3 = (struct HICON__*)LoadImage(
					hInst,MAKEINTRESOURCE(IDI_HIDEIP_RESTOREIP),IMAGE_ICON,16,16,NULL);
				
				
				
				SendMessage(hRestoreIp,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
				
				DestroyIcon(hI3);
				
				RECT rtM;
				rtM.left = 0;
				rtM.right = 20;
				rtM.top = 0;
				rtM.bottom = 00;
				Button_SetTextMargin(hHideIp,&rtM);

				Button_SetTextMargin(hRestoreIp,&rtM);
				
			}








			break;

		case WM_LBUTTONDOWN:
			break;
//		case WM_CTLCOLORSTATIC:
//			//SetBkColor((HDC)(wParam),GetSysColor(CTLCOLOR_DLG));
//			return (LRESULT)g_brushGeneralPage;
//		case WM_CTLCOLORDLG:
//			return (LRESULT)g_brushGeneralPage;
		case WM_DESTROY:
			DeleteObject(g_brushGeneralPage);
			DestroyWindow(g_hGenWnd);

			break;
		case WM_SIZE:
			{
				HWND hBtn_HideIP = GetDlgItem(hWnd,IDC_HIDEIP_HIDENOW);
				RECT rtHide;
				GetWindowRect(hBtn_HideIP,&rtHide);
				int len_HideIP = rtHide.right - rtHide.left;
				int hig_HideIP = rtHide.bottom - rtHide.top;

				int xPos = LOWORD(lParam)/2-(len_HideIP/2);
				int yPos = (HIWORD(lParam)/4);
				SetWindowPos(hBtn_HideIP,NULL,xPos,yPos,0,0,
					SWP_NOZORDER|SWP_NOSIZE);



				HWND hBtn_RestoreIP = GetDlgItem(hWnd,IDC_HIDEIP_RESTOREIP);

				SetWindowPos(hBtn_RestoreIP,NULL,xPos,yPos,0,0,
					SWP_NOZORDER|SWP_NOSIZE);


				HWND hBtn_StopHideIP = GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH);
				rtHide;
				GetWindowRect(hBtn_StopHideIP,&rtHide);
				len_HideIP = rtHide.right - rtHide.left;
				hig_HideIP = rtHide.bottom - rtHide.top;
				xPos = LOWORD(lParam)/2 + 100;
				yPos = (HIWORD(lParam)/4);
				SetWindowPos(hBtn_StopHideIP,NULL,xPos,yPos,0,0,
					SWP_NOZORDER|SWP_NOSIZE);

				
				
				
				
				
				
				HWND hGroup_HideIP = GetDlgItem(hWnd,IDC_HIDEIP_STATICHIDEIP);
				
				SetWindowPos(hGroup_HideIP,NULL,10,10,LOWORD(lParam) - 15,HIWORD(lParam) - 15,
				SWP_NOZORDER);

				HWND hBtn_Advanced = GetDlgItem(hWnd,IDC_HIDEIP_ADVANCED);
				RECT advancedRect;
				GetWindowRect(hBtn_Advanced,&advancedRect);
				LONG advancedLen = advancedRect.right - advancedRect.left;
				SetWindowPos(hBtn_Advanced,NULL,LOWORD(lParam)-advancedLen-15,
				HIWORD(lParam)-35,0,0,SWP_NOZORDER|SWP_NOSIZE);

				




				HWND hStatic_YourIP = GetDlgItem(hWnd,IDC_HIDEIP_YOURIP);
				
				RECT rtYourIP;
				memset(&rtYourIP,0,sizeof(rtYourIP));
				GetWindowRect(hStatic_YourIP,&rtYourIP);
				int len_YourIP = 0;
				len_YourIP = rtYourIP.right - rtYourIP.left;
				int hig_YourIP = 0;
				hig_YourIP = rtYourIP.bottom - rtYourIP.top;

				int xYourIP = LOWORD(lParam)/4;
				int yYourIP = (HIWORD(lParam)/3 + 30);
				SetWindowPos(hStatic_YourIP,NULL,xYourIP,yYourIP,0,0,
					SWP_NOZORDER|SWP_NOSIZE);


				

				HWND hStatic_IPState = GetDlgItem(hWnd,IDC_HIDEIP_IPSTATE);
				RECT rtStateIP;
				GetWindowRect(hStatic_IPState,&rtStateIP);
				int len_IPState = rtStateIP.right - rtStateIP.left;
				int hig_IPState = rtStateIP.bottom - rtStateIP.top;

				int xIPState = LOWORD(lParam)/4;
				int yIPState = (HIWORD(lParam)/3 + 60);
				SetWindowPos(hStatic_IPState,NULL,xIPState,yIPState,0,0,
					SWP_NOZORDER|SWP_NOSIZE);

				
				int xADLink = LOWORD(lParam)/6;
				xADLink = LOWORD(lParam)-advancedLen-195;
				int yADLink = (HIWORD(lParam)/2 + 120);

				SetWindowPos(hHideIP_ADLink,NULL,xADLink,HIWORD(lParam)-65/*yADLink*/,0,0,
				SWP_NOZORDER|SWP_NOSIZE);

			}
			break;
		case WM_COMMAND:
			{
				DWORD wmId    = LOWORD(wParam); 
				DWORD wmEvent = HIWORD(wParam); 
				// Parse the menu selections:
				switch (wmId)
				{
				case IDC_HIDEIP_ADVANCED://打开高级面板
					::ShowWindow(hGenWnd,SW_HIDE);
					::ShowWindow(hAdvWnd,SW_SHOW);
					break;
				case IDC_HIDEIP_HIDENOW://使用代理按钮
					{						
						if(ProxyInfoHead.next) ClearStack();
						HWND StateWnd=::GetDlgItem(hWnd,IDC_HIDEIP_IPSTATE);
						////::SetWindowText(StateWnd,"正在连接服务器，请稍等...");//
						DWORD dwThreadId; 
						{
							hThread= CreateThread(NULL,0,DownloadAndQueryFile,
								(void*)hWnd,0,&dwThreadId);
						}
						HWND hStopHide = GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH);
						ShowWindow(hStopHide,SW_SHOW);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH),1);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_HIDENOW),0);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_ADVANCED),0);
					}
					break;
				case IDC_HIDEIP_RESTOREIP://恢复
					{
						//restore();
						if(g_RestoreHandle == NULL)
						{
							g_RestoreHandle = (HANDLE)_beginthreadex(NULL,
								0,RestoreIPProc,NULL,0,NULL);
							if(g_RestoreHandle == INVALID_HANDLE_VALUE)
							{
								g_RestoreHandle = NULL;
								return 0;
							}
						}
						
						HWND hHideIP = GetDlgItem(hWnd,IDC_HIDEIP_HIDENOW);
						ShowWindow(hHideIP,SW_SHOW);
						HWND hRestoreIP = GetDlgItem(hWnd,IDC_HIDEIP_RESTOREIP);
						ShowWindow(hRestoreIP,SW_HIDE);




					}
					break;
				case IDC_HIDEIP_SEARCHFAIL:
					{
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH),0);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_HIDENOW),1);
						HWND StateWnd=::GetDlgItem(hWnd,IDC_HIDEIP_IPSTATE);
						
						StopSearch();
						::SendMessage(ProGressWnd,PBM_SETPOS,0,0);
						HWND hStopHide = GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH);
						ShowWindow(hStopHide,SW_HIDE);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_ADVANCED),1);

					}
					break;
				case IDC_HIDEIP_STOPSEARCH:
					{
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH),0);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_HIDENOW),1);
						HWND StateWnd=::GetDlgItem(hWnd,IDC_HIDEIP_IPSTATE);
						
						//::SetWindowText(StateWnd,_T("状态：停止搜索"));
						{
							wchar_t * pText = new wchar_t[1024];
							memset(pText,0,1024*sizeof(wchar_t));
							::SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_STATESTOPSEARCH",pText,L"State:Stop Search"));
							SetDlgItemText(hAdvWnd,IDC_HIDEIP_ADV_IPSTATE,pText);
							delete [] pText;
						}
						StopSearch();
						::SendMessage(ProGressWnd,PBM_SETPOS,0,0);
						HWND hStopHide = GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH);
						ShowWindow(hStopHide,SW_HIDE);
						::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_ADVANCED),1);

					}
					break;
				}
			}

	}
	return false;
}
/****************************************
函数说明：测试代理服务器是否工作
*****************************************/
bool UseProxyDlg::TestProxy(ProxyInfo param)
{
	_TCHAR buf[20];
	::ZeroMemory(buf,sizeof(buf));
	_tcscpy(buf,param.kind);
	::_tcsupr(buf);
		DWORD dwThreadId; 
	if(_tcscmp(buf,_T("HTTP"))==0)
	{
		TestProxyInfo* temp=new TestProxyInfo;
		::ZeroMemory(temp,sizeof(TestProxyInfo));
		char ansiBuf1[255],ansiBuf2[255];
		::ZeroMemory(ansiBuf1,sizeof(ansiBuf1));
		::ZeroMemory(ansiBuf2,sizeof(ansiBuf2));
		WideCharToMultiByte (CP_ACP, 0, param.HostIp, -1, ansiBuf1, sizeof(ansiBuf1), NULL, NULL);
		WideCharToMultiByte (CP_ACP, 0, param.port, -1, ansiBuf2, sizeof(ansiBuf2), NULL, NULL);
		temp->s=TCPconnect(ansiBuf1,ansiBuf2);
		hThread_testpro = CreateThread(NULL,0,TestHttpProxy,(void*)temp,0,&dwThreadId);  
		DWORD WaitResult=WaitForSingleObject(hThread_testpro,MAXTIME);
		if(WAIT_OBJECT_0==WaitResult)
		{
			if(strstr(temp->buf,HTTPRESPOND)!=0)
			{
			::closesocket(temp->s);
			CloseHandle(hThread_testpro);
			hThread_testpro = NULL;
			delete temp;
			return 1;
			}
		}
		else
		{
			DWORD ExitCode=0;
			GetExitCodeThread(hThread_testpro,&ExitCode);
			TerminateThread(hThread_testpro,ExitCode);
			hThread=0;
		}
		//TestHttpProxy(temp);  
			::closesocket(temp->s);
			CloseHandle(hThread_testpro);
			hThread_testpro = NULL;
			delete temp;
			return 0;
	}
	else if(_tcscmp(buf,_T("SOCKS4"))==0||_tcscmp(buf,_T("SOCKS5"))==0)
	{
		TestProxyInfo* temp=new TestProxyInfo;
		::ZeroMemory(temp,sizeof(TestProxyInfo));
		char ansiBuf1[255],ansiBuf2[255];
		::ZeroMemory(ansiBuf1,sizeof(ansiBuf1));
		::ZeroMemory(ansiBuf2,sizeof(ansiBuf2));
		WideCharToMultiByte (CP_ACP, 0, param.HostIp, -1, ansiBuf1, sizeof(ansiBuf1), NULL, NULL);
		WideCharToMultiByte (CP_ACP, 0, param.port, -1, ansiBuf2, sizeof(ansiBuf2), NULL, NULL);
		temp->s=TCPconnect(ansiBuf1,ansiBuf2);
		//TestSockProxy(temp);  
		hThread_testpro = CreateThread(NULL,0,TestSockProxy,(void*)temp,0,&dwThreadId);  
		DWORD WaitResult=WaitForSingleObject(hThread_testpro,MAXTIME);
		if(WAIT_OBJECT_0==WaitResult)
		{
			if(SOCKRESPOND2==*(temp->buf))
			{
				::closesocket(temp->s);
				CloseHandle(hThread_testpro);
				hThread_testpro = NULL;
				delete temp;
				return 1;
			}
		}
		else
		{
			DWORD ExitCode=0;
			GetExitCodeThread(hThread_testpro,&ExitCode);
			int IsSuccess=TerminateThread(hThread_testpro,ExitCode);
			hThread_testpro=0;
			IsSuccess=IsSuccess;
		}
			::closesocket(temp->s);
			CloseHandle(hThread_testpro);
			hThread_testpro = NULL;
			delete temp;
			return 0;
	}
return 0;
}
//测试并把无用的代理去掉
DWORD WINAPI UseProxyDlg::SetProxyList(LPVOID pParam)
{
	ProxyInfo* temp=&ProxyInfoHead;
	int i=0;
	_TCHAR buf[255];
	while(temp->next)
	{

		


		::ZeroMemory(buf,sizeof(buf));
		//_tcscat(buf,_T("状态：正在测试服务器--"));
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024*sizeof(wchar_t));
		_tcscat(buf,LoadItemStringW(L"IDS_HIDEIP_STATETESTINGSERVER",
			pText,L"State:Testing Server"));
		delete [] pText;
		_tcscat(buf,temp->next->HostIp);
		_tcscat(buf,_T("："));
		_tcscat(buf,temp->next->port);
		SetState((HWND)pParam,buf);

		


		if(!TestProxy(*(temp->next)))
		{
			ProxyInfo* temp2=temp->next;
			temp->next=temp2->next;
			delete temp2;
			continue;
		}

		


		AddOneProxyToList(temp->next,i,hAdvWnd);
		if(pParam==hGenWnd)
		{
			//::SendMessage(hMainWnd,FINISHANDUSEPROXY,0,0);
			//ListView_SetItemState(g_IPList,0,LVIS_SELECTED,LVIS_SELECTED);



			char buf1[100],buf2[100],buf3[100];
			::ZeroMemory(buf1,sizeof(buf1));
			::ZeroMemory(buf2,sizeof(buf2));
			::ZeroMemory(buf3,sizeof(buf3));
			WideCharToMultiByte(CP_ACP, 0,temp->next->HostIp , -1, buf1, sizeof(buf1), NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0,temp->next->port , -1, buf2, sizeof(buf2), NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0,temp->next->kind , -1, buf3, sizeof(buf3), NULL, NULL);

			


			UseProxy(buf1,buf2,buf3);
			

			//::SendMessage(hAdvWnd,WM_COMMAND,IDC_HIDEIP_USETHISPROXY,0);
			//SetState((HWND)pParam,_T("已经找到最快的服务器，正在设置为您的电脑默认值！"));
			wchar_t * pText = new wchar_t[1024];
			memset(pText,0,1024*sizeof(wchar_t));
			SetState((HWND)pParam,LoadItemStringW(L"IDS_HIDEIP_SETTINGPROXY",
				pText,L"State:ALREADYFOUNDPROXY,SETTING NOW"));
			delete [] pText;



			::Sleep(1000);
			break;
		}
		temp=temp->next;
		i ++;
		if(i>39)
		{
			break;
		}
	}

	if(pParam!=hGenWnd)
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024*sizeof(wchar_t));
		SetState((HWND)pParam,LoadItemStringW(L"IDS_HIDEIP_SEARCHSUCCESS",
			pText,L"State:IDS_HIDEIP_SEARCHSUCCESS"));
		delete [] pText;

		SendMessage(hAdvWnd,WM_COMMAND,IDC_HIDEIP_ADV_SEARCHFAIL,NULL);
		return 1;
	}			


	wchar_t * pText = new wchar_t[1024];
	memset(pText,0,1024*sizeof(wchar_t));
	SetState((HWND)pParam,LoadItemStringW(L"IDS_HIDEIP_ALREADYFOUNDPROXY",
		pText,L"State:ALREADYFOUNDPROXY!"));
	delete [] pText;

	hThread=0;
	::EnableWindow(GetDlgItem((HWND)pParam,IDC_HIDEIP_STOPSEARCH),0);
	::EnableWindow(GetDlgItem((HWND)pParam,IDC_HIDEIP_HIDENOW),1);
	::EnableWindow(GetDlgItem((HWND)pParam,IDC_HIDEIP_SEARCHNOW),1);




		





	pText = new wchar_t[1024];
	memset(pText,0,1024*sizeof(wchar_t));

	wcscpy(pText,temp->next->HostIp);
	wcscat(pText,L" : ");
	wcscat(pText,temp->next->port);
	
	wcscat(pText,L"  ");

	wchar_t * pArea = new wchar_t[1024];
	memset(pArea,0,1024);

	wcscat(pText,LoadItemStringW(L"IDS_HIDEIP_AREA",pArea,L" area:"));
	delete [] pArea;
	pArea = NULL;
	
	wcscat(pText,L"  ");
	wcscat(pText,temp->next->area);
	SetState((HWND)pParam,pText);
	delete [] pText;

	//wcscpy(pText,L"当前 IP 地址信息：");

	

		




	
	HWND hBtn_RestoreIP = GetDlgItem((HWND)pParam,IDC_HIDEIP_RESTOREIP);
	ShowWindow(hBtn_RestoreIP,SW_SHOW);

	HWND hBtn_HideIP = GetDlgItem((HWND)pParam,IDC_HIDEIP_HIDENOW);
	ShowWindow(hBtn_HideIP,SW_HIDE);
	
	HWND hBtn_StopIP = GetDlgItem((HWND)pParam,IDC_HIDEIP_STOPSEARCH);
	ShowWindow(hBtn_StopIP,SW_HIDE);

	HWND hBtn_Advanced = GetDlgItem((HWND)pParam,IDC_HIDEIP_ADVANCED);
	EnableWindow(hBtn_Advanced,1);


	CloseHandle(hThread_setproxylist);
	hThread_setproxylist = NULL;

	return 1;
}

DWORD WINAPI UseProxyDlg::TestHttpProxy(LPVOID pParam)
{
		TestProxyInfo* temp=(TestProxyInfo*)pParam;
		SOCKET s1=temp->s;
		if(!s1) 
		{
			return 0;
		}
		char buf[255],read_buf[255];
		::ZeroMemory(buf,sizeof(buf));
		::ZeroMemory(read_buf,sizeof(read_buf));
		strcpy(buf,HTTPHEAD);
		int RecvNo;





	try//向服务器发送下载请求
	{

		int result=::send(s1,(LPSTR)buf,strlen(buf)+1,0);
		if(SOCKET_ERROR == result) return 0;

		// 连接失败 
	}
	catch(int i)
	{
		throw i;
		return 0;
	}    




	try
	{//接收应答
		RecvNo=recv(s1,(LPSTR)read_buf,sizeof(read_buf),0);   
	}
	catch(int i)
	{
		throw i;
		return 0;
	}
	::strcpy(temp->buf,read_buf);
return 1;
}

DWORD WINAPI UseProxyDlg::TestSockProxy(LPVOID pParam)
{
		TestProxyInfo* temp=(TestProxyInfo*)pParam;
		SOCKET s1=temp->s;
		if(!s1) 
		{
			return 0;
		}







		char buf[255],read_buf[255];
		::ZeroMemory(buf,sizeof(buf));
		::ZeroMemory(read_buf,sizeof(read_buf));
		strcpy(buf,SOCKHEAD1);
		int RecvNo;
	try//向服务器发送下载请求
	{

		int result=::send(s1,(LPSTR)buf,strlen(buf)+1,0);
		if(SOCKET_ERROR == result)		return 0;





		// 连接失败 
	}
	catch(int i)
	{
		throw i;
		return 0;
	}    
	try
	{//接收应答
		RecvNo=recv(s1,(LPSTR)read_buf,sizeof(read_buf),0);   




	}
	catch(int i)
	{
		throw i;
		return 0;
	}
	if(strcmp(read_buf,SOCKRESPOND1)!=0)
	{
		return 0;
	}
//第二波应答
		::ZeroMemory(buf,sizeof(buf));
		::ZeroMemory(read_buf,sizeof(read_buf));
		::memcpy((LPSTR)buf,SOCKHEAD2,sizeof(SOCKHEAD2));





	try//向服务器发送下载请求
	{

		int result=::send(s1,(LPSTR)buf,10,0);
		if(SOCKET_ERROR == result)		return 0;

		// 连接失败 
	}
	catch(int i)
	{
		throw i;
		return 0;
	}    




	try
	{//接收应答
		RecvNo=recv(s1,(LPSTR)read_buf,sizeof(read_buf),0);   
	}
	catch(int i)
	{
		throw i;
		return 0;
	}
	::strcpy(temp->buf,read_buf);


return 0;
}
/*********************************************************
函数说明：纯API线程下载文件，并保存到当前目录下的同名文件。
参数说明：QueryInfoParam类型，指明了sockt和下载的文件名，主机名就不指定了，直接在连接sock时指定。
/*********************************************************/
DWORD WINAPI UseProxyDlg::DownLoadFile(LPVOID pParam)
{
	const QueryInfoParam* ptemp=(QueryInfoParam*)pParam;
	SOCKET s=ptemp->s;
	HANDLE DllFile1=0;
	const int BUFLEN=0x10000;
	char buf[BUFLEN];
	char read_buf[BUFLEN];
	::ZeroMemory(buf,sizeof(buf));
	::ZeroMemory(read_buf,sizeof(read_buf));

	char buf2[100];
	::ZeroMemory(buf2,sizeof(buf2));
	WideCharToMultiByte(CP_ACP, 0,ptemp->FilePath , -1, buf2, sizeof(buf2), NULL, NULL);

	CHECK_DOWNLOAD_EXIT


	::strcpy(buf,GETFILESTR1);
	::strcat(buf,buf2);
	::strcat(buf,GETFILESTR2);
	int RecvNo=0;
    try//向服务器发送下载请求
    {

		int result=::send(s,buf,strlen(buf),0);
		if(SOCKET_ERROR == result) 		EXITSOCK(s)
		// 连接失败 
    
	}
    catch(int i)
    {
        throw i;
    }    







    try
    {//接收第一波
		RecvNo=recv(s,read_buf,sizeof(read_buf),0);   
	}
    catch(int i)
    {
        throw i;
    }




	char* str_temp1=::strstr(read_buf,("\r\n\r\n"));
	if(str_temp1<=0) EXITSOCK(s)
	str_temp1+=strlen(("\r\n\r\n"));
	_TCHAR strMaMainDll[100];
	::ZeroMemory(strMaMainDll,sizeof(strMaMainDll));
	GetTempPath(100,strMaMainDll);
	if(_T('\\')!=strMaMainDll[_tcslen(strMaMainDll)-1]) strMaMainDll[_tcslen(strMaMainDll)]=_T('\\');//最后一个"\\"改为0

	//::GetCurrentDir(strMaMainDll,100);
	::_tcscat(strMaMainDll,ptemp->FileName);




	DllFile1=::CreateFile(strMaMainDll,FILE_ALL_ACCESS,FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,NULL,NULL);
	if(!DllFile1 || -1==(DWORD)DllFile1) EXITSOCK(s)
	RecvNo=RecvNo+(read_buf-str_temp1);




	do
	{   
		DWORD WriteNo=0;
		if(!WriteFile(DllFile1,str_temp1,RecvNo,&WriteNo,NULL))
		{
			::CloseHandle(DllFile1);
			DllFile1=0;
			::DeleteFile(strMaMainDll);
			EXITSOCK(s)
		}




		try
		{
			RecvNo=recv(s,(LPSTR)read_buf,sizeof(read_buf),0);
		}
		catch(int i)
		{
			throw i;
		}
		str_temp1=read_buf;




	}while(RecvNo>0);

	if(DllFile1)
	{
		::CloseHandle(DllFile1);
		DllFile1=0;
	}
	closesocket(s);
	delete (void*)ptemp;
	return 0;
}

int UseProxyDlg::GetCurrentDir(_TCHAR * str,int strlength)
{
	_TCHAR buf[255];
	::GetModuleFileName(NULL,buf,sizeof(buf));
	while(buf[_tcslen(buf)-1]!=_T('\\') && _tcslen(buf)>0)
	{
		buf[_tcslen(buf)-1]=NULL;
	}
	//容错
	if(strlength<_tcslen(buf)) return 0;

	_tcscpy(str,buf);
	return _tcslen(buf);
}

//sock连接封装函数
 SOCKET   UseProxyDlg::sockconnect(const char*host,const char*service,const char *transport)
 {
 	struct hostent *phe;      /*host information     */
 	struct servent *pse;      /* server information  */ 
 	struct protoent *ppe;     /*protocol information */
 	struct sockaddr_in sin;   /*endpoint IP address  */
 	int s, type;              /* socket description  */

 	memset(&sin,0,sizeof(sin));
 	sin.sin_family=AF_INET;
 	
 	/**** get port number from  service  ****/
 	if( pse=getservbyname((LPSTR)service,(LPSTR)transport) )
 	      sin.sin_port=pse->s_port;                             /*  port */  
 	else if( (sin.sin_port=htons( (u_short)atoi(service)))==0)
 	{
 	   //WSACleanup();
 	   //exit(1);
		return 0;
    }
        
        /**** get IP address from  host name ****/ 
 	if(phe=gethostbyname(host)) 
  	      memcpy(&sin.sin_addr,phe->h_addr,phe->h_length);         /*  host IP address  */
	 else if( (sin.sin_addr.s_addr=inet_addr(host))==INADDR_NONE )
  	{
 	   //WSACleanup();
 	   //exit(1);
    		return 0;
 
	}
        
        /**** get protocol number  from protocol name  ****/
 	if( (ppe=getprotobyname(transport))==0)
 	{
 	   //WSACleanup();
 	   //exit(1);
				return 0;

        }
 
       
 	if(strcmp(transport,("udp"))==0)
    	  type=SOCK_DGRAM;
 	else
    	 type=SOCK_STREAM;    


        /**** creat a socket description ****/ 
	s=socket(PF_INET,type,ppe->p_proto);
	
 	if( s==INVALID_SOCKET)
 	{
 	   //WSACleanup();
 	   //exit(1);
    		return 0;
	}
 
 	if(  connect(s,(struct sockaddr *)&sin,sizeof(sin))==SOCKET_ERROR  )
  	{
 	   //WSACleanup();
 	   //exit(1);
 		return 0;
   }

 	return s; 
 }
SOCKET UseProxyDlg::TCPconnect(const char *host,const char *service)
{
  return  sockconnect(host,service,("tcp"));   
}

DWORD WINAPI UseProxyDlg::DownloadAndQueryFile(LPVOID pParam)
{	
	HWND ListWnd=::GetDlgItem(hAdvWnd,IDC_HIDEIP_IPLIST);
	::SendMessage(ListWnd,LVM_DELETEALLITEMS,0,0);

	HWND StateWnd=::GetDlgItem((HWND)pParam,IDC_HIDEIP_IPSTATE);
	if((HWND)pParam == hAdvWnd)
	{
		StateWnd = GetDlgItem((HWND)pParam,IDC_HIDEIP_ADV_IPSTATE);
	}
	//::SetWindowText(StateWnd,_T("状态：正在连接服务器，请稍等..."));
	wchar_t * pText = new wchar_t[1024];
	memset(pText,0,1024*sizeof(wchar_t));
	SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_CONNECTINGSERVER",pText,
		L"State:CONNECTINGSERVER,PLEASEWAIT"));
	delete [] pText;








	g_CompleteFlag1 = 1;
	g_CompleteFlag2 = 1;
	g_CompleteFlag3 = 1;
	g_CompleteFlag4 = 1;

	g_ExitDownloadThread = 0;

//下载网页
		SOCKET s1,s2,s3,s4;
		s1=TCPconnect(HOST,SERVICE);



		s2=TCPconnect(HOST,SERVICE);


		s3=TCPconnect(HOST,SERVICE);



		s4=TCPconnect(HOST,SERVICE);


		if(s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0)
		{
			if((HWND)pParam == hAdvWnd)
			{
				wchar_t * pText = new wchar_t[1024];
				memset(pText,0,1024*sizeof(wchar_t));
				SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_CANNOTCONNECTSERVER",
					pText,L"State:CANNOTCONNECTSERVER"));
				delete [] pText;
				SendMessage(hAdvWnd,WM_COMMAND,IDC_HIDEIP_ADV_SEARCHFAIL,NULL);
			}
			else
			{
				wchar_t * pText = new wchar_t[1024];
				memset(pText,0,1024*sizeof(wchar_t));
				SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_CANNOTCONNECTSERVER",
					pText,L"State:CANNOTCONNECTSERVER"));
				delete [] pText;
				SendMessage(hGenWnd,WM_COMMAND,IDC_HIDEIP_SEARCHFAIL,NULL);

			}

			return 0;
		}

		DWORD dwThreadId; 
		//第一个网页
		if(s1) 
		{
		::QueryInfoParam *ParamTemp1=new QueryInfoParam;
		::ZeroMemory(ParamTemp1,sizeof(QueryInfoParam));
		ParamTemp1->s=s1;
		::_tcscpy(ParamTemp1->FilePath,FILEPATH);
		::_tcscat(ParamTemp1->FilePath,FILENAME1);
		::_tcscpy(ParamTemp1->FileName,FILENAME1);
		
		hThread1 = CreateThread(NULL,0,DownLoadFile,(void*)ParamTemp1,0,&dwThreadId);
		}




		//第二个网页
		if(s2)
		{
		::QueryInfoParam *ParamTemp2=new QueryInfoParam;
		::ZeroMemory(ParamTemp2,sizeof(QueryInfoParam));
		ParamTemp2->s=s2;
		::_tcscpy(ParamTemp2->FilePath,FILEPATH);
		::_tcscat(ParamTemp2->FilePath,FILENAME2);
		::_tcscpy(ParamTemp2->FileName,FILENAME2);
		hThread2 = CreateThread(NULL,0,DownLoadFile,(void*)ParamTemp2,0,&dwThreadId);  
		}
		//第三个网页



		if(s3)
		{
			::QueryInfoParam *ParamTemp3=new QueryInfoParam;
			::ZeroMemory(ParamTemp3,sizeof(QueryInfoParam));
			ParamTemp3->s=s3;
			::_tcscpy(ParamTemp3->FilePath,FILEPATH);
			::_tcscat(ParamTemp3->FilePath,FILENAME3);
			::_tcscpy(ParamTemp3->FileName,FILENAME3);
			hThread3 = CreateThread(NULL,0,DownLoadFile,(void*)ParamTemp3,0,&dwThreadId);  
		}
		//第四个网页



		if(s4)
		{
			::QueryInfoParam *ParamTemp4=new QueryInfoParam;
			::ZeroMemory(ParamTemp4,sizeof(QueryInfoParam));
			ParamTemp4->s=s4;
			::_tcscpy(ParamTemp4->FilePath,FILEPATH);
			::_tcscat(ParamTemp4->FilePath,FILENAME4);
			::_tcscpy(ParamTemp4->FileName,FILENAME4);
			hThread4 = CreateThread(NULL,0,DownLoadFile,(void*)ParamTemp4,0,&dwThreadId);  
		}



		DWORD state=1;
		if(s1)
		{
			state=::WaitForSingleObject(hThread1,MAXTIME);
			if(ERROR_SUCCESS!=state)//信息文件没有下载全
			{
				g_CompleteFlag1 = 0;
			}
			CloseHandle(hThread1);
			hThread1 = NULL;
		}



		if(s2)
		{
			state=::WaitForSingleObject(hThread2,MAXTIME);
			if(ERROR_SUCCESS!=state)//信息文件没有下载全
			{
				g_CompleteFlag2 = 0;
			}
			CloseHandle(hThread2);
			hThread2 = NULL;
		}



		if(s3)
		{
			state=::WaitForSingleObject(hThread3,MAXTIME);
			if(ERROR_SUCCESS!=state)//信息文件没有下载全
			{
				g_CompleteFlag3 = 0;
			}
			CloseHandle(hThread3);
			hThread3 = NULL;

		}



		if(s4)
		{
			state=::WaitForSingleObject(hThread4,MAXTIME);
			if(ERROR_SUCCESS!=state)//信息文件没有下载全
			{
				g_CompleteFlag4 = 0;
			}
			CloseHandle(hThread4);
			hThread4 = NULL;

		}

		if(g_CompleteFlag1 == 0 && g_CompleteFlag2 == 0 && g_CompleteFlag3 == 0 && 
			g_CompleteFlag4 == 0 )
		{
			if((HWND)pParam == hAdvWnd)
			{
				wchar_t * pText = new wchar_t[1024];
				memset(pText,0,1024*sizeof(wchar_t));
				SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_CANNOTCONNECTSERVER",
					pText,L"State:CANNOTCONNECTSERVER"));
				delete [] pText;
				SendMessage(hAdvWnd,WM_COMMAND,IDC_HIDEIP_ADV_SEARCHFAIL,NULL);
			}
			else
			{
				wchar_t * pText = new wchar_t[1024];
				memset(pText,0,1024*sizeof(wchar_t));
				SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_CANNOTCONNECTSERVER",
					pText,L"State:CANNOTCONNECTSERVER"));
				delete [] pText;
				SendMessage(hGenWnd,WM_COMMAND,IDC_HIDEIP_SEARCHFAIL,NULL);

			}
			return 0;
		}


//下载完毕
		{
			wchar_t * pText = new wchar_t[1024];
			memset(pText,0,1024*sizeof(wchar_t));
			SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_CONNECTEDSERVER",
				pText,L"State:CONNECTEDSERVER"));
			delete [] pText;
			pText = NULL;
		}


		int QueryFlag1 = 0, QueryFlag2 = 0, QueryFlag3 = 0, QueryFlag4 = 0;

		_TCHAR FileBuf[255];
		::ZeroMemory(FileBuf,sizeof(FileBuf));
		_tcscpy(FileBuf,FILENAME1);



		if(g_CompleteFlag1 == 1)
		{
			QueryFlag1 = QueryFile(FileBuf);
		}



		::SendMessage(ProGressWnd,PBM_STEPIT,0,0);
		::ZeroMemory(FileBuf,sizeof(FileBuf));
		_tcscpy(FileBuf,FILENAME2);
		::SendMessage(ProGressWnd,PBM_STEPIT,0,0);
		::ZeroMemory(FileBuf,sizeof(FileBuf));
		_tcscpy(FileBuf,FILENAME3);
		if(g_CompleteFlag3 == 1)
		{
			QueryFlag3 = QueryFile(FileBuf);
		}



		if(g_CompleteFlag2 == 1)
		{
			QueryFlag2 = QueryFile(FileBuf);
		}



		::SendMessage(ProGressWnd,PBM_STEPIT,0,0);
		::ZeroMemory(FileBuf,sizeof(FileBuf));
		_tcscpy(FileBuf,FILENAME4);
		if(g_CompleteFlag4 == 1)
		{
			QueryFlag4 = QueryFile(FileBuf);
		}


		if(QueryFlag1 == 0 && QueryFlag2 == 0 && QueryFlag3 == 0 && QueryFlag4 == 0)
		{
			if((HWND)pParam == hAdvWnd)
			{
				wchar_t * pText = new wchar_t[1024];
				memset(pText,0,1024*sizeof(wchar_t));
				SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_QUERYFILEFAILRE",
					pText,L"State:IDS_HIDEIP_QUERYFILEFAILRE"));
				delete [] pText;
				SendMessage(hAdvWnd,WM_COMMAND,IDC_HIDEIP_ADV_SEARCHFAIL,NULL);
			}
			else
			{
				wchar_t * pText = new wchar_t[1024];
				memset(pText,0,1024*sizeof(wchar_t));
				SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_QUERYFILEFAILRE",
					pText,L"State:IDS_HIDEIP_QUERYFILEFAILRE"));
				delete [] pText;
				SendMessage(hGenWnd,WM_COMMAND,IDC_HIDEIP_SEARCHFAIL,NULL);

			}

			return 0;
		}

		//::SetWindowText(StateWnd,_T("状态：连接到服务器，正在读取代理服务器的数据！"));
		pText = new wchar_t[1024];
		memset(pText,0,1024*sizeof(wchar_t));
		SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_READINGDATA",pText,L"State:READINGDATA"));
		delete [] pText;
		::SendMessage(ProGressWnd,PBM_STEPIT,0,0);



		g_ExitSetProxyThread = 0;
		hThread_setproxylist = CreateThread(NULL,0,SetProxyList,pParam,0,&dwThreadId);


		CloseHandle(hThread);
		hThread = NULL;

		return 0;
}

int UseProxyDlg::QueryFile(const _TCHAR* FileName)
{
	_TCHAR FileNameTemp[255];
	::ZeroMemory(FileNameTemp,sizeof(FileNameTemp));
	_tcscat(FileNameTemp,FileName);

	_TCHAR buf[255];
	::ZeroMemory(buf,sizeof(buf));
	GetTempPath(255,buf);
	if(_T('\\')!=buf[_tcslen(buf)-1]) buf[_tcslen(buf)]=_T('\\');//最后一个"\\"改为0
	_tcscat(buf,FileNameTemp);
	char* ImageBase=(char*)LoadFile(buf);
	if(!ImageBase) 
	{
		//::MessageBox(NULL,_T("无法读取数据"),_T("读取出错"),0);
		//HDMessageBoxW(L"IDS_HIDEIP_CANNOTREADINGDATA",L"State:CANNOTREADINGDATA");
		return 0;
	}
	::strlwr(ImageBase);
	char* TableBegin=strstr(ImageBase,TABLEHEADER1);
	if(TableBegin == NULL)
	{
		return 0;
	}
	char* StrBegin=TableBegin;
	char* TableEnd=strstr(TableBegin,TABLEEND);
	if(-1==(DWORD)StrBegin || !StrBegin)
		return 0;//没有该类型的代理
	StrBegin+=strlen(TABLEHEADER1);
	StrBegin=strstr(StrBegin,TR1);
	char* StrEnd=strstr(StrBegin,TR2);
	ProxyInfo* ProxyInfoEnd=&ProxyInfoHead;
	while(ProxyInfoEnd->next)
	{
		ProxyInfoEnd=ProxyInfoEnd->next;
	}


while(StrBegin<TableEnd || !StrBegin)
{
	//IP
	StrBegin=strstr(StrBegin,TD1);
	if(!StrBegin) break;
	StrBegin=strstr(StrBegin+1,TD1);
	char* StrEnd=strstr(StrBegin,TD2);

	ProxyInfoEnd->next=new ProxyInfo;
	ProxyInfoEnd=ProxyInfoEnd->next;
	::ZeroMemory(ProxyInfoEnd,sizeof(ProxyInfo));
	char* CheckDocIsEnd=strstr(StrBegin,DOC);
	char* DocTdEnd=strstr(StrBegin,TD2);
	while(CheckDocIsEnd<DocTdEnd && CheckDocIsEnd)
	{
		StrBegin=strstr(StrBegin,DOC);
		StrBegin+=strlen(DOC);
		StrEnd=strstr(StrBegin,DOCEND);
		char buf[80];
		::ZeroMemory(buf,sizeof(buf));
		::memcpy(buf,StrBegin,StrEnd-StrBegin);
		_TCHAR buf2[80];
		::ZeroMemory(buf2,sizeof(buf2));
		MultiByteToWideChar (CP_ACP, 0, buf, -1, buf2, sizeof(buf2));
		_tcscat(ProxyInfoEnd->HostIp,buf2);
		CheckDocIsEnd=strstr(StrBegin,DOC);
	}
	//port
	StrBegin=strstr(StrBegin,TD1);
	StrBegin=strstr(StrBegin,HtmlEndCode);
	StrBegin+=strlen(HtmlEndCode);
	StrEnd=strstr(StrBegin,TD2);
	char portBuf[255];
	::ZeroMemory(portBuf,sizeof(portBuf));
	::memcpy(portBuf,StrBegin,StrEnd-StrBegin);
	MultiByteToWideChar (CP_ACP, 0, portBuf, -1, ProxyInfoEnd->port, sizeof(ProxyInfoEnd->port));
//kind
	StrBegin=strstr(StrBegin,TD1);
	StrBegin=strstr(StrBegin,HtmlEndCode);
	StrBegin+=strlen(HtmlEndCode);
	StrEnd=strstr(StrBegin,TD2);
	::ZeroMemory(portBuf,sizeof(portBuf));
	::memcpy(portBuf,StrBegin,StrEnd-StrBegin);
	MultiByteToWideChar (CP_ACP, 0, portBuf, -1, ProxyInfoEnd->kind, sizeof(ProxyInfoEnd->kind));
//	::memcpy(ProxyInfoEnd->kind,StrBegin,StrEnd-StrBegin);
//area
	StrBegin=strstr(StrBegin,TD1);
	StrBegin=strstr(StrBegin,HtmlEndCode);
	StrBegin+=strlen(HtmlEndCode);
	StrEnd=strstr(StrBegin,TD2);
	::ZeroMemory(portBuf,sizeof(portBuf));
	::memcpy(portBuf,StrBegin,StrEnd-StrBegin);
	MultiByteToWideChar (CP_ACP, 0, portBuf, -1, ProxyInfoEnd->area, sizeof(ProxyInfoEnd->area));
	DeletePROXYCN(ProxyInfoEnd->area);
//CheckTime
	StrBegin=strstr(StrBegin,TD1);
	StrBegin=strstr(StrBegin,HtmlEndCode);
	StrBegin+=strlen(HtmlEndCode);
	StrEnd=strstr(StrBegin,TD2);
	::ZeroMemory(portBuf,sizeof(portBuf));
	::memcpy(portBuf,StrBegin,StrEnd-StrBegin);
	MultiByteToWideChar (CP_ACP, 0, portBuf, -1, ProxyInfoEnd->CheckTime, sizeof(ProxyInfoEnd->CheckTime));
//speed
	StrBegin=strstr(StrBegin,TD1);
	StrBegin=strstr(StrBegin,HtmlEndCode);
	StrBegin+=strlen(HtmlEndCode);
	StrEnd=strstr(StrBegin,TD2);
	::ZeroMemory(portBuf,sizeof(portBuf));
	::memcpy(portBuf,StrBegin,StrEnd-StrBegin);
	MultiByteToWideChar (CP_ACP, 0, portBuf, -1, ProxyInfoEnd->szSpeed, sizeof(ProxyInfoEnd->szSpeed));
//	::memcpy(ProxyInfoEnd->szSpeed,StrBegin,StrEnd-StrBegin);
	_TCHAR* temp;
	ProxyInfoEnd->speed=::_tcstod(ProxyInfoEnd->szSpeed,&temp);
//whois
	StrBegin=strstr(StrBegin,TD1);
	StrBegin=strstr(StrBegin,HtmlEndCode);
	StrBegin+=strlen(HtmlEndCode);
	while(HtmlBeginCode==*StrBegin)
	{
	StrBegin=strstr(StrBegin,HtmlEndCode);
	StrBegin+=1;
	}
	StrEnd=strstr(StrBegin,TD2);
	::ZeroMemory(portBuf,sizeof(portBuf));
	::memcpy(portBuf,StrBegin,StrEnd-StrBegin);
	MultiByteToWideChar (CP_ACP, 0, portBuf, -1, ProxyInfoEnd->whois, sizeof(ProxyInfoEnd->whois));

//	::memcpy(ProxyInfoEnd->whois,StrBegin,StrEnd-StrBegin);

//循环到下一行
	StrBegin=strstr(StrBegin,TR1);
	StrEnd=strstr(StrBegin,TR2);
}
	FreeFile();
	return 1;
}

_TCHAR* UseProxyDlg::LoadFile(_TCHAR* filename)
{

	if(basepointer!=0)
	{
		::UnmapViewOfFile(basepointer);
		basepointer=0;
	}
	if(hMapping!=0)
	{
		::CloseHandle(hMapping);
		hMapping=0;
	}
	if(hFile!=0) //文件已经打开
	{
		CloseHandle(hFile);
		hFile=0;
	}

	if((hFile=CreateFile(filename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0))==INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("could not open file:");
		return 0;
	}

	if(!(hMapping=CreateFileMapping(hFile,0,PAGE_READWRITE|SEC_COMMIT,0,0,0)))
	{
		//AfxMessageBox("Mapping failed.");
		CloseHandle(hFile);
		return 0;
	}

	if(!(basepointer=(_TCHAR*)::MapViewOfFile(hMapping,FILE_MAP_WRITE,0,0,0)))
	{
		//AfxMessageBox("View failed.");
		::CloseHandle(hMapping);
		::CloseHandle(hFile);
		return 0;
	}
    return basepointer;
}

//把内存中的映像文件释放掉
int UseProxyDlg::FreeFile(void)
{
	if(basepointer!=0)
	{
		::UnmapViewOfFile(basepointer);
		basepointer=0;
	}
	if(hMapping!=0)
	{
		::CloseHandle(hMapping);
		hMapping=0;
	}
	if(hFile!=0) //文件已经打开
	{
		CloseHandle(hFile);
		hFile=0;
	}
	return 1;
}

void UseProxyDlg::OnInitDlg(HINSTANCE hInst,HWND hWnd)
{
	hMainWnd=hWnd;
	WSADATA wsadata;
	if(::WSAStartup(WAVERS,&wsadata)!=0)
	//::MessageBox(hWnd,_T("无法连接网络"),_T("网络出错"),0);
	HDMessageBoxW(L"IDS_HIDEIP_CANNOTCONNECTNETWORK",L"State:CANNOTCONNECTNETWORK");
	hGenWnd=::CreateDialog(hInst,(LPCTSTR)IDD_TAB_HIDEIP_GENERAL,hWnd,(DLGPROC)GenDlgProc);
	g_hGenWnd = hGenWnd;
	::ShowWindow((HWND)hGenWnd,SW_SHOW);
	hAdvWnd=::CreateDialog(hInst,(LPCTSTR)IDD_TAB_HIDEIP_ADVANCE,hWnd,(DLGPROC)AdvDlgProc);
	g_hAdvWnd = hAdvWnd;
	::ShowWindow((HWND)hAdvWnd,SW_HIDE);
	::ZeroMemory(&ProxyInfoHead,sizeof(ProxyInfo));
	ProGressWnd=GetDlgItem((HWND)hWnd,IDC_DOWORK_PROGRESS);
	::SendMessage(ProGressWnd,PBM_SETPOS,0,0);
	hUseInfoWnd=GetDlgItem((HWND)hWnd,IDC_USE_INFO);
	GetSystemProxy(hUseInfoWnd);

	return;
}

void UseProxyDlg::OnInitAdvDlg(HINSTANCE hInst,HWND hWnd)
{
	wchar_t cColumn_ID[256],cColumn_IP[256],cColumn_Port[256],cColumn_Type[256],
		cColumn_Addr[256],cColumn_Begin[256],cColumn_Respon[256];
	::EnableWindow(GetDlgItem(hWnd,IDC_HIDEIP_STOPSEARCH),0);
	HWND ListWnd=::GetDlgItem(hWnd,IDC_HIDEIP_IPLIST);
	g_IPList = ListWnd;
	::SendMessage(ListWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM) LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	int i=0;
	LV_COLUMN temp;
	::ZeroMemory(&temp,sizeof(LV_COLUMN));
	temp.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	temp.cx=50;
	temp.fmt=LVCFMT_LEFT;
	//temp.pszText=_T("序号");

	
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_ID,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_ID,LoadItemStringW(L"IDS_HIDEIPLIST_ID",pHeaderText,L"ID"));
		temp.pszText = cColumn_ID;
		delete [] pHeaderText;
	}
		

	::SendMessage(ListWnd,LVM_INSERTCOLUMN,i++,(LPARAM)&temp);

	::ZeroMemory(&temp,sizeof(LV_COLUMN));
	temp.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	temp.cx=100;
	temp.fmt=LVCFMT_LEFT;
	//temp.pszText=_T("地址");

	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_IP,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_IP,LoadItemStringW(L"IDS_HIDEIPLIST_IP",pHeaderText,L"IP"));
		temp.pszText = cColumn_IP;
		delete [] pHeaderText;
	}
	
	
	::SendMessage(ListWnd,LVM_INSERTCOLUMN,i++,(LPARAM)&temp);

	::ZeroMemory(&temp,sizeof(LV_COLUMN));
	temp.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	temp.cx=100;
	temp.fmt=LVCFMT_LEFT;
	//temp.pszText=_T("端口");

	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Port,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Port,LoadItemStringW(L"IDS_HIDEIPLIST_PORT",pHeaderText,L"Port"));
		temp.pszText = cColumn_Port;
		delete [] pHeaderText;
	}
	
	
	
	
	::SendMessage(ListWnd,LVM_INSERTCOLUMN,i++,(LPARAM)&temp);

	::ZeroMemory(&temp,sizeof(LV_COLUMN));
	temp.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	temp.cx=100;
	temp.fmt=LVCFMT_LEFT;
	//temp.pszText=_T("类型");
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Type,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Type,LoadItemStringW(L"IDS_HIDEIPLIST_TYPE",pHeaderText,L"Type"));
		temp.pszText = cColumn_Type;
		delete [] pHeaderText;
	}
	::SendMessage(ListWnd,LVM_INSERTCOLUMN,i++,(LPARAM)&temp);

	::ZeroMemory(&temp,sizeof(LV_COLUMN));
	temp.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	temp.cx=100;
	temp.fmt=LVCFMT_LEFT;
	//temp.pszText=_T("地区");
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Addr,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Addr,LoadItemStringW(L"IDS_HIDEIPLIST_ADDRESS",pHeaderText,L"Port"));
		temp.pszText = cColumn_Addr;
		delete [] pHeaderText;
	}
	::SendMessage(ListWnd,LVM_INSERTCOLUMN,i++,(LPARAM)&temp);

	::ZeroMemory(&temp,sizeof(LV_COLUMN));
	temp.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	temp.cx=100;
	temp.fmt=LVCFMT_LEFT;
	//temp.pszText=_T("验证时间");
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Begin,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Begin,LoadItemStringW(L"IDS_HIDEIPLIST_BEGIN",pHeaderText,L"Time"));
		temp.pszText = cColumn_Begin;
		delete [] pHeaderText;
	}
	::SendMessage(ListWnd,LVM_INSERTCOLUMN,i++,(LPARAM)&temp);

	::ZeroMemory(&temp,sizeof(LV_COLUMN));
	temp.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	temp.cx=100;
	temp.fmt=LVCFMT_LEFT;
	//temp.pszText=_T("响应");
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Respon,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Respon,LoadItemStringW(L"IDS_HIDEIPLIST_RESPONSE",pHeaderText,L"Response"));
		temp.pszText = cColumn_Respon;
		delete [] pHeaderText;
	}
	::SendMessage(ListWnd,LVM_INSERTCOLUMN,i++,(LPARAM)&temp);

	//if(!ProxyInfoHead.next) 
	//{
	//	::DownloadAndQueryFile();
	//}
return;
}


void UseProxyDlg::SetListColumn()
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle = 0;
	HWND ListWnd=::GetDlgItem(hAdvWnd,IDC_HIDEIP_IPLIST);




	wchar_t cColumn_ID[256],cColumn_IP[256],cColumn_Port[256],cColumn_Type[256],
		cColumn_Addr[256],cColumn_Begin[256],cColumn_Respon[256];
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 50;

	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_ID,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_ID,LoadItemStringW(L"IDS_HIDEIPLIST_ID",pHeaderText,L"ID"));
		lvColumn.pszText = cColumn_ID;
		delete [] pHeaderText;
	}
//	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
	ListView_SetColumn(ListWnd,0,&lvColumn);
	lvColumn.cx    = 100;

	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_IP,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_IP,LoadItemStringW(L"IDS_HIDEIPLIST_IP",pHeaderText,L"IP"));
		lvColumn.pszText = cColumn_IP;
		delete [] pHeaderText;
	}
//	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
	ListView_SetColumn(ListWnd,1,&lvColumn);
	lvColumn.cx   = 100;


	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Port,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Port,LoadItemStringW(L"IDS_HIDEIPLIST_PORT",pHeaderText,L"Port"));
		lvColumn.pszText = cColumn_Port;
		delete [] pHeaderText;
	}



	ListView_SetColumn(ListWnd,2,&lvColumn);
	lvColumn.cx   = 100;



	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Type,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Type,LoadItemStringW(L"IDS_HIDEIPLIST_TYPE",pHeaderText,L"Type"));
		lvColumn.pszText = cColumn_Type;
		delete [] pHeaderText;
	}


	ListView_SetColumn(ListWnd,3,&lvColumn);
	lvColumn.cx   = 100;



	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Addr,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Addr,LoadItemStringW(L"IDS_HIDEIPLIST_ADDRESS",pHeaderText,L"Area"));
		lvColumn.pszText = cColumn_Addr;
		delete [] pHeaderText;
	}

	ListView_SetColumn(ListWnd,4,&lvColumn);
	lvColumn.cx   = 100;



	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Begin,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Begin,LoadItemStringW(L"IDS_HIDEIPLIST_BEGIN",pHeaderText,L"Time"));
		lvColumn.pszText = cColumn_Begin;
		delete [] pHeaderText;
	}


	ListView_SetColumn(ListWnd,5,&lvColumn);
	lvColumn.cx   = 100;



	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Begin,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Begin,LoadItemStringW(L"IDS_HIDEIPLIST_BEGIN",pHeaderText,L"Time"));
		lvColumn.pszText = cColumn_Begin;
		delete [] pHeaderText;
	}


	ListView_SetColumn(ListWnd,6,&lvColumn);
	lvColumn.cx   = 100;




	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(cColumn_Respon,0,256 * sizeof(wchar_t));
		wcscpy(cColumn_Respon,LoadItemStringW(L"IDS_HIDEIPLIST_RESPONSE",pHeaderText,L"Response"));
		lvColumn.pszText = cColumn_Respon;
		delete [] pHeaderText;
	}


	ListView_SetColumn(ListWnd,7,&lvColumn);



//	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumn);
//	ListView_SetColumn(g_TrayList,2,&lvColumn);
//	lvStyle = ListView_GetExtendedListViewStyle(g_TrayList);
//	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
//	ListView_SetExtendedListViewStyleEx(g_TrayList,0,lvStyle);








	return;
}







void UseProxyDlg::SetListContent(HWND hWnd)
{
	//移除原来内容
	HWND ListWnd=::GetDlgItem(hWnd,IDC_HIDEIP_IPLIST);
	::SendMessage(ListWnd,LVM_DELETEALLITEMS,0,0);
	//加入新的内容
	ProxyInfo* temp=ProxyInfoHead.next;
	int i=0;
	while(temp)
	{
		_TCHAR buf[20];
		::ZeroMemory(buf,sizeof(buf));
		_stprintf(buf,_T("%d"),i);
		LV_ITEM item;
		::ZeroMemory(&item,sizeof(LV_ITEM));
		item.mask=LVIF_TEXT;
		item.iItem=i;
		item.iSubItem=0;
		item.pszText=buf;
		::SendMessage(ListWnd,LVM_INSERTITEM,0,(LPARAM)&item);

		int f=1;
		LVITEM lvi;
		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->HostIp;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->port;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->kind;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->area;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->CheckTime;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR)temp->szSpeed;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		i++;
		temp=temp->next;
	}
	return;
}

void UseProxyDlg::ClearStack()
{
	ProxyInfo * temp=ProxyInfoHead.next;
	while(temp)
	{
		ProxyInfoHead.next=temp->next;
		delete temp;
		temp=ProxyInfoHead.next;
	}
	return;
}

void UseProxyDlg::DeletePROXYCN(_TCHAR* str)
{
	if(!str || _tcslen(str)<=0 ) return;
	_tcsupr(str);
	_TCHAR* temp=_tcsstr(str,DELETESTR);
	if(!temp) return;
	int MoMaxLen=_tcslen(str)-(temp-str)-_tcslen(DELETESTR)+1;
	for(int i=0;i<MoMaxLen;i++)
	{
		temp[i]=temp[i+_tcslen(DELETESTR)];
	}
	if(_tcslen(str)==0) 
	{
		//::_tcscpy(str,_T("未知"));
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024*sizeof(wchar_t));
		::_tcscpy(str,LoadItemStringW(L"IDS_HIDEIP_UNKNOWNADDRESS",
			pText,L"State:UNKNOWNADDRESS"));
		delete [] pText;
		pText = NULL;
	}
	return;
}

void UseProxyDlg::SetState(HWND hWnd,_TCHAR* str)
{
		HWND StateWnd=::GetDlgItem((HWND)hWnd,IDC_HIDEIP_IPSTATE);
		if(hWnd == hAdvWnd)
		{
			StateWnd=::GetDlgItem((HWND)hWnd,IDC_HIDEIP_ADV_IPSTATE);
		}
		::SetWindowText(StateWnd,str);
		return ;
}

void UseProxyDlg::AddOneProxyToList(ProxyInfo* temp,int i,HWND hWnd)
{
		HWND ListWnd=::GetDlgItem(hWnd,IDC_HIDEIP_IPLIST);
		_TCHAR buf[20];
		::ZeroMemory(buf,sizeof(buf));
		_stprintf(buf,_T("%d"),i);
		LV_ITEM item;
		::ZeroMemory(&item,sizeof(LV_ITEM));
		item.mask=LVIF_TEXT;
		item.iItem=i;
		item.iSubItem=0;
		item.pszText=buf;
		::SendMessage(ListWnd,LVM_INSERTITEM,0,(LPARAM)&item);

		int f=1;
		LVITEM lvi;
		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->HostIp;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->port;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->kind;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->area;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR) temp->CheckTime;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi.iSubItem = f++;
		lvi.pszText = (LPTSTR)temp->szSpeed;
		::SendMessage(ListWnd, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

	return;
}


unsigned __stdcall UseProxyDlg::RestoreIPProc(void * pParam)
{
	int iShowDlg = 1;
	if(pParam != NULL)
	{
		iShowDlg = *(int *)pParam;
	}
	restore(iShowDlg);

	CloseHandle(g_RestoreHandle);
	g_RestoreHandle = NULL;



	return 1;
}



bool UseProxyDlg::restore(int iShowDlg)
{
	INTERNET_PER_CONN_OPTION_LISTA list;
	INTERNET_PER_CONN_OPTION_LISTW list2;
	DWORD   dwBufSize = sizeof(list);
	DWORD   dwBufSize2 = sizeof(list2);
	
	// Fill out list struct.
	list.dwSize = sizeof(list);
	list2.dwSize = sizeof(list2);
	
	// NULL == LAN, otherwise connectoid name.
	list.pszConnection = NULL;
	list2.pszConnection = NULL;
	
	// Set three options.
	list.dwOptionCount = 4;
	list.pOptions = new INTERNET_PER_CONN_OPTIONA[4];
	list2.dwOptionCount = 4;
	list2.pOptions = new INTERNET_PER_CONN_OPTIONW[4];
	
	// Make sure the memory was allocated.
	if(NULL == list.pOptions)
	{
		//Return FALSE if the memory wasn't allocated.
		return FALSE;
	}
	if(NULL == list2.pOptions)
	{
		//Return FALSE if the memory wasn't allocated.
		return FALSE;
	}
	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT;
	list2.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	list2.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT;
	
	// Set proxy name.
	list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	list.pOptions[1].Value.pszValue = NULL;
	list2.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	list2.pOptions[1].Value.pszValue = NULL;
	
	// Set proxy override.
	list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	list.pOptions[2].Value.pszValue = NULL;
	list2.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	list2.pOptions[2].Value.pszValue = NULL;
	
	list.pOptions[3].dwOption=(INTERNET_PER_CONN_AUTODISCOVERY_FLAGS);  
	list.pOptions[3].Value.ftValue.dwHighDateTime=0x14;
	list2.pOptions[3].dwOption=(INTERNET_PER_CONN_AUTODISCOVERY_FLAGS);  
	list2.pOptions[3].Value.ftValue.dwHighDateTime=0x14;
	
	if(!InternetSetOptionW(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list2, dwBufSize2))
	{
		return FALSE;
	}
	if(!InternetSetOptionA(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize))
	{
		return FALSE;
	}
	RASENTRYNAMEA   Entries[15];
	::ZeroMemory(Entries,sizeof(Entries));
	DWORD   cb,cEntries,dwRet;   
	Entries[0].dwSize=sizeof(RASENTRYNAMEA);   
	cb=sizeof(Entries);   
	cEntries=0;   
	dwRet=RasEnumEntriesA(NULL,   NULL,   &Entries[0],   &cb,   &cEntries);   
	for(int i=0;i<15;i++)
	{
		if(strcmp(Entries[i].szEntryName,"")==0) break;
		WCHAR w_temp[255];
		::ZeroMemory(w_temp,sizeof(w_temp));
		MultiByteToWideChar (CP_ACP, 0, Entries[i].szEntryName, -1, w_temp, sizeof(w_temp));
		list2.pszConnection=w_temp;
		if(!InternetSetOptionW(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list2, dwBufSize2))
		{
			return false;
		}
		list.pszConnection=Entries[i].szEntryName;
		if(!InternetSetOptionA(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize))
		{
			return false;
		}
	}
	if(!InternetSetOptionW(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0))
	{
		return false;
	}
	if(!InternetSetOptionA(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0))
	{
		return false;
	}
// 	::MessageBox(NULL,_T("已经将您的系统恢复!"),_T("恢得成功"),0);
// 	GetSystemProxy(hUseInfoWnd);



	if(iShowDlg == 1)
	{
		HDMessageBoxW(L"IDS_HIDEIP_SYSTEMRESTORE",
			L"State:SYSTEMRESTORE");
	}
	wchar_t * pText = new wchar_t[1024];
	memset(pText,0,1024*sizeof(wchar_t));
	
	SetDlgItemText(hGenWnd,IDC_HIDEIP_IPSTATE,LoadItemStringW(L"IDS_HIDEIP_SYSTEMRESTORE",pText,
		L"State:SYSTEMRESTORE"));
	SetDlgItemText(hAdvWnd,IDC_HIDEIP_ADV_IPSTATE,LoadItemStringW(L"IDS_HIDEIP_SYSTEMRESTORE",pText,
		L"State:SYSTEMRESTORE"));
	delete [] pText;
	
	
	ShowWindow(GetDlgItem(hGenWnd,IDC_HIDEIP_RESTOREIP),SW_HIDE);
	ShowWindow(GetDlgItem(hGenWnd,IDC_HIDEIP_HIDENOW),SW_SHOW);
	GetSystemProxy(hUseInfoWnd);




	return true;
}





// bool UseProxyDlg::restore(int iShowDlg)
// {
// 	HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut; 
// 	if( ERROR_SUCCESS != RegOpenKeyEx(hKeyIn,
// 		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"),
// 		0, KEY_CREATE_LINK | KEY_WRITE | KEY_READ | KEY_NOTIFY, &hKeyOut)) 
// 	{ 
// 		::RegCloseKey(hKeyOut);
// 		return FALSE; 
// 	} 
// 	RegDeleteValue(hKeyOut,_T("ProxyServer"));
// 	RegDeleteValue(hKeyOut,_T("ProxyEnable"));
// 	::RegCloseKey(hKeyOut);
// 	if(!InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0))
// 	{
// 		return FALSE;
// 	}
// 	//::MessageBox(NULL,_T("您的系统已经恢复！"),_T("完成"),0);
// 	if(iShowDlg == 1)
// 	{
// 		//::_tcscpy(str,_T("未知"));
// 		HDMessageBoxW(L"IDS_HIDEIP_SYSTEMRESTORE",
// 			L"State:SYSTEMRESTORE");
// 	}
// 	wchar_t * pText = new wchar_t[1024];
// 	memset(pText,0,1024*sizeof(wchar_t));
// 	
// 	SetDlgItemText(hGenWnd,IDC_HIDEIP_IPSTATE,LoadItemStringW(L"IDS_HIDEIP_SYSTEMRESTORE",pText,
// 		L"State:SYSTEMRESTORE"));
// 	SetDlgItemText(hAdvWnd,IDC_HIDEIP_ADV_IPSTATE,LoadItemStringW(L"IDS_HIDEIP_SYSTEMRESTORE",pText,
// 		L"State:SYSTEMRESTORE"));
// 	delete [] pText;
// 
// 
// 	ShowWindow(GetDlgItem(hGenWnd,IDC_HIDEIP_RESTOREIP),SW_HIDE);
// 	ShowWindow(GetDlgItem(hGenWnd,IDC_HIDEIP_HIDENOW),SW_SHOW);
// 	GetSystemProxy(hUseInfoWnd);
// 		return true; 
// 
// }

bool UseProxyDlg::UseProxy(char* HostIp,char* port,char* PatrolKind)
{
//使设置生效
//****添加代码
    INTERNET_PER_CONN_OPTION_LISTA list;
	INTERNET_PER_CONN_OPTION_LISTW list2;
    DWORD   dwBufSize = sizeof(list);
    DWORD   dwBufSize2 = sizeof(list2);

    // Fill out list struct.
    list.dwSize = sizeof(list);
	list2.dwSize=sizeof(list2);
    // NULL == LAN, otherwise connectoid name.
    list.pszConnection = NULL;
    list2.pszConnection = NULL;

    // Set three options.
    list.dwOptionCount = 3;
    list.pOptions = new INTERNET_PER_CONN_OPTIONA[3];
    list2.dwOptionCount = 3;
    list2.pOptions = new INTERNET_PER_CONN_OPTIONW[3];

    // Make sure the memory was allocated.
    if(NULL == list.pOptions)
    {
        //Return FALSE if the memory wasn't allocated.
        return false;
    }
    if(NULL == list2.pOptions)
    {
        //Return FALSE if the memory wasn't allocated.
        return false;
    }
    // Set flags.
    list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT |PROXY_TYPE_PROXY;
    list2.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list2.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT |PROXY_TYPE_PROXY;

    // Set proxy name.
    list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    list2.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	char buf2[255],patrol[20];
	WCHAR buf22[255],patrol2[20],w_temp[255];
	::ZeroMemory(buf2,sizeof(buf2));
	::ZeroMemory(patrol,sizeof(patrol));
	::ZeroMemory(buf22,sizeof(buf22));
	::ZeroMemory(patrol2,sizeof(patrol2));
	strcpy(patrol,PatrolKind);
	strupr(patrol);
	::ZeroMemory(w_temp,sizeof(w_temp));
	MultiByteToWideChar (CP_ACP, 0, PatrolKind, -1, w_temp, sizeof(w_temp));
	wcscpy(patrol2,w_temp);
	_wcsupr(patrol2);

	if(strcmp(patrol,"SOCKS4")==0 || strcmp(patrol,"SOCKS5")==0) strcpy(buf2,"socks");
	else strcpy(buf2,PatrolKind);
	if(_tcscmp(patrol2,_T("SOCKS4"))==0 || _tcscmp(patrol2,_T("SOCKS5"))==0) _tcscpy(buf22,_T("socks"));
	else 
	{
		_tcscpy(buf22,w_temp);
	}
	strcat(buf2,"=");
	strcat(buf2,HostIp);
	strcat(buf2,":");
	strcat(buf2,port);

	_tcscat(buf22,L"=");
	::ZeroMemory(w_temp,sizeof(w_temp));
	MultiByteToWideChar (CP_ACP, 0, HostIp, -1, w_temp, sizeof(w_temp));
	_tcscat(buf22,w_temp);
	_tcscat(buf22,L":");
	::ZeroMemory(w_temp,sizeof(w_temp));
	MultiByteToWideChar (CP_ACP, 0, port, -1, w_temp, sizeof(w_temp));
	_tcscat(buf22,w_temp);

    list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT|PROXY_TYPE_PROXY;
    list2.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list2.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT|PROXY_TYPE_PROXY;

    // Set proxy name.
    list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    list.pOptions[1].Value.pszValue = buf2;
    list2.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    list2.pOptions[1].Value.pszValue = buf22;

    // Set proxy override.
    list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    list.pOptions[2].Value.pszValue = "local";
    list2.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    list2.pOptions[2].Value.pszValue = L"local";


if(!InternetSetOptionW(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list2, dwBufSize2))
{
return FALSE;
}
if(!InternetSetOptionA(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize))
{
return FALSE;
}

RASENTRYNAMEA   Entries[15];
::ZeroMemory(Entries,sizeof(Entries));
DWORD   cb,cEntries,dwRet;   
Entries[0].dwSize=sizeof(RASENTRYNAMEA);   
//WINDOWSNT4.0(0x400) 0x19c 
//WINDOWS9x/me        0x2A4
//WINDOWS2k           0x2B4
//WINDOWSxp/2003      0x2C0

cb=sizeof(Entries);   
cEntries=0;   
dwRet=RasEnumEntriesA(NULL,   NULL,   &Entries[0],   &cb,   &cEntries);  
if(dwRet!=0) 
{
	char buf[255];
	::ZeroMemory(buf,sizeof(buf));
	::sprintf(buf,"找网络拨号连接出错,错误号:%d",dwRet);
	::MessageBoxA(NULL,buf,"提醒",0);
}
for(int i=0;i<15;i++)
{
if(Entries[i].szEntryName==0 || strcmp(Entries[i].szEntryName,"")==0) 
{
	//::MessageBox(NULL,_T("未找到网络拨号连接"),_T("提醒"),0);
	continue;
}
list.pszConnection=Entries[i].szEntryName;
::ZeroMemory(w_temp,sizeof(w_temp));
MultiByteToWideChar (CP_ACP, 0, Entries[i].szEntryName, -1, w_temp, sizeof(w_temp));
list2.pszConnection=w_temp;
if(!InternetSetOptionW(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list2, dwBufSize2))
{
return false;
}
if(!InternetSetOptionA(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize))
{
return false;
}
}
//****添加结束
 if(!InternetSetOptionW(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0))
 {
  return FALSE;
 }
 if(!InternetSetOptionA(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0))
 {
  return FALSE;
 }
GetSystemProxy(hUseInfoWnd);
return true;
}




// bool UseProxyDlg::UseProxy(char* HostIp,char* port,char* PatrolKind)
// {
// char l_just[128];
// char patrol[20];
// ::ZeroMemory(l_just,sizeof(l_just));
// ::ZeroMemory(patrol,sizeof(patrol));
// strcpy(patrol,PatrolKind);
// strupr(patrol);
// if(strcmp(patrol,"SOCKS4")==0 || strcmp(patrol,("SOCKS5"))==0) strcpy(patrol,("socks"));
// else strcpy(patrol,PatrolKind);
// strcat(l_just,patrol);
// strcat(l_just,("="));
// strcat(l_just,HostIp);
// strcat(l_just,(":"));
// strcat(l_just,port);
// ::strlwr(l_just);
// 
// HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut; 
// if( ERROR_SUCCESS != RegOpenKeyExA(hKeyIn, ("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_CREATE_LINK | KEY_WRITE | KEY_READ | KEY_NOTIFY, &hKeyOut)) 
// { 
// 	::RegCloseKey(hKeyOut);
// 	return FALSE; 
// } 
// ULONG regsize = 0; 
// if(ERROR_SUCCESS != RegQueryValueExA(hKeyOut, ("ProxyServer"), NULL, NULL, NULL, &regsize)) 
// {
// 	if(ERROR_SUCCESS !=::RegSetValueExA(hKeyOut,("ProxyServer"),0,REG_SZ,(LPBYTE)l_just,strlen(l_just)+1))
// 	{
// 		::RegCloseKey(hKeyOut);
// 		return FALSE; 
// 	}
// 	else
// 	{
// 		DWORD temp=1;
//  		if(ERROR_SUCCESS !=::RegSetValueExA(hKeyOut,("ProxyEnable"),0,REG_DWORD,(BYTE*)&temp,sizeof(DWORD)))		return FALSE; 
// 		::RegCloseKey(hKeyOut);
// 		return true;
// 	}
// } 
// char pValue[255];
// memset(pValue, 0x00, sizeof(pValue)); 
// if(ERROR_SUCCESS != RegQueryValueExA(hKeyOut, ("ProxyServer"), NULL, NULL,(LPBYTE)pValue, &regsize)) 
// {
// 	::RegCloseKey(hKeyOut);
// 	return FALSE; 
// } 
// //找到协议
// char* oldproxy=pValue; 
// ::strlwr(oldproxy);
// int pos = 0; 
// pos = (DWORD)strstr(oldproxy,patrol);
// if(-1==pos) 
// { 
// pos = 0; 
// }
// char buf[255];
// ::ZeroMemory(buf,sizeof(buf));
// if(pos!=0)
// {
// 	char* pos1 = 0;
// 	pos1 = strstr((char*)pos,(";"));
// 	if(-1 == (DWORD)pos1 || 0==(DWORD)pos1) 
// 	{ 
// 	pos1 = oldproxy+strlen(oldproxy);
// 	} 
// ::memcpy(buf,oldproxy,((DWORD)pos-(DWORD)oldproxy)*sizeof(char));
// ::strcat(buf,l_just);
// ::strcat(buf,(char*)pos1);
// }
// else
// {
// ::strcat(buf,l_just);
// ::strcat(buf,(";"));
// ::strcat(buf,oldproxy);
// }
// 
// DWORD temp=1;
//  	if(ERROR_SUCCESS !=::RegSetValueExA(hKeyOut,("ProxyEnable"),0,REG_DWORD,(BYTE*)&temp,sizeof(DWORD)))
// 		return FALSE; 
// 	if(ERROR_SUCCESS !=::RegSetValueExA(hKeyOut,("ProxyServer"),0,REG_SZ,(LPBYTE)buf,strlen(buf)+1))
// 		return FALSE; 
// 	::RegCloseKey(hKeyOut);
// //使设置生效
//  if(!InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0))
//  {
//   return FALSE;
//  }
// 
// 
// return true;
// }

void UseProxyDlg::StopSearch()
{
	if(hThread_setproxylist)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(hThread_setproxylist,&ExitCode);
		BOOL IsSuccess=TerminateThread(hThread_setproxylist,ExitCode);
		CloseHandle(hThread_setproxylist);
		hThread_setproxylist = NULL;
	}




	if(hThread_testpro)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(hThread_testpro,&ExitCode);
		BOOL IsSuccess=TerminateThread(hThread_testpro,ExitCode);
		CloseHandle(hThread_testpro);
		hThread_testpro = NULL;
	}


	if(hThread)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(hThread,&ExitCode);
		BOOL IsSuccess=TerminateThread(hThread,ExitCode);
		CloseHandle(hThread);
		hThread = NULL;
	}


	if(hThread1)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(hThread1,&ExitCode);
		BOOL IsSuccess=TerminateThread(hThread1,ExitCode);
		CloseHandle(hThread1);
		hThread1 = NULL;
	}
	if(hThread2)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(hThread2,&ExitCode);
		BOOL IsSuccess=TerminateThread(hThread2,ExitCode);
		CloseHandle(hThread2);
		hThread2 = NULL;
	}
	if(hThread3)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(hThread3,&ExitCode);
		BOOL IsSuccess=TerminateThread(hThread3,ExitCode);
		CloseHandle(hThread3);
		hThread3 = NULL;
	}
	if(hThread4)
	{
		DWORD ExitCode=0;
		GetExitCodeThread(hThread4,&ExitCode);
		BOOL IsSuccess=TerminateThread(hThread4,ExitCode);
		CloseHandle(hThread4);
		hThread4 = NULL;
	}



	ClearStack();
}

bool UseProxyDlg::GetSystemProxy(HWND StateWnd)
{
	HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut; 
	if( ERROR_SUCCESS != RegOpenKeyEx(hKeyIn, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_CREATE_LINK | KEY_WRITE | KEY_READ | KEY_NOTIFY, &hKeyOut)) 
	{ 
		::RegCloseKey(hKeyOut);
		//::SetWindowText(StateWnd,_T("目前代理使用情况：无法读取系统信息！"));

		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024*sizeof(wchar_t));
		SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_CANNOTREADSYSTEMINFO",
			pText,L"Current Proxy State:CANNOTREADSYSTEMINFO"));
		delete [] pText;
		pText = NULL;

		return FALSE; 
	} 

	_TCHAR pValue[255],buf[255];
	memset(pValue, 0x00, sizeof(pValue)); 
	::ZeroMemory(buf,sizeof(buf));

	ULONG regsize = 0; 
	if(ERROR_SUCCESS != RegQueryValueEx(hKeyOut, _T("ProxyServer"), NULL, NULL, NULL, &regsize)) 
	{
		//::SetWindowText(StateWnd,_T("目前代理使用情况：未使用任何代理！"));

		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024*sizeof(wchar_t));
		SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_NOTUSEPROXY",
			pText,L"Current Proxy State:NOTUSEPROXY"));
		delete [] pText;
		pText = NULL;
		
		
		
		
		
		::RegCloseKey(hKeyOut);
		return FALSE; 
	}

	if(ERROR_SUCCESS != RegQueryValueEx(hKeyOut, _T("ProxyServer"), NULL, NULL,(LPBYTE)pValue, &regsize)) 
	{
		//::SetWindowText(StateWnd,_T("目前代理使用情况：未使用任何代理！"));
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024*sizeof(wchar_t));
		SetWindowText(StateWnd,LoadItemStringW(L"IDS_HIDEIP_NOTUSEPROXY",
			pText,L"Current Proxy State:NOTUSEPROXY"));
		delete [] pText;
		pText = NULL;
		::RegCloseKey(hKeyOut);
		return FALSE; 
	}
	_tcscpy(buf,_T("目前代理使用情况："));

	wchar_t * pText = new wchar_t[1024];
	memset(pText,0,1024*sizeof(wchar_t));
	_tcscpy(buf,LoadItemStringW(L"IDS_HIDEIP_CURRENTPROXYSTATE",
		pText,L"Current Proxy State:"));
	delete [] pText;
	pText = NULL;
	
	
	_tcscat(buf,pValue);
	::SetWindowText(StateWnd,buf);
	::RegCloseKey(hKeyOut);
	return true; 
}

int UseProxyDlg::create(HWND hWnd,HINSTANCE hInstance)
{
	hInst=hInstance;
	g_TabWnd[6] = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TAB_HIDEIP),hWnd,(DLGPROC)WndProc);

	return 0;
//	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_DLG), NULL, (DLGPROC)WndProc);
}