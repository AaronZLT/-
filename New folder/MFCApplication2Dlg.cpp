
// MFCApplication2Dlg.cpp : implementation file
//
#define _CRT_SECURE_NO_DEPRECATE
#define Grammer true
#pragma warning(disable: 4996)
#include"iostream"
#include"string"
#include"fstream"
#include"queue"
#include"math.h"
#include"string.h"
#include"vector"
#include"stack"

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include "ErrorInfo.h"

#include"TokenDetect.h"
#include"Grammer.h"
#include"GlobleVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

extern ifstream fin;
extern Tree* GrammerTree;
bool ifOpenfile = false;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication2Dlg dialog



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
	, snlPath_cs(_T(""))
	, editBox(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, SNLFilePath);
	DDX_Control(pDX, IDC_EDIT1, Outtxt);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, snlPath_cs);
	DDX_Text(pDX, IDC_EDIT1, editBox);
	DDX_Control(pDX, IDC_EDIT2, TokenList);
	DDX_Control(pDX, IDC_EDIT3, GrammerList);
	DDX_Control(pDX, IDC_PROGRESS2, Progress);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::Analysis)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CMFCApplication2Dlg::OnEnChangeMfceditbrowse1)
	ON_EN_CHANGE(IDC_EDIT2, &CMFCApplication2Dlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDOK, &CMFCApplication2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication2Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication2Dlg::OnEnChangeEdit1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS2, &CMFCApplication2Dlg::OnNMCustomdrawProgress2)
END_MESSAGE_MAP()


// CMFCApplication2Dlg message handlers

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
	Progress.SetStep(1);
	Progress.SetRange(0, 100);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication2Dlg::Analysis()
{
	// TODO: Add your control notification handler code here
	Progress.SetPos(0);
	if (ifOpenfile) {
		fin.open(CT2A(snlPath_cs.GetString()), ios_base::in);
	}
	else {
		CString snl;
		char c;
		Outtxt.GetWindowTextW(snl);
		string str = CT2A(snl.GetString());
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == '\r') {
				str.erase(i, 1);
				//str.erase()
				continue;
			}
			Progress.StepIt();
		}
		snl = CA2T(str.c_str());
		CString path("program_cache.txt");
		CStdioFile csf;
		csf.Open(path, CFile::modeCreate | CFile::modeWrite);
		
		csf.WriteString(snl);
		csf.Close();
		fin.open("program_cache.txt", ios_base::in);
	}
	ResetAll();
	Initiation();
	getToken();
	CString tokenlist;
	tokenlist = CA2T(OutputToken_s().c_str());
	fin.close();
	TokenList.SetWindowTextW(tokenlist);

	GrammerTree = Program();
	vector<int>* v = new vector<int>();
	v->push_back(0);
	CString grammerlist;
	grammerlist = CA2T(_Display(GrammerTree, 0, v, false).c_str());
	GrammerList.SetWindowTextW(grammerlist);
	Progress.SetPos(100);
	if (ErrCount() > 0) {
		TokenList.SetWindowText(_T(""));
		GrammerList.SetWindowTextW(_T(""));
		ErrorOutput();
	}
}


void CMFCApplication2Dlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	//editBox = snlPath_cs;
	SNLFilePath.GetWindowTextW(snlPath_cs);
	Outtxt.SetWindowTextW(snlPath_cs);
	//UpdateData();
}


void CMFCApplication2Dlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CMFCApplication2Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CMFCApplication2Dlg::OnBnClickedButton2()
{
	Progress.SetPos(0);
	ifOpenfile = true;
	CStdioFile csf;
	csf.Open(snlPath_cs, CFile::modeRead);
	CString tempstring;
	CString snlstring;
	while (csf.ReadString(tempstring))
	{
		snlstring.Append(tempstring);
		snlstring += "\r\n";
		Progress.StepIt();
	}
	Progress.SetPos(100);
	Outtxt.SetWindowTextW(snlstring);
	csf.Close();
}


void CMFCApplication2Dlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	ifOpenfile = false;
}


void CMFCApplication2Dlg::OnNMCustomdrawProgress2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CMFCApplication2Dlg::ErrorOutput()
{
	CString errorCS;
	errorCS = CA2T(_ERROROUTPUT().c_str());

	ErrorInfo* pDlg = new ErrorInfo;
	pDlg->Create(IDD_DIALOG1, this);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->ErrorEdit.SetWindowTextW(errorCS);
}
