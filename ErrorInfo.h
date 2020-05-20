#pragma once


// ErrorInfo dialog

class ErrorInfo : public CDialogEx
{
	DECLARE_DYNAMIC(ErrorInfo)

public:
	ErrorInfo(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ErrorInfo();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit ErrorEdit;
};
