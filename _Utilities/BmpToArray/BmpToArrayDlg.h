// BmpToArrayDlg.h : header file
//

#if !defined(AFX_BMPTOARRAYDLG_H__EB377E24_9408_4A0F_9BBE_E507461FDD8F__INCLUDED_)
#define AFX_BMPTOARRAYDLG_H__EB377E24_9408_4A0F_9BBE_E507461FDD8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayDlg dialog

class CBmpToArrayDlg : public CDialog
{
// Construction
public:
	CBmpToArrayDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBmpToArrayDlg)
	enum { IDD = IDD_BMPTOARRAY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpToArrayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBmpToArrayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonPict();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPTOARRAYDLG_H__EB377E24_9408_4A0F_9BBE_E507461FDD8F__INCLUDED_)
