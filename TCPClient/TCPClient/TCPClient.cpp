// TCPClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

	// 윈속 초기화
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

	// 데이터 통신에 사용할 변수
	char szBuffer[BUFSIZE*4];
	ZeroMemory(szBuffer, BUFSIZE * 4);
	int iLength = 0;

	// 서버와 데이터 통신
	while(true)
	{
		int iDataLength = 0;
		bool bReturn = false;
		// 데이터 입력
		ZeroMemory(szBuffer, sizeof(szBuffer));
		printf("\n[보낼 데이터] ");
		if(fgets(szBuffer, BUFSIZE *4, stdin) == NULL)
			break;

		iDataLength = strlen(szBuffer);

		// '\n' 문자 제거
		iLength = strlen(szBuffer);
		if(szBuffer[iLength-1] == '\n')
		{
			szBuffer[iLength-1] = '\0';
		}

		if(strlen(szBuffer) == 0)
			break;

		iReturnValue = send(pSocket, (char*)&iDataLength, LENGTH, 0);

		// 데이터 보내기
		iReturnValue = send(pSocket, szBuffer, strlen(szBuffer), 0);
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		printf("[TCP클라이언트] %d 바이트를 보냈습니다.\n", iReturnValue);

		// 데이터 받기
		bReturn = ReceiveData(iReturnValue, szBuffer, pSocket, 0);
		if(bReturn == false)
		{
			return 0;
		}

		// 받은 데이터 출력
		szBuffer[iReturnValue] = '\0';
		printf("[받은 데이터] %s\n", szBuffer);
	}

	// closesocket()
	closesocket(pSocket);
	pSocket = NULL;

	//윈속 종료
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
		//데이터 받기
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



