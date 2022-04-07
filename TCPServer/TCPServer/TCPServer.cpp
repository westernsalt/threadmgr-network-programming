// TCPServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "winsock2.h"
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

#define BUFSIZE 512
#define LENGTH	4

void err_quit(char* msg);
void err_display(char* msg);
bool ReceiveData(int _iLength, char* _szBuffer, SOCKET _pSock, int _iFlag);
bool PopReceiveDataLength(char* _szBuffer, SOCKET _pSock, int _iFlag);

UINT WINAPI CommunicateThread(void* arg);

int _tmain(int argc, _TCHAR* argv[])
{
	int iReturnValue = 0;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET pListenSock = socket(AF_INET, SOCK_STREAM, 0);

	if(pListenSock == INVALID_SOCKET)
		err_quit("socket()");

	// bind()
	SOCKADDR_IN tServerAddress;
	ZeroMemory(&tServerAddress, sizeof(tServerAddress));
	tServerAddress.sin_family = AF_INET;
	tServerAddress.sin_port = htons(9000);
	tServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	iReturnValue = bind(pListenSock, (SOCKADDR *)&tServerAddress, sizeof(tServerAddress));

	if(iReturnValue == SOCKET_ERROR)
	{
		err_display("bind()");
		return 0;
	}

	// listen ()
	iReturnValue = listen(pListenSock, SOMAXCONN);

	if(iReturnValue == SOCKET_ERROR)
	{
		err_display("listen()");
		return 0;
	}

	// 데이터 통신에 사용할 변수
	SOCKET pClientSock;
	SOCKADDR_IN tClientAddress;
	int iAddrLength = 0;
	char szBuffer[BUFSIZE * 4];

	ZeroMemory(&pClientSock, sizeof(SOCKET));
	ZeroMemory(&tClientAddress, sizeof(SOCKADDR_IN));
	ZeroMemory(szBuffer, BUFSIZE * 4);

	// 스레드 함수
	HANDLE hThread = NULL;
	DWORD dwThreadID = 0;

	while(true)
	{
		//accept()
		iAddrLength = sizeof(tClientAddress);
		pClientSock = accept(pListenSock, (SOCKADDR*) &tClientAddress, &iAddrLength);
		if(pClientSock == INVALID_SOCKET)
		{
			err_display("accept()");
			continue;
		}

		printf("\n[TCP서버] 클라이언트 접속 : IP주소 = %s, 포트번호 = %d\n",
			inet_ntoa(tClientAddress.sin_addr), ntohs(tClientAddress.sin_port));

		hThread = (HANDLE)_beginthreadex(NULL, 0, CommunicateThread, (void*)pClientSock, 0, (unsigned*)&dwThreadID);
		if(hThread == NULL)
		{
			printf("스레스 생성에 실패했습니다.\n");
		}
		else
			CloseHandle(hThread);
	}

	// closesocket()
	closesocket(pListenSock);
	pListenSock = NULL;

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

bool PopReceiveDataLength(char* _szBuffer, SOCKET _pSock, int _iFlag)
{
	int iReturnValue = 0;
	int iCount = LENGTH;
	char* szBuffer = _szBuffer;

	while(iCount > 0)
	{
		//데이터 받기
		iReturnValue = recv(_pSock, _szBuffer, LENGTH, _iFlag);

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

UINT WINAPI CommunicateThread( void* arg )
{
	SOCKET pClientSock = (SOCKET)arg;
	char szBuffer[BUFSIZE * 4];
	SOCKADDR_IN tClientAddress;
	int iAddrLength = 0;
	int iReturnValue = 0;
	ZeroMemory(&tClientAddress, sizeof(tClientAddress));
	ZeroMemory(szBuffer, BUFSIZE * 4);

	iAddrLength = sizeof(tClientAddress);
	getpeername(pClientSock, (SOCKADDR*)&tClientAddress, &iAddrLength);

	while(true)
	{
		int iLength = 0;
		bool bReturn = false;

		//데이터 받기
		bReturn = PopReceiveDataLength((char*)&iLength, pClientSock, 0);
		if(bReturn == false)
		{
			return 0;
		}

		//	널문자 삭제.
		iLength -= 1;

		bReturn = ReceiveData(iLength, szBuffer, pClientSock, 0);
		if(bReturn == false)
		{
			return 0;
		}

		// 받은 데이터 출력
		szBuffer[iLength] = '\0';
		printf("[TCP/%s:%d] %s\n", inet_ntoa(tClientAddress.sin_addr),
			ntohs(tClientAddress.sin_port), szBuffer);

		// 데이터 보내기
		iReturnValue = send(pClientSock, szBuffer, iLength, 0);
		if(iReturnValue == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
	}

	// closesocket()
	closesocket(pClientSock);
	pClientSock = NULL;

	printf("[TCP서버] 클라이언트 종료: IP 주소 = %s, 포트 번호 = %d\n", 
		inet_ntoa(tClientAddress.sin_addr), ntohs(tClientAddress.sin_port));

	return 0;
}
