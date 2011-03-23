
#include <list>


#include "OfflineBrowser.h"










struct ProxyInfo
{
	wchar_t cIP[20];
	wchar_t cPort[10];
	wchar_t cContry[128];
	wchar_t cType[20];
};





typedef list <ProxyInfo *> ProxyList;




class CHideIP
{
public:
	CHideIP();
	~CHideIP();



	BOOL GetServerPage(wchar_t * pUrl);
	
	BOOL GetFastProxy(ProxyInfo * pInfo);

	
	BOOL UseProxy(ProxyInfo * pInfo);


	BOOL HideIP();



protected:

	ProxyList m_Proxy;


	wchar_t * cContent;

	const char *sKeyName, *sValName, *sAccName, *sFlgName, *sBypName;


};
