
// Chat Room(Server).h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChatRoomServerApp: 
// �йش����ʵ�֣������ Chat Room(Server).cpp
//

class CChatRoomServerApp : public CWinApp
{
public:
	CChatRoomServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChatRoomServerApp theApp;