// ObjectThread.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
�����ϰ� �����ؼ�,
������Ŭ������ �����, �����尡 �ڽ��� ������ �ڽ��� �޸� �������� �����ϸ�,
�������� ����� ���ο��� �����ϵ� �����尡 �������� �޸𸮸� ������ �� �ְ� �ؾ��ϰ�
�ٸ� ������ �Ŵ����� �������� ������ ����, ���Ḹ�� �����ؾ��Ѵ�.

1. ������� �ڽ��� �޸� ������ ������
2. ������ �Ŵ����� �ټ��� �����带 �����Ѵ�
3. ���ο����� �������� ���Ḧ �����Ѵ�
*/