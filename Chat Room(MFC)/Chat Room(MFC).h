
// Chat Room(MFC).h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChatRoomMFCApp: 
// �йش����ʵ�֣������ Chat Room(MFC).cpp
//

class CChatRoomMFCApp : public CWinApp
{
public:
	CChatRoomMFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChatRoomMFCApp theApp;