
// mazeGenerator.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CmazeGeneratorApp:
// �� Ŭ������ ������ ���ؼ��� mazeGenerator.cpp�� �����Ͻʽÿ�.
//

class CmazeGeneratorApp : public CWinApp
{
public:
	CmazeGeneratorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CmazeGeneratorApp theApp;