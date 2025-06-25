// PaintView ���ʵ��

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Paint.h"
#endif

#include "PaintDoc.h"
#include "PaintView.h"
#include "resource.h"
#include "CSetPenSizeDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// PaintView

IMPLEMENT_DYNCREATE(PaintView, CView)

BEGIN_MESSAGE_MAP(PaintView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_SAVE, &PaintView::OnFileSave)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_LINE, &PaintView::OnDrawLineSegment)
	ON_COMMAND(IDM_RECTANGLE, &PaintView::OnDrawRectangle)
	ON_COMMAND(IDM_CIRCLE, &PaintView::OnDrawCircle)
	ON_COMMAND(IDM_ECLIPSE, &PaintView::OnDrawEllipse)
	ON_COMMAND(IDM_SET_COLOR, &PaintView::OnSetColor)
	ON_COMMAND(ID_SET_PENCIL, &PaintView::OnSetPencil)
	ON_COMMAND(IDM_LINWIDTH, &PaintView::OnSetPenSize)
	ON_COMMAND(IDM_TEXT, &PaintView::OnText)
	ON_COMMAND(IDM_SET_ERASER, &PaintView::OnSetEraser)
	ON_COMMAND(ID_32785, &PaintView::OnPenSize1)
	ON_COMMAND(ID_32786, &PaintView::OnPenSize3)
	ON_COMMAND(ID_32787, &PaintView::OnPenSize5)
	ON_COMMAND(ID_32788, &PaintView::OnPenSize8)
END_MESSAGE_MAP()

// PaintView ����/����

PaintView::PaintView() noexcept
{
	// TODO: �ڴ˴���ӹ������
	m_PenSize = 3;
	m_PenColor = RGB(0, 0, 0);
	m_BrushColor = RGB(0, 0, 0);
	m_PointBegin = CPoint(0, 0);
	m_PointEnd = CPoint(0, 0);
	m_DrawType = DrawType::Pencil; // Ĭ�ϻ���
	m_TextId = 10086;
}

PaintView::~PaintView()
{
	if (m_Edit != nullptr) {
		delete m_Edit;
		m_Edit = nullptr;
	}
}

BOOL PaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// PaintView ��ͼ

void PaintView::OnDraw(CDC* /*pDC*/)
{
	PaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// PaintView ��ӡ

BOOL PaintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void PaintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void PaintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// PaintView ���

#ifdef _DEBUG
void PaintView::AssertValid() const
{
	CView::AssertValid();
}

void PaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

PaintDoc* PaintView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(PaintDoc)));
	return (PaintDoc*)m_pDocument;
}
#endif //_DEBUG


// PaintView ��Ϣ�������


void PaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_PointBegin = m_PointEnd = point; // ��ʼ��begin
	m_TextPos = point;

	// ����㻭һ�ʣ���һ���㣩
	CClientDC dc(this);

	CView::OnLButtonDown(nFlags, point);
}


void PaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// �ƶ���ʱ�������ס
	if (nFlags & MK_LBUTTON) {
		CClientDC dc(this);
		CPen newPen, * oldPen;
		newPen.CreatePen(PS_SOLID, m_PenSize, m_PenColor); // ����һ֧��
		oldPen = dc.SelectObject(&newPen); // ѡ����֧��

		switch (m_DrawType) {
		case DrawType::LineSegment:

			dc.SetROP2(R2_NOTXORPEN); // �ٻ�һ�ο��Խ��ղŵ����������Բ����ػ������������Ļ������"��"

			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd); // ����һ�ε������������㻭�ĵ㣩

			dc.MoveTo(m_PointBegin);
			dc.LineTo(point); // ��һ���µ�

			m_PointEnd = point; // ��һ�ο��԰���һ�εĸǵ�

			break;

		case DrawType::Rectangle:
		{
			dc.SetROP2(R2_NOTXORPEN); // ѡ����ʵ���ɫ������ɫ��ʵ���ɫ��
			dc.SelectStockObject(5); // ͸����ˢ�����⸲��

			CRect rectP1(m_PointBegin, m_PointEnd); // ������յ�
			dc.Rectangle(rectP1);

			CRect rectP2(m_PointBegin, point); // ������յ�
			dc.Rectangle(rectP2);

			m_PointEnd = point;
			break;
		}

		case DrawType::Circle:
		{
			dc.SetROP2(R2_NOTXORPEN); // ѡ����ʵ���ɫ������ɫ��ʵ���ɫ��
			dc.SelectStockObject(5); // ͸����ˢ�����⸲��

			CRect rectP1(m_PointBegin, m_PointEnd); // ������յ�
			ConvertToSquare(rectP1);
			dc.Ellipse(rectP1);

			CRect rectP2(m_PointBegin, point); // ������յ�
			ConvertToSquare(rectP2);
			dc.Ellipse(rectP2);

			m_PointEnd = point;
			break;
		}
		case DrawType::Ellipse:
		{
			dc.SetROP2(R2_NOTXORPEN); // ѡ����ʵ���ɫ������ɫ��ʵ���ɫ��
			dc.SelectStockObject(5); // ͸����ˢ�����⸲��

			CRect rectP1(m_PointBegin, m_PointEnd); // ������յ�
			dc.Ellipse(rectP1);

			CRect rectP2(m_PointBegin, point); // ������յ�
			dc.Ellipse(rectP2);

			m_PointEnd = point;
			break;
		}

		case DrawType::Text:
		{
			// ÿ��һ����λ�ã����µĿ�����ɵĿ�
			if (m_Edit != nullptr) {
				delete m_Edit;
			}

			// ��ʾ�ı���
			CEdit* myEdit = new CEdit(); // ��new��סʱ��һֱ����
			myEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(m_PointBegin, point), this, m_TextId);

			m_Edit = myEdit;

			myEdit->ShowWindow(SW_SHOW);

			// �����ı���Ϻ�ɾ������������

			break;
		}

		case DrawType::Pencil:
		{
			m_PointBegin = m_PointEnd;
			m_PointEnd = point;
			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd);
			break;
		}

		case DrawType::Eraser:
		{
			COLORREF pColor = dc.GetBkColor();
			CPen newPen(PS_SOLID, m_PenSize, pColor);
			dc.SelectObject(&newPen);

			m_PointBegin = m_PointEnd;
			m_PointEnd = point;
			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd);
			break;
		}


		default:
			
			break;
		}

		dc.SelectObject(oldPen); // �˳�switch��ѱ��û�������������ȷ���ͷ�
	}


	CView::OnMouseMove(nFlags, point);
}

void PaintView::ConvertToSquare(CRect& rect)
{
	int width = rect.Width();
	int height = rect.Height();

	int diff = abs(width - height);

	if (width > height)
	{
		int newY = rect.top - diff / 2;
		rect.SetRect(rect.left, newY, rect.right, newY + width);
	}
	else if (height > width)
	{
		int newX = rect.left - diff / 2;
		rect.SetRect(newX, rect.top, newX + height, rect.bottom);
	}
}


void PaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	// �ٻ�һ�飬ʹ�ཻ�ĵ㲻��հ�
	// 
	// ����
	CClientDC dc(this);
	CPen newPen, * oldPen;
	newPen.CreatePen(PS_SOLID, m_PenSize, m_PenColor); // ����һ֧��
	oldPen = dc.SelectObject(&newPen); // ѡ����֧��

	// �߶�
	switch (m_DrawType) {
		case DrawType::LineSegment:
			dc.MoveTo(m_PointBegin);
			dc.LineTo(point); 
		break;

		// ʹ�ཻ�����հף����߶�ͬ��
		case DrawType::Rectangle:
		{
			dc.SelectStockObject(PS_NULL); // ͸����ˢ������м�Ĳ��֣������⸲��
			CRect rectP2(m_PointBegin, point); // ������յ�
			dc.Rectangle(rectP2);

			break;
		}
		case DrawType::Ellipse:
		{
			dc.SelectStockObject(PS_NULL); // ͸����ˢ������м�Ĳ��֣������⸲��
			CRect rectP2(m_PointBegin, point); // ������յ�
			dc.Ellipse(rectP2);

			break;
		}
		case DrawType::Circle:
		{
			dc.SelectStockObject(PS_NULL); // ͸����ˢ������м�Ĳ��֣������⸲��
			CRect rectP2(m_PointBegin, point); // ������յ�
			ConvertToSquare(rectP2);
			dc.Ellipse(rectP2);
			break;
		}
		case DrawType::Text:
		{
			// ʵ�ֶ�̬�϶�


			// ��ʾ�ı���
			CEdit* myEdit = new CEdit(); // ��new��סʱ��һֱ����
			myEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(m_PointBegin, point), this, m_TextId);
			myEdit->ShowWindow(SW_SHOW);

			// ÿ��һ����λ�ã����µĿ�����ɵĿ�
			if (nullptr != m_Edit) {
				delete m_Edit;
			}
			
			// new�Ķ�������ͷŵ��������ڴ�й©
			// �ǿ�ָ���ʱ��ֱ�Ӵ��������
			m_Edit = myEdit;

			myEdit->ShowWindow(SW_SHOW);

			break;
		}

		 default:

			break;
	}

	dc.SelectObject(oldPen);
	CView::OnLButtonUp(nFlags, point);
}




void PaintView::OnDrawLineSegment()
{
	m_DrawType = DrawType::LineSegment;

	// TODO: �ڴ���������������
}


void PaintView::OnDrawRectangle()
{
	m_DrawType = DrawType::Rectangle;

	// TODO: �ڴ���������������
}


void PaintView::OnDrawCircle()
{
	m_DrawType = DrawType::Circle;

	// TODO: �ڴ���������������
}


void PaintView::OnDrawEllipse()
{
	m_DrawType = DrawType::Ellipse;

	// TODO: �ڴ���������������
}


void PaintView::OnSetColor()
{
	// TODO: �ڴ���������������
	// ѡ��ɫ�ĶԻ���
	CColorDialog dlg;
	// ��ʾ�Ի���
	if (IDOK == dlg.DoModal()) {
		m_PenColor = dlg.GetColor(); // ȷ����ɫ���浽m_PenColor
		// ������������Ӳ������
	}
}


void PaintView::OnSetPenSize()
{
	// TODO: �ڴ���������������
	CSetPenSizeDialog dlg;
	//MessageBox((LPCTSTR)this->m_PenSize, _T("������"));
	dlg.m_PenSize = this->m_PenSize;
	if (IDOK == dlg.DoModal()) {
		this->m_PenSize = dlg.m_PenSize;
	}
}


void PaintView::OnText()
{
	m_DrawType = DrawType::Text; // ǿö�����ͣ�������������

	// TODO: �ڴ���������������
}


BOOL PaintView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (m_DrawType == DrawType::Text)
		{
			CString pStr;
			m_Edit->GetWindowTextW(pStr);
			delete m_Edit;
			// �ͷ�֮��Ҫ��ֵ
			m_Edit = nullptr;

			CClientDC dc(this);
			// ����ĵط�
			dc.TextOutW(m_TextPos.x, m_TextPos.y, pStr);

			return TRUE;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}


void PaintView::OnSetPencil()
{
	// TODO: �ڴ���������������
	m_DrawType = DrawType::Pencil;
}


void PaintView::OnSetEraser()
{
	// TODO: �ڴ���������������
	m_DrawType = DrawType::Eraser;
}


void PaintView::OnFileSave() // ����
{
	// TODO: �ڴ���������������
	CClientDC dc(this);
	CRect rect;
	BOOL showMsgTag;   //�Ƿ�Ҫ������ͼ�񱣴�ɹ��Ի���" 
	BOOL saveTag;
	CString saveFilePath;
	GetClientRect(&rect);                  //��ȡ������С
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	//��������λͼ
	HDC hdc = CreateCompatibleDC(dc);      //��������DC���Ա㽫ͼ�񱣴�Ϊ��ͬ�ĸ�ʽ

	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);
	//��λͼѡ��DC�������淵��ֵ 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	//����ĻDC��ͼ���Ƶ��ڴ�DC��
	CImage image;
	image.Attach(hbitmap);                //��λͼת��Ϊһ��ͼ��
	saveTag = FALSE;
	if (!saveTag)                      //���ͼ���ǵ�һ�α�д��,��򿪶Ի���
	{
		showMsgTag = TRUE;
		CString  strFilter = _T("λͼ�ļ�|*.bmp|JPEG ͼ���ļ�|*.jpg|GIFͼ���ļ� | *.gif | PNGͼ���ļ� | *.png |������ʽ| *.* |");
		CFileDialog dlg(FALSE, _T("bmp"), _T("iPaint1.bmp"), NULL, strFilter);
		if (dlg.DoModal() != IDOK)
			return;
		CString strFileName;          //����û�û��ָ���ļ���չ������Ϊ�����һ��
		CString strExtension;
		strFileName = dlg.m_ofn.lpstrFile;
		if (dlg.m_ofn.nFileExtension = 0)               //��չ����ĿΪ0
		{
			switch (dlg.m_ofn.nFilterIndex)
			{
			case 1:
				strExtension = "bmp"; break;
			case 2:
				strExtension = "jpg"; break;
			case 3:
				strExtension = "gif"; break;
			case 4:
				strExtension = "png"; break;
			default:
				break;
			}
			strFileName = strFileName + "." + strExtension;
		}
		saveFilePath = strFileName;     //saveFilePathΪ�����е�ȫ�ֱ���,����ΪCString
	}
	else
	{
		showMsgTag = FALSE;

	}
	//AfxMessageBox(saveFilePath);               //��ʾͼ�񱣴��ȫ·��(�����ļ���)
	HRESULT hResult = image.Save(saveFilePath);     //����ͼ��
	if (FAILED(hResult))
	{
		MessageBox(_T("����ͼ���ļ�ʧ�ܣ�"));
	}
	else
	{
		if (showMsgTag)
			MessageBox(_T("�ļ�����ɹ���"));
	}
	image.Detach();
	SelectObject(hdc, hOldMap);
}


void PaintView::OnPenSize1()
{
	// TODO: �ڴ���������������
	this->m_PenSize = 1;
}


void PaintView::OnPenSize3()
{
	// TODO: �ڴ���������������
	this->m_PenSize = 3;
}


void PaintView::OnPenSize5()
{
	// TODO: �ڴ���������������
	this->m_PenSize = 5;
}


void PaintView::OnPenSize8()
{
	// TODO: �ڴ���������������
	this->m_PenSize = 8;
}
