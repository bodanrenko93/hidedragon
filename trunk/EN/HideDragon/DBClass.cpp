

#include "DBClass.h"
#include <windows.h>
#include <Shlwapi.h>


extern HINSTANCE hInst;










DBClass::DBClass()
{
	try
	{
		char * pName = new char[1024];
		memset(pName,0,1024);


		GetModuleFileNameA(hInst,pName,1024);
		PathRemoveFileSpecA(pName);
		strcat(pName,"\\HDDB.db");

		m_DB.open(pName);
		delete [] pName;
		pName = NULL;
		CreateARTable();
		CreateHMTable();
		CreateBKHKTable();
		CreateUserNameTable();
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
	}

}






DBClass::~DBClass()
{
	m_DB.close();
}





int DBClass::CreateARTable()
{
	try
	{
		if(m_DB.tableExists("AutoReserve") == false)
		{
			m_DB.execDML("create table AutoReserve(ProcessName char(1024),NameLen int);");
		}
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}




int DBClass::CreateHMTable()
{
	try
	{
		if(m_DB.tableExists("HabitMemory") == false)
		{
			m_DB.execDML("create table HabitMemory(ProcessName "
				"char(1024),NameLen int,UseTime int);");
		}

	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}





int DBClass::CreateBKHKTable()
{
	try
	{
		if(m_DB.tableExists("BKHotkey") == false)
		{
			m_DB.execDML("create table BKHotkey(aHK int, PID char(20),ProcessName char(30),"
				"Title char(2048),hWnd int,MF int,VK int,wHK float);");
				 
		}

	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}





int DBClass::CreateSizePosTable()
{
	try
	{
		if(m_DB.tableExists("SizePos") == false)
		{
			m_DB.execDML("create table SizePos(id int, left int, right int,top int,"
				"bottom int);");
			SizePosItem SPI;
			SPI.id = 0;
			SPI.left = 0;
			SPI.right = 0;
			SPI.top = 0;
			SPI.bottom = 0;
			InsertSizePosItem(&SPI);
		}
		

	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}


int DBClass::CreateIconTitleTable()
{
	try
	{
		if(m_DB.tableExists("IconTitle") == false)
		{
			m_DB.execDML("create table IconTitle(ProcessName char(150), IconPath char(3072), Title char(3072));");
		}
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}






int DBClass::CreateAutoHideTable()
{
	try
	{
		if(m_DB.tableExists("AutoHide") == false)
		{
			m_DB.execDML("create table AutoHide(ProcessName char(1024),Type int);");
		}
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}
	
	
	return 1;
}






int DBClass::AddAutoHideItem(AutoHideItem * pItem)
{
	CreateAutoHideTable();
	int iRet = InsertAutoHideItem(pItem);



	return iRet;
}



int DBClass::InsertAutoHideItem(AutoHideItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		
		char * pName = new char[1024];
		memset(pName,0,1024);
		
		
		
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,
			50,pName,1024,NULL,FALSE);
		
		
		
		
		
		sprintf(cSql,"select * from AutoHide where ProcessName = '%s';",strlwr(pName));
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		int iFlagExist = 0;
		if(!q.eof())
		{
			iFlagExist = 1;
		}
		q.finalize();
		
		memset(cSql,0,1024);
		if(iFlagExist == 1)
		{
			delete [] cSql;
			
			delete [] pName;
		
			return 0;
		}
		else
		{
			sprintf(cSql,"insert into AutoHide values ('%s','%d'); ",
				strlwr(pName),pItem->iType);
			
			int nRows = m_DB.execDML(cSql);
		}
		
		delete [] cSql;
		
		delete [] pName;
	
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}
	
	
	return 1;
}





int DBClass::DelAutoHideItem(AutoHideItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);


		char * pName = new char[1024];
		memset(pName,0,1024);
		
		
		
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,
			1024,pName,1024,NULL,FALSE);
		
		
		sprintf(cSql,"delete from AutoHide where ProcessName = '%s';",strlwr(pName));
		int nRows = m_DB.execDML(cSql);
		
		delete [] cSql;
		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}



	
	
	return 1;
}



int DBClass::InsertARItem(sFileItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cFileName,1024,pName,1024,NULL,FALSE);

		
		sprintf(cSql,"insert into AutoReserve values ('%s',%d); ",pName,strlen(pName));
		int nRows = m_DB.execDML(cSql);

		delete [] cSql;

		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}



	return 1;
}




int DBClass::InsertSizePosItem(SizePosItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);

		
		sprintf(cSql,"insert into SizePos values (%d,%d,%d,%d,%d); ",pItem->id,pItem->left,pItem->right,
			pItem->top,pItem->bottom);
		int nRows = m_DB.execDML(cSql);

		delete [] cSql;

		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}



	return 1;
}




int DBClass::InsertIconTitleItem(IconTitleItem * pItem, int iFlag /* == 0*/)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);

		char * pTitle = new char[1024 * 4];
		memset(pTitle,0,1024 * 4);

		char * pIconPath = new char[1024 * 4];
		memset(pIconPath,0,1024 * 4);



		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,
			50,pName,1024,NULL,FALSE);


		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cTitle,
			1024*3,pTitle,1024 * 4,NULL,FALSE);


		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cIconPath,
			1024*3,pIconPath,1024 * 4,NULL,FALSE);


		sprintf(cSql,"select * from IconTitle where ProcessName = '%s';",strlwr(pName));
		CppSQLite3Query q = m_DB.execQuery(cSql);

		int iFlagExist = 0;
		if(!q.eof())
		{
			iFlagExist = 1;
		}
		q.finalize();

		memset(cSql,0,1024);
		if(iFlagExist == 1)
		{
			if(iFlag == 0)
			{
				sprintf(cSql,"update IconTitle set IconPath = '%s'"
					"where ProcessName = '%s';",pIconPath,strlwr(pName));
			}
			else
			{
				sprintf(cSql,"update IconTitle set Title = '%s'"
					"where ProcessName = '%s';",pTitle,strlwr(pName));
			}

			int nRows = m_DB.execDML(cSql);
		}
		else
		{
			sprintf(cSql,"insert into IconTitle values ('%s','%s','%s'); ",
				strlwr(pName),pIconPath,pTitle);

			int nRows = m_DB.execDML(cSql);
		}

		delete [] cSql;

		delete [] pName;

		delete [] pTitle;

		delete [] pIconPath;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}



	return 1;
}



int DBClass::UpdateSizePosItem(SizePosItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);

		
		sprintf(cSql,"update SizePos set left = %d , right = %d , top = %d , bottom = %d where id = 0",pItem->left,pItem->right
			,pItem->top,pItem->bottom);

		
		int nRows = m_DB.execDML(cSql);

		delete [] cSql;

		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}



	return 1;
}


int DBClass::SaveSizePos(SizePosItem * pItem)
{
	CreateSizePosTable();
	UpdateSizePosItem(pItem);


	return 1;
}



int DBClass::SaveChangedIconTitle(IconTitleItem * pItem,int iFlag/* == 0*/)
{
	CreateIconTitleTable();
	InsertIconTitleItem(pItem,iFlag);


	return 1;
}




int DBClass::InsertHMItem(sHMF_Item * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cName,1024,pName,1024,NULL,FALSE);

		
		sprintf(cSql,"insert into HabitMemory values ('%s',%d,%d); ",pName,
			strlen(pName),pItem->iTimes);
		int nRows = m_DB.execDML(cSql);

		delete [] cSql;

		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}



	return 1;
}



int DBClass::InsertBKHKItem(sBKWHotkey * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);

		char * pTitle = new char[2048];
		memset(pTitle,0,2048);

		char * pID = new char[256];
		memset(pID,0,256);



		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,
			30,pName,1024,NULL,FALSE);


		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cTitle,
			2048,pTitle,2048,NULL,FALSE);


		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cPID,
			20,pID,256,NULL,FALSE);





		memset(cSql,0,1024);
		sprintf(cSql,"select * from BKHotkey where MF = %d and VK = %d;",pItem->uMF,pItem->uVK);
		CppSQLite3Query q = m_DB.execQuery(cSql);

		int iFlagExist = 0;
		if(!q.eof())
		{
			q.finalize();
			memset(cSql,0,1024);
			sprintf(cSql,"delete from BKHotkey where MF = %d and VK = %d;",pItem->uMF,pItem->uVK);
			int nRows = m_DB.execDML(cSql);
		}




		sprintf(cSql,"select * from BKHotkey where ProcessName = '%s';",pName);
		q = m_DB.execQuery(cSql);

		iFlagExist = 0;
		if(!q.eof())
		{
			iFlagExist = 1;
		}
		q.finalize();

		memset(cSql,0,1024);
		if(iFlagExist == 1)
		{
			sprintf(cSql,"update BKHotkey set aHK = %d ,PID = '%s', ProcessName = '%s',Title = '%s',hWnd = %d ,MF = %d ,VK = %d,"
				"wHK = %d where ProcessName = '%s';",pItem->aHK,pID,pName,pTitle,pItem->hWnd,pItem->uMF,
				pItem->uVK,pItem->wHK,pName);

			int nRows = m_DB.execDML(cSql);	
		}
		else
		{
			sprintf(cSql,"insert into BKHotkey values (%d,'%s','%s','%s',%d,%d,%d,%d); ",pItem->aHK,
				pID,pName,pTitle,pItem->hWnd,pItem->uMF,pItem->uVK,pItem->wHK);

			int nRows = m_DB.execDML(cSql);
		}

		delete [] cSql;

		delete [] pName;

		delete [] pTitle;

		delete [] pID;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}



	return 1;
}











int DBClass::IsInARFL(sFileItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cFileName,1024,pName,1024,NULL,FALSE);

		sprintf(cSql,"select * from AutoReserve where ProcessName = '%s';",pName);
		CppSQLite3Query q = m_DB.execQuery(cSql);
		delete [] cSql;
		delete [] pName;
		

		if(!q.eof())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return -1;
	}

}





int DBClass::IsInHM(sHMF_Item * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cName,1024,pName,1024,NULL,FALSE);

		sprintf(cSql,"select * from HabitMemory where ProcessName = '%s';",pName);
		CppSQLite3Query q = m_DB.execQuery(cSql);
		delete [] cSql;
		delete [] pName;
		

		if(!q.eof())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return -1;
	}

}



int DBClass::IsInBKHK(sBKWHotkey * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,30,pName,1024,NULL,FALSE);

		sprintf(cSql,"select * from BKHotkey where ProcessName = '%s';",pName);
		CppSQLite3Query q = m_DB.execQuery(cSql);
		delete [] cSql;
		delete [] pName;
		

		if(!q.eof())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return -1;
	}

}




int DBClass::DelARItem(sFileItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cFileName,1024,pName,1024,NULL,FALSE);

		sprintf(cSql,"delete from AutoReserve where ProcessName = '%s';",pName);
		int nRows = m_DB.execDML(cSql);

		delete [] cSql;
		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}




int DBClass::DeleteIconTitleItem(IconTitleItem * pItem,int iFlag /* == 0*/)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,50,pName,1024,NULL,FALSE);



		memset(cSql,0,1024);
		if(iFlag == 0)
		{
			sprintf(cSql,"update IconTitle set IconPath = ''"
				"where ProcessName = '%s';",strlwr(pName));
		}
		else
		{
			sprintf(cSql,"update IconTitle set Title = ''"
				"where ProcessName = '%s';",strlwr(pName));
		}
		int nRows = m_DB.execDML(cSql);
		
		

		char * cIconPath = new char[1024 * 4];
		char * cTitle = new char[1024 * 4];

		memset(cIconPath,0,1024 * 4);
		memset(cTitle,0,1024 * 4);

		memset(cSql,0,1024);
		sprintf(cSql,"select IconPath,Title from IconTitle where ProcessName = '%s';",pName);
		CppSQLite3Query q = m_DB.execQuery(cSql);

		if(!q.eof())
		{
			strcpy(cIconPath,q.getStringField(0));
			strcpy(cTitle,q.getStringField(1));
		}
		q.finalize();



		memset(cSql,0,1024);
		if(strcmp(cIconPath,"") == 0 && strcmp(cTitle,"") == 0)
		{
			sprintf(cSql,"delete from IconTitle where ProcessName = '%s';",pName);
			nRows = m_DB.execDML(cSql);
		}
		
		delete [] cIconPath;
		delete [] cTitle;

		delete [] cSql;
		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}

int DBClass::DelHMItem(sHMF_Item * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cName,1024,pName,1024,NULL,FALSE);

		sprintf(cSql,"delete from HabitMemory where ProcessName = '%s';",pName);
		int nRows = m_DB.execDML(cSql);

		delete [] cSql;
		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}



int DBClass::DelBKHKItem(sBKWHotkey * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,1024,pName,1024,NULL,FALSE);

		sprintf(cSql,"delete from BKHotkey where ProcessName = '%s';",pName);
		int nRows = m_DB.execDML(cSql);

		delete [] cSql;
		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}



int DBClass::AddHMTime(sHMF_Item * pItem)
{
	try
	{
		int iTime = 0;

		char cSql[1024];
		memset(cSql,0,1024);
		char pName [1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cName,1024,pName,1024,NULL,FALSE);


		memset(cSql,0,1024);
		sprintf(cSql,"select UseTime from HabitMemory where ProcessName = '%s';",pName);
		CppSQLite3Query q = m_DB.execQuery(cSql);

		if(!q.eof())
		{
			iTime = q.getIntField(0);
			iTime ++;
		}
		q.finalize();

		sFileItem * pARItem = NULL;
		if(iTime > 5)
		{
			pARItem = new sFileItem;
			pARItem->lSize = pItem->lSize * 2;
			pARItem->cFileName = pItem->cName;
			InsertARItem(pARItem);
			pARItem->cFileName = NULL;
			delete pARItem;
		}

		memset(cSql,0,1024);
		sprintf(cSql,"update HabitMemory set UseTime = %d "
			"where ProcessName = '%s';",iTime,pName);

		int nRows = m_DB.execDML(cSql);
		
		//delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}




int DBClass::ReadHK(sBKWHotkey * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		char * pName = new char[1024];
		memset(pName,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,1024,pName,1024,NULL,FALSE);


		sprintf(cSql,"select PID,aHK,Title,hWnd,MF,VK,wHK from BKHotkey "
			"where ProcessName = '%s';",pName);
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		delete [] cSql;
		delete [] pName;



		char cPID[20];
		memset(cPID,0,20);


		char * cTitle = new char[2048];
		memset(cTitle,0,2048);


		if(!q.eof())
		{
			strcpy(cPID,q.getStringField(0));
			pItem->aHK = q.getIntField(1);
			strcpy(cTitle,q.getStringField(2));
			pItem->hWnd = (HWND)q.getIntField(3);
			pItem->uMF = q.getIntField(4);
			pItem->uVK = q.getIntField(5);
			pItem->wHK = q.getIntField(6);
		}

		
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cPID,20,pItem->cPID,20);
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cTitle,2048,pItem->cTitle,2048);


		delete [] cTitle;
		q.finalize();



	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}


	return 1;
}


int DBClass::ReadSizePos(SizePosItem * pItem)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		char * pName = new char[1024];
		memset(pName,0,1024);


		sprintf(cSql,"select left,right,top,bottom from SizePos "
			"where id = 0;",pName);
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		delete [] cSql;
		delete [] pName;


		if(!q.eof())
		{
			pItem->left = q.getIntField(0);
			pItem->right = q.getIntField(1);
			pItem->top = q.getIntField(2);
			pItem->bottom = q.getIntField(3);
		}


		q.finalize();
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}





	return 0;
}


int DBClass::ReadIconTitleItem(IconTitleItem * pItem,int * pExist)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		char * pName = new char[1024];
		memset(pName,0,1024);


		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pItem->cProcessName,50,pName,1024,NULL,FALSE);


		
		sprintf(cSql,"select IconPath,Title from IconTitle "
			"where ProcessName = '%s';",strlwr(pName));
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		delete [] cSql;
		delete [] pName;


		if(!q.eof())
		{
			char * cIconPath = new char[1024*4];
			memset(cIconPath,0,1024*4);
			char * cTitle = new char[1024*4];
			memset(cTitle,0,1024*4);

			strcpy(cIconPath,q.getStringField(0));
			strcpy(cTitle,q.getStringField(1));



			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cIconPath,1024*4,pItem->cIconPath,1024 *3);
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cTitle,1024*4,pItem->cTitle,1024 *3);

			
			delete [] cIconPath;
			delete [] cTitle;


			*pExist = 1;
		}
		else
		{
			*pExist = 0;
		}


		q.finalize();
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}

	return 1;
}







int DBClass::ReadAHItem(AutoHideItem * pItem,int * pExist,FUNC_INSERTAUTOLIST * pInsertAutoHideList)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		
		

		
		sprintf(cSql,"select* from AutoHide ");
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		delete [] cSql;
		
		
		int iCountFlag = 0;
		
		while(!q.eof())
		{
			char * cProcessName = new char[1024];
			memset(cProcessName,0,1024);
			int iType = 0;


			
			
			
			strcpy(cProcessName,q.getStringField(0));
			iType =  q.getIntField(1);
			
			
			
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cProcessName,1024,pItem->cProcessName,1024);
			
			pItem->iType = iType;
			

			pInsertAutoHideList(pItem);
			
			
			delete [] cProcessName;
			
			iCountFlag ++;
			q.nextRow();
			*pExist = 1;
		}
		
		
		
		q.finalize();


		if(iCountFlag == 0)
		{
			return 0;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}
	
	return 1;
}






int DBClass::CreateUserNameTable()
{
	try
	{
		if(m_DB.tableExists("UserName") == false)
		{
			m_DB.execDML("create table UserName(Name char(1024));");
		}
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}
	
	return 1;
}





int DBClass::SaveUserName(wchar_t * cUserName)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);
		
		char * pName = new char[1024];
		memset(pName,0,1024);
		
		
		
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,cUserName,
			50,pName,1024,NULL,FALSE);
		
		
		
		sprintf(cSql,"select * from UserName ;");
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		int iFlagExist = 0;
		if(!q.eof())
		{
			iFlagExist = 1;
		}
		q.finalize();
		
		memset(cSql,0,1024);
		if(iFlagExist == 1)
		{
			sprintf(cSql,"update UserName set Name = '%s'"
				";",pName,strlwr(pName));

		
			int nRows = m_DB.execDML(cSql);
		}
		else
		{
			sprintf(cSql,"insert into UserName values ('%s'); ",
				pName);
			
			int nRows = m_DB.execDML(cSql);
		}
		
		delete [] cSql;
		
		delete [] pName;
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}

	return 1;
}




int DBClass::ReadUserName(wchar_t * cUserName)
{
	try
	{
		char * cSql = new char[1024];
		memset(cSql,0,1024);

	
			
		
		sprintf(cSql,"select Name from UserName "
			";");
		CppSQLite3Query q = m_DB.execQuery(cSql);
		
		delete [] cSql;
		
		if(!q.eof())
		{
			char * pName = new char[1024];
			memset(pName,0,1024);
			
			strcpy(pName,q.getStringField(0));
			
			
			
			
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pName,1024,cUserName,1024);
			
			
			
			delete [] pName;
		}

		
		q.finalize();
	}
	catch (CppSQLite3Exception& e)
	{
		OutputDebugStringA(e.errorMessage());
		return 0;
	}
	
	return 1;
}

















