
// Chat Room(Server)Dlg.h : 头文件
//

#pragma once


// CChatRoomServerDlg 对话框
class CChatRoomServerDlg : public CDialogEx
{
// 构造
public:
	CChatRoomServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHATROOMSERVER_DIALOG };

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
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_ctrEditReceive;
	CEdit* m_ctrEdit = &m_ctrEditReceive;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnStnClickedStaticStatus();
	afx_msg void OnEnChangeEdit3();
};
