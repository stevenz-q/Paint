// PaintView ��

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
#ifndef _DEBUG 
inline PaintDoc* PaintView::GetDocument() const
   { return reinterpret_cast<PaintDoc*>(m_pDocument); }
#endif

