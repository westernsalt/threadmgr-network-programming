// UDPClient.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 512

void err_quit(char* msg);
void err_display(char* msg);

int _tmain(int argc, _TCHAR* argv[])
{
	int iReturnValue = 0;

	// ���� �ʱ�ȭ
	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET pSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(pSocket == INVALID_SOCKET)
	{
		err_display("socket()");
		return 0;
	}

	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN tServerAddress;
	ZeroMemory(&tServerAddress, sizeof(tServerAddress));
	tServerAddress.sin_family = AF_INET;
	tServerAddress.sin_port = htons(9000);
	tServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	// ������ ��ſ� ����� ����
	SOCKADDR_IN tPeerAddress;
	int iAddrLength = 0;
	char szBuffer[BUFSIZE + 1];
	int iLength = 0;

	ZeroMemory(&tPeerAddress, sizeof(SOCKADDR_IN));
	ZeroMemory(szBuffer, BUFSIZE + 1);

	// ������ ������ ���
	while(true)
	{
		// ������ �Է�
		printf("\n[���� ������] ");
		
		if(fgets(szBuffer, BUFSIZE+1, stdin) == NULL)
			break;

		// '\n' ���� ����
		iLength = strlen(szBuffer);

		if(szBuffer[iLength-1] == '\n')
		{
			szBuffer[iLength-1] = '\0';
		}

		if(strlen(szBuffer) == 0)
		{
			break;
		}

		// ������ ������
		iReturnValue = sendto(pSocket, szBuffer, strlen(szBuffer), 0, (SOCKADDR*)&tServerAddress, sizeof(tServerAddress));
		
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("sendto()");
			continue;
		}

		printf("[UDPŬ���̾�Ʈ] %d ����Ʈ�� ���½��ϴ�.\n", iReturnValue);

		// ������ �ޱ�
		iAddrLength = sizeof(tPeerAddress);
		iReturnValue = recvfrom(pSocket, szBuffer, BUFSIZE, 0, (SOCKADDR*)&tPeerAddress, &iAddrLength);
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("recvfrom()");
			continue;
		}

		// �۽����� IP �ּ� üũ
		if(memcmp(&tPeerAddress, &tServerAddress, sizeof(tPeerAddress)))
		{
			printf("[����] �߸��� �������Դϴ�\n");
			continue;
		}

		// ���� ������ ���
		szBuffer[iReturnValue] = '\0';
		printf("[UDPŬ���̾�Ʈ] %d ����Ʈ�� �޾ҽ��ϴ�.\n", iReturnValue);
		printf("[���� ������] %s\n", szBuffer);
	}

	// closesocket()
	closesocket(pSocket);
	pSocket = NULL;

	// ���� ����
	WSACleanup();
	return 0;
}

void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}