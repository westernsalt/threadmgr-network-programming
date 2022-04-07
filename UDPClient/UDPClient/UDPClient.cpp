// UDPClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

	// 소켓 주소 구조체 초기화
	SOCKADDR_IN tServerAddress;
	ZeroMemory(&tServerAddress, sizeof(tServerAddress));
	tServerAddress.sin_family = AF_INET;
	tServerAddress.sin_port = htons(9000);
	tServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN tPeerAddress;
	int iAddrLength = 0;
	char szBuffer[BUFSIZE + 1];
	int iLength = 0;

	ZeroMemory(&tPeerAddress, sizeof(SOCKADDR_IN));
	ZeroMemory(szBuffer, BUFSIZE + 1);

	// 서버와 데이터 통신
	while(true)
	{
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		
		if(fgets(szBuffer, BUFSIZE+1, stdin) == NULL)
			break;

		// '\n' 문자 제거
		iLength = strlen(szBuffer);

		if(szBuffer[iLength-1] == '\n')
		{
			szBuffer[iLength-1] = '\0';
		}

		if(strlen(szBuffer) == 0)
		{
			break;
		}

		// 데이터 보내기
		iReturnValue = sendto(pSocket, szBuffer, strlen(szBuffer), 0, (SOCKADDR*)&tServerAddress, sizeof(tServerAddress));
		
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("sendto()");
			continue;
		}

		printf("[UDP클라이언트] %d 바이트를 보냈습니다.\n", iReturnValue);

		// 데이터 받기
		iAddrLength = sizeof(tPeerAddress);
		iReturnValue = recvfrom(pSocket, szBuffer, BUFSIZE, 0, (SOCKADDR*)&tPeerAddress, &iAddrLength);
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("recvfrom()");
			continue;
		}

		// 송신자의 IP 주소 체크
		if(memcmp(&tPeerAddress, &tServerAddress, sizeof(tPeerAddress)))
		{
			printf("[오류] 잘못된 데이터입니다\n");
			continue;
		}

		// 받은 데이터 출력
		szBuffer[iReturnValue] = '\0';
		printf("[UDP클라이언트] %d 바이트를 받았습니다.\n", iReturnValue);
		printf("[받은 데이터] %s\n", szBuffer);
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