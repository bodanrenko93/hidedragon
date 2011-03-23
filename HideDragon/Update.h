
#if !defined(AFX_UPDATE_H__6C8EC96F_4275_4BB8_9F1B_E9BCF62D54EB__INCLUDED_)
#define AFX_UPDATE_H__6C8EC96F_4275_4BB8_9F1B_E9BCF62D54EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"


#endif // !defined(AFX_UPDATE_H__6C8EC96F_4275_4BB8_9F1B_E9BCF62D54EB__INCLUDED_)

struct URL_Info
{
	char * cURLs;
	long lSize;

};

struct VersionInfo
{
	float fVersionNum;
	int iSiteNum;
	URL_Info * sUI;
};


int ReadVersionInfoFile(VersionInfo* virf)
{
	HANDLE hviFile_Read=NULL;
	DWORD dwFileSize = 0,dwReaded = 0,dwError = 0;
	char  * cContent_Read = NULL,*cTemp = NULL;

	
	hviFile_Read = CreateFile(L"VersionInfo29.hdv",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hviFile_Read == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Open versioninfo28.hdv error.");
		dwError = GetLastError();
		return 0;
	}
	
	dwFileSize = GetFileSize(hviFile_Read,NULL);
	cContent_Read = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	ReadFile(hviFile_Read,cContent_Read,dwFileSize,&dwReaded,NULL);

	cTemp = cContent_Read;
	virf->fVersionNum = *(float*)cContent_Read;
	cContent_Read += sizeof(float);
	virf->iSiteNum = *(int*)cContent_Read;
	cContent_Read += sizeof(int);
	virf->sUI = (URL_Info*)VirtualAlloc(NULL,sizeof(URL_Info)*virf->iSiteNum,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	for(int i = 0;i< virf->iSiteNum;i++)
	{
		virf->sUI[i].lSize = *(long*)cContent_Read;
		cContent_Read += sizeof(long);
		virf->sUI[i].cURLs = NULL;
		virf->sUI[i].cURLs = (char * )VirtualAlloc(NULL,200,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		if (virf->sUI[i].cURLs == NULL)
		{
			MessageBox(NULL,L"ERROR-----ReadVersionInfoFile----VirtualAlloc",L"HD",MB_OK);
			return 0;
		}
		memcpy(virf->sUI[i].cURLs,cContent_Read,virf->sUI[i].lSize);
		cContent_Read += virf->sUI[i].lSize;
	}
	
	VirtualFree(cContent_Read,0,MEM_RELEASE);
	CloseHandle(hviFile_Read);
	DeleteFile(L"VersionInfo28.hdv");
	//MessageBox(NULL,"OK3","HD",MB_OK);
	
	return 1;
}
