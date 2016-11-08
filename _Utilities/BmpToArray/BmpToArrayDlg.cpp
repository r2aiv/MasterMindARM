// BmpToArrayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BmpToArray.h"
#include "BmpToArrayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayDlg dialog

CBmpToArrayDlg::CBmpToArrayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBmpToArrayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBmpToArrayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBmpToArrayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBmpToArrayDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBmpToArrayDlg, CDialog)
	//{{AFX_MSG_MAP(CBmpToArrayDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PICT, OnButtonPict)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayDlg message handlers

BOOL CBmpToArrayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBmpToArrayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBmpToArrayDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBmpToArrayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBmpToArrayDlg::OnButtonPict() 
{



	FILE* out_file;
	CString str;
	CBitmap bitmap;
	BITMAP bmp;
	unsigned char bmp_array[132*132*4];	

	//unsigned char bmp_array[255];	


	static char BASED_CODE szFilter[] = "Bitmap Files (*.bmp)|*.bmp||";	
	
	//CFileDialog FileDialog(TRUE, "olimex", "select bitmap", OFN_HIDEREADONLY & OFN_OVERWRITEPROMPT, szFilter);
	CFileDialog FileDialog(TRUE, NULL, NULL, NULL, szFilter);

	if(FileDialog.DoModal() == IDOK) {

		str = FileDialog.GetPathName();

		//HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE & LR_CREATEDIBSECTION);
		HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		
		
		if(hBitmap) {

			if(bitmap.DeleteObject()) 
				bitmap.Detach();

			bitmap.Attach(hBitmap);

			bitmap.GetBitmap(&bmp);

			bitmap.GetBitmapBits(bmp.bmHeight*bmp.bmWidth*4, bmp_array);

			//bitmap.GetBitmapBits(255, bmp_array);

		}
		else {
			MessageBox("Cannot load bitmap");
		}
	}



	if( (out_file = fopen( "bmp.h", "w")) == NULL )
	{
		//printf( "The file %s was not opened\n\r",Filename );
		//return(0);
		MessageBox("Cannot create bmp.h file!!!");
	}


	fprintf(out_file,"/*************************************************************************\n");
	fprintf(out_file," *\n");
	fprintf(out_file," *	Author: Slavcho Tomov\n");
	fprintf(out_file," *\n");
	fprintf(out_file," *    (c) Copyright Olimex 2006\n");
	fprintf(out_file," *\n");
	fprintf(out_file," **************************************************************************/\n");

	fprintf(out_file,"const unsigned char bmp [] = {\n");


	unsigned char   data_on_line = 8;
	fprintf(out_file,"\t\t\t\t");

	unsigned char   Data_1;
	unsigned char   Data_2;
	unsigned char   Data_3;	

	//Row = 1;
	//for (int i=0; i<bmp.bmHeight; i++)
	//{
		

		for(int j=0; j<4*bmp.bmHeight*bmp.bmWidth; j+=8)
		{
		
			Data_1 = (unsigned char)((bmp_array[j+0]&0xF0) | ((bmp_array[j+1]>>4)&0x0F));
			Data_2 = (unsigned char)((bmp_array[j+2]&0xF0) | ((bmp_array[j+4]>>4)&0x0F));
			Data_3 = (unsigned char)((bmp_array[j+5]&0xF0) | ((bmp_array[j+6]>>4)&0x0F));
			
			// normanlize
			fprintf(out_file,"0x%0.2x, ",Data_1);
			fprintf(out_file,"0x%0.2x, ",Data_2);
			fprintf(out_file,"0x%0.2x, ",Data_3);
			
			if(--data_on_line == 0)
			{
				data_on_line = 8;
				fprintf(out_file,"\n\t\t\t\t");
			}
		}

	//}

	
	fprintf(out_file,"\n};\n");
	fclose(out_file);


}
