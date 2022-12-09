
// Chat Room(Server)Dlg.cpp : ʵ���ļ�
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
SOCKET clientSocket[128]; // �ͻ���socket
in_addr clientAddr[128];  // �ͻ���IP��ַ

SOCKADDR_IN addr = { 0 };

PHOSTENT hostinfo;        // �����û���
LPCSTR host_ip;           // ����IP��ַ

CString Words, Name;
int n_client;
CEdit *pedit;             //������ָ��

DWORD communication(LPVOID n)
{
	//ͨ��
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
			sprintf_s(buff, "[�ͻ���%s�ѶϿ�]\r\n\r\n", inet_ntoa(clientAddr[k]));
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

	// 4.��
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
	if (r == -1){
		SetDlgItemText(hwnd, IDC_STATIC_STATUS, "��ʧ��");
		return	FALSE;
	}
	else SetDlgItemText(hwnd, IDC_STATIC_STATUS, "�󶨳ɹ�");
	Sleep(500);

	// 5.����
	if (listen(serverSocket, 10) == -1){
		SetDlgItemText(hwnd, IDC_STATIC_STATUS, "����ʧ��");
		return FALSE;
	}
	else SetDlgItemText(hwnd, IDC_STATIC_STATUS, "�����ɹ�");
	Sleep(500);

	// 6.�ȴ��ͻ�����
	SOCKADDR_IN cAddr = { 0 };
	int len = sizeof(cAddr);
	int i = 0;
	while (i++ < 128){
		SetDlgItemText(hwnd, IDC_STATIC_STATUS, "�ȴ��ͻ�������");
		clientSocket[i] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		if (clientSocket[i] == SOCKET_ERROR){
			SetDlgItemText(hwnd, IDC_STATIC_STATUS, "��رճ���");
			closesocket(serverSocket);
			WSACleanup();
		}
		else{
			SetDlgItemText(hwnd, IDC_STATIC_STATUS, "�����ӳɹ�");
			Sleep(500);
			n_client++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communication, (LPVOID)i, NULL, NULL);
			char buff[256];
			memset(buff, 0, 256);
			clientAddr[i] = cAddr.sin_addr;
			sprintf_s(buff, "[�ͻ���%s������]\r\n\r\n", inet_ntoa(cAddr.sin_addr));
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CChatRoomServerDlg �Ի���



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


// CChatRoomServerDlg ��Ϣ�������

BOOL CChatRoomServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	CWnd *Address_IP = GetDlgItem(IDC_IPADDRESS1);
	Address_IP->EnableWindow(false);
	CWnd *Button_Connect4 = GetDlgItem(IDC_BUTTON4);
	Button_Connect4->EnableWindow(false);
	CWnd *Button_Connect5 = GetDlgItem(IDC_BUTTON5);
	Button_Connect5->EnableWindow(false);
	pedit = (CEdit*)GetDlgItem(IDC_EDIT1);

	Name = "ChatRoom������";

	//1.����Э��汾
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); //����2.2�汾��ͨ��Э��
	if (LOBYTE(wsadata.wVersion) != 2 ||
		HIBYTE(wsadata.wVersion) != 2){
		SetDlgItemText(IDC_STATIC_STATUS, "����汾ʧ��");
		return FALSE;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "����汾�ɹ�");
	Sleep(500);

	//2.����Socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET){
		SetDlgItemText(IDC_STATIC_STATUS, "����Socketʧ��");
		return FALSE;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "����Socket�ɹ�");
	Sleep(500);

	//3.����Э���ַ��return FALSE;

	char host_name[255]; 
	gethostname(host_name, sizeof(host_name));
	hostinfo = gethostbyname(host_name);
	host_ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
	SetDlgItemText(IDC_IPADDRESS1, host_ip);

	addr.sin_family = AF_INET; //Э��
	addr.sin_addr.S_un.S_addr = inet_addr(host_ip); //����   ����
	addr.sin_port = htons(10086); //����˿�   Ӧ��
	SetDlgItemText(IDC_STATIC_STATUS, "���IP��ַ�ɹ�");
	Sleep(500);

	SetDlgItemText(IDC_STATIC_STATUS, "����򿪷�����");
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatRoomServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatRoomServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatRoomServerDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CChatRoomServerDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CChatRoomServerDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//������

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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�Ͽ�����
	char buff[256];

	sprintf_s(buff, "[������%s�ѹر�]\r\n\r\n", host_ip);
	for (int client_num = 0; client_num <= n_client; client_num++){
		send(clientSocket[client_num], buff, 255, NULL);
	}
	Words += buff;
	SetDlgItemText(IDC_EDIT1, Words);

	closesocket(serverSocket);
	WSACleanup();
	SetDlgItemText(IDC_STATIC_STATUS, "�������رճɹ�");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CChatRoomServerDlg::OnBnClickedButton6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���
	Words = "";
	SetDlgItemText(IDC_EDIT1, Words);
}

void CChatRoomServerDlg::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CChatRoomServerDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//������Ϣ
	CString buff, Time;
	SYSTEMTIME st;
	
	SetDlgItemText(IDC_STATIC_STATUS, "������Ϣ�ɹ�");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDIT3, Name);
	CWnd *Address_BUTTON;
	Address_BUTTON = GetDlgItem(IDC_BUTTON5);
	Address_BUTTON->EnableWindow(false);
	Address_BUTTON = GetDlgItem(IDC_EDIT3);
	Address_BUTTON->EnableWindow(false);
	Address_BUTTON = GetDlgItem(IDC_BUTTON6);
	Address_BUTTON->EnableWindow(true);
}
