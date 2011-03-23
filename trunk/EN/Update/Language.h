














int SetCurrentLanguage();
int SetDialogText(HWND * pDialog,UINT uID);
BOOL CALLBACK EnumDlgChildProc(HWND hwnd,LPARAM lParam);
wchar_t * LoadItemString(wchar_t * pItemID,wchar_t * pText,wchar_t * pDefault = NULL);int GetSysLangLoadLang();
int LoadLanguage(wchar_t * pLanguage,wchar_t * pFilePath);
int LoadLanguageBaseID(LANGID LangID = 0x0802);
int HDMessageBox(wchar_t * cStringID,wchar_t * cDefault,HWND hParentWnd = NULL,
				 UINT uType = MB_OK|MB_ICONINFORMATION);


















