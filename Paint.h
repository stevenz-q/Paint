// Ӧ�ó������ͷ�ļ�
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ���� 'pch.h' ������ PCH"
#endif

#include "resource.h"       // ������
// Paint:
class Paint : public CWinApp
{
public:
	Paint() noexcept;


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Paint theApp;
