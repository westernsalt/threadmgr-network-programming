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

//	�����带 �����ϰ� �ش� �������� �̺�Ʈ�� �迭�� �ֽ��ϴ�.
bool CThreadMgr::CreateThread()
{
	for(int i = 0 ; i < THREAD_COUNT; i++)
	{
		CAnotherThreadClass* newThread = NULL;
		newThread = new CAnotherThreadClass;

		if(newThread != NULL)
		{
			//	������� ������ ���ÿ� ����˴ϴ�.
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
	//	�����带 ���� �ı��ϸ� ����ߴ� ���ҽ��� �ʱ�ȭ��ŵ�ϴ�.
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