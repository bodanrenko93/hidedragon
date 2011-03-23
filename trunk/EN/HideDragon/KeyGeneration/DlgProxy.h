// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__E0EE1D99_8C28_4214_BD93_30F3F99CDF7D__INCLUDED_)
#define AFX_DLGPROXY_H__E0EE1D99_8C28_4214_BD93_30F3F99CDF7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyGenerationDlg;

/////////////////////////////////////////////////////////////////////////////
// CKeyGenerationDlgAutoProxy command target

class CKeyGenerationDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CKeyGenerationDlgAutoProxy)

	CKeyGenerationDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CKeyGenerationDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyGenerationDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CKeyGenerationDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CKeyGenerationDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CKeyGenerationDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CKeyGenerationDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__E0EE1D99_8C28_4214_BD93_30F3F99CDF7D__INCLUDED_)
