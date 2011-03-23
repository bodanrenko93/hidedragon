// KeyGeneration.h : main header file for the KEYGENERATION application
//

#if !defined(AFX_KEYGENERATION_H__0E8FBB55_956D_4402_9659_2C0CF56CC5E0__INCLUDED_)
#define AFX_KEYGENERATION_H__0E8FBB55_956D_4402_9659_2C0CF56CC5E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeyGenerationApp:
// See KeyGeneration.cpp for the implementation of this class
//

class CKeyGenerationApp : public CWinApp
{
public:
	CKeyGenerationApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyGenerationApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKeyGenerationApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYGENERATION_H__0E8FBB55_956D_4402_9659_2C0CF56CC5E0__INCLUDED_)
