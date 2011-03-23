



#include "Sqlite3\cppsqlite3.h"
#include "HDStruct.h"



typedef int FUNC_INSERTAUTOLIST (AutoHideItem * pItem);



class DBClass
{
public:

	DBClass();

	virtual ~DBClass();




public:

	int CreateARTable();
	int InsertARItem(sFileItem * pItem);
	int IsInARFL(sFileItem * pItem);
	int DelARItem(sFileItem * pItem);


	int CreateHMTable();
	int InsertHMItem(sHMF_Item * pItem);
	int IsInHM(sHMF_Item * pItem);
	int DelHMItem(sHMF_Item * pItem);
	int AddHMTime(sHMF_Item * pItem);



	int CreateBKHKTable();
	int InsertBKHKItem(sBKWHotkey * pItem);
	int IsInBKHK(sBKWHotkey * pItem);
	int DelBKHKItem(sBKWHotkey * pItem);
	int AddBKHKItem(sBKWHotkey * pItem);
	int ReadHK(sBKWHotkey * pItem);





	int CreateSizePosTable();
	int InsertSizePosItem(SizePosItem * pItem);
	int UpdateSizePosItem(SizePosItem * pItem);
	int SaveSizePos(SizePosItem * pItem);
	int ReadSizePos(SizePosItem * pItem);
	


	
	int CreateIconTitleTable();
	int InsertIconTitleItem(IconTitleItem * pItem,int iFlag = 0);
	int DeleteIconTitleItem(IconTitleItem * pItem,int iFlag = 0);
	int ReadIconTitleItem(IconTitleItem * pItem,int * pExist);
	int SaveChangedIconTitle(IconTitleItem * pItem,int iFlag = 0);
	
	
	int ReadIconTitleItem(StatusTxtLinkItem * pItem);
	int GetStatusTextLink(wchar_t * pText,wchar_t * pLink);
	




	int AddAutoHideItem(AutoHideItem * pItem);
	int CreateAutoHideTable();
	int InsertAutoHideItem(AutoHideItem * pItem);
	int ReadAHItem(AutoHideItem * pItem,int * pExist,FUNC_INSERTAUTOLIST * pInsertAutoHideList);
	int GetAHCount();
	int DelAutoHideItem(AutoHideItem * pItem);
	int ClearAutoHide();


	int CreateUserNameTable();
	int SaveUserName(wchar_t * cUserName);
	int ReadUserName(wchar_t * cUserName);


protected:
	
	CppSQLite3DB m_DB;

};







