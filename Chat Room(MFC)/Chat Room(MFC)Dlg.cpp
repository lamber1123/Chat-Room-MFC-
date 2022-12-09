
// Chat Room(MFC)Dlg.cpp : ʵ���ļ�
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
CEdit *pedit; //������ָ��

DWORD communication()
{
	//ͨ��
	char buff[256];
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	MessageBox(hwnd, "���ӳɹ�", "״̬��Ϣ", MB_OK);
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
			SetDlgItemText(hwnd, IDC_STATIC_STATUS, "����������ʧ��");
			WSACleanup();
			return TRUE;
		}
		Sleep(100);
	}
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


// CChatRoomMFCDlg �Ի���



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


// CChatRoomMFCDlg ��Ϣ�������

BOOL CChatRoomMFCDlg::OnInitDialog()
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
	CWnd *Address_BUTTON;
	Address_BUTTON = GetDlgItem(IDC_BUTTON4);
	Address_BUTTON->EnableWindow(false);
	Address_BUTTON = GetDlgItem(IDC_EDIT3);
	Address_BUTTON->EnableWindow(false);
	Address_BUTTON = GetDlgItem(IDC_BUTTON5);
	Address_BUTTON->EnableWindow(false);

	pedit = (CEdit*)GetDlgItem(IDC_EDIT1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatRoomMFCDlg::OnPaint()
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
HCURSOR CChatRoomMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChatRoomMFCDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//1.����Э��汾
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); //����2.2�汾��ͨ��Э��
	if (LOBYTE(wsadata.wVersion) != 2 ||
		HIBYTE(wsadata.wVersion) != 2){
		SetDlgItemText(IDC_STATIC_STATUS, "����汾ʧ��");
		return;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "����汾�ɹ�");
	Sleep(100);

	//2.����Socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET){
		SetDlgItemText(IDC_STATIC_STATUS, "����Socketʧ��");
		return;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "����Socket�ɹ�");
	Sleep(100);

	//3.����IP�ؼ���ַ
	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	BYTE nf1, nf2, nf3, nf4;
	pIP->GetAddress(nf1, nf2, nf3, nf4);
	CString IPaddr;
	IPaddr.Format("%d.%d.%d.%d", nf1, nf2, nf3, nf4);
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET; //Э��
	addr.sin_addr.S_un.S_addr = inet_addr(IPaddr); //����   ����
	addr.sin_port = htons(10086); //����˿�   Ӧ��
	Sleep(100);

	//4.���ӷ�����
	int r = connect(serverSocket, (sockaddr*)&addr, sizeof(addr));
	if (r == -1){
		SetDlgItemText(IDC_STATIC_STATUS, "����������ʧ��");
		return;
	}
	else SetDlgItemText(IDC_STATIC_STATUS, "���������ӳɹ�");

	// ��� ��������:host_name IP��ַ:host_ip
	char host_name[255];
	gethostname(host_name, sizeof(host_name));
	PHOSTENT hostinfo = gethostbyname(host_name);
	LPCSTR host_ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
	Name = host_name;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communication, NULL, NULL, NULL);

	//���Ӱ�ťʧЧ
	CWnd *Button_Connect = GetDlgItem(IDC_BUTTON1);
	Button_Connect->EnableWindow(false);
	CWnd *Address_IP = GetDlgItem(IDC_IPADDRESS1);
	Address_IP->EnableWindow(false);
	CWnd *Address_Again = GetDlgItem(IDC_BUTTON4);
	Address_Again->EnableWindow(true);

	//Edit1ֻ��
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->SetReadOnly(true);
}


void CChatRoomMFCDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//������Ϣ
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���
	Words = "";
	SetDlgItemText(IDC_EDIT1, Words);
}


void CChatRoomMFCDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CChatRoomMFCDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CChatRoomMFCDlg::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�Ͽ�����
	closesocket(serverSocket);
	SetDlgItemText(IDC_STATIC_STATUS, "���������ӶϿ�");
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


void CChatRoomMFCDlg::OnBnClickedButton6()
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
