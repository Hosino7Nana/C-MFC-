#pragma once


// m_CustomDlg 对话框

class m_CustomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(m_CustomDlg)

public:
	m_CustomDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~m_CustomDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CustomDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
