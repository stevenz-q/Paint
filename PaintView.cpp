// PaintView 类的实现

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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

// PaintView 构造/析构

PaintView::PaintView() noexcept
{
	// TODO: 在此处添加构造代码
	m_PenSize = 3;
	m_PenColor = RGB(0, 0, 0);
	m_BrushColor = RGB(0, 0, 0);
	m_PointBegin = CPoint(0, 0);
	m_PointEnd = CPoint(0, 0);
	m_DrawType = DrawType::Pencil; // 默认画笔
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// PaintView 绘图

void PaintView::OnDraw(CDC* /*pDC*/)
{
	PaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// PaintView 打印

BOOL PaintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void PaintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void PaintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// PaintView 诊断

#ifdef _DEBUG
void PaintView::AssertValid() const
{
	CView::AssertValid();
}

void PaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

PaintDoc* PaintView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(PaintDoc)));
	return (PaintDoc*)m_pDocument;
}
#endif //_DEBUG


// PaintView 消息处理程序


void PaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_PointBegin = m_PointEnd = point; // 初始化begin
	m_TextPos = point;

	// 在起点画一笔（画一个点）
	CClientDC dc(this);

	CView::OnLButtonDown(nFlags, point);
}


void PaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 移动的时候左键按住
	if (nFlags & MK_LBUTTON) {
		CClientDC dc(this);
		CPen newPen, * oldPen;
		newPen.CreatePen(PS_SOLID, m_PenSize, m_PenColor); // 创建一支笔
		oldPen = dc.SelectObject(&newPen); // 选择换哪支笔

		switch (m_DrawType) {
		case DrawType::LineSegment:

			dc.SetROP2(R2_NOTXORPEN); // 再画一次可以将刚才的消掉，可以不用重绘就能随意在屏幕画了再"擦"

			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd); // 把上一次的线清理掉（起点画的点）

			dc.MoveTo(m_PointBegin);
			dc.LineTo(point); // 画一个新的

			m_PointEnd = point; // 下一次可以把这一次的盖掉

			break;

		case DrawType::Rectangle:
		{
			dc.SetROP2(R2_NOTXORPEN); // 选择合适的颜色（背景色或笔的颜色）
			dc.SelectStockObject(5); // 透明画刷，避免覆盖

			CRect rectP1(m_PointBegin, m_PointEnd); // 起点与终点
			dc.Rectangle(rectP1);

			CRect rectP2(m_PointBegin, point); // 起点与终点
			dc.Rectangle(rectP2);

			m_PointEnd = point;
			break;
		}

		case DrawType::Circle:
		{
			dc.SetROP2(R2_NOTXORPEN); // 选择合适的颜色（背景色或笔的颜色）
			dc.SelectStockObject(5); // 透明画刷，避免覆盖

			CRect rectP1(m_PointBegin, m_PointEnd); // 起点与终点
			ConvertToSquare(rectP1);
			dc.Ellipse(rectP1);

			CRect rectP2(m_PointBegin, point); // 起点与终点
			ConvertToSquare(rectP2);
			dc.Ellipse(rectP2);

			m_PointEnd = point;
			break;
		}
		case DrawType::Ellipse:
		{
			dc.SetROP2(R2_NOTXORPEN); // 选择合适的颜色（背景色或笔的颜色）
			dc.SelectStockObject(5); // 透明画刷，避免覆盖

			CRect rectP1(m_PointBegin, m_PointEnd); // 起点与终点
			dc.Ellipse(rectP1);

			CRect rectP2(m_PointBegin, point); // 起点与终点
			dc.Ellipse(rectP2);

			m_PointEnd = point;
			break;
		}

		case DrawType::Text:
		{
			// 每到一个新位置，用新的框替代旧的框
			if (m_Edit != nullptr) {
				delete m_Edit;
			}

			// 显示文本框
			CEdit* myEdit = new CEdit(); // 用new按住时能一直存在
			myEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(m_PointBegin, point), this, m_TextId);

			m_Edit = myEdit;

			myEdit->ShowWindow(SW_SHOW);

			// 输入文本完毕后删掉框，留下字体

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

		dc.SelectObject(oldPen); // 退出switch后把笔拿回来，否则不能正确地释放
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 再画一遍，使相交的点不会空白
	// 
	// 矩形
	CClientDC dc(this);
	CPen newPen, * oldPen;
	newPen.CreatePen(PS_SOLID, m_PenSize, m_PenColor); // 创建一支笔
	oldPen = dc.SelectObject(&newPen); // 选择换哪支笔

	// 线段
	switch (m_DrawType) {
		case DrawType::LineSegment:
			dc.MoveTo(m_PointBegin);
			dc.LineTo(point); 
		break;

		// 使相交处不空白，与线段同理
		case DrawType::Rectangle:
		{
			dc.SelectStockObject(PS_NULL); // 透明画刷（填充中间的部分），避免覆盖
			CRect rectP2(m_PointBegin, point); // 起点与终点
			dc.Rectangle(rectP2);

			break;
		}
		case DrawType::Ellipse:
		{
			dc.SelectStockObject(PS_NULL); // 透明画刷（填充中间的部分），避免覆盖
			CRect rectP2(m_PointBegin, point); // 起点与终点
			dc.Ellipse(rectP2);

			break;
		}
		case DrawType::Circle:
		{
			dc.SelectStockObject(PS_NULL); // 透明画刷（填充中间的部分），避免覆盖
			CRect rectP2(m_PointBegin, point); // 起点与终点
			ConvertToSquare(rectP2);
			dc.Ellipse(rectP2);
			break;
		}
		case DrawType::Text:
		{
			// 实现动态拖动


			// 显示文本框
			CEdit* myEdit = new CEdit(); // 用new按住时能一直存在
			myEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(m_PointBegin, point), this, m_TextId);
			myEdit->ShowWindow(SW_SHOW);

			// 每到一个新位置，用新的框替代旧的框
			if (nullptr != m_Edit) {
				delete m_Edit;
			}
			
			// new的对象必须释放掉，否则内存泄漏
			// 是空指针的时候，直接存进来就行
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

	// TODO: 在此添加命令处理程序代码
}


void PaintView::OnDrawRectangle()
{
	m_DrawType = DrawType::Rectangle;

	// TODO: 在此添加命令处理程序代码
}


void PaintView::OnDrawCircle()
{
	m_DrawType = DrawType::Circle;

	// TODO: 在此添加命令处理程序代码
}


void PaintView::OnDrawEllipse()
{
	m_DrawType = DrawType::Ellipse;

	// TODO: 在此添加命令处理程序代码
}


void PaintView::OnSetColor()
{
	// TODO: 在此添加命令处理程序代码
	// 选颜色的对话框
	CColorDialog dlg;
	// 显示对话框
	if (IDOK == dlg.DoModal()) {
		m_PenColor = dlg.GetColor(); // 确认颜色，存到m_PenColor
		// 可以在这里面加测试语句
	}
}


void PaintView::OnSetPenSize()
{
	// TODO: 在此添加命令处理程序代码
	CSetPenSizeDialog dlg;
	//MessageBox((LPCTSTR)this->m_PenSize, _T("哈哈！"));
	dlg.m_PenSize = this->m_PenSize;
	if (IDOK == dlg.DoModal()) {
		this->m_PenSize = dlg.m_PenSize;
	}
}


void PaintView::OnText()
{
	m_DrawType = DrawType::Text; // 强枚举类型，得用类名调用

	// TODO: 在此添加命令处理程序代码
}


BOOL PaintView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (m_DrawType == DrawType::Text)
		{
			CString pStr;
			m_Edit->GetWindowTextW(pStr);
			delete m_Edit;
			// 释放之后要赋值
			m_Edit = nullptr;

			CClientDC dc(this);
			// 输出的地方
			dc.TextOutW(m_TextPos.x, m_TextPos.y, pStr);

			return TRUE;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}


void PaintView::OnSetPencil()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Pencil;
}


void PaintView::OnSetEraser()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Eraser;
}


void PaintView::OnFileSave() // 保存
{
	// TODO: 在此添加命令处理程序代码
	CClientDC dc(this);
	CRect rect;
	BOOL showMsgTag;   //是否要弹出”图像保存成功对话框" 
	BOOL saveTag;
	CString saveFilePath;
	GetClientRect(&rect);                  //获取画布大小
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	//创建兼容位图
	HDC hdc = CreateCompatibleDC(dc);      //创建兼容DC，以便将图像保存为不同的格式

	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);
	//将位图选入DC，并保存返回值 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	//将屏幕DC的图像复制到内存DC中
	CImage image;
	image.Attach(hbitmap);                //将位图转化为一般图像
	saveTag = FALSE;
	if (!saveTag)                      //如果图像是第一次被写入,则打开对话框
	{
		showMsgTag = TRUE;
		CString  strFilter = _T("位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF图像文件 | *.gif | PNG图像文件 | *.png |其他格式| *.* |");
		CFileDialog dlg(FALSE, _T("bmp"), _T("iPaint1.bmp"), NULL, strFilter);
		if (dlg.DoModal() != IDOK)
			return;
		CString strFileName;          //如果用户没有指定文件扩展名，则为其添加一个
		CString strExtension;
		strFileName = dlg.m_ofn.lpstrFile;
		if (dlg.m_ofn.nFileExtension = 0)               //扩展名项目为0
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
		saveFilePath = strFileName;     //saveFilePath为视类中的全局变量,类型为CString
	}
	else
	{
		showMsgTag = FALSE;

	}
	//AfxMessageBox(saveFilePath);               //显示图像保存的全路径(包含文件名)
	HRESULT hResult = image.Save(saveFilePath);     //保存图像
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}
	else
	{
		if (showMsgTag)
			MessageBox(_T("文件保存成功！"));
	}
	image.Detach();
	SelectObject(hdc, hOldMap);
}


void PaintView::OnPenSize1()
{
	// TODO: 在此添加命令处理程序代码
	this->m_PenSize = 1;
}


void PaintView::OnPenSize3()
{
	// TODO: 在此添加命令处理程序代码
	this->m_PenSize = 3;
}


void PaintView::OnPenSize5()
{
	// TODO: 在此添加命令处理程序代码
	this->m_PenSize = 5;
}


void PaintView::OnPenSize8()
{
	// TODO: 在此添加命令处理程序代码
	this->m_PenSize = 8;
}
