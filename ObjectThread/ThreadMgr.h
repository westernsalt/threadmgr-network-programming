#pragma once
#include "stdafx.h"

class CAnotherThreadClass;

class CThreadMgr
{
	std::vector<CAnotherThreadClass*>	m_vecThread;

public:	
	bool	CreateThread();
	void	DestroyThread();
public:
	CThreadMgr(void);
	~CThreadMgr(void);
};

