
// RubiksCube.h : RubiksCube ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CRubiksCubeApp:
// �� Ŭ������ ������ ���ؼ��� RubiksCube.cpp�� �����Ͻʽÿ�.
//

class CRubiksCubeApp : public CWinApp
{
public:
	CRubiksCubeApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRubiksCubeApp theApp;
