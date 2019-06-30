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

#define IsFlagSet(var, EnumClass, flag)  ((var & EnumClass::flag) == EnumClass::flag)

// ApiInitializer is used to perform one-time initialization of the API, using
// a singleton object to wrap the initialization (the call to LoadEngine).
ref class ApiInitializer {
	public:
	static ApiInitializer ^instance= gcnew ApiInitializer();
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


ref class ApiObjectCounter {
	public:
		enum class ApiObjectType {
			otTypeFirst = 0,
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
			otIndexCache = 10,
			otListIndexJob = 11,
			otTypeCount = 12
			};


		static void OnCreate(ApiObjectType type)
		{	if ((type >= ApiObjectType::otTypeFirst) && (type < ApiObjectType::otTypeCount))
				count[(int) type]++;
		}
		static void OnDelete(ApiObjectType type)
		{
			if ((type >= ApiObjectType::otTypeFirst) && (type < ApiObjectType::otTypeCount))
				count[(int) type]--;
		}


		static void GetObjectCount(DString& dest) {
			static const char *names[] = {
				"WordListBuilder", "FileConverter", "IndexJob",
				"JobErrorInfo", "SearchFilter", "SearchJob",
				"SearchReportJob", "SearchResults", "Options", "Server", "IndexCache",
				"ListIndexJob", 0 };
			for (int i = 0; names[i]; ++i) {
				if (count[i])
					dest << names[i] << " " << count[i] << " ";
				}
			}

	protected:
		static array<int> ^count = gcnew array<int>(12);

	};


#define INIT			if (!ApiInitializer::Loaded()) throw("Unable to load dten600.dll");
#define CountCreate(x)  INIT;	 ApiObjectCounter::OnCreate(ApiObjectCounter::ApiObjectType::x);
#define CountDelete(x)	 ApiObjectCounter::OnDelete(ApiObjectCounter::ApiObjectType::x);



namespace dtSearch { namespace Engine {

	ref class SearchResultsItem;

	ref class Converters {
		public:
		static array<int> ^MakeNetArray(long *data, int count) ;
		static void GetNetArray(DArrayAsVector<long> *darray, array<int> ^netArray);
		static void GetNetArrayL(DArrayAsVector<long> *darray, array<long>^ netArray);
		static void GetNetArray(DArrayAsVector<unsigned long> *darray, array<int>^ netArray);
		
		//static void GetNetArray(DArrayAsVector<long> *darray, array<long> ^netArray);
		static array<String^> ^ MakeNetArray(const char *buf);
		static SearchResultsItem ^MakeResultsItem(dtsSearchResultsItem *pItem);
		static String ^MakeNetString(const char *s);
		static StringDictionary ^MakeDictionary(const char *s);
		static DateTime MakeNetDate(dtsFileDate& dtsDate);
	};



	/// <summary>
	/// Internal Base class for dtSearch.Engine objects that implement IDisposable.
	/// </summary>
	/// <remarks>
	/// DisposeUnmanagedItems() may be called from Dispose or a Finalizer.
	/// DisposeManagedItems() may be called only from Dispose.
	/// </remarks>

	public ref class DisposableObject
		: public Object, public IDisposable {
	public:
		DisposableObject() {
			m_bDisposedManaged = false;
			m_bDisposedUnmanaged = false;
			}
		// Destructor -- invoked as "Dispose"
		virtual ~DisposableObject() {
			CallDisposeManagedItems();
			CallDisposeUnmanagedItems();
			}
		// Finalizer -- *may* be called by garbage collector
		!DisposableObject() {
			CallDisposeUnmanagedItems();
		}
	protected:
		virtual void DisposeManagedItems() {
			// Override this to clean managed items
			}
		virtual void DisposeUnmanagedItems() {
			// Override this to clean unmanaged items
			}
	private:
		void CallDisposeManagedItems() {
			if (!m_bDisposedManaged) {
				m_bDisposedManaged = true;
				DisposeManagedItems();
				}
		}
		void CallDisposeUnmanagedItems() {
			if (!m_bDisposedUnmanaged) {
				m_bDisposedUnmanaged = true;
				DisposeUnmanagedItems();
			}
		}
		bool m_bDisposedManaged;
		bool m_bDisposedUnmanaged;
		};

}}
