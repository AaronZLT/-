
// MFCApplication2Dlg.h : header file
//

#pragma once


// CMFCApplication2Dlg dialog
class CMFCApplication2Dlg : public CDialogEx
{
// Construction
public:
	CMFCApplication2Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void Analysis();
	afx_msg void OnEnChangeMfceditbrowse1();
	CMFCEditBrowseCtrl SNLFilePath;
	CEdit Outtxt;
	CString snlPath_cs;
	CString editBox;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	CEdit TokenList;
	afx_msg void OnEnChangeEdit1();
	CEdit GrammerList;
	afx_msg void OnNMCustomdrawProgress2(NMHDR* pNMHDR, LRESULT* pResult);
	CProgressCtrl Progress;
	CEdit ProgressStatus;
	afx_msg void ErrorOutput();
};
