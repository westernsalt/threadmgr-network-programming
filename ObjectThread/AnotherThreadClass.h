#pragma once

#include "stdafx.h"
#include "ThreadClass.h"

class CAnotherThreadClass : public CThreadClass
{
	int m_iCount;

public:
	virtual int		Run();
	virtual void	Destroy();

	static UINT WINAPI ThreadFunction(LPVOID _arg);

public:
	CAnotherThreadClass(void);
	~CAnotherThreadClass(void);
};

