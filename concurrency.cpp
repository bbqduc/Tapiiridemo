#include "concurrency.h"
#include <stdexcept>
#ifdef _WIN32
	#include <process.h>
#endif

namespace pwskoag
{
	#ifdef _WIN32
		unsigned __stdcall C_Thread::M_ThreadInit(void* args)
		{
			C_Thread::C_Data* d=(C_Thread::C_Data*)args;
			d->m_Func(d->m_Arg);
			return 0;
		}
		C_Thread::C_Thread(t_ThreadFunc f, void* args) : m_Thread(0), m_Data(C_Data(f, args))
		{
			m_Thread=(HANDLE)_beginthreadex(NULL,0,M_ThreadInit,&m_Data,0,NULL);
			if(!m_Thread)
			{
				throw std::runtime_error("Error creating a thread.");
			}
		}
	#else
		void* C_Thread::M_ThreadInit(void* args)
		{
			C_Thread::C_Data* d=(C_Thread::C_Data*)args;
			d->m_Func(d->m_Arg);
			return NULL;
		}
		C_Thread::C_Thread(t_ThreadFunc f, void* args) : m_Thread(0), m_Data(C_Data(f, args))
		{
			int ret=pthread_create(&m_Thread, NULL, M_ThreadInit, &m_Data);
			if(ret)
			{
				throw std::runtime_error("Error creating a thread.");
			}
		}
		void C_Thread::M_Start(t_ThreadFunc f, void* args)
		{
			m_Thread=0;
			m_Data=C_Data(f, args);
			int ret=pthread_create(&m_Thread, NULL, M_ThreadInit, &m_Data);
			if(ret)
			{
				throw std::runtime_error("Error creating a thread.");
			}
		}
	#endif

	void C_Thread::M_Join()
	{
		#ifdef _WIN32
			WaitForSingleObject(m_Thread, INFINITE);
		#else
			pthread_join(m_Thread, NULL);
		#endif
	}
	C_Thread::~C_Thread()
	{
		#ifdef _WIN32
			if(m_Thread)
			{
				M_Join();
				CloseHandle(m_Thread);
				m_Thread=NULL;
			}
		#else
			if(m_Thread)
			{
				M_Join();
				pthread_detach(m_Thread);
				m_Thread=0;
			}
		#endif
	}

	C_Mutex::C_Mutex()
	{
		#ifdef _WIN32
			InitializeCriticalSection(&m_Mutex);
		#else
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&m_Mutex, &attr);
			pthread_mutexattr_destroy(&attr);
		#endif
	}
	C_Mutex::~C_Mutex()
	{
		#ifdef _WIN32
			DeleteCriticalSection(&m_Mutex);
		#else
			pthread_mutex_destroy(&m_Mutex);
		#endif
	}
	void C_Mutex::M_Lock()
	{
		#ifdef _WIN32
			EnterCriticalSection(&m_Mutex);
		#else
			pthread_mutex_lock(&m_Mutex);
		#endif
	}
	void C_Mutex::M_Unlock()
	{
		#ifdef _WIN32
			LeaveCriticalSection(&m_Mutex);
		#else
			pthread_mutex_unlock(&m_Mutex);
		#endif
	}

	C_CondVar::C_CondVar()
	{
		#ifdef _WIN32
			InitializeConditionVariable(&m_Cond);
		#else
			pthread_cond_init(&m_Cond,NULL);
		#endif
	}
	void C_CondVar::M_Wait()
	{
		#ifdef _WIN32
			m_Mutex.M_Lock();
			SleepConditionVariableCS(&m_Cond, &m_Mutex.m_Mutex, INFINITE);
			m_Mutex.M_Unlock();
		#else
			m_Mutex.M_Lock();
			pthread_cond_wait(&m_Cond, &m_Mutex.m_Mutex);
			m_Mutex.M_Unlock();
		#endif
	}
	void C_CondVar::M_SignalOne()
	{
		#ifdef _WIN32
			WakeConditionVariable(&m_Cond);
		#else
			pthread_cond_signal(&m_Cond);
		#endif
	}
	void C_CondVar::M_Signal()
	{
		#ifdef _WIN32
			WakeAllConditionVariable(&m_Cond);
		#else
			pthread_cond_broadcast(&m_Cond);
		#endif
	}
}
