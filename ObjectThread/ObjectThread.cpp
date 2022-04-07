// ObjectThread.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ThreadClass.h"
#include "AnotherThreadClass.h"
#include "ThreadMgr.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CThreadMgr ThreadMgr;
	
	ThreadMgr.CreateThread();
	
	int iResult = 0;
	iResult = getchar();
	ThreadMgr.DestroyThread();
	return 0;
}

/*
간단하게 생각해서,
스레드클래스를 만들고, 스레드가 자신의 할일을 자신의 메모리 공간에서 실행하며,
스레드의 종료는 메인에서 유도하되 스레드가 스스로의 메모리를 정리할 수 있게 해야하고
다만 스레드 매니저는 스레드의 생성과 실행, 종료만을 관리해야한다.

1. 스레드는 자신의 메모리 공간을 가진다
2. 스레드 매니저는 다수의 스레드를 관리한다
3. 메인에서는 스레드의 종료를 유도한다
*/