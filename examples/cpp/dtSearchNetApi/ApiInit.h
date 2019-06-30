#pragma once

namespace dtSearch {

	class ApiInit {
		public:
			enum ObjectType {
				WordListBuilder = 0,
				FileConverter = 1, 
				IndexJob = 2,
				JobErrorInfo = 3,
				SearchFilter = 4,
				SearchJob = 5, 
				SearchReportJob = 6,
				SearchResults = 7,
				TypeCount = 8
				};
			ApiObject(ObjectType t) {
				FirstInit();
				InterlockedIncrement(&allocatedObjectCount[t]);
				m_type = t;
				}
			~ApiObject() {
				InterlockedDecrement(&allocatedObjectCount[m_type]);
				}		
			static void GetObjectCount(DString& dest) {
				static const char *names[] = {
					"WordListBuilder", "FileConverter", "IndexJob", 
					"JobErrorInfo", "SearchFilter", "SearchJob", 
					"SearchReportJob", "SearchResults", 0 };
				for (int i = 0; names[i]; ++i) {
					if (allocatedObjectCount[i])
						dest << names[i] << " " << allocatedObjectCount[i] << " ";
					}
				}
		protected:
			ObjectType m_type;
			static LONG allocatedObjectCount[TypeCount];
			static void FirstInit();
		};

};
