/**
 * Offline Browser Component
 * @author Muhammad Sheraz Siddiqi
 * @version 1.0
 * @email sheraz_attari@hotmail.com
 * @copyright You're free to use it but I am not responsisble for any bugs
 */


#ifndef _H_OFFLINEBROWSER_H_
#define _H_OFFLINEBROWSER_H_

#include <windows.h>
#include <wininet.h>
#include <urlmon.h>
#include <mshtml.h>
#include <string>
#include <vector>
#import <mshtml.tlb>

class COfflineBrowser
{
public:
	COfflineBrowser();
	virtual ~COfflineBrowser();

protected:
	typedef std::vector<std::string>  StringArray;
	std::string m_sDir;
	std::string m_sHtml;
	StringArray m_dirArray;
	StringArray m_fileArray;
	std::string m_sHost;
	std::string m_sScheme;
	std::string m_sGuid;
	

public:
	void SetDir(const std::string& sDir) { m_sDir = sDir + m_sGuid + "\\";}
	std::string GetDir() { return m_sDir; }
	void SetHtml(const std::string& sHtml) { m_sHtml = sHtml; }
	std::string GetHtml() { return m_sHtml; }
	void SetHost(const std::string& sHost) { m_sHost = sHost; }
	std::string GetHost() { return m_sHost; }
	void SetGuid(const std::string& sGuid) { m_sGuid = sGuid; }
	std::string GetGuid() { return m_sGuid; }
	bool IsAbsolute(const std::string& sSrc);
	std::string TrimHostName(const std::string& sUrl);

public:
	void LoadHtml(const std::string& sUrl, bool bDownload=false);
	void BrowseOffline();
	bool DownloadResource(std::string sUrl, std::string sPath);
	void CreateDirectories(std::string sSrc, std::string sBasePath);
	void DeleteDirectory(std::string sDir);
	void EmptyDirectory(std::string sDir);
	bool GetAttribute(MSHTML::IHTMLElementPtr pElem, BSTR bstrText, std::string& sValue);
	void SetAttribute(MSHTML::IHTMLElementPtr pElem, BSTR bstrText, const std::string& sValue);
	void SaveHtml(std::string sHtml);
};

#endif
