

#include"hideip.h"







BOOL CHideIP::GetServerPage(wchar_t * pUrl)
{
	COfflineBrowser obj;
	char szUrl[1024];
	strcpy(szUrl,pUrl/*"http://www.proxycn.com/html_proxy/securitylevel1-1.html"*/);

	
	obj.SetDir("c:\\MyTemp\\");
	obj.LoadHtml(szUrl, true);
	obj.BrowseOffline();


}



BOOL CHideIP::UseProxy(ProxyInfo * pInfo)
{



}


void CHideIP::SetProxy_initialize( )
{
    sKeyName = "Software\\ProxyToggle";
    sValName = "SavedProxyConfig";
    sAccName = "AccessType";
    sFlgName = "ProxyActive";
    sBypName = "BypassList";
}

void CHideIP::SetProxy_activate( const char *address )
{
    // read config
    unsigned long        nSize = 4096;
    char                 szBuf[4096] = { 0 };

    // Read IE settings
    INTERNET_PROXY_INFO* pInfo = (INTERNET_PROXY_INFO*)szBuf;

    if(!InternetQueryOption(NULL, INTERNET_OPTION_PROXY, pInfo, &nSize))
    {
       printf("InternetQueryOption failed! (%d)\n", GetLastError());
       exit( 100 );
    }
    // if necessary, store config
    const char *save = pInfo->lpszProxy;
    HKEY kValue;

    int res;
    BYTE Value[0x100];
    ULONG length = 0x100, Type;
    res = RegOpenKeyEx( HKEY_CURRENT_USER, sKeyName, 0, 
                        KEY_SET_VALUE | KEY_QUERY_VALUE, &kValue );
    if (res==ERROR_SUCCESS)
    {
        BYTE retVal = 0;
        if( ( res = RegQueryValueEx(kValue, sFlgName, NULL, 
                                    &Type, Value, &length) ) == ERROR_SUCCESS )
        {
            if( Type != REG_SZ )
            {
               printf("Corrupted registry entry\n" );
               exit( 400 );
            }
            retVal = *Value;
        }
        if( retVal )
        {
            RegCloseKey( kValue );
            return;
        }
    }
    else
        if( ( res = RegCreateKey( HKEY_CURRENT_USER, sKeyName, &kValue ) ) != ERROR_SUCCESS )
        {
           printf("Unable to save configuration\n" );
           exit( 600 );
        }

    *Value = 1;
    if( ( res = RegSetValueEx( kValue, sFlgName, 0, REG_SZ, 
                                    Value, sizeof( BYTE ) ) ) != ERROR_SUCCESS)
    {
       printf("Unable to update configuration\n" );
       exit( 500 );
    }
    if( ( res = RegSetValueEx( kValue, sAccName, 0, REG_SZ, (BYTE*)&(pInfo->dwAccessType), 
                               sizeof( DWORD ) ) ) != ERROR_SUCCESS)
    {
       printf("Unable to save configuration\n" );
       exit( 400 );
    }
    if( pInfo->dwAccessType != PROXY_TYPE_DIRECT )
    {
        if( ( res = RegSetValueEx( kValue, sValName, 0, REG_SZ, 
                                  (BYTE*)pInfo->lpszProxy, 1 + strlen( pInfo->lpszProxy ) ) ) 
            != ERROR_SUCCESS)
        {
           printf("Unable to save configuration\n" );
           exit( 400 );
        }
        if( ( res = RegSetValueEx( kValue, sBypName, 0, REG_SZ, 
                                  (BYTE*)pInfo->lpszProxyBypass, 
                                   1 + strlen( pInfo->lpszProxyBypass ) ) ) != ERROR_SUCCESS)
        {
           printf("Unable to save configuration\n" );
           exit( 400 );
        }
    }
    RegCloseKey( kValue );
    RegCloseKey( HKEY_CURRENT_USER );

    // Set IE settings

    INTERNET_PER_CONN_OPTION_LIST list;
    BOOL    bReturn;
    DWORD   dwBufSize = sizeof(list);

    // Fill out list struct.
    list.dwSize = sizeof(list);

    // NULL == LAN, otherwise connectoid name.
    list.pszConnection = NULL;

    // Set three options.
    list.dwOptionCount = 3;
    list.pOptions = new INTERNET_PER_CONN_OPTION[3];

    // Make sure the memory was allocated.
    if(NULL == list.pOptions)
    {
        //Return FALSE if the memory wasn't allocated.
        return;
    }

    // Set flags.
    list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT |
        PROXY_TYPE_PROXY;

    // Set proxy name.
    list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    list.pOptions[1].Value.pszValue = (char*)address;

    // Add proxy name to bypass list.
    CString sAddr = address, bypass = pInfo->lpszProxyBypass;
    if( bypass.GetLength( ) )
        bypass += ";";
    int idx;
    if( ( idx = sAddr.Find( "//" ) ) >= 0 )
        sAddr = sAddr.Mid( idx + 2 );
    if( ( idx = sAddr.FindOneOf( ":/" ) ) >= 0 )
        sAddr = sAddr.Left( idx );
    bypass += sAddr;
    list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    list.pOptions[2].Value.pszValue = bypass.GetBuffer( 0 );

    // Set the options on the connection.
    bReturn = InternetSetOption(NULL,
        INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);

    // Notify settings change.
    bReturn = InternetSetOption(NULL,
        INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0 );

    // Free the allocated memory.
    bypass.ReleaseBuffer( );
    delete [] list.pOptions;
}

void CHideIP::SetProxy_deactivate()
{
    HKEY kValue;

    LONG res;
    BYTE Value[0x100], Bypass[0x100];
    ULONG length = 0x100, blength = 0x100, Type;
    unsigned long        nSize = 4096;
    char                 szBuf[4096] = { 0 };

    res = RegOpenKeyEx( HKEY_CURRENT_USER, sKeyName, 0, 
                        KEY_SET_VALUE | KEY_QUERY_VALUE, &kValue );
    if (res==ERROR_SUCCESS)
    {
        BYTE retVal = 0;
        if( ( res = RegQueryValueEx(kValue, sFlgName, NULL, 
                                    &Type, Value, &length) ) == ERROR_SUCCESS )
        {
            if( Type != REG_SZ )
            {
               printf("Corrupted registry entry\n" );
               exit( 400 );
            }
            retVal = *Value;
        }
        if( retVal )
        {
            *Value = 0;
            if( ( res = RegSetValueEx( kValue, sFlgName, 0, REG_SZ, Value, 
                                       sizeof( BYTE ) ) ) != ERROR_SUCCESS)
            {
               printf("Unable to update configuration\n" );
               exit( 500 );
            }

            INTERNET_PER_CONN_OPTION_LIST list;
            BOOL    bReturn;
            DWORD   dwBufSize = sizeof(list);

            // Fill out list struct.
            list.dwSize = sizeof(list);

            // NULL == LAN, otherwise connectoid name.
            list.pszConnection = NULL;

            // Set three options.
            list.dwOptionCount = 2;
            list.pOptions = new INTERNET_PER_CONN_OPTION[3];

            // Make sure the memory was allocated.
            if(NULL == list.pOptions)
            {
                //Return FALSE if the memory wasn't allocated.
                return;
            }

            // Set flags.
            list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
            
            length = sizeof( DWORD );
            if( ( res = RegQueryValueEx( kValue, sAccName, NULL, &Type, 
                                       (BYTE*)&(list.pOptions[0].Value.dwValue), &length ) ) 
                                              != ERROR_SUCCESS )
            {
               printf("Unable to restore configuration\n" );
               exit( 700 );
            }

            // Set proxy name and bypass.
            if( list.pOptions[0].Value.dwValue != PROXY_TYPE_DIRECT )
            {
                list.dwOptionCount = 3;
                length = 0x100;
                if( ( res = RegQueryValueEx( kValue, sValName, NULL, 
                                             &Type, Value, &length ) ) != ERROR_SUCCESS )
                {
                   printf("Unable to restore configuration\n" );
                   exit( 700 );
                }
                list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
                list.pOptions[1].Value.pszValue = (char*)Value;

                if( ( res = RegQueryValueEx( kValue, sBypName, NULL, 
                                          &Type, Bypass, &blength ) ) != ERROR_SUCCESS )
                {
                   printf("Unable to restore configuration\n" );
                   exit( 700 );
                }
                list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
                list.pOptions[2].Value.pszValue = (char*)Bypass;
            }

            // Set the options on the connection.
            bReturn = InternetSetOption(NULL,
                INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);

            // Notify settings change.
            bReturn = InternetSetOption(NULL,
                INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0 );

            // Free the allocated memory.
            delete [] list.pOptions;

            RegCloseKey( kValue );
        }
    }

    RegCloseKey( HKEY_CURRENT_USER );
}


































