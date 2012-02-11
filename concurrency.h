#pragma once
#ifdef _WIN32
#include <windows.h>
#else
	#include <pthread.h>
#endif
#include <iostream>

class C_Thread
{
	private:
		typedef void(*t_ThreadFunc)(void*);

		#ifdef _WIN32
			HANDLE m_Thread;
			static unsigned __stdcall M_ThreadInit(void* args);
		#else
			pthread_t m_Thread;
			static void* M_ThreadInit(void* args);
		#endif
		
		struct C_Data
		{
			C_Data() : m_Func(NULL), m_Arg(NULL) {}
			C_Data(t_ThreadFunc f, void* a) : m_Func(f), m_Arg(a) {}
			t_ThreadFunc 	m_Func;
			void*			m_Arg;
		};

		C_Data m_Data;
	public:
		 C_Thread() : m_Thread(0) {}
		 C_Thread(t_ThreadFunc f, void* args=NULL);
		 void M_Start(t_ThreadFunc f, void* args=NULL);
		 ~C_Thread();
		 void M_Join();
};

class C_Mutex
{
	friend class C_CondVar;
	private:
		#ifdef _WIN32
			CRITICAL_SECTION m_Mutex;
		#else
			pthread_mutex_t m_Mutex;
		#endif
	public:
		 C_Mutex();
		 ~C_Mutex();
		 void M_Lock();
		 void M_Unlock();
};

class C_Lock
{
	private:
		C_Mutex* m_Mutex;
	public:
		 C_Lock(C_Mutex& m) : m_Mutex(&m) {m_Mutex->M_Lock();}
		 ~C_Lock() {m_Mutex->M_Unlock();}
};

class C_CondVar
{
	private:
		#ifdef _WIN32
			CONDITION_VARIABLE m_Cond;
		#else
			pthread_cond_t m_Cond;
		#endif
		C_Mutex m_Mutex;
	public:
		 C_CondVar();
		#ifndef _WIN32
			~C_CondVar() {pthread_cond_destroy(&m_Cond);}
		#endif
		 void M_Wait();
		 void M_SignalOne();
		 void M_Signal();
};
