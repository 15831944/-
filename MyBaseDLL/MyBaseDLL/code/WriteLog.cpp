#include "StdAfx.h"
#include "WriteLog.h"

CWriteLog::CWriteLog(void)
{

}

CWriteLog::~CWriteLog(void)
{

}

CWriteLog::CWriteLog(LPCTSTR strFile)
{
	m_strFilePath = strFile;
}

//�����ļ�·��
void CWriteLog::setPath(LPCTSTR strFile)
{
	m_strFilePath = strFile;
}
//д����
bool CWriteLog::writeLog(LPCTSTR strLog, bool bIsAddTime)
{
	if (m_strFilePath.IsEmpty())
	{
		AfxMessageBox(_T("��û��ָ��log·��"));
		return false;
	}
	CString strFile = m_strFilePath + _T("\\error.log");
	CStdioFile File;
	if(!File.Open(strFile,CFile::modeReadWrite))     
	{     
		if (!File.Open(strFile, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
		{
			return false;
		} 
	}    	
	CString strError;
	if (bIsAddTime)
	{
		CTime tCurrentTime = CTime::GetCurrentTime() ;
		strError.Format(_T("%d-%d-%d %d:%d:%d -----%s\n"), tCurrentTime.GetYear(), tCurrentTime.GetMonth(),
			tCurrentTime.GetDay(), tCurrentTime.GetHour(), tCurrentTime.GetMinute(), tCurrentTime.GetSecond(),
			strLog);
	}
	else
	{
		strError = strLog;
		strError += _T("\n");
	}
	File.SeekToEnd();
	File.WriteString(strError);
	File.Close();
	return true;
}

bool CWriteLog::writeCsvLog(LPCTSTR strLog, bool bIsAddTime /*= false*/)
{
	if (m_strFilePath.IsEmpty())
	{
		AfxMessageBox(_T("��û��ָ��CSV·��"));
		return false;
	}
	CString strFile = m_strFilePath + _T("\\error.csv");
	CStdioFile File;
	if(!File.Open(strFile,CFile::modeReadWrite))     
	{     
		if (!File.Open(strFile, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
		{
			return false;
		} 
	}    	
	CString strError;

	if (bIsAddTime)
	{
		CTime tCurrentTime = CTime::GetCurrentTime() ;
		strError.Format(_T("%s,%d-%d-%d %d:%d:%d\n"), strLog, tCurrentTime.GetYear(), tCurrentTime.GetMonth(),
			tCurrentTime.GetDay(), tCurrentTime.GetHour(), tCurrentTime.GetMinute(), tCurrentTime.GetSecond());
	}
	else
	{
		strError = strLog;
		strError += _T("\n");
	}
	File.SeekToEnd();
	File.WriteString(strError);
	File.Close();
	return true;
}
