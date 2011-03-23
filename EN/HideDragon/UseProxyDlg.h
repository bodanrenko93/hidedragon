// UseProxyDlg.h: interface for the UseProxyDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USEPROXYDLG_H__806C035D_02DD_445D_83C1_8ECF40CD00AE__INCLUDED_)
#define AFX_USEPROXYDLG_H__806C035D_02DD_445D_83C1_8ECF40CD00AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "winsock.h"
#include "commctrl.h"
#include <TCHAR.h>







//结构体
struct QueryInfoParam
{
		SOCKET s;
		_TCHAR FilePath[80];
		_TCHAR FileName[80];
};
struct ProxyInfo
{
	_TCHAR HostIp[80];
	_TCHAR port[80];
	_TCHAR kind[80];
	_TCHAR area[80];
	_TCHAR CheckTime[80];
	_TCHAR whois[80];
	double speed;
	_TCHAR szSpeed[80];
	ProxyInfo* next;
};
struct TestProxyInfo
{
	SOCKET s;
	char buf[4096];
};

class UseProxyDlg  
{
public:
	UseProxyDlg();
	virtual ~UseProxyDlg();
public:
static HWND hMainWnd;
static HWND hAdvWnd;
static HWND hGenWnd;
static HWND hUseInfoWnd;
static ProxyInfo ProxyInfoHead;
static HWND ProGressWnd;
static HANDLE hFile;
static HANDLE hMapping;
static _TCHAR* basepointer;
static HANDLE hThread;
static HINSTANCE hInst;								// current instance

public:
//全局函数
int create(HWND hWnd,HINSTANCE hInstance);
static LRESULT CALLBACK GenDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdvDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static DWORD WINAPI DownLoadFile(LPVOID pParam);
static int GetCurrentDir(_TCHAR * str,int strlength);
static SOCKET sockconnect(const char *host,const char *service,const char *transport);
static SOCKET TCPconnect(const char *host,const char *service);
static DWORD WINAPI DownloadAndQueryFile(LPVOID pParam);
static _TCHAR* LoadFile(_TCHAR* filename);
//把内存中的映像文件释放掉
static int FreeFile(void);
static int QueryFile(const _TCHAR* FileName);
static void OnInitDlg(HINSTANCE hInst,HWND hWnd);
static void OnInitAdvDlg(HINSTANCE hInst,HWND hWnd);
static void SetListContent(HWND hWnd);
static void ClearStack();
static void DeletePROXYCN(_TCHAR* str);
static DWORD WINAPI TestHttpProxy(LPVOID pParam);
static DWORD WINAPI TestSockProxy(LPVOID pParam);
static void SetListColumn();
static bool TestProxy(ProxyInfo param);
static void SetState(HWND hWnd,_TCHAR* str);
static void AddOneProxyToList(ProxyInfo* temp,int i,HWND ListWnd);
static bool UseProxy(char* HostIp,char* port,char* PatrolKind);
static bool restore(int iShowDlg = 1);
static void StopSearch();
static bool GetSystemProxy(HWND StateWnd);
static DWORD WINAPI UseProxyDlg::SetProxyList(LPVOID pParam);
static unsigned __stdcall RestoreIPProc(void * pParam);
static bool RestoreIP();

};

#endif // !defined(AFX_USEPROXYDLG_H__806C035D_02DD_445D_83C1_8ECF40CD00AE__INCLUDED_)
