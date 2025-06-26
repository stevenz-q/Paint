// CSetPenSizeDialog.cpp: 实现文件
#include "pch.h"
#include "Paint.h"
#include "afxdialogex.h"
#include "CSetPenSizeDialog.h"

// CSetPenSizeDialog 对话框
IMPLEMENT_DYNAMIC(CSetPenSizeDialog, CDialogEx)

// 构造函数
CSetPenSizeDialog::CSetPenSizeDialog(CWnd* pParent)
	: CDialogEx(IDD_LINEWIDTHBOX, pParent), m_PenSize(3)
{
}
// 析构函数
CSetPenSizeDialog::~CSetPenSizeDialog()
{
}
// 对话框数据交换
void CSetPenSizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PENSIZE_INPUT, m_PenSize);
}
// 消息映射
BEGIN_MESSAGE_MAP(CSetPenSizeDialog, CDialogEx)
END_MESSAGE_MAP()
