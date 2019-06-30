
#ifndef DTHREAD_H
    #define DTHREAD_H

#ifdef __UNIX__
#include <pthread.h>
#else
#include <process.h>
#endif

#pragma warning(push)
#pragma warning(disable: 4511 4512) // copy ctor
#pragma warning(disable: 4640) // construction of local static object is not thread safe

#ifdef _WIN32
	typedef void (*DThreadFunc)(void *);
#else
	typedef void *(*DThreadFunc)(void *);
#endif

#ifdef _WIN32
	#if _MSC_VER < 1300
			#define ThreadHandleType unsigned int 
	#else
			#define ThreadHandleType uintptr_t 
	#endif
#else
		#define ThreadHandleType pthread_t 
#endif



class DThread {
	public:
		DThread() {
			m_pFunc = 0;
			m_pData = 0;
			m_thread = 0;
			}
		void setFunc(DThreadFunc func) {
			m_pFunc = func;
			}
		void setData(void *p) {
			m_pData = p;
			}
		void start();
		
		ThreadHandleType getThread() {
			return m_thread;
			}
	protected:
		void *m_pData;
		DThreadFunc m_pFunc;
		ThreadHandleType m_thread;
	};

inline void DThread::start()
{
#if defined(HAVE_WINAPI)
	#ifdef _MT
		m_thread = _beginthread(m_pFunc, 0, m_pData);
	#else
		m_thread = 0;
		m_pFunc(m_pData);
	#endif
#elif defined(__UNIX__)
	pthread_create(&m_thread, 0, m_pFunc, m_pData);
#endif
}

#ifdef _WIN32

class DCriticalSection
{
  public:
    DCriticalSection();
   ~DCriticalSection();
   int lock();
   void lock(const char *) {
	   lock();
   }
   void unlock();

    class Lock {
      public:
        Lock(DCriticalSection&);
       ~Lock();
      private:
        DCriticalSection& CritSecObj;
    };
    friend class DCriticalSection::Lock;
    static DCriticalSection &theCriticalSection();
  private:
    CRITICAL_SECTION CritSec;
    DCriticalSection(const DCriticalSection&);
    const DCriticalSection& operator =(const DCriticalSection&);
};

#define SerializeBlock          { DCriticalSection::Lock l__(DCriticalSection::theCriticalSection());
#define EndSerializedBlock      }
#define DeclareSerializedObject	DCriticalSection m_criticalSection__;
#define SerializeObjectAccess   DCriticalSection::Lock l__(m_criticalSection__);


inline DCriticalSection &DCriticalSection::theCriticalSection()
{   static DCriticalSection cs;
    return cs;
}

inline DCriticalSection::DCriticalSection()
{
  ::InitializeCriticalSection(&CritSec);
}

inline DCriticalSection::~DCriticalSection()
{
  ::DeleteCriticalSection(&CritSec);
}

inline int DCriticalSection::lock()
{
	::EnterCriticalSection(&CritSec);
		return 0;
}

inline void DCriticalSection::unlock()
{
	::LeaveCriticalSection(&CritSec);
}

inline DCriticalSection::Lock::Lock(DCriticalSection& sec)
:
  CritSecObj(sec)
{
	::EnterCriticalSection(&CritSecObj.CritSec);
}

inline DCriticalSection::Lock::~Lock()
{
  ::LeaveCriticalSection( (CRITICAL_SECTION *) &CritSecObj.CritSec);
}

#endif

#pragma warning(pop)

#endif
