
#include "driver.h"

#include <tchar.h>
#include <TlHelp32.h>




BOOL	CDriver::GetPath(TCHAR * strPath)
{
	TCHAR sDrive[10];
	TCHAR sDir[256];
	TCHAR sFilename[256],Filename[256];
	TCHAR sExt[256];
	
	GetModuleFileName(NULL, Filename, 256);
//	_splitpath(Filename, sDrive, sDir, sFilename, sExt);
	_wsplitpath(Filename, sDrive, sDir, sFilename, sExt);
	_tcscpy(strPath,sDrive);
	_tcscat(strPath,sDir);

	strPath[_tcslen(strPath)-1] = _T('\0');
	
	return TRUE;
}

CDriver::CDriver()
{
	m_iHidedCount = 0;
	TCHAR tpath[MAX_PATH];
	ZeroMemory(tpath,MAX_PATH);
	GetPath(tpath);
	_tcscat(tpath,L"\\HideProc.sys");

	//strncpy(m_szLinkName, "slHideProc", 55);
	wcsncpy(m_szLinkName, L"slHideProc", 55);
	m_bStarted = FALSE;
	m_bCreateService = FALSE;
	m_hSCM = m_hService = NULL;
	m_hDriver = INVALID_HANDLE_VALUE;

	// ��SCM������
	m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(m_hSCM == NULL)
	{
//		MessageBox(0, L"�򿪷�����ƹ�����ʧ��\n", 
//				L"��������Ϊ����ӵ��AdministratorȨ��\n", 0);
		return;
	}

	// ������򿪷���
	m_hService = ::CreateService((SC_HANDLE)m_hSCM, m_szLinkName, m_szLinkName, SERVICE_ALL_ACCESS, 
				SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, 
				tpath, NULL, 0, NULL, NULL, NULL);
	if(m_hService == NULL)
	{
		// ��������ʧ�ܣ���������Ϊ�����Ѿ����ڣ����Ի�Ҫ��ͼ����
		int nError = ::GetLastError();
		if(nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			m_hService = ::OpenService((SC_HANDLE)m_hSCM, m_szLinkName, SERVICE_ALL_ACCESS);
		}
	}
	else
	{
		m_bCreateService = TRUE;
	}

	if(!StartDriver()||!OpenDevice())
	{
		//MessageBox(NULL,L"��������ʧ��!",L"����",MB_OK|MB_ICONERROR);
		//ExitProcess(2);
	}


	m_listProcess.clear();


}

CDriver::CDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName)
{
	m_iHidedCount = 0;
	//strncpy(m_szLinkName, pszLinkName, 55);
	wcsncpy(m_szLinkName, pszLinkName, 55);
	m_bStarted = FALSE;
	m_bCreateService = FALSE;
	m_hSCM = m_hService = NULL;
	m_hDriver = INVALID_HANDLE_VALUE;

	// ��SCM������
	m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(m_hSCM == NULL)
	{
//		MessageBox(0, L"�򿪷�����ƹ�����ʧ��\n", 
//				L"��������Ϊ����ӵ��AdministratorȨ��\n", 0);
		return;
	}

	// ������򿪷���
	m_hService = ::CreateService((SC_HANDLE)m_hSCM, m_szLinkName, m_szLinkName, SERVICE_ALL_ACCESS, 
				SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, 
				pszDriverPath, NULL, 0, NULL, NULL, NULL);
	if(m_hService == NULL)
	{
		// ��������ʧ�ܣ���������Ϊ�����Ѿ����ڣ����Ի�Ҫ��ͼ����
		int nError = ::GetLastError();
		if(nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			m_hService = ::OpenService((SC_HANDLE)m_hSCM, m_szLinkName, SERVICE_ALL_ACCESS);
		}
	}
	else
	{
		m_bCreateService = TRUE;
	}
	
	if(!StartDriver()||!OpenDevice())
	{
//		MessageBox(NULL,L"��������ʧ��!",L"����",MB_OK|MB_ICONERROR);
		ExitProcess(2);
	}
}

CDriver::~CDriver()
{
	// �ر��豸���
	if(m_hDriver != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hDriver);
	// ��������˷��񣬾ͽ�֮ɾ��
	if(m_bCreateService)
	{
		StopDriver();
		::DeleteService((SC_HANDLE)m_hService);	
	}
	// �رվ��
	if(m_hService != NULL)
		::CloseServiceHandle((SC_HANDLE)m_hService);
	if(m_hSCM != NULL)
		::CloseServiceHandle((SC_HANDLE)m_hSCM);
}

BOOL CDriver::StartDriver()
{
	if(m_bStarted)
		return TRUE;
	if(m_hService == NULL)
		return FALSE;
	// ��������
	if(!::StartService((SC_HANDLE)m_hService, 0, NULL))
	{
		int nError = ::GetLastError();
		if(nError == ERROR_SERVICE_ALREADY_RUNNING)
			m_bStarted = TRUE;
		else
			::DeleteService((SC_HANDLE)m_hService);
	}
	else
	{
		// �����ɹ��󣬵ȴ������������״̬
		int nTry = 0;
		SERVICE_STATUS ss;
		::QueryServiceStatus((SC_HANDLE)m_hService, &ss);
		while(ss.dwCurrentState == SERVICE_START_PENDING && nTry++ < 80)
		{
			::Sleep(50);
			::QueryServiceStatus((SC_HANDLE)m_hService, &ss);
		}
		if(ss.dwCurrentState == SERVICE_RUNNING)
			m_bStarted = TRUE;
	}
	return m_bStarted;
}

BOOL CDriver::StopDriver()
{
	if(!m_bStarted)
		return TRUE;
	if(m_hService == NULL)
		return FALSE;
	// ֹͣ����
	SERVICE_STATUS ss;
	if(!::ControlService((SC_HANDLE)m_hService, SERVICE_CONTROL_STOP, &ss))
	{
		if(::GetLastError() == ERROR_SERVICE_NOT_ACTIVE)
			m_bStarted = FALSE;
	}
	else
	{
		// �ȴ�������ȫֹͣ����
		int nTry = 0;
		while(ss.dwCurrentState == SERVICE_STOP_PENDING && nTry++ < 80)
		{
			::Sleep(50);
			::QueryServiceStatus((SC_HANDLE)m_hService, &ss);
		}
		if(ss.dwCurrentState == SERVICE_STOPPED)
			m_bStarted = FALSE;
	}
	return !m_bStarted;
}

BOOL CDriver::OpenDevice()
{
	if(m_hDriver != INVALID_HANDLE_VALUE)
		return TRUE;

	// "\\.\"��Win32�ж��屾�ؼ�����ķ�����
	// m_szLinkName���豸����ķ����������ƣ������½ڻ���ϸ����
	//char sz[256] = "";
	wchar_t sz[256] = L"";
	wsprintf(sz, L"\\\\.\\%s", m_szLinkName);
	// �����������������豸
	m_hDriver = ::CreateFile(sz,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return (m_hDriver != INVALID_HANDLE_VALUE);
}

DWORD CDriver::IoControl(DWORD nCode, PVOID pInBuffer, 
		DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount)
{
	if(m_hDriver == INVALID_HANDLE_VALUE)
		return -1;
	// �����������Ϳ��ƴ���
	DWORD nBytesReturn;
	BOOL bRet = ::DeviceIoControl(m_hDriver, nCode, 
		pInBuffer, nInCount, pOutBuffer, nOutCount, &nBytesReturn, NULL);
	if(bRet)
		return nBytesReturn;
	else
		return -1;
}






BOOL CDriver::GetHidedProcessName(DWORD dwPID,wchar_t * cProcessName)
{
	listProInfo::iterator pNode;



	for(pNode = m_listProcess.begin();pNode != m_listProcess.end(); pNode ++)
	{
		ProcessInfo * pI =  *pNode;
		if(pI->dwPID == dwPID)
		{
			wcscpy(cProcessName,pI->cProcessName);
			return TRUE;
		}
	}
	

	return FALSE;
}




int CDriver::GetProcessName(DWORD lPID,wchar_t * pName)
{
	PROCESSENTRY32 pe32;


	HANDLE hProcessSnap = NULL;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First((void*)hProcessSnap,&pe32);
	while(Process32Next((void*)hProcessSnap,&pe32))
	{
		if(pe32.th32ProcessID == lPID)
		{
			if(wcscmp(wcslwr(pe32.szExeFile),L"iexplore.exe")==0)
			{
				wcscpy(pName,pe32.szExeFile);
			}
			else if(wcscmp(wcslwr(pe32.szExeFile),L"explorer.exe") == 0)
			{
				wcscpy(pName,pe32.szExeFile);
			}
			else
			{
				wcscpy(pName,pe32.szExeFile);
			}
		}
	}
	CloseHandle((HANDLE)hProcessSnap);




	return 1;
}



int CDriver::GetHidedProcessCount()
{

	return m_listProcess.size();
}




int CDriver::GetHidedProcessByIdx(int idx,wchar_t * cProcessName)
{
	listProInfo::iterator pNode;



//	for(pNode = m_listProcess.begin();pNode != m_listProcess.end(); pNode ++)
//	{
//		ProcessInfo * pI =  *pNode;
//		if(pI->dwPID == dwPID)
//		{
//			wcscpy(cProcessName,pI->cProcessName);
//			return TRUE;
//		}
//	}


	ProcessInfo * pI = NULL;
	pI = m_listProcess[idx];
	

	wcscpy(cProcessName,pI->cProcessName);


	return 1;
}


int CDriver::NotExistProcessList(DWORD dwPID)
{
	listProInfo::iterator pNode;



	for(pNode = m_listProcess.begin();pNode != m_listProcess.end(); pNode ++)
	{
		ProcessInfo * pI =  *pNode;
		if(pI->dwPID == dwPID)
		{
			return FALSE;
		}
	}
	

	return TRUE;
}


void CDriver::HideProc(DWORD dwPID)
{
	char info[256] = {0};
//	sprintf(info,"��Ҫ���صĽ���id:%d",dwPID);
//	MessageBox(NULL,info,"��Ϣ",MB_OK|MB_ICONINFORMATION);
	ProcessInfo * pInfo = new ProcessInfo;
	pInfo->dwPID = dwPID;
	GetProcessName(dwPID, pInfo->cProcessName);





	char tmp[5];
	IoControl(IOCTL_HIDEPID, &dwPID, sizeof(DWORD),
		tmp, 5);
	m_iHidedCount ++;
	if(m_iHidedCount >= 39)
	{
		delete pInfo;
		pInfo = NULL;
		return;
	}
	

	if(NotExistProcessList(dwPID))
	{
		m_listProcess.push_back(pInfo);
	}
	else
	{
		delete pInfo;
		pInfo = NULL;
	}



}

void CDriver::ShowProc(DWORD dwPID)
{
	char info[256] = {0};
//	sprintf(info,"��Ҫ��ʾ�Ľ���id:%d",dwPID);
//	MessageBox(NULL,info,"��Ϣ",MB_OK|MB_ICONINFORMATION);
	char tmp[5];
	IoControl(IOCTL_SHOWPID, &dwPID, sizeof(DWORD),
		tmp, 5);



	listProInfo::iterator pNode;



	for(pNode = m_listProcess.begin();pNode != m_listProcess.end(); pNode ++)
	{
		ProcessInfo * pI =  *pNode;
		if(pI->dwPID == dwPID)
		{
			m_listProcess.erase(pNode);
			delete pI;
			pI = NULL;

			return;
		}
	}

}








