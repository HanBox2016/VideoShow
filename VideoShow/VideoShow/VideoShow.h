
// VideoShow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoShowApp:
// �йش����ʵ�֣������ VideoShow.cpp
//

class CVideoShowApp : public CWinApp
{
public:
	CVideoShowApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVideoShowApp theApp;