// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include "stdlib.h"
#include <iostream>
#include <windows.h>
#include <process.h>
#include <vector>

#define THREAD_COUNT 2
#define PURE		=0
enum eThreadType
{
	THREAD_SUBSTRACT,
	THREAD_ADD,
	THREAD_END,
};