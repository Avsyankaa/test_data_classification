#include <stdafx.h>
#include <ApiConvertJob.h>
 
void CInternalApiConvertJob::BuildInputStream(const char *filename, const void *pData, int len)
{    m_pFileStream.dealloc();

    m_pMemoryStream = new DMemoryInputStream;
    m_pMemoryStream->setBinaryData((const char *)pData, len);
    m_pMemoryStream->setName(filename);
} 

void CInternalApiConvertJob::BuildInputStream(const char *filename)
{    m_pMemoryStream.dealloc();
    m_pFileStream = new DFileInputStream;
    m_errorText.clear();
    m_pFileStream->open(filename);
    if (!m_pFileStream->good())
        m_errorText << "Unable to open input file: " << filename << " Error = "
            << m_pFileStream->getLastError();
}

void CInternalApiConvertJob::Execute()
{    DInputStreamBase *is = 0;
    if (!m_pMemoryStream.isEmpty())
        is = m_pMemoryStream;
    else if (!m_pFileStream.isEmpty()) {
        if (!m_pFileStream->good()) {
            debugLog("Invalid file input stream", m_errorText);
            errorHandler.handleMessage(dtsErAccFile,
                m_errorText);
            return;
            }
        is = m_pFileStream;
        }
    else if (!InputText.isBlank() || InputFields.getCount()) {
        debugLog("Building input stream for InputFields and InputText only");
        BuildInputStream("", "", 0);
        is = m_pMemoryStream;
        }

    if (is) {
        is->Fields = InputFields;
        if (!InputText.isBlank()) {
            char tmp[2];
            tmp[0] = ' ';
            tmp[1] = '\0';
            is->Fields.append(tmp);
            is->Fields.append(InputText);
            }
        SetInputStream(*is->getInterface());
        }
    DFileConvertJob::Execute();
}
