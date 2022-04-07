#include "StdAfx.h"
#include "ThreadMgr.h"

#include "ThreadClass.h"
#include "AnotherThreadClass.h"


CThreadMgr::CThreadMgr(void)
{
}


CThreadMgr::~CThreadMgr(void)
{
}

//	스레드를 생성하고 해당 스레드의 이벤트를 배열에 넣습니다.
bool CThreadMgr::CreateThread()
{
	for(int i = 0 ; i < THREAD_COUNT; i++)
	{
		CAnotherThreadClass* newThread = NULL;
		newThread = new CAnotherThreadClass;

		if(newThread != NULL)
		{
			//	스레드는 생성과 동시에 실행됩니다.
			newThread->CreateThread();
			newThread->GetEventHandle();
			m_vecThread.push_back(newThread);
		}
		else
		{
			delete newThread;
			newThread = NULL;
		}
	}

	return true;
}


void CThreadMgr::DestroyThread()
{
	//	스레드를 전부 파괴하며 사용했던 리소스를 초기화시킵니다.
	int iCount = m_vecThread.size();
	for(int i = 0; i< iCount; i++)
	{
		m_vecThread[i]->Destroy();
	}

	iCount = m_vecThread.size();
	for(int i = 0; i< iCount; i++)
	{
		delete m_vecThread[i];
		m_vecThread[i] = NULL;
	}
}