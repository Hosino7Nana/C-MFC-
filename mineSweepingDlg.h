
// mineSweepingDlg.h: 头文件
//

#pragma once


// CmineSweepingDlg 对话框
class CmineSweepingDlg : public CDialogEx
{
// 构造
public:
	CmineSweepingDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINESWEEPING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	INT m_startX;
	INT m_startY; //记录雷区左上角的坐标
	INT m_dis;  // 雷区每一方块的边长
	INT m_colCount, m_rowCount;  // 雷区的行列数
	CRect m_timeRect, m_mineCntRect;  // 计时框、剩余雷数框
	CBitmap m_showBitmap[4];  
	// 0为计时器的图片、1为剩余雷数的图片、2为地雷的图片、3为旗帜的图片 4张图
	INT m_second, m_secondStep;  // 经过秒数，达到一秒走的步数
	INT m_mineCount, m_leftCount;  // 总雷数,剩余雷数
	INT m_isMine[100][100];  
	// 记录当前方格是否是雷，1为雷，0不为雷，-1不可为雷
	INT m_aroundMineCnt[100][100];  // 记录周围八个方格的雷数，0-8
	INT m_aroundFlgCnt[100][100];// 记录某一格周围旗子的数量 0-8
	INT m_blockState[100][100];  
	// 记录方格的状态，0为可点击，1为已点开，2为标记
	CBitmap m_mineCntBitmap[9];  //  0-8个地雷的图片 9张图 
	BOOL m_firstClick;  // 标记是否为第一次点击
	BOOL m_isOver;  // 标记游戏是否已结束
	CRect m_faceRect;  // 显示笑脸的位置
	CBitmap m_faceBitmap[5];  
	// 0为笑脸弹起，1为笑脸按下，2为惊慌，3为胜利，4为失败  5张图
	INT m_faceState;  // 记录笑脸的状态
	INT m_openedCount;  // 记录点开方格的数量

	VOID zeroHit(INT col, INT row);  // 点击周围雷数为0的方格
	VOID gameRestart();  // 重新开始游戏，用于一些数值的恢复
	VOID gameStart(INT col, INT row);  // 游戏开始，生成雷区
	VOID gameFail();  // 点击的方格是雷，游戏失败
	VOID gameWin();  // 点开所有不是雷的方格，游戏胜利
	BOOL isGameWin();  // 判断游戏是否胜利
	BOOL isVaildClick(INT col, INT row);  // 判断点击的坐标是否为合法点击
	INT getColCount(); //获取行数
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEasy(); //简单
	afx_msg void OnMedium(); //中等
	afx_msg void OnHard(); //困难
	afx_msg void OnCustom(); //自定义
	afx_msg void OnStart(); //开局
	afx_msg void OnAbout();

	static CmineSweepingDlg* s_pmDlg;

	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	
	
	afx_msg void OnHelp();
};
