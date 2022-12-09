
// Chat Room(Server)Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Chat Room(Server).h"
#include "Chat Room(Server)Dlg.h"
#include "afxdialogex.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKET serverSocket;
SOCKET clientSocket[128]; // 客户端socket
in_addr clientAddr[128];  // 客户端IP地址

SOCKADDR_IN addr = { 0 };

PHOSTENT hostinfo;        // 本机用户名
LPCSTR host_ip;           // 本机IP地址

CString Words, Name;
int n_client;
CEdit *pedit;             //滚动条指针

DWORD communication(LPVOID n)
{
	//通信
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	char buff[256];
	int k = (int)n;

	while (TRUE){
		memset(buff, 0, sizeof(buff));
		int r = recv(clientSocket[k], buff, sizeof(buff), NULL);
		if (r > 0) {
			Words += buff;
			SetDlgItemText(hwnd, IDC_EDIT1, Words);
			pedit->LineScroll(pedit->GetLineCount());
			for (int i = 0; i <= n_client; i++){
				send(clientSocket[i], buff, sizeof(buff), NULL);
			}
		}
		else {
			memset(buff, 0, sizeof(buff));
			sprintf_s(buff, "[客户端%s已断开]\r\n\r\n", inet_ntoa(clientAddr[k]));
			Words += buff;
			SetDlgItemText(hwnd, IDC_EDIT1, Words);
			pedit->LineScroll(pedit->GetLineCount());
			for (int i = 0; i <= n_client; i++){
				send(clientSocket[i], buff, sizeof(buff), NULL);
			}
			closesocket(clientSocket[k]);
			return TRUE;
		}
	}
}

DWORD connection()
{
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;

	// 4.绑定
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
	if (r == -1){
		SetDlgItemText(hwnd, IDC_STATIC_STATUS, "绑定失败");
		return	FALSE;
	}
	else SetDlgItemText(hwnd, IDC_STATIC_STATUS, "绑定成功");
	Sleep(500);

	// 5.监听
	if (listen(serverSocket, 10) == -1){
		SetDlgItemText(hwnd, IDC_STATIC_STATUS, "监听失败");
		return FALSE;
	}
	else SetDlgItemText(hwnd, IDC_STATIC_STATUS, "监听成功");
	Sleep(500);

	// 6.等待客户连接
	SOCKADDR_IN cAddr = { 0 };
	int len = sizeof(cAddr);
	int i = 0;
	while (i++ < 128){
		SetDlgItemText(hwnd, IDC_STATIC_STATUS, "等待客户端连接");
		clientSocket[i] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		if (clientSocket[i] == SOCKET_ERROR){
			SetDlgItemText(hwnd, IDC_STATIC_STATUS, "请关闭程序");
			closesocket(serverSocket);
			WSACleanup();
		}
		else{
			SetDlgItemText(hwnd, IDC_STATIC_STATUS, "新连接成功");
			Sleep(500);
			n_client++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communication, (LPVOID)i, NULL, NULL);
			char buff[256];
			memset(buff, 0, 256);
			clientAddr[i] = cAddr.sin_addr;
			sprintf_s(buff, "[客户端%s已连接]\r\n\r\n", inet_ntoa(cAddr.sin_addr));
			Words += buff;
			SetDlgItemText(hwnd, IDC_EDIT1, Words);
			pedit->LineScroll(pedit->GetLineCount());
			for (int client_num = 0; client_num <= n_client; client_num++){
				send(clientSocket[client_num], buff, 255, NULL);
			}
		}
		Sleep(500);
	}
	return FALSE;
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


// CChatRoomServerDlg 对话框



CChatRoomServerDlg::CChatRoomServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatRoomServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatRoomServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChatRoomServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CChatRoomServerDlg::OnEnChangeEdit1)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CChatRoomServerDlg::OnIpnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatRoomServerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CChatRoomServerDlg::OnBnClickedButton4)
	ON_STN_CLICKED(IDC_STATIC_STATUS, &CChatRoomServerDlg::OnStnClickedStaticStatus)
	ON_BN_CLICKED(IDC_BUTTON6, &CChatRoomServerDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatRoomServerDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT3, &CChatRoomServerDlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON3, &CChatRoomServerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CChatRoomServerDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CChatRoomServerDlg 消息处理程序

BOOL CChatRoomServerDlg::OnInitDialog()
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

	CWnd *Address_IP = GetDlgItem(IDC_IPADDRESS1);
	Address_IP->EnableWindow(false);
	CWnd *Button_Connect4 = GetDlgItem(IDC_BUTTON4);
	Button_Connect4->EnableWindow(false);
	CWnd *Button_Connect5 = GetDlgItem(IDC_BUTTON5);
	Button_Connect5->EnableWindow(false);
	pedit = (CEdit*)GetDlgItem(IDC_EDIT1);

	Name = "ChatRoom服务器";

	//1.请求协议版本
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); //请求2.2版本的通信协议
	if (LOBYTE(wsadata.wVersion) != 2 ||
		HIBYTE(wsadata.wVersion) != 2){
		SetDlgItemText(IDC_STATIC_STATUS, "请求版本失败");
		return FALSE;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "请求版本成功");
	Sleep(500);

	//2.创建Socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET){
		SetDlgItemText(IDC_STATIC_STATUS, "创建Socket失败");
		return FALSE;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "创建Socket成功");
	Sleep(500);

	//3.创建协议地址族return FALSE;

	char host_name[255]; 
	gethostname(host_name, sizeof(host_name));
	hostinfo = gethostbyname(host_name);
	host_ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
	SetDlgItemText(IDC_IPADDRESS1, host_ip);

	addr.sin_family = AF_INET; //协议
	addr.sin_addr.S_un.S_addr = inet_addr(host_ip); //主机   电脑
	addr.sin_port = htons(10086); //网络端口   应用
	SetDlgItemText(IDC_STATIC_STATUS, "获得IP地址成功");
	Sleep(500);

	SetDlgItemText(IDC_STATIC_STATUS, "点击打开服务器");
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatRoomServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatRoomServerDlg::OnPaint()
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
HCURSOR CChatRoomServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatRoomServerDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CChatRoomServerDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CChatRoomServerDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码

	//打开连接

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)connection, NULL, NULL, NULL);

	CWnd *Button_Connect = GetDlgItem(IDC_BUTTON1);
	Button_Connect->EnableWindow(false);
	CWnd *Address_IP = GetDlgItem(IDC_IPADDRESS1);
	Address_IP->EnableWindow(false);
	CWnd *Address_Again = GetDlgItem(IDC_BUTTON4);
	Address_Again->EnableWindow(true);

}


void CChatRoomServerDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码

	//断开连接
	char buff[256];

	sprintf_s(buff, "[服务器%s已关闭]\r\n\r\n", host_ip);
	for (int client_num = 0; client_num <= n_client; client_num++){
		send(clientSocket[client_num], buff, 255, NULL);
	}
	Words += buff;
	SetDlgItemText(IDC_EDIT1, Words);

	closesocket(serverSocket);
	WSACleanup();
	SetDlgItemText(IDC_STATIC_STATUS, "服务器关闭成功");
	Sleep(500);

	CWnd *Button_Connect = GetDlgItem(IDC_BUTTON1);
	Button_Connect->EnableWindow(false);
	CWnd *Button_Break = GetDlgItem(IDC_BUTTON4);
	Button_Break->EnableWindow(false);
	CWnd *Address_IP = GetDlgItem(IDC_IPADDRESS1);
	Address_IP->EnableWindow(false);
}


void CChatRoomServerDlg::OnStnClickedStaticStatus()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CChatRoomServerDlg::OnBnClickedButton6()
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


void CChatRoomServerDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	//清空
	Words = "";
	SetDlgItemText(IDC_EDIT1, Words);
}

void CChatRoomServerDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CChatRoomServerDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码

	//发送消息
	CString buff, Time;
	SYSTEMTIME st;
	
	SetDlgItemText(IDC_STATIC_STATUS, "发送消息成功");
	Sleep(500);
	GetLocalTime(&st);
	Time.Format("%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
	GetDlgItemText(IDC_EDIT2, buff);
	buff = Name + "(" + Time + "):" + "\r\n" + buff + "\r\n\r\n";
	Words += buff;
	SetDlgItemText(IDC_EDIT1, Words);
	pedit->LineScroll(pedit->GetLineCount());
	for (int i = 1; i <= n_client; i++){
		send(clientSocket[i], buff, 255, NULL);
	}
	
	Sleep(500);
}


void CChatRoomServerDlg::OnBnClickedButton5()
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
