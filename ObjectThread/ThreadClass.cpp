#include "StdAfx.h"
#include "ThreadClass.h"


CThreadClass::CThreadClass(void)
{
	m_bRun = true;
	m_dwThreadID = 0;
	m_hThread = NULL;
	m_hEvent = NULL;
}

CThreadClass::~CThreadClass(void)
{

}

void CThreadClass::SetbRun( bool _bRun )
{
	m_bRun = _bRun;
}

UINT WINAPI CThreadClass::ThreadFunction( LPVOID _arg )
{
	CThreadClass* ThreadClass = (CThreadClass*)_arg;
	MSG msg;
	while(GetMessage(&msg, NULL, WM_QUIT, 0))
		ThreadClass->Run();
	return 0;
}

bool CThreadClass::CreateThread()
{
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, (LPVOID)this, 0, (unsigned*)&m_dwThreadID);
	if(m_hThread == NULL)
	{
		printf("Thread 생성에 실패했습니다.\n");
		m_hThread = NULL;
		return false;
	}

	//	자동 리셋 이벤트로 설정했으며 처음은 비신호 상태로 시작한다.
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_hEvent == NULL)
		return false;

	return true;
}
