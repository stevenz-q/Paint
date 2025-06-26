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
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// PaintView
IMPLEMENT_DYNCREATE(PaintView, CView)
// PaintView 消息映射
BEGIN_MESSAGE_MAP(PaintView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_SAVE, &PaintView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &PaintView::OnFileSave)
	ON_COMMAND(ID_FILE_PAGE_SETUP, &PaintView::ONLoadFile)
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
	ON_COMMAND(ID_SET_CLEAR, &PaintView::OnSetClear)
	ON_COMMAND(IDM_TEXT, &PaintView::OnText)
	ON_COMMAND(IDM_SET_ERASER, &PaintView::OnSetEraser)
	ON_COMMAND(ID_32785, &PaintView::OnPenSize1)
	ON_COMMAND(ID_32786, &PaintView::OnPenSize3)
	ON_COMMAND(ID_32787, &PaintView::OnPenSize5)
	ON_COMMAND(ID_32788, &PaintView::OnPenSize8)
	ON_COMMAND(ID_ISFilled, &PaintView::OnSetIsfilled)
	ON_UPDATE_COMMAND_UI(ID_ISFilled, &PaintView::OnUpdateIsfilled)
END_MESSAGE_MAP()

// PaintView 构造
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
	m_Edit = nullptr; // 初始化编辑框为空指针
	m_IsFilled = 0; // 是否填充，默认不填充
}

// PaintView 析构
PaintView::~PaintView()
{
	if (m_Edit != nullptr) {
		delete m_Edit;
		m_Edit = nullptr;
	}
}

// PaintView 预创建窗口
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

// PaintView 打印开始和结束
void PaintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

// PaintView 打印结束
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
		CBrush brush;
		if (m_IsFilled) {
			brush.CreateSolidBrush(m_BrushColor); // 创建填充画刷
			dc.SelectObject(&brush); // 选择画刷
		}
		else {
			dc.SelectStockObject(NULL_BRUSH); // 使用透明画刷
		}
		switch (m_DrawType) {
		case DrawType::LineSegment:// 绘制线段

			dc.SetROP2(R2_NOTXORPEN); // 再画一次可以将刚才的消掉，可以不用重绘就能随意在屏幕画了再"擦"

			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd); // 把上一次的线清理掉（起点画的点）

			dc.MoveTo(m_PointBegin);
			dc.LineTo(point); // 画一个新的

			m_PointEnd = point; // 下一次可以把这一次的盖掉

			break;

		case DrawType::Rectangle: // 绘制矩形
		{
			dc.SetROP2(R2_NOTXORPEN); // 选择合适的颜色（背景色或笔的颜色）
			//dc.SelectStockObject(NULL_BRUSH); // 使用透明画刷，避免填充颜色


			CRect rectP1(m_PointBegin, m_PointEnd); // 起点与终点
			dc.Rectangle(rectP1);
			CRect rectP2(m_PointBegin, point); // 起点与终点
			dc.Rectangle(rectP2);

			m_PointEnd = point;
			break;
		}

		case DrawType::Circle: // 绘制圆形
		{
			dc.SetROP2(R2_NOTXORPEN); // 选择合适的颜色（背景色或笔的颜色）
			//dc.SelectStockObject(5); // 透明画刷，避免覆盖

			CRect rectP1(m_PointBegin, m_PointEnd); // 起点与终点
			ConvertToSquare(rectP1);
			dc.Ellipse(rectP1);

			CRect rectP2(m_PointBegin, point); // 起点与终点
			ConvertToSquare(rectP2);
			dc.Ellipse(rectP2);

			m_PointEnd = point;
			break;
		}
		case DrawType::Ellipse:// 绘制椭圆
		{
			dc.SetROP2(R2_NOTXORPEN); // 选择合适的颜色（背景色或笔的颜色）
			//dc.SelectStockObject(5); // 透明画刷，避免覆盖

			CRect rectP1(m_PointBegin, m_PointEnd); // 起点与终点
			dc.Ellipse(rectP1);

			CRect rectP2(m_PointBegin, point); // 起点与终点
			dc.Ellipse(rectP2);

			m_PointEnd = point;
			break;
		}

		case DrawType::Text: // 文本输入
		{
			// 如果没有输入框，就创建一个
			if (m_Edit == nullptr) {
				m_Edit = new CEdit(); // 用new按住时能一直存在
				m_Edit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(m_PointBegin, point), this, m_TextId);
				m_Edit->ShowWindow(SW_SHOW);
			}

			// 更新文本框位置
			m_Edit->MoveWindow(CRect(m_PointBegin, point)); // 更新位置

			m_TextPos = point; // 记录文本位置

			break;
		}
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

		case DrawType::Pencil: // 铅笔
		{
			m_PointBegin = m_PointEnd;
			m_PointEnd = point;
			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd);
			break;
		}

		case DrawType::Eraser: // 擦除
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

		dc.SelectObject(oldPen); // 选择换哪支笔
	}


	CView::OnMouseMove(nFlags, point);
}

// 将矩形转换为正方形
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

// 鼠标左键抬起时，结束绘制
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

// 绘制线段
void PaintView::OnDrawLineSegment()
{
	m_DrawType = DrawType::LineSegment;

	// TODO: 在此添加命令处理程序代码
}

// 绘制矩形
void PaintView::OnDrawRectangle()
{
	m_DrawType = DrawType::Rectangle;

	// TODO: 在此添加命令处理程序代码
}

// 绘制圆形
void PaintView::OnDrawCircle()
{
	m_DrawType = DrawType::Circle;

	// TODO: 在此添加命令处理程序代码
}

// 绘制椭圆
void PaintView::OnDrawEllipse()
{
	m_DrawType = DrawType::Ellipse;

	// TODO: 在此添加命令处理程序代码
}

// 设置颜色对话框
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

// 设置画笔大小对话框
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

// 预处理消息
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

// 设置画笔为铅笔
void PaintView::OnSetPencil()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Pencil;
}

// 设置橡皮擦
void PaintView::OnSetEraser()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Eraser;
}

// 打开文件:加载图片
void PaintView::ONLoadFile()
{
	CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpeg;*.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*|"), this);
	fileDlg.DoModal();
	CString StrFilePath = fileDlg.GetPathName();
	if (StrFilePath == _T(""))
	{
		return;
	}
	CDC* pdc = GetDC();
	CRect rect;
	GetClientRect(&rect);
	CImage image;
	image.Load(StrFilePath);
	image.BitBlt(*pdc, 40, 40, rect.Width(), rect.Height(), 0, 0);
	image.Destroy();    //放置内存泄漏。
}

// 保存文件:保存图片
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
	HRESULT hResult = image.Save(saveFilePath);  //保存图像
	if (FAILED(hResult))
	{
		MessageBox(_T("保存文件失败！"));
	}
	else
	{
		if (showMsgTag)
			MessageBox(_T("文件保存成功！"));
	}
	image.Detach();
	SelectObject(hdc, hOldMap);
}

// 设置画笔大小为1
void PaintView::OnPenSize1()
{
	// TODO: 在此添加命令处理程序代码
	this->m_PenSize = 1;
}

// 设置画笔大小为3
void PaintView::OnPenSize3()
{
	// TODO: 在此添加命令处理程序代码
	this->m_PenSize = 3;
}

// 设置画笔大小为5
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

// 清空
void PaintView::OnSetClear()
{
		//将绘图缓冲区清空为白色背景
		CClientDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		dc.FillSolidRect(rect, RGB(255, 255, 255));
	// TODO: 在此添加控件通知处理程序代码
}

void PaintView::OnSetIsfilled()
{
	m_IsFilled = !m_IsFilled; // 切换填充状态
}

void PaintView::OnUpdateIsfilled(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	// 更新填充状态
	if (m_IsFilled) {
	pCmdUI->SetCheck(1); // 选中
	}
	else {
	pCmdUI->SetCheck(0); // 不选中
	}
}
