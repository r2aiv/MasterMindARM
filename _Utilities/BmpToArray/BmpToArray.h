// BmpToArray.h : main header file for the BMPTOARRAY application
//

#if !defined(AFX_BMPTOARRAY_H__070518A8_67AB_4296_A2B8_E8CE093ABF00__INCLUDED_)
#define AFX_BMPTOARRAY_H__070518A8_67AB_4296_A2B8_E8CE093ABF00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayApp:
// See BmpToArray.cpp for the implementation of this class
//

class CBmpToArrayApp : public CWinApp
{
public:
	CBmpToArrayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpToArrayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBmpToArrayApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPTOARRAY_H__070518A8_67AB_4296_A2B8_E8CE093ABF00__INCLUDED_)
