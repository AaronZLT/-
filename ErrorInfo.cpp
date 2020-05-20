// ErrorInfo.cpp : implementation file
//

#include "pch.h"
#include "ErrorInfo.h"
#include "afxdialogex.h"
#include "resource.h"


// ErrorInfo dialog

IMPLEMENT_DYNAMIC(ErrorInfo, CDialogEx)

ErrorInfo::ErrorInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

ErrorInfo::~ErrorInfo()
{
}

void ErrorInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ErrorEdit);
}


BEGIN_MESSAGE_MAP(ErrorInfo, CDialogEx)
END_MESSAGE_MAP()


// ErrorInfo message handlers
