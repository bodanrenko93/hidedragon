
#include "Sqlite3\cppsqlite3.h"
#include "HDStruct.h"






class CWebDB
{
public:

	CWebDB();
	CWebDB(char * pName);

	virtual ~CWebDB();




public:

	int Init();
	int ReadIconTitleItem(StatusTxtLinkItem * pItem);
	int GetStatusTextLink(int Id , wchar_t * pText,wchar_t * pLink);
	int ReadWndAdsItem(StatusTxtLinkItem * pItem);
	int GetWndTextLink(int Id ,wchar_t * pText,wchar_t * pLink);
	
	int GetCount();




protected:
	
	int m_iCount;
	CppSQLite3DB m_DB;
	char m_Name[MAX_PATH];

};



