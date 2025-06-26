
// PaintView ��Ľӿ�
//

#pragma once
#include "PaintDoc.h"

class PaintView : public CView
{
protected: // �������л�����
	PaintView() noexcept;
	DECLARE_DYNCREATE(PaintView)

// ����
public:
	PaintDoc* GetDocument() const;

// ����
public:
	int m_PenSize;
	COLORREF m_PenColor, m_BrushColor;
	CPoint m_PointBegin, m_PointEnd;
	void ConvertToSquare(CRect& rect);
	// �ж����ڻ�����ʲô���͵��߻�ͼ��
	enum class DrawType {
		LineSegment, Circle, Rectangle, Ellipse, Pencil, Eraser, Text
	}m_DrawType;
	int m_TextId;// �ı�ID
	int m_IsFilled;// �Ƿ����
	CEdit* m_Edit = nullptr;
	CPoint m_TextPos = CPoint(0, 0);

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~PaintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnDrawLineSegment();
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawEllipse();
	afx_msg void OnSetColor();
	afx_msg void OnSetPenSize();
	afx_msg void OnText();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetPencil();
	afx_msg void OnSetEraser();
	afx_msg void PaintView::ONLoadFile();
	afx_msg void OnFileSave();
	afx_msg void OnPenSize1();
	afx_msg void OnPenSize3();
	afx_msg void OnPenSize5();
	afx_msg void OnPenSize8();
	afx_msg void OnSetClear();
	afx_msg void OnUpdateIsfilled(CCmdUI* pCmdUI);
	afx_msg void PaintView::OnSetIsfilled();
};


//class CDragRect 
//{
//public:
//    void OnInitDialog(HWND hwnd);
//
//    void OnSize(int x, int y);
//    void OnPaint(HDC hdc);
//
//    void OnMouseDown(int x, int y);
//    void OnMouseUp();
//    void OnMouseMove(int x, int y);
//
//    // ���ε���С��С
//    void SetMinSize(int width, int height) { mMinWidth = width; mMinHeight = height; }
//
//protected:
//    // ��������
//    static bool mIsAtRect(RECT& rect, int x, int y) {
//        if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom) {
//            return true;
//        }
//        return false;
//    }
//
//    // �����½�(���ô�С)������
//    static bool mIsAtSERect(RECT& rect, int x, int y) {
//        RECT rct;
//        int width = 5;
//        rct.left = rect.right - width;
//        rct.top = rect.bottom - width;
//        rct.right = rect.right + width;
//        rct.bottom = rect.bottom + width;
//
//        return mIsAtRect(rct, x, y);
//    }
//
//    // ��ȡ���
//    static HCURSOR yGetWndCursor(HWND hWnd) {
//        DWORD currCur = ::GetClassLong(hWnd, GCL_HCURSOR);
//        return (HCURSOR)currCur;
//    }
//
//    // ���ù��
//    static void ySetWndCursor(HWND hWnd, HCURSOR hCursor) {
//        DWORD current = ::GetClassLong(hWnd, GCL_HCURSOR);
//        if (current != (LONG)hCursor) {
//            ::SetClassLong(hWnd, GCL_HCURSOR, (LONG)hCursor);
//        }
//    }
//
//public:
//    static HWND mHwnd; //�������ڵĴ���
//    static RECT rectDrag; // �϶��ľ������
//    static bool mIsMouseDown;
//    static POINT mMouseDownLocation; // ��갴��ʱ��λ��
//    static RECT mouseDownRect; // ��¼��갴��ʱrectDrag��ֵ�� ���ڼ����϶���Ĵ�С
//    HCURSOR mCurSizeAll, mCurSizeNWSE, mCurDefault; //���
//    int mMinWidth, mMinHeight; // ���ε���С��С
//    int mMargin; // ���ξ����Ե����С����
//};










#ifndef _DEBUG  // DrawLine2021213380View.cpp �еĵ��԰汾
inline PaintDoc* PaintView::GetDocument() const
   { return reinterpret_cast<PaintDoc*>(m_pDocument); }
#endif

