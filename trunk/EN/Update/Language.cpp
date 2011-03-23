
#include <windows.h>
#include "Language.h"
#include <stdio.h>













//MultiLanguage variables
WIN32_FIND_DATA FFD;




extern HWND hMainDlg;






int SetCurrentLanguage()
{
	int iRet = 0;
	wchar_t * cSettingFile = new wchar_t [1024];
	memset(cSettingFile,0,1024 * sizeof(wchar_t));
	//GetModuleFileName(NULL,cSettingFile,1024);
	GetCurrentDirectory(1024,cSettingFile);
	wcscat(cSettingFile,L"\\Language\\LangSetting.ini");
	HANDLE hSettingFile = CreateFileW(cSettingFile,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	


	if(hSettingFile == INVALID_HANDLE_VALUE)
	{
//		delete [] cSettingFile;
		if(WritePrivateProfileStringW(L"Setting",L"CurrentLanguage",L"English",cSettingFile) == 0)
		{
			OutputDebugString(L"Write Language Error.");
		}
		if(WritePrivateProfileStringW(L"Setting",L"CurrentLanguageID",L"First",cSettingFile) == 0)
		{
			OutputDebugString(L"Write Language Error.");
		}

		//return iRet;
	}
	else
	{
		CloseHandle(hSettingFile);
	}
	wchar_t szSection[20],*szValue = new wchar_t[1024],szKey[30];
	memset(szSection,0,20 * sizeof(wchar_t));
	memset(szKey,0,30 * sizeof(wchar_t));
	memset(szValue,0,1024 * sizeof(wchar_t));
	wcscpy(szSection,L"Setting");
	wcscpy(szKey,L"CurrentLanguage");


	if(GetPrivateProfileStringW(szSection,L"CurrentLanguageID",L"",szValue,1024,cSettingFile) != 0)
	{
		if(wcscmp(szValue,L"First") == 0)
		{
			int iRet = GetSysLangLoadLang();
			if(iRet == 1)
			{
				delete [] cSettingFile;
				delete [] szValue;

				return 1;
			}
		}
	}


	if(GetPrivateProfileStringW(szSection,szKey,L"",szValue,1024,cSettingFile) != 0)
	{
		wchar_t * cFilePath = new wchar_t[1024];
		memset(cFilePath,0,1024 * sizeof(wchar_t));
		iRet = LoadLanguage(szValue,cFilePath);
		delete [] cFilePath;
	}
	delete [] cSettingFile;
	delete [] szValue;
	
	return iRet;
}

int SetDialogText(HWND * pDialog,UINT uID)
{
	wchar_t szSection [20];
	memset(szSection,0,20*sizeof(wchar_t));
	wcscpy(szSection,L"String");

	
	wchar_t * szKey,cDlgID[20];
	memset(cDlgID,0,20*sizeof(wchar_t));
	szKey = new wchar_t[512];
	memset(szKey,0,512*sizeof(wchar_t));

	wchar_t * szDefault = L"";

	//读对话框标题
	swprintf(szKey,L"IDD%d_Title",uID);
	wchar_t * pLangFilePath = new wchar_t[1024];
	memset(pLangFilePath,0,1024*sizeof(wchar_t));
	GetCurrentDirectoryW(1024,pLangFilePath);
	wcscat(pLangFilePath,L"\\Language\\");
	wchar_t * pFileName = new wchar_t[1024];
	wcscat(pLangFilePath,FFD.cFileName);
	wchar_t * pData = new wchar_t[1024];
	memset(pData,0,1024*sizeof(wchar_t));
	DWORD dwSize = 1024;

	if(GetPrivateProfileStringW(szSection,szKey,szDefault,
			pData,dwSize,pLangFilePath) != 0)
	{
		SetWindowTextW(*pDialog,pData);
		
	}

	delete [] pData;
	delete [] pLangFilePath;
	delete [] szKey;

	//写入各个子控件的标题文字
	EnumChildWindows(*pDialog,EnumDlgChildProc,(LPARAM)&uID);

	return 1;
}


BOOL CALLBACK EnumDlgChildProc(HWND hwnd,LPARAM lParam)
{
	UINT uDlgID = *(UINT*)lParam;
	wchar_t szSection[20];
	memset(szSection,0,20 * sizeof(wchar_t));
	wcscpy(szSection,L"String");

	wchar_t * szDefault = L"";
	DWORD dwSize = 1024;
	wchar_t * pData = new wchar_t[dwSize];


	wchar_t * pLangFilePath = new wchar_t[1024];
	GetCurrentDirectoryW(1024,pLangFilePath);
	wcscat(pLangFilePath,L"\\Language\\");
	wcscat(pLangFilePath,FFD.cFileName);

	wchar_t * szKey = new wchar_t[512];
	memset(szKey,0,512 * sizeof(wchar_t));
	int iCtrlID = GetDlgCtrlID(hwnd);
	swprintf(szKey,L"IDD%d_%d",uDlgID,iCtrlID);


	if(GetPrivateProfileStringW(szSection,szKey,szDefault,
			pData,dwSize,pLangFilePath) != 0)
	{
		SetWindowTextW(hwnd,pData);
	}




	delete [] pLangFilePath;
	delete [] pData;
	delete [] szKey;
	
	return TRUE;
}

wchar_t * LoadItemString(wchar_t * pItemID,wchar_t * pText,wchar_t * pDefault/* = NULL*/)
{	
	wchar_t * pLangFilePath = new wchar_t[1024];
	memset(pLangFilePath,0,1024*sizeof(wchar_t));
	GetCurrentDirectory(1024,pLangFilePath);
	wcscat(pLangFilePath,L"\\Language\\");
	wcscat(pLangFilePath,FFD.cFileName);

	memset(pText,0,1024*sizeof(wchar_t));
	if(GetPrivateProfileStringW(L"String",pItemID,L"",
		pText,1024,pLangFilePath) != 0)
	{
		delete [] pLangFilePath;
		return pText;
	}
	else
	{
		delete [] pLangFilePath;
		if(pDefault)
		{
			return pDefault;
		}
		else
		{
			return L"";
		}
	}

}


int GetSysLangLoadLang()
{
	LANGID SysLangID = GetSystemDefaultLangID();
	int iRet = LoadLanguageBaseID(SysLangID);
	
	
	return iRet;
}

int LoadLanguage(wchar_t * pLanguage /* = NULL*/,wchar_t * pFilePath)
{
	wchar_t szSection[20];
	memset(szSection,0,20 * sizeof(wchar_t));
	wcscpy(szSection,L"Setting");

	wchar_t szKey[] = L"Language",*szLang;
	DWORD dwSize = 1000;
	
	if(pLanguage != NULL)
	{
		szLang = pLanguage;
	}
	else
	{
	}
	
	
	wchar_t * szFilePath = new wchar_t[1024];
	memset(szFilePath,0,1024 * sizeof(wchar_t));
	wchar_t * cIniFile = new wchar_t[1024];
	memset(cIniFile,0,1024 * sizeof(wchar_t));
	GetCurrentDirectoryW(1024,cIniFile);
	wcscat(cIniFile,L"\\Language\\*.ini");
	bool bFound = false;
	HANDLE hFindFile = FindFirstFileW(cIniFile,&FFD);
	BOOL bRet = TRUE;
	while(bRet && hFindFile != INVALID_HANDLE_VALUE)
	{
		wchar_t * szValue = new wchar_t[1024];
		wchar_t * szLanguageID = new wchar_t[1024];
		memset(szLanguageID,0,1024 * sizeof(wchar_t));
		memset(szValue,0,1024 * sizeof(wchar_t));
		GetCurrentDirectoryW(1024,szFilePath);
		wcscat(szFilePath,L"\\Language\\");
		wcscat(szFilePath,FFD.cFileName);
		if(GetPrivateProfileStringW(szSection,szKey,L"",szValue,
			1024,szFilePath) != 0)
		{
			if(GetPrivateProfileStringW(szSection,L"LanguageID",L"",szLanguageID,
				1024,szFilePath) != 0)
			{

			}
			
			if(wcscmp(szValue,szLang) == 0)
			{
				if(pFilePath != NULL)
				{
					wcscpy(pFilePath,szFilePath);
				}
				bFound = true;
				GetCurrentDirectory(1024,szFilePath);
				wcscat(szFilePath,L"\\Language\\");
				wcscat(szFilePath,L"LangSetting.ini");

				if(WritePrivateProfileString(szSection,L"CurrentLanguage",szValue,szFilePath) == 0)
				{
					OutputDebugString(L"Write Language Error.");
				}
				if(WritePrivateProfileString(szSection,L"CurrentLanguageID",szLanguageID,szFilePath) == 0)
				{
					OutputDebugString(L"Write Language Error.");
				}
								
				delete [] szValue;
				delete [] szLanguageID;

				break;
			}
		}
		bRet = FindNextFile(hFindFile,&FFD);
		DWORD dwErr = 0;
		if(bRet == 0)
		{
			dwErr = GetLastError();
		}
		delete [] szValue;
		delete [] szLanguageID;

	}
	FindClose(hFindFile);
	delete [] szFilePath;
	delete [] cIniFile;
	


	if(!bFound)
	{
		return 0;
	}
	return 1;
}





int LoadLanguageBaseID(LANGID LangID)
{
	wchar_t szSection[20];
	memset(szSection,0,20 * sizeof(wchar_t));
	wcscpy(szSection,L"Setting");


	wchar_t szKey[] = L"LanguageID",*szLang;
	DWORD dwSize = 1000;
	
	wchar_t pLanguageID[128];
	memset(pLanguageID,0,128 * sizeof(wchar_t));
	swprintf(pLanguageID,L"%04X",LangID);
	

	
	if(pLanguageID != NULL)
	{
		szLang = pLanguageID;
	}
	else
	{
	}
	
	
	wchar_t * szFilePath = new wchar_t[1024];
	memset(szFilePath,0,1024 * sizeof(wchar_t));
	wchar_t * cIniFile = new wchar_t[1024];
	memset(cIniFile,0,1024 * sizeof(wchar_t));
	GetCurrentDirectoryW(1024,cIniFile);
	wcscat(cIniFile,L"\\Language\\*.ini");
	bool bFound = false;
	HANDLE hFindFile = FindFirstFileW(cIniFile,&FFD);
	BOOL bRet = TRUE;
	while(bRet && hFindFile != INVALID_HANDLE_VALUE)
	{
		wchar_t * szValue = new wchar_t[1024];
		wchar_t * szCurrentLang = new wchar_t[1024];
		memset(szCurrentLang,0,1024 * sizeof(wchar_t));
		memset(szValue,0,1024 * sizeof(wchar_t));
		GetCurrentDirectoryW(1024,szFilePath);
		wcscat(szFilePath,L"\\Language\\");
		wcscat(szFilePath,FFD.cFileName);
		if(GetPrivateProfileStringW(szSection,szKey,L"",szValue,
			1024,szFilePath) != 0)
		{
			if(wcscmp(szValue,szLang) == 0)
			{
				bFound = true;
				if(GetPrivateProfileStringW(szSection,L"Language",L"",szCurrentLang,
					1024,szFilePath) != 0)
				{
					
				}

				memset(szFilePath,0,1024 * sizeof(wchar_t));
				GetCurrentDirectoryW(1024,szFilePath);
				wcscat(szFilePath,L"\\Language\\");
				wcscat(szFilePath,L"LangSetting.ini");

				if(WritePrivateProfileStringW(szSection,L"CurrentLanguageID",szValue,szFilePath) == 0)
				{
					OutputDebugString(L"Write Language Error.");
				}
				if(WritePrivateProfileStringW(szSection,L"CurrentLanguage",szCurrentLang,szFilePath) == 0)
				{
					OutputDebugString(L"Write Language Error.");
				}

								
				delete [] szValue;
				delete [] szCurrentLang;

				break;
			}
		}
		bRet = FindNextFileW(hFindFile,&FFD);
		DWORD dwErr = 0;
		if(bRet == 0)
		{
			dwErr = GetLastError();
		}
		delete [] szValue;
		delete [] szCurrentLang;
	}
	FindClose(hFindFile);
	delete [] szFilePath;
	delete [] cIniFile;

	if(!bFound)
	{
		return 0;
	}
	return 1;
}



int HDMessageBox(wchar_t * cStringID,wchar_t * cDefault,HWND hParentWnd,UINT uType)
{
	wchar_t * pText = new wchar_t[1024];
	memset(pText,0, 1024 * sizeof(wchar_t));
	wchar_t * pTitle = new wchar_t [1024];
	wcscpy(pTitle,LoadItemString(L"IDS_MAINDIALOG_TITLE",pTitle,L"隐身专家"));
	if(hParentWnd == NULL)
	{
		hParentWnd = hMainDlg;
	}
	int iRet = MessageBoxW(hParentWnd,LoadItemString(cStringID,pText,cDefault),pTitle,uType);
	delete [] pTitle;
	delete [] pText;
	
	
	return iRet;
}






