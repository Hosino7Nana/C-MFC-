// m_CustomDlg.cpp: 实现文件
//

#include "pch.h"
#include "mineSweeping.h"
#include "m_CustomDlg.h"
#include "afxdialogex.h"
#include "mineSweepingDlg.h"


// m_CustomDlg 对话框

IMPLEMENT_DYNAMIC(m_CustomDlg, CDialogEx)

m_CustomDlg::m_CustomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CustomDlg, pParent)
{

}

m_CustomDlg::~m_CustomDlg()
{
}

void m_CustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(m_CustomDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &m_CustomDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// m_CustomDlg 消息处理程序





void m_CustomDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString RowStr; 
	CString ColStr; 
	CString MineCntStr;
	//获取用户输入的数据
	GetDlgItemText(IDC_EDIT1, RowStr);
	GetDlgItemText(IDC_EDIT2, ColStr);
	GetDlgItemText(IDC_EDIT3, MineCntStr);
	//把数据转化为整数
	int Height = _ttoi(RowStr); //_ttoi()将字符串转化为整数
	int Width=_ttoi(ColStr);
	int MineCnt=_ttoi(MineCntStr);
	//判断数据是否合法（宽:9-30,高:9-16,雷:10-432）
	//10<雷数<(长×宽×0.9)
	if (Height < 9) Height = 9;
	if (Height > 16) Height = 16;
	if (Width < 9) Width = 9;
	if (Width > 30) Width = 30;
	if (MineCnt < 10) MineCnt = 10;
	if (MineCnt > (Height * Width * 0.9)) MineCnt = Height * Width * 0.9;
	//把数字传给主窗口 重新生成游戏界面
	CmineSweepingDlg* pmDlg = CmineSweepingDlg::s_pmDlg;
	pmDlg->m_rowCount = Height;
	pmDlg->m_colCount = Width;
	pmDlg->m_mineCount = MineCnt;
	pmDlg->m_leftCount = pmDlg->m_mineCount;
	pmDlg->gameRestart();

	CDialogEx::OnOK();
}
