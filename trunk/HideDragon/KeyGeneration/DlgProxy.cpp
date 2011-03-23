// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "KeyGeneration.h"
#include "DlgProxy.h"
#include "KeyGenerationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyGenerationDlgAutoProxy

IMPLEMENT_DYNCREATE(CKeyGenerationDlgAutoProxy, CCmdTarget)

CKeyGenerationDlgAutoProxy::CKeyGenerationDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CKeyGenerationDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CKeyGenerationDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CKeyGenerationDlgAutoProxy::~CKeyGenerationDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CKeyGenerationDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CKeyGenerationDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CKeyGenerationDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CKeyGenerationDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CKeyGenerationDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IKeyGeneration to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {1590B9AE-8845-4A55-9085-14AF62EACBB5}
static const IID IID_IKeyGeneration =
{ 0x1590b9ae, 0x8845, 0x4a55, { 0x90, 0x85, 0x14, 0xaf, 0x62, 0xea, 0xcb, 0xb5 } };

BEGIN_INTERFACE_MAP(CKeyGenerationDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CKeyGenerationDlgAutoProxy, IID_IKeyGeneration, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {C460231D-EBA5-4316-BD60-4B3F59C59926}
IMPLEMENT_OLECREATE2(CKeyGenerationDlgAutoProxy, "KeyGeneration.Application", 0xc460231d, 0xeba5, 0x4316, 0xbd, 0x60, 0x4b, 0x3f, 0x59, 0xc5, 0x99, 0x26)

/////////////////////////////////////////////////////////////////////////////
// CKeyGenerationDlgAutoProxy message handlers
