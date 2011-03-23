#ifndef CLEARDLG_H
#define CLEARDLG_H
#pragma once
#include "Commctrl.h"

class CClearDlg
{
public:







static LRESULT CALLBACK MainDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static void InitMainDlg(HWND	hWnd);
static DWORD WINAPI DoWork(LPVOID pParam);
static void EmptyDirectory(const wchar_t * dir);
static BOOL IsWindows2k();
static BOOL IsWindowsNT();
static BOOL CreateNewSD(PSID pSid, SECURITY_DESCRIPTOR* pSD, PACL* ppDacl);
static BOOL GetOldSD(HKEY hKey, LPCTSTR pszSubKey, BYTE** pSD);
static BOOL GetUserSid(PSID* ppSid);
static void GetSidString(PSID pSid, LPTSTR szBuffer);
static BOOL RegSetPrivilege(HKEY hKey, LPCTSTR pszSubKey,SECURITY_DESCRIPTOR* pSD, BOOL bRecursive);
static void ClearForm();
static void EmptyRegKey(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR KeyName);
static void DeleteRegValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR ValueName);
static void GetTreeItemText(HWND TreeWnd,wchar_t* buf,int len,HTREEITEM h);
static void DeleteSpecifyedLnkInOneDir(const wchar_t* PathBuf,const wchar_t* LnkName);
static LRESULT CALLBACK ThreadDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static BOOL NotSearchIP();






public:
static HANDLE hThread;
static HWND hThreadWnd;
static HWND hMainWnd;
static HWND hTreeWnd;
static HCURSOR g_hCursorLink;
static HCURSOR g_hCursorLink_org;
static HINSTANCE hInst;								// current instance

public:
	CClearDlg(void);
	~CClearDlg(void);
	void Create(HWND hWnd,HINSTANCE hInstance);

};
#endif