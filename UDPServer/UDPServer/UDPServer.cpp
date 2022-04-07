// UDPServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "winsock2.h"
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 512

void err_quit(char* msg);
void err_display(char* msg);

int _tmain(int argc, _TCHAR* argv[])
{
	int iReturnValue = 0;

	// 윈속 초기화
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

	//bind()
	SOCKADDR_IN tServerAddress;
	ZeroMemory(&tServerAddress, sizeof(tServerAddress));
	tServerAddress.sin_family = AF_INET;
	tServerAddress.sin_port = htons(9000);
	tServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	
	iReturnValue = bind(pSocket, (SOCKADDR *)&tServerAddress, sizeof(tServerAddress));

	if(iReturnValue == SOCKET_ERROR)
	{
		err_display("bind()");
		return 0;
	}

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN tClientAddress;
	int iAddrLength = 0;
	char szBuffer[BUFSIZE+1];

	ZeroMemory(&tClientAddress, sizeof(SOCKADDR_IN));
	ZeroMemory(szBuffer, BUFSIZE+1);

	// 클라이언트와 데이터 통신
	while(true)
	{
		// 데이터 받기
		iAddrLength = sizeof(tClientAddress);
		iReturnValue = recvfrom(pSocket, szBuffer, BUFSIZE, 0, 
			(SOCKADDR *)&tClientAddress, &iAddrLength);
		
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("recvfrom()");
			continue;
		}

		// 받은 데이터 출력
		szBuffer[iReturnValue] = '\0';
		printf("[UDP/%s : %d] %s\n", inet_ntoa(tClientAddress.sin_addr), ntohs(tClientAddress.sin_port), szBuffer);

		// 데이터 보내기
		iReturnValue = sendto(pSocket, szBuffer, iReturnValue, 0, (SOCKADDR*)&tClientAddress, sizeof(tClientAddress));
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("sendto()");
			continue;
		}
	}

	// closesocket()
	closesocket(pSocket);
	pSocket = NULL;

	// 윈속 종료
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