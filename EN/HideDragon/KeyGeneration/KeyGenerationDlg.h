// KeyGenerationDlg.h : header file
//

#if !defined(AFX_KEYGENERATIONDLG_H__F5EF9612_3CC9_4C78_A738_592B1B24A519__INCLUDED_)
#define AFX_KEYGENERATIONDLG_H__F5EF9612_3CC9_4C78_A738_592B1B24A519__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyGenerationDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CKeyGenerationDlg dialog

class CKeyGenerationDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyGenerationDlg);
	friend class CKeyGenerationDlgAutoProxy;

// Construction
public:
	CKeyGenerationDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CKeyGenerationDlg();

// Dialog Data
	//{{AFX_DATA(CKeyGenerationDlg)
	enum { IDD = IDD_KEYGENERATION_DIALOG };
	CString	m_edit5;
	CString	m_edit6;
	CString	m_edit1;
	CString	m_edit2;
	CString	m_edit3;
	CString	m_edit4;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyGenerationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CKeyGenerationDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CKeyGenerationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYGENERATIONDLG_H__F5EF9612_3CC9_4C78_A738_592B1B24A519__INCLUDED_)
