// TCPClient.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 512
#define LENGTH	4

void err_quit(char *msg);
void err_display(char *msg);
bool ReceiveData(int _iLength, char* _szBuffer, SOCKET _pSock, int _iFlag);

int _tmain(int argc, _TCHAR* argv[])
{
	int iReturnValue = 0;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET pSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(pSocket == INVALID_SOCKET)
	{
		err_display("socket()");
		return 0;
	}

	//connect()
	SOCKADDR_IN tServerAddress;
	ZeroMemory(&tServerAddress, sizeof(tServerAddress));
	tServerAddress.sin_family = AF_INET;
	tServerAddress.sin_port = htons(9000);
	tServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	iReturnValue = connect(pSocket, (SOCKADDR *)&tServerAddress, sizeof(tServerAddress));
	if(iReturnValue == SOCKET_ERROR)
	{
		err_display("connect()");
		return 0;
	}

	// ������ ��ſ� ����� ����
	char szBuffer[BUFSIZE*4];
	ZeroMemory(szBuffer, BUFSIZE * 4);
	int iLength = 0;

	// ������ ������ ���
	while(true)
	{
		int iDataLength = 0;
		bool bReturn = false;
		// ������ �Է�
		ZeroMemory(szBuffer, sizeof(szBuffer));
		printf("\n[���� ������] ");
		if(fgets(szBuffer, BUFSIZE *4, stdin) == NULL)
			break;

		iDataLength = strlen(szBuffer);

		// '\n' ���� ����
		iLength = strlen(szBuffer);
		if(szBuffer[iLength-1] == '\n')
		{
			szBuffer[iLength-1] = '\0';
		}

		if(strlen(szBuffer) == 0)
			break;

		iReturnValue = send(pSocket, (char*)&iDataLength, LENGTH, 0);

		// ������ ������
		iReturnValue = send(pSocket, szBuffer, strlen(szBuffer), 0);
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		printf("[TCPŬ���̾�Ʈ] %d ����Ʈ�� ���½��ϴ�.\n", iReturnValue);

		// ������ �ޱ�
		bReturn = ReceiveData(iReturnValue, szBuffer, pSocket, 0);
		if(bReturn == false)
		{
			return 0;
		}

		// ���� ������ ���
		szBuffer[iReturnValue] = '\0';
		printf("[���� ������] %s\n", szBuffer);
	}

	// closesocket()
	closesocket(pSocket);
	pSocket = NULL;

	//���� ����
	WSACleanup();
	return 0;
}

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

bool ReceiveData(int _iLength, char* _szBuffer, SOCKET _pSock, int _iFlag)
{
	int iReturnValue = 0;
	int iCount = _iLength;
	char* szBuffer = _szBuffer;

	while(iCount > 0)
	{
		//������ �ޱ�
		iReturnValue = recv(_pSock, szBuffer, BUFSIZE, _iFlag);

		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("recv()");
			return false;
		}
		else if (iReturnValue == 0)
		{
			return false;
		}

		iCount -= iReturnValue;
		szBuffer += iReturnValue;
	}

	return true;
}



