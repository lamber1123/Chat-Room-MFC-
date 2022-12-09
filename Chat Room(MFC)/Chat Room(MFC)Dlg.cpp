
// Chat Room(MFC)Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Chat Room(MFC).h"
#include "Chat Room(MFC)Dlg.h"
#include "afxdialogex.h"
#include <string.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKET serverSocket;
CString Words, Name;
CEdit *pedit; //滚动条指针

DWORD communication()
{
	//通信
	char buff[256];
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	MessageBox(hwnd, "连接成功", "状态信息", MB_OK);
	while (TRUE){
		memset(buff, 0, sizeof(buff));
		int r = recv(serverSocket, buff, 255, NULL);
		if (r > 0){
			Words += buff;
			SetDlgItemText(hwnd, IDC_EDIT1, Words);
			pedit->LineScroll(pedit->GetLineCount());
		}
		else {
			closesocket(serverSocket);
			SetDlgItemText(hwnd, IDC_STATIC_STATUS, "服务器连接失败");
			WSACleanup();
			return TRUE;
		}
		Sleep(100);
	}
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatRoomMFCDlg 对话框



CChatRoomMFCDlg::CChatRoomMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatRoomMFCDlg::IDD, pParent)
	, m_strEditReceiveMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatRoomMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT1, m_strEditReceiveMsf);
	//  DDX_Control(pDX, IDC_EDIT1, m_ctrEditReceive);
	DDX_Control(pDX, IDC_EDIT1, m_ctrEditReceive);
	//  DDX_Text(pDX, IDC_EDIT1, m_strEditReceviveMsg);
	DDX_Text(pDX, IDC_EDIT1, m_strEditReceiveMsg);
}

BEGIN_MESSAGE_MAP(CChatRoomMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChatRoomMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CChatRoomMFCDlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT1, &CChatRoomMFCDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatRoomMFCDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &CChatRoomMFCDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON4, &CChatRoomMFCDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CChatRoomMFCDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CChatRoomMFCDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CChatRoomMFCDlg 消息处理程序

BOOL CChatRoomMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO:  在此添加额外的初始化代码
	CWnd *Address_BUTTON;
	Address_BUTTON = GetDlgItem(IDC_BUTTON4);
	Address_BUTTON->EnableWindow(false);
	Address_BUTTON = GetDlgItem(IDC_EDIT3);
	Address_BUTTON->EnableWindow(false);
	Address_BUTTON = GetDlgItem(IDC_BUTTON5);
	Address_BUTTON->EnableWindow(false);

	pedit = (CEdit*)GetDlgItem(IDC_EDIT1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatRoomMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatRoomMFCDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChatRoomMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChatRoomMFCDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	//1.请求协议版本
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); //请求2.2版本的通信协议
	if (LOBYTE(wsadata.wVersion) != 2 ||
		HIBYTE(wsadata.wVersion) != 2){
		SetDlgItemText(IDC_STATIC_STATUS, "请求版本失败");
		return;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "请求版本成功");
	Sleep(100);

	//2.创建Socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET){
		SetDlgItemText(IDC_STATIC_STATUS, "创建Socket失败");
		return;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "创建Socket成功");
	Sleep(100);

	//3.连接IP控件地址
	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	BYTE nf1, nf2, nf3, nf4;
	pIP->GetAddress(nf1, nf2, nf3, nf4);
	CString IPaddr;
	IPaddr.Format("%d.%d.%d.%d", nf1, nf2, nf3, nf4);
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET; //协议
	addr.sin_addr.S_un.S_addr = inet_addr(IPaddr); //主机   电脑
	addr.sin_port = htons(10086); //网络端口   应用
	Sleep(100);

	//4.连接服务器
	int r = connect(serverSocket, (sockaddr*)&addr, sizeof(addr));
	if (r == -1){
		SetDlgItemText(IDC_STATIC_STATUS, "服务器连接失败");
		return;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "服务器连接成功");

	// 获得 本机名称:host_name IP地址:host_ip
	char host_name[255];
	gethostname(host_name, sizeof(host_name));
	PHOSTENT hostinfo = gethostbyname(host_name);
	LPCSTR host_ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
	Name = host_name;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communication, NULL, NULL, NULL);

	//连接按钮失效
	CWnd *Button_Connect = GetDlgItem(IDC_BUTTON1);
	Button_Connect->EnableWindow(false);
	CWnd *Address_IP = GetDlgItem(IDC_IPADDRESS1);
	Address_IP->EnableWindow(false);
	CWnd *Address_Again = GetDlgItem(IDC_BUTTON4);
	Address_Again->EnableWindow(true);

	//Edit1只读
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->SetReadOnly(true);
}


void CChatRoomMFCDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	//发送消息
	CString buff, Time;
	SYSTEMTIME st;
	GetLocalTime(&st);
	Time.Format("%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
	GetDlgItemText(IDC_EDIT2, buff);
	buff = Name + "(" + Time + "):" + "\r\n" + buff + "\r\n\r\n";
	send(serverSocket, buff, strlen(buff), NULL);

	pedit->LineScroll(pedit->GetLineCount());

}	

void CChatRoomMFCDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	//清空
	Words = "";
	SetDlgItemText(IDC_EDIT1, Words);
}


void CChatRoomMFCDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CChatRoomMFCDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CChatRoomMFCDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	//断开连接
	closesocket(serverSocket);
	SetDlgItemText(IDC_STATIC_STATUS, "服务器连接断开");
	WSACleanup();
	CWnd *Button_Connect = GetDlgItem(IDC_BUTTON1);
	Button_Connect->EnableWindow(true);
	CWnd *Button_Break = GetDlgItem(IDC_BUTTON4);
	Button_Break->EnableWindow(false);
	CWnd *Address_IP = GetDlgItem(IDC_IPADDRESS1);
	Address_IP->EnableWindow(true);
}


void CChatRoomMFCDlg::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT3, Name);
	CWnd *Address_BUTTON;
	Address_BUTTON = GetDlgItem(IDC_BUTTON5);
	Address_BUTTON->EnableWindow(false);
	Address_BUTTON = GetDlgItem(IDC_EDIT3);
	Address_BUTTON->EnableWindow(false);
	Address_BUTTON = GetDlgItem(IDC_BUTTON6);
	Address_BUTTON->EnableWindow(true);
}


void CChatRoomMFCDlg::OnBnClickedButton6()
{
	// TODO:  在此添加控件通知处理程序代码
	CWnd *Address_BUTTON;
	Address_BUTTON = GetDlgItem(IDC_BUTTON5);
	Address_BUTTON->EnableWindow(true);
	Address_BUTTON = GetDlgItem(IDC_EDIT3);
	Address_BUTTON->EnableWindow(true);
	Address_BUTTON = GetDlgItem(IDC_BUTTON6);
	Address_BUTTON->EnableWindow(false);
}
