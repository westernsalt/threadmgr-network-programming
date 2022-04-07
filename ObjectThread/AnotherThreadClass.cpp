#include "StdAfx.h"
#include "AnotherThreadClass.h"

CAnotherThreadClass::CAnotherThreadClass(void)
{
	m_iCount = 0;
}

CAnotherThreadClass::~CAnotherThreadClass(void)
{
	m_dwThreadID = NULL;
	m_hEvent = NULL;
	m_hThread = NULL;
}

int CAnotherThreadClass::Run()
{
	printf("Thread %d : %d\n", m_dwThreadID, m_iCount++);
	return 0;
}

void CAnotherThreadClass::Destroy()
{
	PostThreadMessage(GetThreadId(m_hThread), WM_QUIT, 0, 0); 
	CloseHandle(m_hThread);

	m_hThread = NULL;
	m_dwThreadID = 0;

	CloseHandle(m_hEvent);
	m_hEvent = NULL;
}
