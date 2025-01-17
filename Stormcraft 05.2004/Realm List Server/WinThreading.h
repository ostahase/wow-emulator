#pragma once
#include "Threading.h"

#include <windows.h>
#include <time.h>

#define CLock CWinLock
#define CSemaphore CWinSemaphore
#define CThread CWindowsThread
#define CSignal CWinSignal

class CWinSemaphore : public CPortableSemaphore
{
public:
	CRITICAL_SECTION s;
	CWinSemaphore()
	{
		InitializeCriticalSection(&s);
	}

	~CWinSemaphore()
	{
//		if (s.LockCount>=0)
//		{
//			DeleteCriticalSection(&s);
//		}
		DeleteCriticalSection(&s);
	};
};

class CWinLock : public CPortableLock
{
public:
	CWinLock()
	{
		s=NULL;
		Locked=false;
	};
	CWinLock(CWinSemaphore* sem,bool bLocked=true)
	{
		s=&sem->s;
		if (bLocked && s)
		{
			EnterCriticalSection(s);
			Locked=true;
		}
		else
			Locked=false;

	};
	~CWinLock()
	{
		Unlock();
	};

	void Lock()
	{
		if (s)
		{
			EnterCriticalSection(s);
			Locked=true;
		}
	};

	void Unlock()
	{
		if (Locked)
		{
			LeaveCriticalSection(s);
			Locked=false;
		}
	};
	bool IsLocked()
	{
		return Locked;
	};
	void SetSemaphore(CPortableSemaphore* sem)
	{
		s=&((CWinSemaphore*)sem)->s;
	};
private:
	CRITICAL_SECTION *s;
	bool Locked;

};

class CWinSignal : public CPortableSignal
{
public:
	CWinSignal(void)
	{
		InitializeCriticalSection(&waiting);
		InitializeCriticalSection(&signaling);
	}
	~CWinSignal(void)
	{
		DeleteCriticalSection(&waiting);
		DeleteCriticalSection(&signaling);
	}

	void Wait(unsigned long Timeout, CPortableLock* unlockme=NULL)
	{
		EnterCriticalSection(&waiting);
		if (unlockme)
			((CWinLock*)unlockme)->Unlock();
		time_t start=time(0);
		int scount=0;
		{
			while(signaling.OwningThread==0)
			{
				if ((unsigned long)(time(0)-start)>Timeout)
				{
					LeaveCriticalSection(&waiting);
					return;
				}
				if (scount++>5)
				{
					scount=0;
					Sleep(1);
				}
			}
		}
	}


	void Wait(CPortableLock* unlockme=NULL)
	{
		EnterCriticalSection(&waiting);
		if (unlockme)
			((CWinLock*)unlockme)->Unlock();
		time_t start=time(0);
		int scount=0;
		{
			while(signaling.OwningThread==0)
			{
				/*
				if (time(0)-start>Timeout)
				{
					LeaveCriticalSection(&waiting);
					return;
				}
				*/
				if (scount++>5)
				{
					scount=0;
					Sleep(1);
				}
			}
		}
	}



	void Release()
	{
		LeaveCriticalSection(&waiting);
	}

	void Signal(bool* WhileTrue=0)
	{
		EnterCriticalSection(&signaling);
		int scount=0;
		while(waiting.OwningThread==0 && (!WhileTrue || *WhileTrue))
		{
			if (scount++>5)
			{
				scount=0;
				Sleep(1);
			}
		}
		EnterCriticalSection(&waiting);
		LeaveCriticalSection(&signaling);
		LeaveCriticalSection(&waiting);
	}
private:
	CRITICAL_SECTION waiting;
	CRITICAL_SECTION signaling;
	bool bWaiting;
};

static DWORD CWindowsThreadFunction(void *ThreadInstance);

class CWindowsThread : public CPortableThread
{
public:
	CWinSemaphore Threading;
	CWindowsThread()
	{
		ThreadReady = false;
		bThreading = false;
		CloseThread=false;
	};
	~CWindowsThread()
	{
		if (bThreading)
			EndThread();
	};

	void BeginThread(PTHREAD_START_ROUTINE function, void* pInfo)
	{
		if (bThreading)
			return;
		Info=pInfo;
		ThreadReady=false;
		CloseThread=false;
		DWORD ThreadID;
		CreateThread(NULL,0,(PTHREAD_START_ROUTINE)function,this,0,&ThreadID);
		while(!ThreadReady)
		{
			Sleep(0);
		}
	};

	void EndThread()
	{
		CloseThread=true;
		CWinLock L(&Threading,true);
	};
};