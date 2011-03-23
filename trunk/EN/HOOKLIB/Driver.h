///////////////////////////////////////////////////
// Driver.h文件

#ifndef __DRIVER_H__
#define __DRIVER_H__
#include <wtypes.h>
#include <deque>




#include <Winsvc.h>	// 为了使用SCM函数
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
// 构造函数和析构函数
	// 构造函数，pszDriverPath为驱动所在目录，pszLinkName为符号连接名字
	// 在类的构造函数中，将试图创建或打开服务，
	CDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName);
	// 析构函数。在这里，将停止服务，
	virtual ~CDriver();

	void HideProc(DWORD dwPID);

	void ShowProc(DWORD dwPID);


	BOOL GetHidedProcessName(DWORD dwPID,wchar_t * cProcessName);

	int GetProcessName(DWORD lPID,wchar_t * pName);

	int GetHidedProcessCount();

	int GetHidedProcessByIdx(int idx,wchar_t * cProcessName);

	int NotExistProcessList(DWORD dwPID);

	


	BOOL GetPath(TCHAR * strPath);
// 属性
	// 此驱动是否可用
	virtual BOOL IsValid() { return (m_hSCM != NULL && m_hService != NULL); }

// 操作
	// 开启服务。也就是说驱动的DriverEntry函数将被调用
	virtual BOOL StartDriver();
	// 结束服务。即驱动程序的DriverUnload例程将被调用
	virtual BOOL StopDriver();
	
	// 打开设备，即取得到此驱动的一个句柄
	virtual BOOL OpenDevice();

	// 向设备发送控制代码
	virtual DWORD IoControl(DWORD nCode, PVOID pInBuffer, 
			DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount);
// 实现
protected:
//	char m_szLinkName[56];	// 符号连接名称
	wchar_t m_szLinkName[56];	// 符号连接名称


	BOOL m_bStarted;	// 指定服务是否启动
	BOOL m_bCreateService;	// 指定是否创建了服务

	HANDLE m_hSCM;		// SCM数据库句柄
	HANDLE m_hService;	// 服务句柄
	HANDLE m_hDriver;	// 设备句柄

	int m_iHidedCount;

	listProInfo m_listProcess;


};



#endif // __DRIVER_H__