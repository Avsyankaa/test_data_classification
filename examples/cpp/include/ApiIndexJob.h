#ifndef ApiIndexJob_H
#	define ApiIndexJob_H

#include <dthread.h>

namespace InternalApiIndexJob {

 struct CVerifyStatusWithBuffers : public dtsVerifyStatusInfo {
	CVerifyStatusWithBuffers() {
			clear();
		}
	CVerifyStatusWithBuffers& operator=(const CVerifyStatusWithBuffers& other) {
			copy(other);
			return *this;
		}
	CVerifyStatusWithBuffers& operator=(const dtsVerifyStatusInfo& other) {
			copy(other);
			return *this;
		}
	void clear() { 
			kbytesInIndex = 0;
			kwordsInIndex = 0;
			percentDone = 0;
			step = 0;
			wordsVerified = 0;
			docsVerified = 0;
			wordsToVerify = 0;
			docsToVerify = 0;
			textBuf.clear();
			text = textBuf.str();
			}
	void copy(const dtsVerifyStatusInfo& other) {
			kbytesInIndex = other.kbytesInIndex;
			kwordsInIndex = other.kwordsInIndex;
			percentDone = other.percentDone;
			step = other.step;
			wordsVerified = other.wordsVerified;
			wordsToVerify = other.wordsToVerify;
			docsVerified = other.docsVerified;
			docsToVerify = other.docsToVerify;
			textBuf = other.text;
			text = textBuf.str();
		};
	DString textBuf;
	};

struct CMergeStatusWithBuffers : public dtsMergeStatusInfo {
	CMergeStatusWithBuffers() {
			clear();
		}
	CMergeStatusWithBuffers& operator=(const CMergeStatusWithBuffers& other) {
			copy(other);
			return *this;
		}
	CMergeStatusWithBuffers& operator=(const dtsMergeStatusInfo& other) {
			copy(other);
			return *this;
		}
	void copy(const dtsMergeStatusInfo& other) {
			textBuf = other.text;
			text = textBuf.str();

			docsChecked = other.docsChecked;
			docsToMerge = other.docsToMerge;
			docsMerged = other.docsMerged;
			wordsToMerge = other.wordsToMerge;
			wordsMerged = other.wordsMerged;
			step = other.step;
			docsAdded = other.docsAdded;
			docsSkipped = other.docsSkipped;
			docsUpdated = other.docsUpdated;
			percentDone = other.percentDone;
			}
	void clear() {
			textBuf.clear();
			docsChecked = 0;
			docsToMerge = 0;
			docsMerged = 0;
			wordsToMerge = 0;
			wordsMerged = 0;
			step = 0;
			docsAdded = 0;
			docsSkipped = 0;
			docsUpdated = 0;
			percentDone = 0;
			text = textBuf.str();
		}
	DString textBuf;
};



struct CIndexProgressWithBuffers : public dtsIndexProgressInfo {
    DString indexNameBuf;
    DString indexPathBuf;
    DString driveNameBuf;
    DString file_nameBuf;
    DString file_locationBuf;
    DString file_openFailMessageBuf;
    DString file_typeBuf;

    CIndexProgressWithBuffers() {
        clear();
        }
    CIndexProgressWithBuffers(const dtsIndexProgressInfo& other) {
        copy(other);
        }
    CIndexProgressWithBuffers& operator=(const dtsIndexProgressInfo& other) {
        copy(other);
        return *this;
        }
    CIndexProgressWithBuffers& operator=(const CIndexProgressWithBuffers& other) {
        copy(other);
        return *this;
        }
    void copy(const dtsIndexProgressInfo & other) {
        indexNameBuf = other.indexName;
        indexPathBuf = other.indexPath;
        driveNameBuf = other.driveName;
        file_nameBuf = other.file.name;
        file_locationBuf = other.file.location;
        file_openFailMessageBuf = other.file.openFailMessage;
        file_typeBuf = other.file.type;

        updateType = other.updateType;
        step = other.step;
        percentDone = other.percentDone;
        bytesToIndex = other.bytesToIndex;
        filesToIndex = other.filesToIndex;
        filesToCheck = other.filesToCheck;
        filesChecked = other.filesChecked;
        docBytesReadKB = other.docBytesReadKB;
        bytesRead = other.bytesRead;
        filesRead = other.filesRead;
        docsRead = other.docsRead;
        file.size = other.file.size;
        file.bytesRead = other.file.bytesRead;
        file.percentDone = other.file.percentDone;
        file.typeId = other.file.typeId;
        docBytesMerged = other.docBytesMerged;
        currMergePercent = other.currMergePercent;
        docsInIndex = other.docsInIndex;
        wordsInIndex = other.wordsInIndex;
        elapsedSeconds = other.elapsedSeconds;
        diskAvail = other.diskAvail;
        binaryCount = other.binaryCount;
        openFailures = other.openFailures;
        compressPercentDone = other.compressPercentDone;
        encryptedCount = other.encryptedCount;
        diskAvailKB = other.diskAvailKB;
        docsRemovedFromIndex = other.docsRemovedFromIndex;
        bytesToIndex = other.bytesToIndex;
        bytesToIndexKB = other.bytesToIndexKB;
        bytesReadKB = other.bytesReadKB;
        docBytesMerged = other.docBytesMerged;
		bytesToIndex64 = other.bytesToIndex64;
		bytesRead64 = other.bytesRead64;
		docBytesRead64 = other.docBytesRead64;
		estRemaining = other.estRemaining;

        linkBuffers();

        }
    void clear() {
        indexNameBuf.clear();
        indexPathBuf.clear();
        driveNameBuf.clear();
        file_nameBuf.clear();
        file_locationBuf.clear();
        file_openFailMessageBuf.clear();
        file_typeBuf.clear();

        updateType = 0;
        step = 0;
        percentDone = 0;
        bytesToIndex = 0;
        filesToIndex = 0;
        filesToCheck = 0;
        filesChecked = 0;
        docBytesReadKB = 0;
        bytesRead = 0;
        filesRead = 0;
        docsRead = 0;
        file.size = 0;
        file.bytesRead = 0;
        file.percentDone = 0;
        file.typeId = 0;
        docBytesMerged = 0;
        currMergePercent = 0;
        docsInIndex = 0;
        wordsInIndex = 0;
        elapsedSeconds = 0;
        diskAvail = 0;
        binaryCount = 0;
        openFailures = 0;
        compressPercentDone = 0;
        encryptedCount = 0;
        diskAvailKB = 0;
        docsRemovedFromIndex = 0;
        bytesToIndex = 0;
        bytesToIndexKB = 0;
        bytesReadKB = 0;
        docBytesMerged = 0;
		bytesToIndex64 = 0;
		bytesRead64 = 0;
		docBytesRead64 = 0;
		estRemaining = 0;

        linkBuffers();

        }


    void linkBuffers() {
        indexName = indexNameBuf.str();
        indexPath = indexPathBuf.str();
        driveName = driveNameBuf.str();
        file.name = file_nameBuf.str();
        file.location = file_locationBuf.str();
        file.openFailMessage = file_openFailMessageBuf.str();
        file.type = file_typeBuf.str();
        };
    };

class CJobStatus {
    public:
        CIndexProgressWithBuffers UpdateStatus;
        CMergeStatusWithBuffers MergeStatus;
        CVerifyStatusWithBuffers VerifyStatus;
        int fUpdateStatusHasData;
        int fMergeStatusHasData;
        int fVerifyStatusHasData;
        int updateVersion;
        enum {
			NothingActive, UpdateStatusIsActive, MergeStatusIsActive, VerifyStatusIsActive
			} WhichIsActive;

        CJobStatus()
            { clear();
			}
		void clear() {
            UpdateStatus.clear();
            MergeStatus.clear();
            VerifyStatus.clear();
            fMergeStatusHasData = 0;
            fVerifyStatusHasData = 0;
            fUpdateStatusHasData = 0;
            WhichIsActive = NothingActive;
            updateVersion = 0;
            };
        void update(const dtsIndexProgressInfo& other) {
            UpdateStatus.copy(other);
            WhichIsActive = UpdateStatusIsActive;
            fUpdateStatusHasData = true;
            updateVersion++;
            }
        void update(const dtsMergeStatusInfo& other) {
            MergeStatus.copy(other);
            WhichIsActive = MergeStatusIsActive;
            fMergeStatusHasData = true;
            updateVersion++;
            }
        void update(const dtsVerifyStatusInfo& other) {
            VerifyStatus.copy(other);
            WhichIsActive = VerifyStatusIsActive;
            fVerifyStatusHasData = true;
            updateVersion++;
            }
        void copy(const CJobStatus& other, int fLazy) {
			if (fLazy && (updateVersion == other.updateVersion))
				return;
			fUpdateStatusHasData = other.fUpdateStatusHasData;
			fMergeStatusHasData = other.fMergeStatusHasData;
			fVerifyStatusHasData = other.fVerifyStatusHasData;
			WhichIsActive = other.WhichIsActive;
			if (fUpdateStatusHasData)
				UpdateStatus.copy(other.UpdateStatus);
			if (fMergeStatusHasData)
				MergeStatus.copy(other.MergeStatus);
			if (fVerifyStatusHasData)
				VerifyStatus.copy(other.VerifyStatus);
			updateVersion = other.updateVersion;
            }
	};

class CJobStatusTS : private CJobStatus {
    protected:
        DeclareSerializedObject;
    public:
    	CJobStatusTS() { }

        void clear() {
            SerializeObjectAccess
            CJobStatus::clear();
			}
        void update(const dtsIndexProgressInfo& other) {
            SerializeObjectAccess;
            CJobStatus::update(other);
            }
        void update(const dtsMergeStatusInfo& other) {
            SerializeObjectAccess;
            CJobStatus::update(other);
            }
        void update(const dtsVerifyStatusInfo& other) {
            SerializeObjectAccess;
            CJobStatus::update(other);
            }
        void copyTo(CJobStatus& other, int fLazy) {
            SerializeObjectAccess;
            other.copy(*this, fLazy);
            }
    };


class CJobQueue : public DJobBase {
	public:
		CJobQueue() :
			DJobBase(baseJob)
					{
					}
		virtual void Execute();
		void Add(DJobBase& jobToAdd) {
			jobs.append(&jobToAdd);
			jobToAdd.SetParentJob(*this);
			}
		void ClearJobs() {
			jobs.clear();
			}
	private:
		DArrayAsVector<DJobBase *> jobs;
		dtsJobBase baseJob;
	};


class CApiIndexJob : public CJobQueue {
	private:
		CJobStatusTS Status;
	public:
		DIndexJob UpdateJob;
		DIndexMergeJob MergeJob;
		DIndexVerifyJob VerifyJob;


		CApiIndexJob() {
			}
		virtual void Execute();
		virtual void handleMessage(dtsMessage& msg);
		virtual void OnIndexStatusUpdate(dtsIndexProgressInfo& ) {
				}
		virtual void OnMergeStatusUpdate(dtsMergeStatusInfo& ) {
				}
		virtual void OnVerifyStatusUpdate(dtsVerifyStatusInfo& ) {
				}
		virtual void OnCheckAbort() {
				}
				
		void GetStatus(CJobStatus& dest, int fLazy = true) {
				Status.copyTo(dest, fLazy);
			}
	};
	
/*

	Build and set up DStreamBase from the properties and document
	passed via the DataSource API.
	
	Can be used with disk files, byte arrays, or stream classes.
	
	To use, set up the properties as returned by the DataSource
	such as ModifiedDate, CreatedDate, Fields, Name, etc.
	and then call MakeInputStream.
	
	When the DataSource is passing in a Stream to use as the binary 
	document contents, create a DInputStreamBase-derived object to
	represent the Stream and call MakeInputStream() on it.
	MakeInputStream will use the passed-in object rather than
	creating a new stream and will set it up with the Filename,
	ModifiedDate, etc.


*/

class CInputStreamMaker {
	public:
		CInputStreamMaker() {
			HaveDocBytes = false;
			DocIsFile = false;
			DocBytesCount = false;
			docBytesBuffer = 0;
			bOutOfMemory = false;
			}
		DStringProperty Text;
		DStringProperty DisplayName;
		DStringProperty Name;
		bool HaveDocBytes;
		bool DocIsFile;
		size_t DocBytesCount;
		DStringSet Fields;
		dtsFileDate ModifiedDate;
		dtsFileDate CreatedDate;
		
		void SetFields(const char *s);
		void SetFields(DStringSet& s) {
			Fields.copy(s);
			}
		bool MakeInputStream(DInputStreamBase *streamToUse = 0);
		void *GetDocBytesBuffer() {
			return docBytesBuffer;
			}
		void SetDocBytes(void *p, size_t size);
		
		// Transfers ownership of the stream to the caller
		int ReturnInputStream(dtsInputStream& dest);
		
		// Access the interface while leaving ownership 
		dtsInputStream& GetInterface() {
			return *pInputStream->getInterface();
			}
		
		DInputStreamBase *Detach() {
			return pInputStream.detach();
			}
		void Clear();
		bool WasOutOfMemory() {
			return bOutOfMemory;
			}
	protected:
		dtsFileInfo FileInfo;
		void *docBytesBuffer;
		DHeapPtr<DInputStreamBase> pInputStream;
		bool bOutOfMemory;
		
		void initFileInfo();
		void logInfo();
	};		


}; // namespace ApiIndexJob


#endif
