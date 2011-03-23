
#include "webdb.h"
#include <windows.h>
#include <Shlwapi.h>
#include <ShlObj.h>

extern HINSTANCE hInst;





CWebDB::CWebDB()
{
	memset(m_Name,0,MAX_PATH);
	strcpy(m_Name,"webstatus.db");
}


CWebDB::CWebDB(char * pName)
{
	memset(m_Name,0,MAX_PATH);
	if(pName == NULL)
	{
		strcpy(m_Name,"webstatus.db");
		return;
	}
	else
	{
		strcpy(m_Name,pName);
	}
}

int CWebDB::Init()
{
	try
	{
		char * pName = new char[1024];
		memset(pName,0,1024);

		SHGetSpecialFolderPathA(NULL, pName, CSIDL_PERSONAL, FALSE);
		strcat(pName,"\\HideDragon");
		CreateDirectoryA(pName,NULL);
		
		strcat(pName,"\\downloads\\");
		strcat(pName,m_Name);
		

		m_DB.open(pName);
		delete [] pName;

		pName = NULL;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}




	return 1;
}





CWebDB::~CWebDB()
{
	m_DB.close();
}


int CWebDB::ReadIconTitleItem(StatusTxtLinkItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);



		sprintf(cSql,"select * from web "
			"where ID = '%d';",pItem->id);
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		delete [] cSql;



		char * cText = new char[1024];
		memset(cText,0,1024);

		char * cLink = new char[1024];
		memset(cLink,0,1024);

		if(!q.eof())
		{
			pItem->id = q.getIntField(0);
			strcpy(cText,q.getStringField(1));
			strcpy(cLink,q.getStringField(2));
		}

		
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cText,1024,pItem->pText,1024);
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cLink,1024,pItem->pLink,1024);


		delete [] cText;
		delete [] cLink;

		q.finalize();

	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}





	return 1;
}

int CWebDB::ReadWndAdsItem(StatusTxtLinkItem * pItem)
{
	
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);



		sprintf(cSql,"select * from web "
			"where ID = '%d';",pItem->id);
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		delete [] cSql;



		char * cText = new char[1024];
		memset(cText,0,1024);

		char * cLink = new char[1024];
		memset(cLink,0,1024);

		if(!q.eof())
		{
			pItem->id = q.getIntField(0);
			strcpy(cText,q.getStringField(1));
			strcpy(cLink,q.getStringField(2));
		}

		
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cText,1024,pItem->pText,1024);
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cLink,1024,pItem->pLink,1024);


		delete [] cText;
		delete [] cLink;

		q.finalize();

	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}




	return 1;
}


int CWebDB::GetStatusTextLink(int Id ,wchar_t * pText,wchar_t * pLink)
{
	StatusTxtLinkItem * pItem = new StatusTxtLinkItem;
	pItem->id = Id;
	pItem->pText = new wchar_t[1024];
	memset(pItem->pText,0,1024 * sizeof(wchar_t));
	pItem->pLink = new wchar_t[1024];
	memset(pItem->pLink,0,1024 * sizeof(wchar_t));

	ReadIconTitleItem(pItem);


	wcscpy(pText,pItem->pText);
	wcscpy(pLink,pItem->pLink);

	delete [] pItem->pText;
	delete [] pItem->pLink;
	delete [] pItem;


	return 1;
}



int CWebDB::GetWndTextLink(int Id ,wchar_t * pText,wchar_t * pLink)
{
	StatusTxtLinkItem * pItem = new StatusTxtLinkItem;
	pItem->id = Id;
	pItem->pText = new wchar_t[1024];
	memset(pItem->pText,0,1024 * sizeof(wchar_t));
	pItem->pLink = new wchar_t[1024];
	memset(pItem->pLink,0,1024 * sizeof(wchar_t));

	ReadWndAdsItem(pItem);


	wcscpy(pText,pItem->pText);
	wcscpy(pLink,pItem->pLink);

	delete [] pItem->pText;
	delete [] pItem->pLink;
	delete [] pItem;


	return 1;
}










int CWebDB::GetCount()
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);


		sprintf(cSql,"select ID from web "
			";");
		
        CppSQLite3Table t = m_DB.getTable(cSql);

		m_iCount = 0;
		m_iCount = t.numRows();
		delete [] cSql;

		t.finalize();

	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}



	return m_iCount;
}

