/**
 * Offline Browser Component
 * @author Muhammad Sheraz Siddiqi
 * @version 1.0
 * @email sheraz_attari@hotmail.com
 * @copyright You're free to use it but I am not responsisble for any bugs
 */

#include "OfflineBrowser.h"
#include <atlbase.h>
#include <fstream>

/**
 * Constructor
 */

COfflineBrowser::COfflineBrowser()
{
	//Initialize COM
	CoInitialize(NULL);

	m_sScheme = "http://";

	//Generate GUID to avoid overwritting previously saved web pages
	GUID guid;
	CoCreateGuid(&guid);
	wchar_t szBuffer[1024];
	sprintf(szBuffer, "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1],guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5],guid.Data4[6], guid.Data4[7]);
	m_sGuid = szBuffer;
	m_sGuid = "proxypage";
}

/**
 * Destructor
 */

COfflineBrowser::~COfflineBrowser()
{
	//Uninitialize COM
	CoUninitialize();
}

/**
 * Gets HTML from the Given URL if bDownload is true and gets Host and Scheme out of the URL
 * @param sUrl Web page URL
 * @param bDownload if true Html is downloaded from the specified URL otherwise it is assumed
 * that HTML is already populated using SetHtml() function
 */
void COfflineBrowser::LoadHtml(const std::string& sUrl, bool bDownload /*= false*/)
{
	if(bDownload)
	{
		//Download Web Page using WININET
		HINTERNET hNet = InternetOpen("Offline Browser", INTERNET_OPEN_TYPE_PRECONFIG/*INTERNET_OPEN_TYPE_PROXY*/,
									  NULL, NULL, 0);
		if(hNet == NULL)
		{
			DWORD dwErr = GetLastError();
			return;
		}

		HINTERNET hFile = InternetOpenUrl(hNet, sUrl.c_str(), NULL, 0, 0, 0); 
		if(hFile == NULL)
			return;

		while(true)
		{
			const int MAX_BUFFER_SIZE = 65536;
			unsigned long nSize = 0;
			wchar_t szBuffer[MAX_BUFFER_SIZE+1];
			BOOL bRet = InternetReadFile(hFile, szBuffer, MAX_BUFFER_SIZE, &nSize);
			if(!bRet || nSize <= 0)
				break;
			szBuffer[nSize] = '\0';
			m_sHtml += szBuffer;
		}
	}

	//Get host name and scheme(protocol) out of URL
	int nStart = sUrl.find("://");
	if(nStart < 0)
		nStart = 0;
	else 
	{
		nStart += strlen("://");
		m_sScheme = sUrl.substr(0, nStart);
	}
	std::string sTemp = sUrl.substr(nStart, sUrl.length()-nStart);
	int nEnd = sTemp.find("/");
	if(nEnd < 0)
		nEnd = sUrl.length();
	sTemp = sTemp.substr(0, nEnd);
	
	if(sTemp[sTemp.length()-1] != '/')
		sTemp += "/";

	SetHost(sTemp);
}

/**
 * Download all the resources from the specified URL, it is assumed that the required member variables
 * are populated prior to this call
 * @param sUrl Relative resource URL
 * @param sPath Path of the folder where to save the downloaded resources
 * @return true if successfullly downloaded false otherwise
 */
bool COfflineBrowser::DownloadResource(std::string sUrl, std::string sPath)
{
	if(sUrl[0] ==  '/')
		sUrl = sUrl.substr(1, sUrl.length()-1);
	//Make absolute URL of the resource
	std::string sTemp = m_sScheme + m_sHost;
	sTemp += sUrl;
	
	//Make absolute location on hard disk to save the file
	std::string sTemp2 = m_sDir;
	sTemp2 += sPath;
	for(int a=0;a<sTemp2.length();a++)
	{
		if(sTemp2[a] == '/')
			sTemp2[a] = '\\';
	}

	//Download specified resource
	if(URLDownloadToFile(NULL, sTemp.c_str(), sTemp2.c_str(), 0, NULL) == S_OK)
		return true;
	else return false;
}

/**
 * Creates all the directories necessary to copy resource to have same relative path i.e.
 * img/myfiles/img.gif will create two directories img and inside img it creates a new dir
 * myfiles
 * @param sSrc Resource URL (it could be absolute or relative)
 * @param sBasePath Base folder that will hold all the newly created folders.
 */
void COfflineBrowser::CreateDirectories(std::string sSrc, std::string sBasePath)
{
	if(sSrc[0] == '\\')
		sSrc = sSrc.substr(1, sSrc.length()-1);

	//Keep creating directories until we've Directory separator '/' or '\\' in the path
	while(true)
	{
		int nIndex = sSrc.find('\\');
		if(nIndex < 0)
		{
			nIndex = sSrc.find('/');
			if(nIndex < 0)
				break;
		}

		std::string sDir = sSrc.substr(0, nIndex);
		sDir = sBasePath + sDir; 
		
		CreateDirectory(sDir.c_str(), NULL);
		
		sSrc = sSrc.substr(nIndex+1, sSrc.length()-nIndex-1);
		sBasePath = sDir + '\\';
	}
}

/**
 * Delete the directory
 * @param sDir Directory to delete
 */
void COfflineBrowser::DeleteDirectory(std::string sDir)
{
	//Clean up previously filled arrays
	m_dirArray.erase(m_dirArray.begin(), m_dirArray.end());
	m_fileArray.erase(m_fileArray.begin(), m_fileArray.end());

	//Empty directory fist before creating
	EmptyDirectory(sDir);

	//Delete all the files
	for(int a=m_fileArray.size()-1;a>=0;a--)
		DeleteFile(m_fileArray[a].c_str());

	//Delete every directory now which are now empty
	for(a=m_dirArray.size()-1;a>=0;a--)
		RemoveDirectory(m_dirArray[a].c_str());
}

/**
 * Empty the directory i.e. delete all the files inside it
 * @param sDir Directory to empty
 */
void COfflineBrowser::EmptyDirectory(std::string sDir)
{
	//Keep iterating through the directory if you find a directory recursively call itself and 
	//if you wont find a file  add it to the vector
	m_dirArray.push_back(sDir);

	WIN32_FIND_DATA findData;

	std::string sFile = sDir + "\\*";
	HANDLE hFind = FindFirstFile(sFile.c_str(), &findData);

	if(hFind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if(strcmpi(findData.cFileName, ".") == 0 || strcmpi(findData.cFileName, "..") == 0)
			continue;

		std::string sDeleteFile = sDir + "\\" + findData.cFileName;

		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			EmptyDirectory(sDeleteFile);	
		else 
			m_fileArray.push_back(sDeleteFile);

	}while(FindNextFile(hFind, &findData));

	FindClose(hFind);
}

/**
 * Shows if the resource URL is absolute or relative
 * @param sSrc Resource URL
 * @return true if URL is absolute false if it is relative
 */
bool COfflineBrowser::IsAbsolute(const std::string& sSrc)
{
	int nRet = 0;
	if(sSrc[0] == '/')
		return true;
	if((nRet = sSrc.find(":\\")) > 0)
		return true;
	if((nRet = sSrc.find("://")) > 0)
		return true;
	return false;
}

/**
 * It Downloads all the resources, main function of concern, all the required members variables must 
 * be populated for it to work correctly
 */
void COfflineBrowser::BrowseOffline()
{
	USES_CONVERSION;

	//Delete if this directory exists, it is a disputed call, you can safely comment it out
	DeleteDirectory(m_sDir);
	//Create the directory again
	CreateDirectory(m_sDir.c_str(), NULL);
	//SaveHtml(m_sHtml);
	
	//Create HTML Document which will be hold the html that was have 
	MSHTML::IHTMLDocument2Ptr pDoc;
	HRESULT hr = CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, 
								  IID_IHTMLDocument2, (void**)&pDoc);
	if(pDoc == NULL)
		return;

	//Load HTML to Html Document
	SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	VARIANT *param;
	bstr_t bsData = (LPCTSTR)m_sHtml.c_str();
	hr =  SafeArrayAccessData(psa, (LPVOID*)&param);
	param->vt = VT_BSTR;
	param->bstrVal = (BSTR)bsData;
	
	//write your buffer
	hr = pDoc->write(psa);	
	//closes the document, "applying" your code  
	hr = pDoc->close();	

	//Don't forget to free the SAFEARRAY!
	SafeArrayDestroy(psa);

	//Iterate through all the elements in the document
	MSHTML::IHTMLElementCollectionPtr pCollection = pDoc->all;

	for(long a=0;a<pCollection->length;a++)
	{
		std::string sValue;
		IHTMLElementPtr pElem = pCollection->item( a );
		//If src attribute is found that means we've a resource to download
		if(GetAttribute(pElem, L"src", sValue))
		{
			//If resource URL is relative
			if(!IsAbsolute(sValue))
			{
				if(sValue[0] == '/')
					sValue = sValue.substr(1, sValue.length()-1);
				//Create directories needed to hold this resource
				//CreateDirectories(sValue, m_sDir);
				//Download the resource
				if(1)//!DownloadResource(sValue, sValue))
				{
					std::string sTemp = m_sScheme + m_sHost;
					sTemp += sValue;
					//Update src to the new src and put the original src attribute as
					//srcdump just for future references
					if(sTemp[0] == '/')
						sTemp = sTemp.substr(1, sTemp.length()-1);
					SetAttribute(pElem, L"src", sTemp);
					SetAttribute(pElem, L"srcdump", sValue);
				}
				//Unable to download the resource
				else
				{
					//Put srcdump same as src, It if for no use, I just put it to make
					//HTML DOM consistent
					SetAttribute(pElem, L"srcdump", sValue);
				}
			}
			//If resource URL is absolute
			else
			{
				std::string sTemp;
				//Make URL relative
				sTemp = TrimHostName(sValue);
				//Create directories needed to hold this resource
				//CreateDirectories(sTemp, m_sDir);
				//Dowload the resource
				if(1)//DownloadResource(sTemp, sTemp))
				{
					//Update src to the new src and put the original src attribute as
					//srcdump just for future references
					if(sTemp[0] == '/')
						sTemp = sTemp.substr(1, sTemp.length()-1);
					SetAttribute(pElem, L"src", sTemp);
					SetAttribute(pElem, L"srcdump", sValue);
				}
			}
		}
	}
	
	//Get upated HTML out of amendments we made and save it to the described directory
	MSHTML::IHTMLDocument3Ptr pDoc3 = pDoc;
	MSHTML::IHTMLElementPtr pDocElem;
	pDoc3->get_documentElement(&pDocElem);
	BSTR bstrHtml;
	pDocElem->get_outerHTML(&bstrHtml);
	std::string sNewHtml((const wchar_t*)OLE2T(bstrHtml));
	SaveHtml(sNewHtml);
}

/**
 * Gets value of specified attribute out of some HTML Tag
 * @param pElem HTML element pointer
 * @param bstrText Name of the attribute
 * @param sValue Reference of the string that will hold the value
 * @return true if attribute is present false otherwise
 */
bool COfflineBrowser::GetAttribute(MSHTML::IHTMLElementPtr pElem, BSTR bstrText, std::string& sValue)
{
	try
	{
		CComVariant var = pElem->getAttribute(bstrText, 0);
		USES_CONVERSION;
		if (var.vt == VT_BSTR && var.bstrVal != NULL)
			sValue = (LPCTSTR)OLE2T(var.bstrVal);
		else
			sValue = "";
	}
	//Attribute not present
	catch (_com_error e)
	{
		sValue = "";
	}
	//Special condition for about:blank, keep remember it
	int nIndex = sValue.find("about:blank");
	if(nIndex == 0)
	{
		int nLen = strlen("about:blank");
		sValue = sValue.substr(nLen, sValue.length()-nLen);
	}
	return sValue.length() ? true : false;
}

/**
 * Sets value of specified attribute in some HTML Tag
 * @param pElem HTML element pointer
 * @param bstrText Name of the attribute
 * @param sValue Value of the attribute
 */
void COfflineBrowser::SetAttribute(MSHTML::IHTMLElementPtr pElem, BSTR bstrText, const std::string& sValue)
{
	try
	{
		pElem->setAttribute(bstrText, sValue.c_str(), 0);
	}
	//Some erroneous condition occur, it shouldn't be thrown under normal conditions
	catch(_com_error e)
	{
		int a;
		a=0;
	}
}

/**
 * Save html to hard disk
 * @param sHtml HTML to save
 */
void COfflineBrowser::SaveHtml(std::string sHtml)
{
	std::fstream myFile;
	std::string sTemp = m_sDir + m_sGuid + ".htm";
	myFile.open(sTemp.c_str(), std::ios::out);
	myFile.write(sHtml.c_str(), sHtml.length()+1);
	myFile.close();
}

/**
 * Removes host name from the URL, in other words, it transforms absolute URL into relative  URL
 * @param sUrl Absolute URL to transform
 * @return Relative URL
 */
std::string COfflineBrowser::TrimHostName(const std::string& sUrl)
{
	//Search for the scheme and host name and trim it from the URL
	int nStart = sUrl.find("://");
	if(nStart < 0)
	{
		nStart = sUrl.find(":\\");
		if(nStart < 0)
			return sUrl;
		else nStart += strlen(":\\");
	}
	else nStart += strlen("://");
	std::string sTemp = sUrl.substr(nStart, sUrl.length()-nStart);
	
	nStart = sTemp.find("/");
	if(nStart < 0)
	{
		nStart = sTemp.find("\\");
		if(nStart < 0)
			return sTemp;
		else nStart += strlen("\\");
	}
	else nStart += strlen("/");

	sTemp = sTemp.substr(nStart, sTemp.length()-nStart);

	return sTemp;
}
