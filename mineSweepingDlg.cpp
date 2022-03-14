
// mineSweepingDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "mineSweeping.h"
#include "mineSweepingDlg.h"
#include "afxdialogex.h"
#include "m_CustomDlg.h"
#include "CHelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CmineSweepingDlg* CmineSweepingDlg::s_pmDlg = NULL;  //初始化静态指针

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmineSweepingDlg 对话框



CmineSweepingDlg::CmineSweepingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINESWEEPING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CmineSweepingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmineSweepingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_COMMAND(ID_32772, &CmineSweepingDlg::OnEasy)
	ON_COMMAND(ID_32773, &CmineSweepingDlg::OnMedium)
	ON_COMMAND(ID_32774, &CmineSweepingDlg::OnHard)
	ON_COMMAND(ID_32776, &CmineSweepingDlg::OnAbout)
	ON_COMMAND(ID_32777, &CmineSweepingDlg::OnStart)
	ON_COMMAND(ID_32779, &CmineSweepingDlg::OnCustom)
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_32781, &CmineSweepingDlg::OnHelp)
END_MESSAGE_MAP()


// CmineSweepingDlg 消息处理程序

BOOL CmineSweepingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->SetWindowPos(NULL, 0, 0, 1024, 650, SWP_NOZORDER);//窗口大小 1024*650
	this->CenterWindow();//居中显示窗口
	
	//加载计时器 计雷器 地雷 旗帜的图标
	m_showBitmap[0].LoadBitmapW(IDB_BITMAP6);  
	m_showBitmap[1].LoadBitmapW(IDB_BITMAP7);
	m_showBitmap[2].LoadBitmapW(IDB_BITMAP8);
	m_showBitmap[3].LoadBitmapW(IDB_BITMAP9);
	//加载笑脸的图片
	m_faceRect = CRect(480, 16, 480 + 48, 16 + 48);
	m_faceBitmap[0].LoadBitmapW(IDB_BITMAP1);
	m_faceBitmap[1].LoadBitmapW(IDB_BITMAP2);
	m_faceBitmap[2].LoadBitmapW(IDB_BITMAP3);
	m_faceBitmap[3].LoadBitmapW(IDB_BITMAP5);
	m_faceBitmap[4].LoadBitmapW(IDB_BITMAP4);
	
	//初始化各项数值
	m_startX = 130;  m_startY = 75;  m_dis = 25;//雷区左上角坐标和每个格子的边长
	m_colCount = 30;  m_rowCount = 16;  //默认30列 16行 根据难度改变
	m_timeRect = CRect(200, 500, 200+128, 500+48);
	m_mineCntRect = CRect(600, 500, 600+128, 500+48);
	m_faceState = 0;
	m_second = 0;  m_secondStep = 0;
	m_mineCount = 99;  m_leftCount = m_mineCount; //默认 雷数99 根据难度改变
	m_openedCount = 0;
	for (int i = 0; i < m_colCount; ++i)
	{
		for (int j = 0; j < m_rowCount; ++j)
		{
			m_isMine[i][j] = 0;
			m_aroundMineCnt[i][j] = 0;
			m_blockState[i][j] = 0;
			m_aroundFlgCnt[i][j] = 0;
		}
	}
	//加载表示周围地雷个数的图标
	m_mineCntBitmap[0].LoadBitmapW(IDB_BITMAP18);
	m_mineCntBitmap[1].LoadBitmapW(IDB_BITMAP10);
	m_mineCntBitmap[2].LoadBitmapW(IDB_BITMAP11);
	m_mineCntBitmap[3].LoadBitmapW(IDB_BITMAP12);
	m_mineCntBitmap[4].LoadBitmapW(IDB_BITMAP13);
	m_mineCntBitmap[5].LoadBitmapW(IDB_BITMAP14);
	m_mineCntBitmap[6].LoadBitmapW(IDB_BITMAP15);
	m_mineCntBitmap[7].LoadBitmapW(IDB_BITMAP16);
	m_mineCntBitmap[8].LoadBitmapW(IDB_BITMAP17);

	OnHard();//游戏开始默认Hard难度

	s_pmDlg = this;
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmineSweepingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmineSweepingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect clientRect;
		GetClientRect(&clientRect);  // 获取客户区矩形
		dc.FillSolidRect(&clientRect, RGB(180, 180, 180));  // 绘制灰色背景
		CBrush brush;
		brush.CreateSolidBrush(RGB(0, 247, 255));  //画方格的画刷
		CRect rect(m_startX, m_startY,
			m_startX + m_dis * m_colCount, m_startY + m_dis * m_rowCount);
		dc.FillRect(&rect, &brush);  // 绘制蓝色雷区
		CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));  // 黑色宽度为1的画笔
		CPen grayPen(PS_SOLID, 1, RGB(110, 110, 110));  // 灰色宽度为1的画笔
		CPen whitePen(PS_SOLID, 1, RGB(255, 255, 255));  // 白色宽度为1的画笔
		// 绘制雷区
		for (int i = 0; i < m_colCount; ++i) 
		{
			for (int j = 0; j < m_rowCount; ++j)
			{
				// 绘制黑线，分隔出各个方格
				dc.SelectObject(blackPen);
				dc.MoveTo(m_startX + i * m_dis, m_startY + j * m_dis);
				dc.LineTo(m_startX + i * m_dis + m_dis - 1, m_startY + j * m_dis);
				dc.LineTo(m_startX + i * m_dis + m_dis - 1, m_startY + j * m_dis + m_dis - 1);
				dc.LineTo(m_startX + i * m_dis, m_startY + j * m_dis + m_dis - 1);
				dc.LineTo(m_startX + i * m_dis, m_startY + j * m_dis);
				// 绘制灰线，增加立体效果
				dc.SelectObject(grayPen);
				dc.MoveTo(m_startX + i * m_dis + 2, m_startY + j * m_dis + 2);
				dc.LineTo(m_startX + i * m_dis + m_dis - 2, m_startY + j * m_dis + 2);
				dc.LineTo(m_startX + i * m_dis + m_dis - 2, m_startY + j * m_dis + m_dis - 2);
				dc.LineTo(m_startX + i * m_dis + 2, m_startY + j * m_dis + m_dis - 2);
				dc.LineTo(m_startX + i * m_dis + 2, m_startY + j * m_dis + 2);
			}
		}
		// 在雷区周围绘制白线，增加立体效果
		dc.SelectObject(whitePen);
		dc.MoveTo(m_startX - 2, m_startY - 2);
		dc.LineTo(m_startX + 2 + m_colCount * m_dis, m_startY - 2);
		dc.LineTo(m_startX + 2 + m_colCount * m_dis, m_startY + m_rowCount * m_dis + 2);
		dc.LineTo(m_startX - 2, m_startY + m_rowCount * m_dis + 2);
		dc.LineTo(m_startX - 2, m_startY - 2);
		// 绘制计时框、剩余雷数框
		CBrush brush2;
		brush2.CreateSolidBrush(RGB(255, 255, 255));  
		dc.FillRect(&m_timeRect, &brush2);
		dc.FillRect(&m_mineCntRect, &brush2);
		CDC srcDc;
		srcDc.CreateCompatibleDC(&dc);
		srcDc.SelectObject(m_showBitmap[0]);  // 选择加载好的图片
		// 绘制选择的图片
		dc.BitBlt(m_timeRect.left , m_timeRect.top, 48, 48, &srcDc, 0, 0, SRCAND);
		srcDc.SelectObject(m_showBitmap[1]);
		dc.BitBlt(m_mineCntRect.left , m_mineCntRect.top, 48, 48, &srcDc, 0, 0, SRCAND);
		// 绘制经过时间、剩余雷数
		CString str;
		str.Format(_T("%d"), m_second);
		CFont font;
		font.CreatePointFont(260, _T("宋体"));  // 创建文字字体
		CSize strSize;
		dc.SelectObject(font);
		strSize = dc.GetTextExtent(str);  // 获取文字的大小
		dc.SetBkColor(RGB(255, 255, 255));  // 数字背景
		dc.SetTextColor(RGB(255, 0, 0));  // 数字-白色
		dc.TextOutW(m_timeRect.left + (m_timeRect.Width() - strSize.cx) / 1.5,
			m_timeRect.top + (m_timeRect.Height() - strSize.cy) / 2, str);//居中
		str.Format(_T("%d"), m_leftCount);
		strSize = dc.GetTextExtent(str);
		dc.TextOutW(m_mineCntRect.left + (m_mineCntRect.Width() - strSize.cx) / 1.5,
			m_mineCntRect.top + (m_mineCntRect.Height() - strSize.cy) / 2, str);
		// 绘制笑脸
		srcDc.SelectObject(m_faceBitmap[m_faceState]);
		dc.BitBlt(m_faceRect.left, m_faceRect.top, 48, 48, &srcDc, 0, 0, SRCCOPY);
		// 绘制雷区方格点开的贴图
		for (int i = 0; i < m_colCount; ++i) 
		{
			for (int j = 0; j < m_rowCount; ++j) 
			{
				if (m_blockState[i][j] == 1) 
				{
					if (m_isMine[i][j] == 1) 
					{  // 绘制地雷
						srcDc.SelectObject(m_showBitmap[2]);
						dc.BitBlt(m_startX + i * m_dis + 3, m_startY + j * m_dis + 3,
							24, 24, &srcDc, 0, 0, SRCCOPY);
					}
					else 
					{  // 绘制数字
						srcDc.SelectObject(m_mineCntBitmap[m_aroundMineCnt[i][j]]);
						dc.BitBlt(m_startX + i * m_dis + 3, m_startY + j * m_dis + 3,
							24, 24, &srcDc, 0, 0, SRCCOPY);
					}
				}
				else if (m_blockState[i][j] == 2) 
				{  // 绘制旗帜
					srcDc.SelectObject(m_showBitmap[3]);
					dc.BitBlt(m_startX + i * m_dis + 3, m_startY + j * m_dis + 3,
						24, 24, &srcDc, 0, 0, SRCCOPY);
				}
			}
		}

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmineSweepingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID CmineSweepingDlg::zeroHit(INT col, INT row)
{//点击周围雷数为零的方格时，除了显示出当前方格之外，还需要显示出周围的所有方格
	m_blockState[col][row] = 1;  //将方格状态改为已点开
	++m_openedCount;  //已经点开的方格数加一
	CRect rect;  //当前方格矩形
	rect.left = m_startX + col * m_dis;
	rect.top = m_startY + row * m_dis;
	rect.right = rect.left + m_dis;
	rect.bottom = rect.top + m_dis;
	InvalidateRect(&rect);  //重绘当前方格，根据当前状态选择贴图

	if (isVaildClick(col - 1, row)) {  // 左
		if (m_blockState[col - 1][row] == 0) {
			if (m_aroundMineCnt[col - 1][row] == 0) {
				zeroHit(col - 1, row);  
				//若周围的方格的周围雷数也是0，则递归调用函数
			}
			else {
				m_blockState[col - 1][row] = 1;
				++m_openedCount;
				rect.left = m_startX + (col - 1) * m_dis;
				rect.top = m_startY + row * m_dis;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	}
	if (isVaildClick(col - 1, row-1)) 
	{  //左上
		if (m_blockState[col - 1][row-1] == 0) 
		{
			if (m_aroundMineCnt[col - 1][row - 1] == 0) 
			{
				zeroHit(col - 1, row-1);  //若周围的方格的周围雷数也是0，则递归调用函数
			}
			else 
			{
				m_blockState[col - 1][row - 1] = 1;
				++m_openedCount;
				rect.left = m_startX + (col - 1) * m_dis;
				rect.top = m_startY + (row - 1) * m_dis;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	} 
	if (isVaildClick(col - 1, row + 1)) 
	{  //左下
		if (m_blockState[col - 1][row + 1] == 0) 
		{
			if (m_aroundMineCnt[col - 1][row + 1] == 0) 
			{
				zeroHit(col - 1, row+1);  //若周围的方格的周围雷数也是0，则递归调用函数
			}
			else 
			{
				m_blockState[col - 1][row + 1] = 1;
				++m_openedCount;
				rect.left = m_startX + (col - 1) * m_dis;
				rect.top = m_startY + (row + 1) * m_dis;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	}
	if (isVaildClick(col , row - 1))
	{  //上
		if (m_blockState[col ][row - 1] == 0) 
		{
			if (m_aroundMineCnt[col][row - 1] == 0) {

				zeroHit(col , row - 1);  //若周围的方格的周围雷数也是0，则递归调用函数
			}
			else 
			{
				m_blockState[col ][row - 1] = 1;
				++m_openedCount;
				rect.left = m_startX + (col ) * m_dis;
				rect.top = m_startY + (row - 1) * m_dis;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	}
	if (isVaildClick(col, row + 1)) 
	{  //下
		if (m_blockState[col][row + 1] == 0) 
		{
			if (m_aroundMineCnt[col][row + 1] == 0) 
			{
				zeroHit(col, row + 1);  //若周围的方格的周围雷数也是0，则递归调用函数
			}
			else 
			{
				m_blockState[col][row + 1] = 1;
				++m_openedCount;
				rect.left = m_startX + (col)*m_dis;
				rect.top = m_startY + (row + 1) * m_dis;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	}
	if (isVaildClick(col + 1, row)) 
	{  // 右
		if (m_blockState[col + 1][row] == 0) 
		{
			if (m_aroundMineCnt[col + 1][row] == 0) 
			{
				zeroHit(col + 1, row);  //若周围的方格的周围雷数也是0，则递归调用函数
			}
			else 
			{
				m_blockState[col + 1][row] = 1;
				++m_openedCount;
				rect.left = m_startX + (col + 1) * m_dis;
				rect.top = m_startY + row * m_dis;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	}
	if (isVaildClick(col + 1, row + 1)) //右下
	{  
		if (m_blockState[col + 1][row + 1] == 0) 
		{
			if (m_aroundMineCnt[col + 1][row + 1] == 0)
			{
				zeroHit(col + 1, row + 1);  //若周围的方格的周围雷数也是0，则递归调用函数
			}
			else
			{
				m_blockState[col + 1][row + 1] = 1;
				++m_openedCount;
				rect.left = m_startX + (col + 1) * m_dis;
				rect.top = m_startY + (row + 1) * m_dis;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	}
	if (isVaildClick(col + 1, row - 1))//右上
	{  
		if (m_blockState[col + 1][row - 1] == 0)
		{
			if (m_aroundMineCnt[col + 1][row - 1] == 0) 
			{
				zeroHit(col + 1, row - 1);  //若周围的方格的周围雷数也是0，则递归调用函数
			}
			else
			{
				m_blockState[col + 1][row - 1] = 1;
				++m_openedCount;
				rect.left = m_startX + (col + 1) * m_dis;
				rect.top = m_startY + (row - 1) * m_dis;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	}
}

VOID CmineSweepingDlg::gameRestart()
{
	// 游戏重新开始，恢复一些数值，重绘客户区，恢复为初始界面
	m_second = 0;  m_secondStep = 0;
	m_leftCount = m_mineCount;  m_openedCount = 0;
	m_firstClick = TRUE;  m_isOver = FALSE;
	m_faceState = 0;
	for (int i = 0; i < m_colCount; ++i) {
		for (int j = 0; j < m_rowCount; ++j) {
			m_isMine[i][j] = 0;
			m_aroundMineCnt[i][j] = 0;
			m_blockState[i][j] = 0;
			m_aroundFlgCnt[i][j] = 0;
		}
	}
	CRect rectDlg;
	GetClientRect(rectDlg);
	m_startX = (rectDlg.Width() - (m_colCount * m_dis)) / 2;
	Invalidate();  // 重绘客户区
}

VOID CmineSweepingDlg::gameStart(INT col, INT row)
{
	// 开始格及周围8格没有地雷
	m_isMine[col][row] = -1;
	if (isVaildClick(col - 1, row))
	{  // 左  正左如果非法则左边一列都非法
		m_isMine[col - 1][row] = -1;
		if (isVaildClick(col - 1, row - 1))
		{// 左上
			m_isMine[col - 1][row - 1] = -1;
		}
		if (isVaildClick(col - 1, row + 1))
		{//左下
			m_isMine[col - 1][row + 1] = -1;
		}
		
	}
	if (isVaildClick(col, row - 1))
	{//上
		m_isMine[col][row - 1] = -1;
	}
	if (isVaildClick(col, row + 1))
	{//下
		m_isMine[col][row + 1] = -1;
	}
	if (isVaildClick(col + 1, row))
	{//右  正右边如果非法则右边一列都非法
		m_isMine[col + 1][row] = -1;
		if (isVaildClick(col + 1, row - 1))
		{//右上
			m_isMine[col + 1][row - 1] = -1;
		}
		if (isVaildClick(col + 1, row + 1))
		{//右下
			m_isMine[col + 1][row + 1] = -1;
		}
		
	}
	// 随机生成雷区
	int curCount = 0, mineCol, mineRow;
	srand(time(0));
	while (curCount < m_mineCount) 
	{
		mineCol = rand() % m_colCount;
		mineRow = rand() % m_rowCount;
		if (m_isMine[mineCol][mineRow] == 0) 
		{
			m_isMine[mineCol][mineRow] = 1;
			if (isVaildClick(mineCol - 1, mineRow))
			{//左
				++m_aroundMineCnt[mineCol - 1][mineRow];
				if (isVaildClick(mineCol - 1, mineRow - 1))
				{//左上
					++m_aroundMineCnt[mineCol - 1][mineRow - 1];
				}
				if (isVaildClick(mineCol - 1, mineRow + 1))
				{//左下
					++m_aroundMineCnt[mineCol - 1][mineRow + 1];
				}
			}
			if (isVaildClick(mineCol + 1, mineRow))
			{//右
				++m_aroundMineCnt[mineCol + 1][mineRow];
				if (isVaildClick(mineCol + 1, mineRow - 1))
				{//右上
					++m_aroundMineCnt[mineCol + 1][mineRow - 1];
				}
				if (isVaildClick(mineCol + 1, mineRow + 1))
				{//右下
					++m_aroundMineCnt[mineCol + 1][mineRow + 1];
				}

			}
			if (isVaildClick(mineCol, mineRow - 1))
			{//上
				++m_aroundMineCnt[mineCol][mineRow - 1];
			}
			if (isVaildClick(mineCol, mineRow + 1))
			{//下
				++m_aroundMineCnt[mineCol][mineRow + 1];
			}
			++curCount;
		}
	}
}

VOID CmineSweepingDlg::gameFail()
{
	// 游戏失败，显示所有雷，显示失败脸，游戏结束
	m_isOver = TRUE;
	m_faceState = 4;
	InvalidateRect(&m_faceRect);
	CRect rect;
	for (int i = 0; i < m_colCount; ++i) {
		for (int j = 0; j < m_rowCount; ++j) {
			if (m_isMine[i][j] == 1) {
				m_blockState[i][j] = 1;
				rect.left = m_startX + m_dis * i;
				rect.top = m_startY + m_dis * j;
				rect.right = rect.left + m_dis;
				rect.bottom = rect.top + m_dis;
				InvalidateRect(&rect);
			}
		}
	}
	MessageBox(TEXT("您输了！\n请降低难度试试"));
}

VOID CmineSweepingDlg::gameWin()
{
	// 游戏胜利，显示胜利笑脸，游戏结束
	m_isOver = TRUE;
	m_faceState = 3;
	InvalidateRect(&m_faceRect);
	MessageBox(TEXT("您赢了！"));
}

BOOL CmineSweepingDlg::isGameWin()
{
	// 将所有不是雷的方格点开后，判断为游戏胜利
	if (m_openedCount == (m_colCount * m_rowCount - m_mineCount)) {
		return TRUE;
	}
	return FALSE;
}

BOOL CmineSweepingDlg::isVaildClick(INT col, INT row)
{
	// 判断当前方格是否越界
	if (col >= 0 && col < m_colCount &&
		row >= 0 && row < m_rowCount) {
		return TRUE;
	}
	return FALSE;
}

INT CmineSweepingDlg::getColCount()
{
	return m_colCount;
}


void CmineSweepingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 按下鼠标左键
	// 点击笑脸区域，笑脸变为按下状态
	if (point.x >= m_faceRect.left && point.x <= m_faceRect.right &&
		point.y >= m_faceRect.top && point.y <= m_faceRect.bottom) {
		m_faceState = 1;
		InvalidateRect(&m_faceRect);
	}
	// 点击雷区，笑脸变为惊慌状态
	if (!m_isOver && point.x >= m_startX && point.x <= m_startX + m_dis * m_colCount &&
		point.y >= m_startY && point.y <= m_startY + m_dis * m_rowCount) {
		INT curCol = (point.x - m_startX) / m_dis;  // 计算当前坐标所属方格的列数
		INT curRow = (point.y - m_startY) / m_dis;  // 计算当前坐标所属方格的行数
		m_faceState = 2;
		InvalidateRect(&m_faceRect);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CmineSweepingDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 释放鼠标左键
	// 恢复笑脸为弹起状态
	if (!m_isOver) {
		m_faceState = 0;
		InvalidateRect(&m_faceRect);
	}
	// 鼠标点击笑脸区域，重新开始游戏
	if (point.x >= m_faceRect.left && point.x <= m_faceRect.right &&
		point.y >= m_faceRect.top && point.y <= m_faceRect.bottom) {
		gameRestart();
	}
	// 鼠标点击雷区位置
	if (!m_isOver && point.x >= m_startX && point.x <= m_startX + m_dis * m_colCount &&
		point.y >= m_startY && point.y <= m_startY + m_dis * m_rowCount) {
		INT curCol = (point.x - m_startX) / m_dis;
		INT curRow = (point.y - m_startY) / m_dis;
		if (m_blockState[curCol][curRow] == 0) {
			if (m_firstClick) {  // 第一次点击
				m_firstClick = FALSE;
				SetTimer(1, 50, NULL);  // 开始计时,50ms为一步
				gameStart(curCol, curRow);  // 生成雷区
				zeroHit(curCol, curRow);  // 点击当前方格，周围雷数一定为零
				if (isGameWin()) {  // 判断是否游戏胜利
					KillTimer(1);  // 停止计时
					gameWin();
				}
			}
			else {
				if (m_isMine[curCol][curRow] == 1) {  // 点击雷方格，游戏结束
					KillTimer(1);
					gameFail();
				}
				else {
					if (m_aroundMineCnt[curCol][curRow] == 0) {
						zeroHit(curCol, curRow);
					}
					else {
						m_blockState[curCol][curRow] = 1;
						CRect rect(m_startX + curCol * m_dis, m_startY + curRow * m_dis,
							m_startX + curCol * m_dis + m_dis, m_startY + curRow * m_dis + m_dis);
						InvalidateRect(&rect);
						++m_openedCount;
					}
					if (isGameWin()) {
						KillTimer(1);
						gameWin();
					}
				}
			}
		}
	}

	

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CmineSweepingDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 释放鼠标右键
	// 鼠标点击雷区位置
	if (!m_isOver && point.x >= m_startX && point.x <= m_startX + m_dis * m_colCount &&
		point.y >= m_startY && point.y <= m_startY + m_dis * m_rowCount) {
		INT curCol = (point.x - m_startX) / m_dis;
		INT curRow = (point.y - m_startY) / m_dis;
		CRect rect(m_startX + curCol * m_dis, m_startY + curRow * m_dis,
			m_startX + curCol * m_dis + m_dis, m_startY + curRow * m_dis + m_dis);
		if (m_blockState[curCol][curRow] == 0) 
		{// 标记为雷，显示旗帜，周围方格旗帜数量加1，剩余雷数减1
			m_blockState[curCol][curRow] = 2;
			--m_leftCount;
			InvalidateRect(&m_mineCntRect);
			InvalidateRect(&rect);
			//周围方格旗帜数量增加
			if (isVaildClick(curCol - 1, curRow))
			{//左
				++m_aroundFlgCnt[curCol - 1][curRow];
				if (isVaildClick(curCol - 1, curRow - 1))
				{//左上
					++m_aroundFlgCnt[curCol - 1][curRow - 1];
				}
				if (isVaildClick(curCol - 1, curRow + 1))
				{//左下
					++m_aroundFlgCnt[curCol - 1][curRow + 1];
				}
			}
			if (isVaildClick(curCol + 1, curRow))
			{//右
				++m_aroundFlgCnt[curCol + 1][curRow];
				if (isVaildClick(curCol + 1, curRow - 1))
				{//右上
					++m_aroundFlgCnt[curCol + 1][curRow - 1];
				}
				if (isVaildClick(curCol + 1, curRow + 1))
				{//右下
					++m_aroundFlgCnt[curCol + 1][curRow + 1];
				}

			}
			if (isVaildClick(curCol, curRow - 1))
			{//上
				++m_aroundFlgCnt[curCol][curRow - 1];
			}
			if (isVaildClick(curCol, curRow + 1))
			{//下
				++m_aroundFlgCnt[curCol][curRow + 1];
			}

		}
		else if (m_blockState[curCol][curRow] == 2) 
		{// 取消标记，旗帜消失，周围旗帜数量减1，剩余雷数加1
			m_blockState[curCol][curRow] = 0;
			++m_leftCount;
			InvalidateRect(&m_mineCntRect);
			InvalidateRect(&rect);
			//周围方格旗帜数量减少
			if (isVaildClick(curCol - 1, curRow))
			{//左
				--m_aroundFlgCnt[curCol - 1][curRow];
				if (isVaildClick(curCol - 1, curRow - 1))
				{//左上
					--m_aroundFlgCnt[curCol - 1][curRow - 1];
				}
				if (isVaildClick(curCol - 1, curRow + 1))
				{//左下
					--m_aroundFlgCnt[curCol - 1][curRow + 1];
				}
			}
			if (isVaildClick(curCol + 1, curRow))
			{//右
				--m_aroundFlgCnt[curCol + 1][curRow];
				if (isVaildClick(curCol + 1, curRow - 1))
				{//右上
					--m_aroundFlgCnt[curCol + 1][curRow - 1];
				}
				if (isVaildClick(curCol + 1, curRow + 1))
				{//右下
					--m_aroundFlgCnt[curCol + 1][curRow + 1];
				}

			}
			if (isVaildClick(curCol, curRow - 1))
			{//上
				--m_aroundFlgCnt[curCol][curRow - 1];
			}
			if (isVaildClick(curCol, curRow + 1))
			{//下
				--m_aroundFlgCnt[curCol][curRow + 1];
			}
		}
	}

	

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CmineSweepingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 计时，设定50ms为一步，每20步为1s
	++m_secondStep;
	if (m_secondStep >= 20) {
		m_secondStep = 0;
		++m_second;
		InvalidateRect(&m_timeRect);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CmineSweepingDlg::OnEasy()
{
	// TODO: 在此添加命令处理程序代码
	m_colCount = 9;  m_rowCount = 9;
	m_mineCount = 10;  m_leftCount = m_mineCount;
	
	
	gameRestart();
}


void CmineSweepingDlg::OnMedium()
{
	// TODO: 在此添加命令处理程序代码
	m_colCount = 16;  m_rowCount = 16;
	m_mineCount = 40;  m_leftCount = m_mineCount;
	
	
	gameRestart();
}



void CmineSweepingDlg::OnHard()
{
	// TODO: 在此添加命令处理程序代码
	m_colCount = 30;  m_rowCount = 16;
	m_mineCount = 99;  m_leftCount = m_mineCount;
	
	
	gameRestart();
}


void CmineSweepingDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg myAbout;
	myAbout.DoModal();
}


void CmineSweepingDlg::OnStart()
{
	// TODO: 在此添加命令处理程序代码
	gameRestart();
}



void CmineSweepingDlg::OnCustom()
{
	// TODO: 在此添加命令处理程序代码
	m_CustomDlg custom;
	custom.DoModal();
}


void CmineSweepingDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//按下中键
	// 点击雷区，笑脸变为惊慌状态
	if (!m_isOver && point.x >= m_startX && point.x <= m_startX + m_dis * m_colCount &&
		point.y >= m_startY && point.y <= m_startY + m_dis * m_rowCount) {
		INT curCol = (point.x - m_startX) / m_dis;  // 计算当前坐标所属方格的列数
		INT curRow = (point.y - m_startY) / m_dis;  // 计算当前坐标所属方格的行数
		m_faceState = 2;
		InvalidateRect(&m_faceRect);
	}

	CDialogEx::OnMButtonDown(nFlags, point);
}


void CmineSweepingDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//释放中键
	if (!m_isOver) {
		m_faceState = 0;
		InvalidateRect(&m_faceRect);
	}

	INT curCol = (point.x - m_startX) / m_dis;
	INT curRow = (point.y - m_startY) / m_dis;
	if (!m_isOver && point.x >= m_startX && point.x <= m_startX + m_dis * m_colCount &&
		point.y >= m_startY && point.y <= m_startY + m_dis * m_rowCount &&
		m_aroundFlgCnt[curCol][curRow] == m_aroundMineCnt[curCol][curRow] &&
		m_aroundFlgCnt[curCol][curRow] != 0)
	{//点击中键时如果周围雷数=旗子数(不为0)，则快速开启所有未标记的格子
		CPoint CsrPt;
		GetCursorPos(&CsrPt); //获取鼠标点击的位置
		SetCursorPos(CsrPt.x - m_dis, CsrPt.y);//鼠标移动到左边一格
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
		SetCursorPos(CsrPt.x - m_dis, CsrPt.y - m_dis);//鼠标移动到左上一格
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
		SetCursorPos(CsrPt.x - m_dis, CsrPt.y + m_dis);//鼠标移动到左下一格
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
		SetCursorPos(CsrPt.x, CsrPt.y + m_dis);//鼠标移动到下边一格
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
		SetCursorPos(CsrPt.x, CsrPt.y - m_dis);//鼠标移动到上边一格
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
		SetCursorPos(CsrPt.x + m_dis, CsrPt.y);//鼠标移动到右边一格
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
		SetCursorPos(CsrPt.x + m_dis, CsrPt.y - m_dis);//鼠标移动到右上一格
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
		SetCursorPos(CsrPt.x + m_dis, CsrPt.y + m_dis);//鼠标移动到右下一格
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
		SetCursorPos(CsrPt.x, CsrPt.y);//鼠标回到点击位置
	}

	CDialogEx::OnMButtonUp(nFlags, point);
}


void CmineSweepingDlg::OnHelp()
{
	// TODO: 在此添加命令处理程序代码
	//弹出帮助文档
	CHelpDlg help;
	help.DoModal();
}
