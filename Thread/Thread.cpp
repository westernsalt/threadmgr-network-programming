// Thread.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "stdlib.h"
#include <iostream>
#include <windows.h>
#include <process.h>

UINT WINAPI ThreadFunction(LPVOID _arg);
//UINT WINAPI ThreadDeadLockFunc(LPVOID _arg);
#define THREAD_COUNT	4

CRITICAL_SECTION tCriticalSection;//A, tCriticalSectionB;

typedef struct tContainer
{
	int* iCount;
	DWORD dwThreadID;
} CONTAINER;

int _tmain(int argc, _TCHAR* argv[])
{
	//	스레드 생성을 위한 변수 선언
	HANDLE hThread[THREAD_COUNT] = { 0, };
	//	스레드 아이디값을 저장하기 위한 변수 선언
	DWORD dwThreadID[THREAD_COUNT] = { 0,};

	//	스레드함수에서 접근하기 위한 정수형 변수
	int iCount = 0;

	//	데이터에 공유메모리의 포인터와 스레드 아이디를 넘겨주기 위한 구조체
	CONTAINER tDataPack[THREAD_COUNT];

	//	데이터팩의 포인터를 넘겨주기 때문에 스레드 할당 이후에 ID를 써줘도 되지 않을까?
	//	할당 이후에 사용이 될수도 있으니까 방법을 다르게 하기로 했다.


	InitializeCriticalSection(&tCriticalSection);

	for(int i = 0; i < THREAD_COUNT; i++ )
	{
		tDataPack[i].iCount = &iCount;
		//	각 스레드를 할당
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, (LPVOID)&tDataPack[i], 0, (unsigned*)&(tDataPack[i].dwThreadID));
		if(hThread[i] == NULL)
		{
			printf("%d번 스레드 생성에 실패했습니다.\n", i);
		}
	}

	//	모든 스레드가 종료되었을 경우에 프로그램을 종료해야하므로 해당 함수를 사용했다
	WaitForMultipleObjects(THREAD_COUNT, hThread, true, INFINITE);

	DeleteCriticalSection(&tCriticalSection);

	printf("결과 : %d\n" , iCount);

	//	생성된 스레드의 해제
	for(int i = 0; i < THREAD_COUNT; i++)
	{
		CloseHandle(hThread[i]);
		hThread[i] = NULL;
	}

	/*
	HANDLE hDeadLockThread = NULL;
	DWORD dwDeadLockThreadID = 0;
	//	데드락 발생
	hDeadLockThread = (HANDLE)_beginthreadex(NULL, 0, ThreadDeadLockFunc, NULL, 0, (unsigned*)&dwDeadLockThreadID);
	if(hDeadLockThread == NULL)
	{
		printf("데드락 스레드 생성에 실패했습니다.\n");
	}

	InitializeCriticalSection(&tCriticalSectionA);
	InitializeCriticalSection(&tCriticalSectionB);

	while(true)
	{
	EnterCriticalSection(&tCriticalSectionA);
	printf("주 스레드 (%d) 크리티컬 섹션A 진입 : B 비진입\n", GetCurrentThreadId());
	EnterCriticalSection(&tCriticalSectionB);
	printf("주 스레드 (%d) 크리티컬 섹션A, B 진입\n", GetCurrentThreadId());
	LeaveCriticalSection(&tCriticalSectionB);
	printf("주 스레드 (%d) 크리티컬 섹션B 종료 : A 잔류\n", GetCurrentThreadId());
	LeaveCriticalSection(&tCriticalSectionA);
	printf("주 스레드 (%d) 크리티컬 섹션 전체 종료\n", GetCurrentThreadId());
	Sleep(1000);
	}

	DeleteCriticalSection(&tCriticalSectionA);
	DeleteCriticalSection(&tCriticalSectionB);

	CloseHandle(hDeadLockThread);
	hDeadLockThread = NULL;*/

	return 0;
}

UINT WINAPI ThreadFunction( LPVOID _arg )
{
	CONTAINER* tData = (CONTAINER*)_arg;

	//	크리티컬 섹션 사용을 통해 공용 메모리 사용 동기화

	while(*(tData->iCount) < 100)
	{
		EnterCriticalSection(&tCriticalSection);
		*(tData->iCount) = *(tData->iCount) + 1;
		printf("Thread %d : %d\n" , (tData->dwThreadID), *(tData->iCount));
		LeaveCriticalSection(&tCriticalSection);
	}

	return 0;
}

//UINT WINAPI ThreadDeadLockFunc( LPVOID _arg )
//{
//	//	전역 자원의 락을 동시다발적으로 진행
//	while(true)
//	{
//		EnterCriticalSection(&tCriticalSectionB);
//		printf("DeadLockFunc (%d) 크리티컬 섹션B 진입 : A 비진입\n", GetCurrentThreadId());
//		EnterCriticalSection(&tCriticalSectionA);
//		printf("DeadLockFunc (%d) 크리티컬 섹션B, A 진입\n", GetCurrentThreadId());
//		LeaveCriticalSection(&tCriticalSectionA);
//		printf("DeadLockFunc (%d) 크리티컬 섹션A 종료 : B 잔류\n", GetCurrentThreadId());
//		LeaveCriticalSection(&tCriticalSectionB);
//		printf("DeadLockFunc (%d) 크리티컬 섹션 전체 종료\n", GetCurrentThreadId());
//		Sleep(1000);
//	}
//
//	return 0;
//}