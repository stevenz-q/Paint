
// PaintView 类的接口
//

#pragma once
#include "PaintDoc.h"

class PaintView : public CView
{
protected: // 仅从序列化创建
	PaintView() noexcept;
	DECLARE_DYNCREATE(PaintView)

// 特性
public:
	PaintDoc* GetDocument() const;

// 操作
public:
	int m_PenSize;
	COLORREF m_PenColor, m_BrushColor;
	CPoint m_PointBegin, m_PointEnd;
	void ConvertToSquare(CRect& rect);
	// 判断正在画的是什么类型的线或图案
	enum class DrawType {
		LineSegment, Circle, Rectangle, Ellipse, Pencil, Eraser, Text
	}m_DrawType;
	int m_TextId;// 文本ID
	int m_IsFilled;// 是否填充
	CEdit* m_Edit = nullptr;
	CPoint m_TextPos = CPoint(0, 0);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~PaintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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
//    // 矩形的最小大小
//    void SetMinSize(int width, int height) { mMinWidth = width; mMinHeight = height; }
//
//protected:
//    // 在区域内
//    static bool mIsAtRect(RECT& rect, int x, int y) {
//        if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom) {
//            return true;
//        }
//        return false;
//    }
//
//    // 在右下角(设置大小)区域内
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
//    // 获取光标
//    static HCURSOR yGetWndCursor(HWND hWnd) {
//        DWORD currCur = ::GetClassLong(hWnd, GCL_HCURSOR);
//        return (HCURSOR)currCur;
//    }
//
//    // 设置光标
//    static void ySetWndCursor(HWND hWnd, HCURSOR hCursor) {
//        DWORD current = ::GetClassLong(hWnd, GCL_HCURSOR);
//        if (current != (LONG)hCursor) {
//            ::SetClassLong(hWnd, GCL_HCURSOR, (LONG)hCursor);
//        }
//    }
//
//public:
//    static HWND mHwnd; //矩形所在的窗口
//    static RECT rectDrag; // 拖动的句柄区域
//    static bool mIsMouseDown;
//    static POINT mMouseDownLocation; // 鼠标按下时的位置
//    static RECT mouseDownRect; // 记录鼠标按下时rectDrag的值， 用于计算拖动后的大小
//    HCURSOR mCurSizeAll, mCurSizeNWSE, mCurDefault; //鼠标
//    int mMinWidth, mMinHeight; // 矩形的最小大小
//    int mMargin; // 矩形距离边缘的最小距离
//};










#ifndef _DEBUG  // DrawLine2021213380View.cpp 中的调试版本
inline PaintDoc* PaintView::GetDocument() const
   { return reinterpret_cast<PaintDoc*>(m_pDocument); }
#endif

