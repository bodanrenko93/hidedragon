///////////////////////////////////////////////////
// Driver.h�ļ�

#ifndef __DRIVER_H__
#define __DRIVER_H__
#include <wtypes.h>
#include <deque>




#include <Winsvc.h>	// Ϊ��ʹ��SCM����
#include <winioctl.h>

#define IOCTL_HIDEPID	\
CTL_CODE(FILE_DEVICE_UNKNOWN, 0x830, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SHOWPID	\
CTL_CODE(FILE_DEVICE_UNKNOWN, 0x831, METHOD_BUFFERED, FILE_ANY_ACCESS)



struct ProcessInfo
{
	wchar_t cProcessName[30];
	DWORD dwPID;
};



typedef std::deque <ProcessInfo * > listProInfo;



class CDriver
{

public:

	CDriver();
// ���캯������������
	// ���캯����pszDriverPathΪ��������Ŀ¼��pszLinkNameΪ������������
	// ����Ĺ��캯���У�����ͼ������򿪷���
	CDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName);
	// �����������������ֹͣ����
	virtual ~CDriver();

	void HideProc(DWORD dwPID);

	void ShowProc(DWORD dwPID);


	BOOL GetHidedProcessName(DWORD dwPID,wchar_t * cProcessName);

	int GetProcessName(DWORD lPID,wchar_t * pName);

	int GetHidedProcessCount();

	int GetHidedProcessByIdx(int idx,wchar_t * cProcessName);

	int NotExistProcessList(DWORD dwPID);

	


	BOOL GetPath(TCHAR * strPath);
// ����
	// �������Ƿ����
	virtual BOOL IsValid() { return (m_hSCM != NULL && m_hService != NULL); }

// ����
	// ��������Ҳ����˵������DriverEntry������������
	virtual BOOL StartDriver();
	// �������񡣼����������DriverUnload���̽�������
	virtual BOOL StopDriver();
	
	// ���豸����ȡ�õ���������һ�����
	virtual BOOL OpenDevice();

	// ���豸���Ϳ��ƴ���
	virtual DWORD IoControl(DWORD nCode, PVOID pInBuffer, 
			DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount);
// ʵ��
protected:
//	char m_szLinkName[56];	// ������������
	wchar_t m_szLinkName[56];	// ������������


	BOOL m_bStarted;	// ָ�������Ƿ�����
	BOOL m_bCreateService;	// ָ���Ƿ񴴽��˷���

	HANDLE m_hSCM;		// SCM���ݿ���
	HANDLE m_hService;	// ������
	HANDLE m_hDriver;	// �豸���

	int m_iHidedCount;

	listProInfo m_listProcess;


};



#endif // __DRIVER_H__