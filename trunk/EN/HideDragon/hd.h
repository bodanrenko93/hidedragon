
#define IDT_REFRESH 500
#define IDT_SCANWEB 501
#define IDT_SETTIPTEXT 502
#define IDT_CHECKTI 503
#define IDT_SETADSTEXT 504
#define IDT_SETWNDADSTEXT 505
#define IDT_RECONNECT 506





//#include   "afxtempl.h"   
#include "windows.h"
#include "shlwapi.h"  
#include <list>
#include "HDStruct.h"
#include "commctrl.h"
#include "../hooklib.h"

extern "C"
{
#include "CWebPage.h"
}

using namespace std;



#define HMF_ADDITEM     0x1001
#define HMF_DELITEM     0x1002
#define HMF_ADDTIMES	0x1003
#define HMF_DECTIMES	0x1004
#define HMF_ISHAVEITEM  0x1005



#define	ID_WNDALPHA_RESTORE    0
#define ID_WNDALPHA_LOW		   1
#define ID_WNDALPHA_MIDDLELOW  2
#define ID_WNDALPHA_MIDDLE     3
#define ID_WNDALPHA_MIDDLEHIGH 4
#define ID_WNDALPHA_MOSTHIGH   5











struct sTitleWnd
{
	HWND hWnd;
	int iWndType;
	wchar_t * pOrgTitle;
	wchar_t * pNewTitle;
	HICON  hOrgIcon;
	HICON  hNewIcon;
	HICON  hOrgBigIcon;
	HICON  hNewBigIcon;
};



typedef list <sTitleWnd *> TWList;


struct sIconTitle
{
	TWList twlist;
	int iWndCount;
	wchar_t cPID[20];
	wchar_t cProcessName[30];
};

typedef list <sIconTitle *> ITList;




struct sProperty
{
	TWList twlist;
	wchar_t cPID[20];
	wchar_t cProcessName[30];
	int iWndCount;
	HICON hTrayIcon;
	wchar_t cTransparent[20];
	int bAutoReserve;
	WORD wHK;
};






struct NewProcessInfo
{
	wchar_t * pName;
	DWORD dwPID;

};

typedef list <NewProcessInfo *> ProcessList;


typedef list <AutoHideItem *> DBProcessList;

typedef list <WndAndHandle *> HidedProcessList;



INT_PTR CALLBACK BackTipDialog(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK BkWndHotkeyProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK ChangeWndTitleProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK PropertyProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK GeneralPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK AddtionalPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK ResumeTipProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
VOID CALLBACK  CheckTitleIconTimer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);

INT_PTR CALLBACK ProcessPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);





INT_PTR CALLBACK AllWndPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK TrayPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);

INT_PTR CALLBACK InfoPanelProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK BKPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK WebPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK HideIPPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);
INT_PTR CALLBACK AutoHideProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);


int SaveUserName(wchar_t * cUserName);


//LRESULT CALLBACK ExpandBtnSubclassProc(HWND,UINT,WPARAM,LPARAM);







int ShowBackTipDialog();
int DrawTitleBar(HDC hDC);
int OnNCMouseMove(HDC hWndDC,LPARAM lParam);
int OnNCLBDown(HDC hWndDC,LPARAM lParam);
int OnNCLBUp(HDC hWndDC,LPARAM lParam);
int CreateAutoReserveFile();
int AddARFL(sFileItem *);
int IsInARFL(sFileItem *);
int DelItem(sFileItem *);
int IsUseSkin();
int ChangeWindowStyle(HWND,int);
int SetCurrentLanguage();
int LoadLanguage(wchar_t * pLanguage,wchar_t * pFilePath);
int SetDialogTextW(HWND * pDialog,UINT uID);
BOOL CALLBACK EnumDlgChildProcW(HWND hwnd,LPARAM lParam);
int SetMenuString();
int SetDlgItemStringW(HWND hwnd,UINT uDlgID,UINT uCtrlID);
int MakeLanguageMenu();
int HDMessageBoxW(wchar_t * cStringID,wchar_t * cDefault,HWND hParentWnd = NULL,
				 UINT uType = MB_OK|MB_ICONINFORMATION);

int GetSysLangLoadLang();
int LoadLanguageBaseID(LANGID LangID = 0x0802);
int UpdateLanguagePack();
unsigned __stdcall UpdateLangPack(void * pParam);
unsigned __stdcall AutoUpdateThreadProc(void * pParam);
unsigned __stdcall AutoChangeIconTitle(void * pParam);
unsigned __stdcall AutoHideThreadProc(void * pParam);
unsigned __stdcall GetAdsLinkProc(void * pParam);


int CALLBACK rarCallback(UINT msg,LONG UserData,LONG P1,LONG P2);
int	IsHaveUpdatedFile();
int	IsHaveTempDirectory();
int SetWindowTransparent(HWND,int);
int SetTipText(wchar_t * cID,wchar_t * pDefault);
int ClearTipTextArray();
int AppendTipText(wchar_t * pIDS,wchar_t * pDefault);
int GetFunctionPointer();
int BKCurrentWindow(int);
int ConstructBKSW(wchar_t *,wchar_t *,HWND hWnd = NULL);
int SetBkWndHotkey(int );
int BKRegistered(sBKWHotkey * pWH);
int ChangeWindowIcon(int,wchar_t * cIconPath = NULL);
int ChangeWndTitle(int iSelMark,wchar_t * cTitle = NULL);
int RestoreTitle(int iSelMark,int NotDelDB = 0);
int ShowPropertyDialog(int iSelMark);
int CloseBKTipDialog();
int RestoreIcon(int iSelMark,DWORD lPID = 0,int NotDelDB = 0);
int DelTitleNode();
int DelIconNode();
int FillTitleHandleColumn(HWND hList);
int CreatePropertyDialog(sProperty * pProperty);
int SaveWindowInfo(WndAndHandle * pSW,int iBaSW);
int ReadWndInfo(WndAndHandle * pSW,int * pBaSW);
int FillColumn(HWND hList);
int FillRow(HWND hList,WndAndHandle * pHided);
int TakeToBk(WndAndHandle * pHided);
int BKHided();
wchar_t * LoadItemStringW(wchar_t * pItemID,wchar_t * pText,wchar_t * pDefault = NULL);
int SetWindowListViewColumn(HWND hListView);
int SetBKListColumn(HWND hBKList);
int UnRegisterHK(int iSelMark,WndAndHandle * pW = NULL);
int DelBKHotKeyNode();
int IsInBKList(wchar_t * cPID);
int IsRegister();
int WriteRegCode();
int TipIfExit();
int IsInITList(wchar_t* pTitle);
int Compatible25();
int DeleteDir(wchar_t * pDir);
int IsEuqalExe(HWND hWnd,wchar_t * cExeName,int iCount);
int RestoreAllIconTitle();
int IsInListView(DWORD lPID,wchar_t * cTitle);
DWORD WINAPI WriteRegThread(LPVOID lp);
int GetCurrentDirectory_HD(int iSize,wchar_t * cDir);
int RegisterBKHotkey(WndAndHandle * pWH);
int IsInRegBKHK(WndAndHandle * pItem);
int GetProcessTrayIcon(DWORD lPID,TBBUTTON * pTB,TRAYDATA * pTrayData);
HWND FindTrayToolbarWindow();
int DelInvalidTrayIcon();
HWND FindTaskbarWnd();
int	FillNotExistWnd(int iListFlag = 0);
int IsInSW(HWND hWnd);
int AppendSW(HWND hWnd);
int IsHaveTrayIcon(WndAndHandle * psW ,TRAYDATA * pTD);
int GetProcessName(DWORD lPID,wchar_t * pName);
int IsInBKSW(HWND hWnd);
HWND WINAPI DoCreateTabControl(HWND hwndParent);
HWND WINAPI DoCreateDisplayWindow(HWND hwndParent);
int OnTabSelChange(int iPage);
int ShowAllWndPage();
int ShowTrayPage();
int HideOtherPage(int iPage);
int CreateInfoPanel();
int ExpendInfoPanel(int iVisible = 0);
int ResizeInfoPanel();
int ResizeTabWnd();
int ShowBKPage();
int	AddTrayListHeader();
int	FillTrayList();
int BKTrayIcon();
int EnumAllVisibleTray();
int ReleaseResourceForTrayList();
int AllocResourceForTrayList();
int InitTab();
HWND DoCreateStatusBar();
int SavePosSize();
int SetWindowSizePos();
int WriteOptionFile(OptionFile);
int ChangeMenuState(int iTabNum);
int DrawExpandBtn(DRAWITEMSTRUCT* pdis);
int SaveAutoChangeIconTitle();
int SaveChangedIconInfo(IconTitleItem * pItem);
int IsInIconTitleDB(IconTitleItem * pItem);
int ChangeTitleIcon(DWORD PID,IconTitleItem * pItem);
int SaveChangedTitleInfo(IconTitleItem * pItem);
int ChangeTitle(wchar_t * cTitle,sIconTitle * pIT);
int DeleteIconTitleFromDB(wchar_t * pName,int iFlag = 0);
int CreateAutoChangeThread();
int IsInIconList(IconTitleItem * pItem,wchar_t * cPID);
int IsInTitleList(IconTitleItem * pItem,wchar_t * cPID);
int ReadTimes(int * pTimes);
int WriteTimes(int iTimes);
int	Switch2BKTab();
int SetTabLanguage();
int SetTrayListColumn(HWND hBKList);
int SetStatusLanguage();
int SetTrayTipLanguage();
int ShowWebPage();
int ShowProcessPage();
int	AddProcessListColumn();
int	FillProcessList();
int IsProcessItem(WndAndHandle * pWH);
int GetHideProcessName(DWORD lPID,wchar_t * cHidedName);
int	ShowCleanPage();
int ShowHideIPPage();
int CleanHistory();
int IsInCheckedArray(int idx);
int SetProcessListColumn(HWND hProcessList);
int ResetPriTree();
int	CreateGetAdsLinkThread();
VOID CALLBACK  SetStatusTextLink(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
VOID CALLBACK  SetWndTextLink(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
VOID CALLBACK  ReconnectTimer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
int	SetIPListColumn(HWND g_IPList);
int RestoreIP();
int ShowAutoHidePage();
int Save2AutoHideList(int iSelMark);
int	AddAutoHideListHeader();
int Insert2AutoHideList(AutoHideItem * pItem);
int AddTrayAutoHide();
int CreateAutoHideThread();
int HideAutoHideItem(AutoHideItem * pItem);
int AllocProcessList();
int ResetPorcessList(ProcessList & processList);
int FreeProcessList(ProcessList & processList);
int GetNewProcessName(ProcessList & newProcessList);
int Add2NewProcessList(NewProcessInfo * pInfo);
int GetProcessList(ProcessList & processList);
int SetWindowToTop(int iSelMark);
int ChangeLanguage(WORD loWparam,wchar_t * pMenuString);
unsigned __stdcall ChangeLanguageThreadProc(void * pParam);
int FillList();
int ReadDBProcessList();
int Insert2DBProcessList(AutoHideItem * pItem);
int BKThisIcon(WndAndHandle * pItem);
int HideSelected(WndAndHandle ** pWH,int iSize,int iAutoHide = 0);
int IsInHidedList(WndAndHandle * pItem);
int EnumAllVisibleWindow_au();
BOOL CALLBACK EnumWindowsForAU(HWND hwnd,LPARAM lParam);
int BKCurrentWindow_au(WndAndHandle * pItem);
int AllocAutoHideWindow();
int IsInHidedWndList(WndAndHandle * pItem);
int DelAutoHideItem();
int	ReleaseDBProcessList();
int DelDBProcessList(AutoHideItem* pItem);
int AddDBPorcessList(AutoHideItem* pItem);
int ReleaseTrayHidedList();
int ReleaseAutoHideWindow();
int ResetAutoHideWindow();
int ReleaseHidedWndList();
int SetAHListColumn(HWND g_AutoHideList);
int AddAutoHideFromFile();
DWORD GetOSVersion();
int ConstructBKSW_au(wchar_t * cPID,wchar_t * cTitle, HWND hWnd = NULL);
int HideSelected_au(WndAndHandle **pWH,int iSize);

BOOL CreateImageListForTab();
BOOL DestroyImageListForTab();



int HideWindowInThread(HWND * hWnd);

unsigned __stdcall BKWindowThread(void * pParam);

unsigned __stdcall MonitorBKProc(void * pParam);


int dbview(wchar_t * pInfo);



int BeginBKMonitorThread();


int ReleaseResourceForTrayList_au();
int EnumAllVisibleTray_au();
int AllocResourceForTrayList_au();
int IsInAutoHideList(wchar_t * pName,int iType = 2);



INT_PTR CALLBACK PasswordDlgProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam);



int ShowPasswordDlg();

int regGetBuyURL(wchar_t * publisher, wchar_t * appName, wchar_t * appVer,wchar_t * pBuyUrl );
int BuyHideDragon();





void go();
void thread();
void cleanup();
void setup();







extern "C"
{
	extern long __cdecl DisplayHTMLPage(HWND hwnd, wchar_t* webPageName);
	extern long __cdecl EmbedBrowserObject(HWND hwnd);
	extern long __cdecl SetBrowserRect(HWND hwnd,HWND * pWnd);
	
}



int HMF_Create();
int HMF_Modify(sHMF_Item * pItem,WORD dwFlag);




















