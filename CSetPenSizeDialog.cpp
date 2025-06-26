// CSetPenSizeDialog.cpp: ʵ���ļ�
#include "pch.h"
#include "Paint.h"
#include "afxdialogex.h"
#include "CSetPenSizeDialog.h"

// CSetPenSizeDialog �Ի���
IMPLEMENT_DYNAMIC(CSetPenSizeDialog, CDialogEx)

// ���캯��
CSetPenSizeDialog::CSetPenSizeDialog(CWnd* pParent)
	: CDialogEx(IDD_LINEWIDTHBOX, pParent), m_PenSize(3)
{
}
// ��������
CSetPenSizeDialog::~CSetPenSizeDialog()
{
}
// �Ի������ݽ���
void CSetPenSizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PENSIZE_INPUT, m_PenSize);
}
// ��Ϣӳ��
BEGIN_MESSAGE_MAP(CSetPenSizeDialog, CDialogEx)
END_MESSAGE_MAP()
