// CSetPenSizeDialog.cpp: 实现文件

#include "pch.h"
#include "Paint.h"
#include "afxdialogex.h"
#include "CSetPenSizeDialog.h"


// CSetPenSizeDialog 对话框

IMPLEMENT_DYNAMIC(CSetPenSizeDialog, CDialogEx)

CSetPenSizeDialog::CSetPenSizeDialog(CWnd* pParent)
	: CDialogEx(IDD_LINEWIDTHBOX, pParent), m_PenSize(3)
{

}

CSetPenSizeDialog::~CSetPenSizeDialog()
{
}

void CSetPenSizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PENSIZE_INPUT, m_PenSize);
}


BEGIN_MESSAGE_MAP(CSetPenSizeDialog, CDialogEx)
END_MESSAGE_MAP()


// CSetPenSizeDialog 消息处理程序
