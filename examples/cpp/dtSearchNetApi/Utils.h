#pragma once

using namespace System;
using namespace System::Collections::Specialized;

#ifdef _DEBUG
#define Trace(x)   OutputDebugString(x); 
#else
#	define Trace(x) OutputDebugString(x);
#endif


bool LoadEngine();
bool LoadEngineFromTypelib();
void TriggerDelayLoad();


// ApiInitializer is used to perform one-time initialization of the API, using 
// a singleton object to wrap the initialization (the call to LoadEngine).
private __gc class ApiInitializer {
	public:
	static ApiInitializer *instance=new ApiInitializer();
	static bool bOK = false;

	static ApiInitializer()
	{	bOK = LoadEngine();
	}

	ApiInitializer()
	{	bOK = LoadEngine();
	}
	
	static  bool Loaded() {
		return bOK;
		}

	};

		
__gc class ApiObjectCounter {
	public:	
		__value enum ApiObjectType {
			otWordListBuilder = 0,
			otFileConverter = 1, 
			otIndexJob = 2,
			otJobErrorInfo = 3,
			otSearchFilter = 4,
			otSearchJob = 5, 
			otSearchReportJob = 6,
			otSearchResults = 7,
			otOptions = 8,
			otServer = 9,
			otTypeCount = 10
			};
		ApiObjectCounter()
		{
			for (int i = 0; i < otTypeCount; ++i)
				count[i] = 0;
		}


		static void OnCreate(ApiObjectType type)
		{	if ((type >= 0) && (type < otTypeCount))
				instance->count[type]++;
		}
		static void OnDelete(ApiObjectType type)
		{
			if ((type >= 0) && (type < otTypeCount))
				instance->count[type]--;
		}


		static void GetObjectCount(DString& dest) {
			static const char *names[] = {
				"WordListBuilder", "FileConverter", "IndexJob", 
				"JobErrorInfo", "SearchFilter", "SearchJob", 
				"SearchReportJob", "SearchResults", "Options", "Server", 0 };
			for (int i = 0; names[i]; ++i) {
				if (instance->count[i])
					dest << names[i] << " " << instance->count[i] << " ";
				}
			}

	protected:
		static ApiObjectCounter *instance = new ApiObjectCounter();

		LONG count __nogc[otTypeCount];
	
	};		
		

#define INIT			if (!ApiInitializer::Loaded()) throw("Unable to load dten600.dll");
#define CountCreate(x)  INIT;	 ApiObjectCounter::OnCreate(ApiObjectCounter::ApiObjectType::x);
#define CountDelete(x)	 ApiObjectCounter::OnDelete(ApiObjectCounter::ApiObjectType::x);



namespace dtSearch { namespace Engine {

	public __gc class SearchResultsItem;

	private __gc class Converters {
		public:
		static int MakeNetArray(long *data, int count) __gc[];
		static void GetNetArray(DArrayAsVector<long> *darray, int netArray __gc[]);
		static String *MakeNetArray(const char *buf) __gc[];
		static SearchResultsItem *MakeResultsItem(dtsSearchResultsItem *pItem);
		static String *MakeNetString(const char *s);
		static StringDictionary *MakeDictionary(const char *s);
		static DateTime MakeNetDate(dtsFileDate& dtsDate);
	};
	


	/// <summary>
	/// Internal Base class for dtSearch.Engine objects that implement IDisposable.
	/// </summary>
	/// <exclude/>
	public __gc class DisposableObject
		: public Object, public IDisposable {
	public:
		DisposableObject() {
			m_bDisposed = false;
			m_bDisposedManaged = false;
			m_bDisposedUnmanaged = false;
			}
		void Dispose() {
			if (!m_bDisposed) {
				m_bDisposed = true;
				Dispose(true);
				GC::SuppressFinalize(this);
				}
			}
		virtual ~DisposableObject() {
			Dispose(false);
			}
	protected:
		// Dispose is called from the Finalizer with "false"
		// and can be called from user code with "true"
		// When called from the finalizer, do not
		// dispose managed resources
		virtual void Dispose(bool notInFinalizer) {
			if (notInFinalizer) {
				if (!m_bDisposedManaged) {
					m_bDisposedManaged = true;
					DisposeManagedItems();
					}
				}
			if (!m_bDisposedUnmanaged) {
				m_bDisposedUnmanaged = true;
				DisposeUnmanagedItems();
				}
			}
		virtual void DisposeManagedItems() {
			// Override this to clean managed items
			}
		virtual void DisposeUnmanagedItems() {
			// Override this to clean unmanaged items
			}
	private:
		bool m_bDisposed;
		bool m_bDisposedManaged;
		bool m_bDisposedUnmanaged;
		};

}}
