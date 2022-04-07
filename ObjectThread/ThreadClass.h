#pragma once

#include "stdafx.h"

class CThreadClass
{
protected:
	HANDLE	m_hThread;
	HANDLE	m_hEvent;
	DWORD	m_dwThreadID;
	bool	m_bRun;

public:
	virtual bool	CreateThread();
	virtual int		Run() PURE;
	virtual void	Destroy() PURE;
	virtual void	SetbRun(bool _bRun);

	static UINT WINAPI ThreadFunction(LPVOID _arg);

	HANDLE	GetEventHandle() { return m_hEvent; }

public:
	CThreadClass(void);
	virtual ~CThreadClass(void);
};

