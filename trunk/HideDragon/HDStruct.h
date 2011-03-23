


#ifndef HDStruct_H_
#define HDStruct_H_




#include <windows.h>
#include <list>













struct sFileItem
{
	long lSize;
	wchar_t * cFileName;
};

struct sARFL
{
	long lNum;
	sFileItem * pItem;
};





struct sHMF_Item
{
	long lSize;
	wchar_t * cName;
	int iTimes;
};

struct sHMF
{
	long lNum;
	sHMF_Item * pItem;
};





struct sBKWHotkey
{
	wchar_t cPID[20];
	wchar_t cProcessName[30];
	wchar_t * cTitle;
	UINT uMF;
	UINT uVK;
	WORD wHK;
	ATOM aHK;
	HWND hWnd;
	sBKWHotkey * Next;
};


typedef std::list <sBKWHotkey *> BKHList;






struct SizePosItem
{
	int id;
	LONG left;
	LONG right;
	LONG top;
	LONG bottom;
};


struct IconTitleItem
{
	wchar_t cProcessName[50];
	wchar_t * cIconPath;
	wchar_t * cTitle;
};





struct StatusTxtLinkItem
{
	int id;
	wchar_t * pLink;
	wchar_t * pText;
};



struct AutoHideItem
{

	wchar_t * cProcessName;
	int iType;


		

};
















#endif //HDStruct_H_






