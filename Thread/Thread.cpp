// Thread.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
	//	������ ������ ���� ���� ����
	HANDLE hThread[THREAD_COUNT] = { 0, };
	//	������ ���̵��� �����ϱ� ���� ���� ����
	DWORD dwThreadID[THREAD_COUNT] = { 0,};

	//	�������Լ����� �����ϱ� ���� ������ ����
	int iCount = 0;

	//	�����Ϳ� �����޸��� �����Ϳ� ������ ���̵� �Ѱ��ֱ� ���� ����ü
	CONTAINER tDataPack[THREAD_COUNT];

	//	���������� �����͸� �Ѱ��ֱ� ������ ������ �Ҵ� ���Ŀ� ID�� ���൵ ���� ������?
	//	�Ҵ� ���Ŀ� ����� �ɼ��� �����ϱ� ����� �ٸ��� �ϱ�� �ߴ�.


	InitializeCriticalSection(&tCriticalSection);

	for(int i = 0; i < THREAD_COUNT; i++ )
	{
		tDataPack[i].iCount = &iCount;
		//	�� �����带 �Ҵ�
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, (LPVOID)&tDataPack[i], 0, (unsigned*)&(tDataPack[i].dwThreadID));
		if(hThread[i] == NULL)
		{
			printf("%d�� ������ ������ �����߽��ϴ�.\n", i);
		}
	}

	//	��� �����尡 ����Ǿ��� ��쿡 ���α׷��� �����ؾ��ϹǷ� �ش� �Լ��� ����ߴ�
	WaitForMultipleObjects(THREAD_COUNT, hThread, true, INFINITE);

	DeleteCriticalSection(&tCriticalSection);

	printf("��� : %d\n" , iCount);

	//	������ �������� ����
	for(int i = 0; i < THREAD_COUNT; i++)
	{
		CloseHandle(hThread[i]);
		hThread[i] = NULL;
	}

	/*
	HANDLE hDeadLockThread = NULL;
	DWORD dwDeadLockThreadID = 0;
	//	����� �߻�
	hDeadLockThread = (HANDLE)_beginthreadex(NULL, 0, ThreadDeadLockFunc, NULL, 0, (unsigned*)&dwDeadLockThreadID);
	if(hDeadLockThread == NULL)
	{
		printf("����� ������ ������ �����߽��ϴ�.\n");
	}

	InitializeCriticalSection(&tCriticalSectionA);
	InitializeCriticalSection(&tCriticalSectionB);

	while(true)
	{
	EnterCriticalSection(&tCriticalSectionA);
	printf("�� ������ (%d) ũ��Ƽ�� ����A ���� : B ������\n", GetCurrentThreadId());
	EnterCriticalSection(&tCriticalSectionB);
	printf("�� ������ (%d) ũ��Ƽ�� ����A, B ����\n", GetCurrentThreadId());
	LeaveCriticalSection(&tCriticalSectionB);
	printf("�� ������ (%d) ũ��Ƽ�� ����B ���� : A �ܷ�\n", GetCurrentThreadId());
	LeaveCriticalSection(&tCriticalSectionA);
	printf("�� ������ (%d) ũ��Ƽ�� ���� ��ü ����\n", GetCurrentThreadId());
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

	//	ũ��Ƽ�� ���� ����� ���� ���� �޸� ��� ����ȭ

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
//	//	���� �ڿ��� ���� ���ôٹ������� ����
//	while(true)
//	{
//		EnterCriticalSection(&tCriticalSectionB);
//		printf("DeadLockFunc (%d) ũ��Ƽ�� ����B ���� : A ������\n", GetCurrentThreadId());
//		EnterCriticalSection(&tCriticalSectionA);
//		printf("DeadLockFunc (%d) ũ��Ƽ�� ����B, A ����\n", GetCurrentThreadId());
//		LeaveCriticalSection(&tCriticalSectionA);
//		printf("DeadLockFunc (%d) ũ��Ƽ�� ����A ���� : B �ܷ�\n", GetCurrentThreadId());
//		LeaveCriticalSection(&tCriticalSectionB);
//		printf("DeadLockFunc (%d) ũ��Ƽ�� ���� ��ü ����\n", GetCurrentThreadId());
//		Sleep(1000);
//	}
//
//	return 0;
//}