#include "stdafx.h" 
#include "MyExcel.h" 
#include "DlgTKSZ.h"
#include "CNameCell.h"
#include "CCommonCell.h"
#include "CDrawEnt.h"
#include "Utility.h"
#include "CSheetInfo.h"
#include "CLrmvSheetInfo.h"
#include "DlgProress.h"
//#include "CreateBlkRec.h"
#include "BaseUtils.h"
#include "CLRMVSheet.h"
#include "CDrawLrmvEnt.h"
#include "CDigitalOpticalSheet.h"
#include "CDrawDigitalOptical.h"
#include "CGlobalQuery.h"
#include "CGlobalRWriteFile.h"
#include "CLrmSheetInfo.h"
#include "GLData.h"
#include "CLrmvpSheetInfo.h"

extern CDlgProress *modelessDlg;

//ȫ�ֱ������ڳ�ʼ��ʱ��Ϊ�գ�
CGlobalQuery gQuery;
extern CGlobalRWriteFile gRWriteFile;
CMyExcel::CMyExcel() 
{ 
	strFilePath=_T(""); 
	m_MapData.clear();
	m_datainfo.clear();
	m_nameVec.clear();
	m_modNameVec.clear();

	m_ExcelVec.clear();

	m_blkNameVec.clear();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
} 
CString CMyExcel::GetAppPath() 
{ 
	TCHAR lpFileName[MAX_PATH]; 
	GetModuleFileName(AfxGetInstanceHandle(),lpFileName,MAX_PATH); 

	CString strFileName = lpFileName; 
	int nIndex = strFileName.ReverseFind ('\\'); 

	CString strPath; 

	if (nIndex > 0) 
		strPath = strFileName.Left (nIndex); 
	else
		strPath = ""; 
	return strPath; 
} 

void CMyExcel::Clear()
{
	COleVariant covFalse((short)FALSE); 
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);  
	MyRange.ReleaseDispatch(); 
	MySheet.ReleaseDispatch(); 
	MySheets.ReleaseDispatch(); 
	MyBook.Close(covFalse,_variant_t(strFilePath),covOptional); 
	MyBook.ReleaseDispatch(); 
	MyBooks.Close(); 
	MyBooks.ReleaseDispatch(); 
	MyApp.Quit(); 
	MyApp.ReleaseDispatch(); 
	CoUninitialize(); 

	m_MapData.clear();
	m_datainfo.clear();
	m_nameVec.clear();
	m_modNameVec.clear();

	m_ExcelVec.clear();

	m_blkNameVec.clear();
}

bool CMyExcel::verifyData()
{
	CString strItem;
	strItem = GetItemText(iStartRow, 2);
	CString strLog;		
	CWriteLog log;
	
	if (strItem.IsEmpty())
	{
		log.setPath(CUtility::getLogPath());
		strLog.Format(_T("��������Ϊ��------------sheetName = %s,��%d��,��%d��"), MySheet.GetName(), iStartRow, 2);
		log.writeCsvLog(strLog);
		gQuery.setFatalErrorFlag();
	}
	strItem = GetItemText(iStartRow+1,2);
	if (strItem.IsEmpty())
	{
		log.setPath(CUtility::getLogPath());
		strLog.Format(_T("��������Ϊ��------------sheetName = %s,��%d��,��%d��"), MySheet.GetName(), iStartRow+1, 2);
		log.writeCsvLog(strLog);
		gQuery.setFatalErrorFlag();
	}

	return true;
}

void CMyExcel::readSpecailCol()
{
	//��֤����
	verifyData();
	
	CString strItem, strNumber;
	strNumber = GetItemText(2,2);
	int nLen = MyParserString::ConvertLetterToNumble(strNumber);
	if (nLen <= 0)
	{
		return;
	}
	
	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	//m_nameVec.push_back(strSheetName);
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	gQuery.addSheetNameItem(strSheetName);
	strItem = GetItemText(1,2);
	if (strItem.CompareNoCase(_T("LRMV")) == 0)//���������VPX
	{
		/*if (nLen < 8)
		{
			CString strLog;		
			CWriteLog log;
			log.setPath(CUtility::getLogPath());
			strLog.Format(_T("��������%s����̫С----------- = %s,��%d��,��%d��"), strNumber, 2, 2);
			log.writeCsvLog(strLog);
		}
		else*/
		{
			readLrmvdata(nLen);
		}
		
		return;
	}

	if ((strItem.CompareNoCase(_T("���ֹ�")) == 0))//��������ֹ�
	{
		if (nLen < 2)
		{
			CString strLog;		
			CWriteLog log;
			log.setPath(CUtility::getLogPath());
			strLog.Format(_T("��������%s����̫С----------- = %s,��%d��,��%d��"), strNumber, strSheetName, 2, 2);
			log.writeCsvLog(strLog);
		}
		else
		{
			readDigitalData(nLen);
		}
		return;
	}

	if (strItem.CompareNoCase(_T("ģ���")) == 0)//�����ģ���
	{
		if (nLen < 2)
		{
			CString strLog;		
			CWriteLog log;
			log.setPath(CUtility::getLogPath());
			strLog.Format(_T("��������%s����̫С----------- = %s,��%d��,��%d��"), strNumber, strSheetName, 2, 2);
			log.writeCsvLog(strLog);
		}
		else
		{
			readDigitalData(nLen, 1);
		}
		return;
	}
	if (strItem.CompareNoCase(_T("LRMV-P")) == 0)//����ǲ�ͷ���ݣ�ֱ�ӷ���
	{
		return;
	}
	int nFind = strItem.Find(_T("[ģ��]"));
	bool bIsMod = false;
	if (nFind >=0)
	{
		strItem.Replace(_T("[ģ��]"), _T(""));
		m_modNameVec.push_back(strSheetName);
		bIsMod = true;
	}

	if ((strItem.CompareNoCase(_T("��ͨ")) == 0))//�������ͨ
	{
		if (nLen < 2)
		{
			CString strLog;		
			CWriteLog log;
			log.setPath(CUtility::getLogPath());
			strLog.Format(_T("��������%s����̫С----------- = %s,��%d��,��%d��"), strNumber, strSheetName, 2, 2);
			log.writeCsvLog(strLog);
		}
		else
		{
			readCommonData(nLen);
		}
		return;
	}

	if (nLen < 2)
	{
		CString strLog;		
		CWriteLog log;
		log.setPath(CUtility::getLogPath());
		strLog.Format(_T("��������%s����̫С----------- = %s,��%d��,��%d��"), strNumber, strSheetName, 2, 2);
		log.writeCsvLog(strLog);
	}
	else
	{
		readLrmCommonData(nLen);
	}

	return;
}


void CMyExcel::readLrmvpCol()
{
	//��֤����
	verifyData();

	CString strItem, strNumber;
	strNumber = GetItemText(2,2);
	int nLen = MyParserString::ConvertLetterToNumble(strNumber);
	if (nLen <= 0)
	{
		return;
	}

	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	//m_nameVec.push_back(strSheetName);
	gQuery.addSheetNameItem(strSheetName);
	strItem = GetItemText(1,2);
	if (strItem.CompareNoCase(_T("LRMV-P")) == 0)//�����LRMV-P
	{
		if (nLen < 2)
		{
			CString strLog;		
			CWriteLog log;
			log.setPath(CUtility::getLogPath());
			strLog.Format(_T("��������%s����̫С----------- = %s,��%d��,��%d��"), strNumber, strSheetName, 2, 2);
			log.writeCsvLog(strLog);
		}
		else
		{
			readLrmvpData(nLen);
		}
	}
	return;
}

void CMyExcel::readLrmvCol()
{
	//��֤����
	verifyData();

	CString strItem, strNumber;
	strNumber = GetItemText(2,2);
	int nLen = MyParserString::ConvertLetterToNumble(strNumber);
	if (nLen <= 0)
	{
		return;
	}

	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	//m_nameVec.push_back(strSheetName);
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	gQuery.addSheetNameItem(strSheetName);
	strItem = GetItemText(1,2);
	if (strItem.CompareNoCase(_T("LRMV")) == 0)//�����LRMV
	{
		if (nLen < 2)
		{
			CString strLog;		
			CWriteLog log;
			log.setPath(CUtility::getLogPath());
			strLog.Format(_T("��������%s����̫С----------- = %s,��%d��,��%d��"), strNumber, strSheetName, 2, 2);
			log.writeCsvLog(strLog);
		}
		else
		{
			readLrmvData(nLen);
		}
	}
	return;
}



bool CMyExcel::verifyItemLength(CString strItem)
{
	CString strTmp = MyParserString::RemoveSymbol(_T("]"), strItem);
	int nLen = strTmp.GetLength();
	int nCurLen = gGlobal.GetIniValue(_T("��֤����"), _T("�ַ�����"), 0);
	if (nLen > nCurLen)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CMyExcel::readLrmvdata(int nLen)
{
	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	CString strItem;
	vector<vector<CString> > vData(iRowNum, vector<CString>(nLen));
	CNameCell nameCell;
	m_MapData.clear();
	vData = doNameInfo(nLen);

	
	CLRMVSheet sInfo;
	sInfo.setData(vData, m_MapData, strSheetName, nLen, iRowNum);
	sInfo.doIt();
	//����Ϣ�洢��ȫ��������
	gQuery.addExcelInfo(strSheetName, vData);

	vData.clear();
	m_MapData.clear();

	CSheetStoreData storeData = sInfo.getData();
	gQuery.addLrmvSheetData(storeData);

	gQuery.addLrmvModSheetData(sInfo.getModData());
}

void CMyExcel::writeFile()
{
	if (m_ExcelVec.size() > 0)
	{
		CString strPreFileName = gGlobal.GetIniValue(_T("ͼ������"), _T("�����ļ�·��"));
		strPreFileName = MyOperateFile::GetFileNameByPath(strPreFileName);
		CString strLrmvFile = CUtility::getFilePath() + strPreFileName + _T("---ͷ������.xlsx");
		if (gGlobal.IsFileExist(strLrmvFile))
		{
			::DeleteFile(strLrmvFile);
		}
		int k=0;
		CMyExcel myExcel;
		if (!myExcel.Open())
		{
			return;
		}
		myExcel.SaveAs(strLrmvFile);

		for (vector<pair<CString, vector<vector<CString> > > >::iterator iter = m_ExcelVec.begin(); iter != m_ExcelVec.end(); ++iter)
		{
			CString strFile = iter->first;
			vector<vector<CString> > tmpData = iter->second;
			myExcel.OpenSheet(strFile, false);
			int i=1;
			for (vector<vector<CString> >::iterator iter = tmpData.begin(); iter != tmpData.end(); ++iter)
			{
				vector<CString> tmpVec = *iter;
				int j=1;
				for (vector<CString>::iterator ir = tmpVec.begin(); ir	!= tmpVec.end(); ++ir)
				{
					myExcel.SetItemText(i, j, *ir);
					j++;
				}
				tmpVec.clear();
				i++;
			}
			myExcel.Save();			
			k++;
			tmpData.clear();
		}
		myExcel.Clear();
		acutPrintf(_T("\n�ļ���������%s"), strLrmvFile);
	}
}

void CMyExcel::writeLrmvFile()
{
	if (m_ExcelVec.size() > 0)
	{
		CString strPreFileName = gGlobal.GetIniValue(_T("ͼ������"), _T("ͷ���ļ�·��"));
		strPreFileName = MyOperateFile::GetFileNameByPath(strPreFileName);
		CString strLrmvFile = CUtility::getFilePath() + strPreFileName + _T("---��������.xlsx");
		if (gGlobal.IsFileExist(strLrmvFile))
		{
			::DeleteFile(strLrmvFile);
		}
		int k=0;
		CMyExcel myExcel;
		if (!myExcel.Open())
		{
			return;
		}
		myExcel.SaveAs(strLrmvFile);

		for (vector<pair<CString, vector<vector<CString> > > >::iterator iter = m_ExcelVec.begin(); iter != m_ExcelVec.end(); ++iter)
		{
			CString strFile = iter->first;
			vector<vector<CString> > tmpData = iter->second;
			myExcel.OpenSheet(strFile, false);
			int i=1;
			for (vector<vector<CString> >::iterator iter = tmpData.begin(); iter != tmpData.end(); ++iter)
			{
				vector<CString> tmpVec = *iter;
				int j=1;
				for (vector<CString>::iterator ir = tmpVec.begin(); ir	!= tmpVec.end(); ++ir)
				{
					myExcel.SetItemText(i, j, *ir);
					j++;
				}
				tmpVec.clear();
				i++;
			}
			tmpData.clear();
			myExcel.Save();			
			k++;
		}
		myExcel.Clear();
		acutPrintf(_T("\n�ļ���������%s"), strLrmvFile);
	}
}

map<CString, vector<vector<CString> > > CMyExcel::ReadAllData()
{
	map<CString, vector<vector<CString> > > mapInfo;
	mapInfo.clear();
	pair<CString, vector<vector<CString> > > tmpPair;
	int nCount = GetSheetCount();
	CString strItem, strName;
	for (int i=1; i<=nCount; i++)
	{
		OpenSheet(i);
		GetArea();
		strName = MySheet.GetName();
		strName.Trim();
		strName = CUtility::RemoveString(strName, ICD_PROPERTY_CH);
		vector<vector<CString> > vData(iRowNum+iStartRow-1, vector<CString>(iColNum+iStartCol-1));
		for (int i=1; i<iStartRow + iRowNum; i++)
		{
			for (int j=1; j<=iColNum; j++)
			{
				strItem = GetItemText(i, j);
				vData[i-1][j-1] = strItem;
			}
		}
		tmpPair.first = strName;
		tmpPair.second = vData;
		mapInfo.insert(tmpPair);
		vData.clear();
	}
	return mapInfo;
}

vector<vector<CString> > CMyExcel::ReadActiveSheetData(CString& strSheetName)
{
	CString strItem, strName;
	OpenSheet();
	GetArea();
	
	strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	vector<vector<CString> > vData(iRowNum+iStartRow-1, vector<CString>(iColNum+iStartCol-1));
	for (int i=1; i<iStartRow + iRowNum; i++)
	{
		for (int j=1; j<=iColNum; j++)
		{
			strItem = GetItemText(i, j);
			vData[i-1][j-1] = strItem;
		}
	}
		
	return vData;
}

void CMyExcel::readDigitalData(int nLen, int nType)
{
	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	CString strItem;
	vector<vector<CString> > vData(iRowNum, vector<CString>(nLen));
	CNameCell nameCell;
	m_MapData.clear();
	vData = doNameInfo(nLen);
	
	CDigitalOpticalSheet sInfo;
	sInfo.setData(vData, m_MapData, strSheetName, nLen, iRowNum, nType);
	sInfo.doIt();

	gQuery.addExcelInfo(strSheetName, vData);

	vData.clear();
	m_MapData.clear();

	CSheetStoreData storeData = sInfo.getData();
	gQuery.addDigitalSheetData(storeData);
}

void CMyExcel::readCommonData(int nLen)
{
	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	CString strItem;
	vector<vector<CString> > vData(iRowNum, vector<CString>(nLen));
	vData = doNameInfo(nLen);

	CSheetInfo sInfo;
	sInfo.setData(vData, m_MapData, m_nItem, strSheetName, nLen, iRowNum);
	sInfo.doIt();

	gQuery.addExcelInfo(strSheetName, vData);

	vData.clear();
	m_MapData.clear();

	CSingleSheetData singleData = sInfo.getdata();

	//m_datainfo.insert(make_pair(strSheetName, singleData));
	gQuery.addCommonSheetData(strSheetName, singleData);

}

void CMyExcel::readLrmCommonData(int nLen)
{
	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	CString strItem;
	vector<vector<CString> > vData(iRowNum, vector<CString>(nLen));
	vData = doNameInfo(nLen);

	CLrmSheetInfo sInfo;
	sInfo.setData(vData, m_MapData, m_nItem, strSheetName, nLen, iRowNum);
	sInfo.doIt();
	
	gQuery.addExcelInfo(strSheetName, vData);

	vData.clear();
	m_MapData.clear();

	CSheetStoreData singleData = sInfo.getData();
	
	gQuery.addLrmvSheetData(singleData);
	if (sInfo.IsMod())
	{
		gQuery.addLrmvModSheetData(sInfo.getModData());
	}
}

void CMyExcel::readLrmvpData(int nLen)
{
	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	CString strItem;
	vector<vector<CString> > vData(iRowNum, vector<CString>(nLen));
	vData = doNameInfo(nLen);

	CLrmvpSheetInfo sInfo;
	sInfo.setData(vData, m_MapData, m_nItem, strSheetName, nLen, iRowNum);
	sInfo.doIt();

	vData.clear();
	m_MapData.clear();

	pair<CString, vector<vector<CString> > > tmpPair;
	tmpPair.first = sInfo.getSheetName();
	tmpPair.second = sInfo.getModData();
	m_ExcelVec.push_back(tmpPair);
}

void CMyExcel::readLrmvData(int nLen)
{
	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);
	CString strItem;
	vector<vector<CString> > vData(iRowNum, vector<CString>(nLen));
	vData = doNameInfo(nLen);

	CLRMVSheet sInfo;
	sInfo.setData(vData, m_MapData, strSheetName, nLen, iRowNum);
	sInfo.doIt();

	vData.clear();
	m_MapData.clear();

	pair<CString, vector<vector<CString> > > tmpPair;
	tmpPair.first = sInfo.getSheetName();
	tmpPair.second = sInfo.getModExcelData();
	m_ExcelVec.push_back(tmpPair);
}

CMyExcel::~CMyExcel() 
{ 
	strFilePath=_T(""); 
	m_MapData.clear();
	m_datainfo.clear();
	m_nameVec.clear();
	m_modNameVec.clear();

	m_ExcelVec.clear();
} 
BOOL CMyExcel::Open() 
{ 
	LPDISPATCH lpDisp=NULL; 
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);    
	CoInitialize(NULL); 
	if (!MyApp.CreateDispatch(_T("Excel.Application"),NULL)) 
	{ 
		AfxMessageBox(_T("EXCEL��ʼ��ʱ����!"),MB_OK|MB_ICONERROR); 
		return FALSE; 
	} 
	lpDisp=MyApp.GetWorkbooks(); 
	MyBooks.AttachDispatch(lpDisp,TRUE); 
	lpDisp = MyBooks.Add(covOptional);  
	MyBook.AttachDispatch(lpDisp,TRUE); 
	lpDisp=MyBook.GetWorksheets(); 
	MySheets.AttachDispatch(lpDisp,TRUE); 
	return TRUE; 
} 
BOOL CMyExcel::Open(CString strFile) 
{ 
	LPDISPATCH   lpDisp=NULL; 
	CoInitialize(NULL); 
	if (!MyApp.CreateDispatch(_T("Excel.Application"),NULL)) 
	{ 
		AfxMessageBox(_T("EXCEL��ʼ��ʱ����!"),MB_OK|MB_ICONERROR); 
		return FALSE; 
	} 
	try
	{
		lpDisp=MyApp.GetWorkbooks(); 
		MyBooks.AttachDispatch(lpDisp,TRUE); 
		lpDisp = MyBooks.Open(strFile, vtMissing, vtMissing, vtMissing,  
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,vtMissing); 
		MyBook.AttachDispatch(lpDisp,TRUE); 
		lpDisp=MyBook.GetWorksheets();  
		MySheets.AttachDispatch(lpDisp,TRUE); 
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("�ļ���ʧ�ܣ�����δ����"));
		return FALSE;
	}
	
	strFilePath=strFile; 
	return TRUE; 
} 

int CMyExcel::GetSheetCount()
{
	long len; 
	len=MySheets.GetCount(); 
	return len;
}

BOOL CMyExcel::OpenSheet(CString strSheet, bool bIsAdded) 
{ 
	LPDISPATCH  lpDisp=NULL; 
	long len; 
	len=MySheets.GetCount(); 
	for(long i=1;i<=len;i++) 
	{ 
		lpDisp=MySheets.GetItem((_variant_t)(long)i); 
		MySheet.AttachDispatch(lpDisp,TRUE); 
		CString str=MySheet.GetName(); 
		if(MySheet.GetName()==strSheet) 
		{ 
			if (!bIsAdded)
			{
				Range range = MySheet.GetUsedRange();
				range.ClearContents();
			}
			lpDisp=MySheet.GetCells(); 
			MyRange.AttachDispatch(lpDisp,TRUE); 
			return TRUE; 
		}
		else if (strSheet.IsEmpty())
		{
			MySheet.AttachDispatch(MyApp.GetActiveSheet(), TRUE);
			lpDisp=MySheet.GetCells(); 
			MyRange.AttachDispatch(lpDisp,TRUE); 
			return TRUE;
		}
		else
		{
			if (!bIsAdded)
			{
				if (i==len)
				{
					LPDISPATCH  lpDisp=NULL; 
					lpDisp=MySheets.Add(vtMissing,vtMissing,vtMissing,vtMissing); 
					MySheet.AttachDispatch(lpDisp,TRUE); 
					MySheet.SetName(strSheet); 
					lpDisp=MySheet.GetCells(); 
					MyRange.AttachDispatch(lpDisp,TRUE); 
				}
			}
		}
	}	
	return FALSE; 
} 

bool CMyExcel::OpenSheet(int nCount)
{
	LPDISPATCH  lpDisp=NULL; 
	lpDisp=MySheets.GetItem((_variant_t)(long)nCount); 
	m_nItem = nCount;
	MySheet.AttachDispatch(lpDisp,TRUE); 
	lpDisp=MySheet.GetCells(); 
	MyRange.AttachDispatch(lpDisp,TRUE); 
	return true;
}	

BOOL CMyExcel::OpenExcelDoc()
{
	try 
	{
		LPDISPATCH   lpDisp=NULL; 
		CoInitialize(NULL); 
		if (!MyApp.CreateDispatch(_T("Excel.Application"), NULL)) 
		{ 
			AfxMessageBox(_T("EXCEL��ʼ��ʱ����!"),MB_OK|MB_ICONERROR); 
			return FALSE; 
		} 
		MyApp.SetVisible(TRUE);
		MyApp.SetUserControl(TRUE);

		COleVariant valTemp((long)DISP_E_PARAMNOTFOUND,VT_ERROR);  
		MyBooks.AttachDispatch(MyApp.GetWorkbooks(),TRUE);
		MyBook.AttachDispatch(MyBooks.Add(valTemp));  
		return TRUE;
	}
	catch (CException* e)
	{
		AfxMessageBox(_T("���ش���!"));
	/*	TCHAR   szCause[255];
		CString strFormatted;
		e->GetErrorMessage(szCause, 255);*/
		return FALSE;
	}
	
}

BOOL CMyExcel::SetItemText(long Row,long Col,CString strText) 
{ 
	long lRow=0,lCol=0; 
	/*lRow=GetRowS(); 
	lCol=GetColS(); 
	if(Row>lRow ||Col>lCol) 
	{ 
		CString strText; 
		strText.Format(_T("����(%d,%d)�Ѿ�������(%d,%d)�ķ�Χ,������(%d,%d)����ʧ��!")
			,Row,Col,lRow,lCol,Row,Col); 
		AfxMessageBox(strText,MB_OK|MB_ICONERROR); 
		return FALSE; 
	} */
	MyRange.SetItem(_variant_t(Row), _variant_t(Col), _variant_t(strText)); 
	return TRUE; 
} 
CString CMyExcel::GetItemText(long Row,long Col) 
{ 
	CString strValue=_T(""); 
	VARIANT lpDisp=MyRange.GetItem(_variant_t(Row), _variant_t(Col)); 
	Range rgRgeValue; 
	rgRgeValue.AttachDispatch(lpDisp.pdispVal, TRUE); 
	_variant_t vtVal = rgRgeValue.GetValue(); 
	if (vtVal.vt == VT_EMPTY) 
	{ 
		rgRgeValue.ReleaseDispatch(); 
		strValue = _T(""); 
		rgRgeValue.ReleaseDispatch(); 
		return strValue; 
	} 
	vtVal.ChangeType(VT_BSTR); 
	strValue= vtVal.bstrVal; 
	rgRgeValue.ReleaseDispatch(); 
	return strValue; 
} 


double CMyExcel::GetColumnWidth(long Row,long Col)
{
	double f; 
	MyRange.GetEntireColumn();
	VARIANT lpDisp=MyRange.GetItem(_variant_t(Row), _variant_t(Col)); 
	Range rgRgeValue; 
	rgRgeValue.AttachDispatch(lpDisp.pdispVal, TRUE); 
	f = (rgRgeValue.GetColumnWidth().dblVal); 
	f *= 2.2862;
	rgRgeValue.ReleaseDispatch();
	return f;
}

double CMyExcel::GetRowHeight(long Row,long Col)
{
	double f; 
	VARIANT lpDisp=MyRange.GetItem(_variant_t(Row), _variant_t(Col)); 
	Range rgRgeValue; 
	rgRgeValue.AttachDispatch(lpDisp.pdispVal, TRUE); 
	f = (rgRgeValue.GetRowHeight().dblVal); 
	f *= 0.3612;
	rgRgeValue.ReleaseDispatch();
	return f;
}

void CMyExcel::GetArea()
{
	Range usedRange,range;
	usedRange.AttachDispatch(MySheet.GetUsedRange());
	range.AttachDispatch(usedRange.GetRows());
	iRowNum = range.GetCount();                    //�Ѿ�ʹ�õ�����
	range.AttachDispatch(usedRange.GetColumns());
	iColNum = range.GetCount();                    //�Ѿ�ʹ�õ�����
	iStartRow = usedRange.GetRow();                //��ʹ���������ʼ�У���1��ʼ
	iStartCol = usedRange.GetColumn();             //��ʹ���������ʼ�У���1��ʼ
	range.ReleaseDispatch();  
	usedRange.ReleaseDispatch();  
}



void CMyExcel::SaveAs(CString strPath) 
{ 
	if(IsFileExist(strPath,FALSE)==TRUE) 
		DeleteFile(strPath); 
	MyApp.SetAlertBeforeOverwriting(FALSE);
	MyApp.SetDisplayAlerts(FALSE);
	MyBook.SaveAs(_variant_t(strPath),vtMissing,vtMissing,vtMissing,vtMissing,vtMissing 
		,0,vtMissing,vtMissing,vtMissing,vtMissing); 
	strFilePath=strPath; 
	//  AfxMessageBox(_T("Excel����ɹ�"),MB_OK|MB_ICONINFORMATION); 
} 
void CMyExcel::Save() 
{ 
	MyApp.SetAlertBeforeOverwriting(FALSE);
	MyApp.SetDisplayAlerts(FALSE);
	MyBook.Save(); 
	//  AfxMessageBox(_T("Excel����ɹ�"),MB_OK|MB_ICONINFORMATION); 
} 
void CMyExcel::AddSheet(CString strSheet) 
{ 
	LPDISPATCH  lpDisp=NULL; 
	lpDisp=MySheets.Add(vtMissing,vtMissing,vtMissing,vtMissing); 
	MySheet.AttachDispatch(lpDisp,TRUE); 
	MySheet.SetName(strSheet); 
	lpDisp=MySheet.GetCells(); 
	MyRange.AttachDispatch(lpDisp,TRUE); 
} 
void CMyExcel::GetRange(CString strBegin,CString strEnd) 
{ 
	MyRange.AttachDispatch(MySheet.GetRange(_variant_t(strBegin),_variant_t(strEnd))); 
} 

void CMyExcel::GetRangeArea(CString strBegin,CString strEnd)
{
	Range usedRange,range;
	usedRange.AttachDispatch(MySheet.GetRange(_variant_t(strBegin),_variant_t(strEnd)));
	range.AttachDispatch(usedRange.GetRows());
	iRowNum = range.GetCount();                    //�Ѿ�ʹ�õ�����
	range.AttachDispatch(usedRange.GetColumns());
	iColNum = range.GetCount();                    //�Ѿ�ʹ�õ�����
	iStartRow = usedRange.GetRow();                //��ʹ���������ʼ�У���1��ʼ
	iStartCol = usedRange.GetColumn();             //��ʹ���������ʼ�У���1��ʼ
	range.ReleaseDispatch();  
	usedRange.ReleaseDispatch();  
	//Range range;
	//range.AttachDispatch(MyRange.GetRows());	
	//iRowNum = range.GetCount();                    //�Ѿ�ʹ�õ�����
	//range.AttachDispatch(MyRange.GetColumns());	
	//iColNum = range.GetCount();                    //�Ѿ�ʹ�õ�����
	//iStartRow = MyRange.GetRow();                //��ʹ���������ʼ�У���1��ʼ
	//iStartCol = MyRange.GetColumn();             //��ʹ���������ʼ�У���1��ʼ
	//range.ReleaseDispatch();  
}

void CMyExcel::AutoColFit() 
{ 
	Range rg=MyRange.GetEntireColumn(); 
	rg.AutoFit(); 
	rg.ReleaseDispatch(); 
} 
void CMyExcel::AutoRowFit() 
{ 
	Range rg=MyRange.GetEntireRow(); 
	rg.AutoFit();    
	rg.ReleaseDispatch(); 
} 
void CMyExcel::SetWrapText(BOOL blnTrue) 
{ 
	MyRange.SetWrapText((_variant_t)(short)blnTrue); 
} 
void CMyExcel::SetVisible(BOOL blnVisible) 
{ 
	if(blnVisible==TRUE) 
		if(strFilePath!="") 
		{ 
			if(IsFileExist(strFilePath,FALSE)) 
			{ 
				Exit(); 
				ShellExecute(NULL,_T("open"),strFilePath,NULL,NULL,SW_SHOW); 
			} 
			else
			{ 
				CString strName; 
				strName=_T("·��")+strFilePath+_T("����,���ܴ���ʾ!"); 
				AfxMessageBox(strFilePath,MB_OK|MB_ICONINFORMATION); 
			} 
		} 
		else
		{ 
			AfxMessageBox(_T("���ȴ��ļ���,���ɴ�!"), MB_OK|MB_ICONINFORMATION); 
		} 
} 

void CMyExcel::AutoRange() 
{ 
	LPDISPATCH  lpDisp=NULL; 
	lpDisp=MySheet.GetCells(); 
	MyRange.AttachDispatch(lpDisp,TRUE); 
} 
void CMyExcel::SetMergeCells(BOOL blnTrue) 
{ 
	if(blnTrue==TRUE) 
	{ 
		int i,j; 
		long Row=GetRowS(); 
		long Col=GetColS(); 
		for(j=2;j<=Col;j++) SetItemText(1,j,""); 
		for(i=2;i<=Row;i++) 
			for(j=1;j<=Col;j++) 
				SetItemText(i,j,""); 
	} 
	MyRange.SetMergeCells((_variant_t)(short)blnTrue); 
} 

void CMyExcel::MergeCells(long nRow, long nCol, long nEndRow, long nEndCol)
{
	Range unionRange;
	Range range;
	range.AttachDispatch(MySheet.GetCells());
	unionRange.AttachDispatch(range.GetItem(COleVariant((long)nRow), COleVariant((long)nCol)).pdispVal);

	COleVariant vResult;
	vResult = unionRange.GetMergeCells();
	if (vResult.boolVal == -1)
	{
		range.AttachDispatch(unionRange.GetRows());
		long iUnionRowNum = range.GetCount();
		range.AttachDispatch(unionRange.GetColumns());
		long iUnionStartRow = unionRange.GetRow();
		long iUnionStartCol = unionRange.GetColumn();
	}
	else if (vResult.boolVal == 0)
	{
		range.AttachDispatch(MySheet.GetCells());
		unionRange.AttachDispatch(range.GetItem((COleVariant((long)nRow)), COleVariant((long)nCol)).pdispVal);
		//2��3��
		//unionRange.AttachDispatch(unionRange.GetResize(COleVariant((long)2), COleVariant((long)3)));
		unionRange.AttachDispatch(unionRange.GetResize(COleVariant(long(nEndRow - nRow)), COleVariant(long(nEndCol - nCol + 1))));
		unionRange.Merge(COleVariant((long)0));
		unionRange.SetHorizontalAlignment(COleVariant((short)3));
	}
}

long CMyExcel::GetRowS() 
{ 
	long lRowsNum;  
	Range range,usedRange;  
	usedRange.AttachDispatch(MySheet.GetUsedRange());  
	range.AttachDispatch(usedRange.GetRows());  
	lRowsNum = range.GetCount();  
	range.ReleaseDispatch();  
	usedRange.ReleaseDispatch();  
	return lRowsNum; 
} 
long CMyExcel::GetColS() 
{ 
	long lColNum;  
	Range range,usedRange;  
	usedRange.AttachDispatch(MySheet.GetUsedRange());  
	range.AttachDispatch(usedRange.GetColumns());  
	lColNum = range.GetCount();  
	range.ReleaseDispatch();  
	usedRange.ReleaseDispatch();  
	return lColNum;  
} 

void CMyExcel::SetColumnWidth(int intWidth) 
{ 
	double f=intWidth/8.08; 
	MyRange.SetColumnWidth((_variant_t)(double)f); 
} 
void CMyExcel::SetRowHeight(int intHeight) 
{ 
	double f=intHeight/8.08; 
	MyRange.SetRowHeight((_variant_t)(double)f); 
} 


BOOL CMyExcel::IsFileExist(CString strFn, BOOL bDir) 
{ 
	HANDLE h; 
	LPWIN32_FIND_DATA pFD=new WIN32_FIND_DATA; 
	BOOL bFound=FALSE; 
	if(pFD) 
	{ 
		h=FindFirstFile(strFn,pFD); 
		bFound=(h!=INVALID_HANDLE_VALUE); 
		if(bFound) 
		{ 
			if(bDir) 
				bFound= (pFD->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=NULL; 
			FindClose(h); 
		} 
		delete pFD; 
	} 
	return bFound; 
} 
void CMyExcel::Exit() 
{ 
	COleVariant covFalse((short)FALSE); 
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);  
	MyRange.ReleaseDispatch(); 
	MySheet.ReleaseDispatch(); 
	MySheets.ReleaseDispatch(); 
	MyBook.Close(covFalse,_variant_t(strFilePath),covOptional); 
	MyBook.ReleaseDispatch(); 
	MyBooks.Close(); 
	MyBooks.ReleaseDispatch(); 
	MyApp.Quit(); 
	MyApp.ReleaseDispatch(); 
	CoUninitialize(); 
} 

vector<vector<CString> > CMyExcel::doNameInfo(int nLen)
{
	vector<vector<CString> > vData(iRowNum, vector<CString>(nLen));
	CNameCell nameCell;
	CString strItem;
	CString strSheetName = MySheet.GetName();
	strSheetName.Trim();
	strSheetName = CUtility::RemoveString(strSheetName, ICD_PROPERTY_CH);

	for (int i=iStartRow; i<iStartRow + iRowNum; i++)
	{
		for (int j=iStartCol; j<=nLen; j++)
		{
			strItem = GetItemText(i, j);
			vData[i-1][j-1] = strItem;
			if (strItem.CompareNoCase(_T("����")) == 0)
			{
				nameCell.setItem(i);
				strItem = GetItemText(i, 2);
				if (strItem.IsEmpty())
				{
					CString strLog;		
					CWriteLog log;
					log.setPath(CUtility::getLogPath());
					strLog.Format(_T("��������Ϊ��------------sheetName = %s,��%d�У���%d��"), MySheet.GetName(), i, 2);
					log.writeLog(strLog);
				}
				else if (strItem.Find(_T("[TU]")) >= 0)
				{
					strItem.Replace(_T("[TU]"), _T(""));	
					CString strTemp = strItem;	
					CString strFile;
					int nIndex = strTemp.Find(_T("("));
					if (nIndex >= 0)
					{
						strTemp = MyParserString::GetParenthesesFromString(strTemp);
						strItem = strTemp;		
						strFile = MyBaseUtils::GetAppPath() + _T("TU\\") +strItem + _T(".dwg");
						if (!gGlobal.IsFileExist(strFile))
						{
							CString strLog;		
							CWriteLog log;
							log.setPath(CUtility::getLogPath());
							strLog.Format(_T("����:%s-��Ŀ¼%s�в�����,sheetName = %s,��%d��,��%d��"), strTemp, strFile, strSheetName, i, 2);
							log.writeCsvLog(strLog);
						}
						else
						{
							gQuery.addBlockName(strItem);	
						}
					}
					else
					{
						CString strLog;		
						CWriteLog log;
						log.setPath(CUtility::getLogPath());
						strLog.Format(_T("����:%s�����Ϲ淶,sheetName = %s,��%d��,��%d��"), strTemp, strSheetName, i, 2);
						log.writeCsvLog(strLog);
					}
				}
				nameCell.setName(strItem);
				strItem = GetItemText(i, 3);
				if (strItem.IsEmpty())
				{
					strItem = _T("0");
				}
				nameCell.setStatus(strItem);
				if (i > 4)
				{
					strItem = GetItemText(i-4, j);
					if (strItem.CompareNoCase(_T("ĸ������������")) == 0)
					{
						CString strTmp;
						strTmp = GetItemText(i-4, 2);
						nameCell.setLjqName(strTmp);
						strTmp = GetItemText(i-2, 2);
						nameCell.setWzNo(strTmp);

						strTmp = GetItemText(i-3, 2);
						nameCell.setModLjqName(strTmp);
						strTmp = GetItemText(i-1, 2);
						nameCell.setModWzNo(strTmp);
					}
				}

				//ȡ��Ԥ���ߺŵ�����
				//Ԥ���ߺ�λ�������������еĵ�����
				strItem = GetItemText(i, 4);
				if (!strItem.IsEmpty())
				{
					int nFind = strItem.Find(ICD_PROPERTY_DP_EQUAL);
					CString strType;
					int nSize = 0;
					int nEnd = 0;
					int nStart = 0;
					bool bRet = false;
					if (nFind >= 0)
					{
						strType = ICD_PROPERTY_DP_EQUAL;
						nSize = strType.GetLength();
						nEnd = strItem.Find(_T("]"), nFind);
						if (nEnd > 0)
						{
							CString strTmp = strItem.Mid(nFind+nSize, nEnd-nFind-nSize);
							//����з�ţ�֤���ж���������ݣ���1~22��33~44����
							nFind = strTmp.Find(_T(";"));
							CString strPre;
							if (nFind < 0)
							{
								strPre = strTmp;
							}
							while (nFind > 0)
							{
								strPre = strTmp.Left(nFind);
								bRet = CUtility::ParserStringAndGetStartAndEnd(strPre, nStart, nEnd);
								if (bRet)
								{
									gQuery.addFixNumAreaToStore(strType, nStart, nEnd);
								}

								strTmp = strTmp.Right(strTmp.GetLength() - nFind -1);
								strPre = strTmp;
								nFind = strPre.Find(_T(","));
							}
							bRet = CUtility::ParserStringAndGetStartAndEnd(strPre, nStart, nEnd);
							if (bRet)
							{
								gQuery.addFixNumAreaToStore(strType, nStart, nEnd);
							}
						}
					}
					nFind = strItem.Find(ICD_PROPERTY_RF_EQUAL);
					if (nFind >= 0)
					{
						strType = ICD_PROPERTY_RF_EQUAL;
						nSize = strType.GetLength();
						nEnd = strItem.Find(_T("]"), nFind);
						if (nEnd > 0)
						{
							CString strTmp = strItem.Mid(nFind+nSize, nEnd-nFind-nSize);
							//����з�ţ�֤���ж���������ݣ���1~22��33~44����
							nFind = strTmp.Find(_T(";"));
							CString strPre;
							if (nFind < 0)
							{
								strPre = strTmp;
							}
							while (nFind > 0)
							{
								strPre = strTmp.Left(nFind);
								bRet = CUtility::ParserStringAndGetStartAndEnd(strPre, nStart, nEnd);
								if (bRet)
								{
									gQuery.addFixNumAreaToStore(strType, nStart, nEnd);
								}

								strTmp = strTmp.Right(strTmp.GetLength() - nFind -1);
								strPre = strTmp;
								nFind = strPre.Find(_T(","));
							}
							bRet = CUtility::ParserStringAndGetStartAndEnd(strPre, nStart, nEnd);
							if (bRet)
							{
								gQuery.addFixNumAreaToStore(strType, nStart, nEnd);
							}
						}
					}
				}
				m_MapData.insert(make_pair(i, nameCell));
				nameCell.clear();
			}

			if (!verifyItemLength(strItem))
			{
				CWriteLog log;
				log.setPath(CUtility::getLogPath());
				CString strOut;
				strOut.Format(_T("���ֳ��ȹ���------------sheetName = %s,��%d�У���%d��------%s"), MySheet.GetName(), i, j, strItem);
				log.writeLog(strOut);
			}
		}
	}
	return vData;
}

void CMyExcel::InsertRow(CString strBegin, CString strEnd, long nStart)
{
	Range preRange, curRange;
	preRange.AttachDispatch(MySheet.GetRange(_variant_t(strBegin),_variant_t(strEnd)));
	preRange.AttachDispatch(preRange.GetEntireRow(), TRUE);

	strBegin.Format(_T("A%d"), nStart);
	strEnd.Format(_T("L%d"), nStart);
	curRange.AttachDispatch(MySheet.GetRange(_variant_t(strBegin), _variant_t(strEnd)));
	COleVariant varRange;
	varRange.vt	 = VT_DISPATCH;
	V_DISPATCH(&varRange) = (LPDISPATCH)curRange;
	V_DISPATCH(&varRange)->AddRef();
	preRange.Copy(varRange);
	
	curRange.Insert(vtMissing);
	preRange.ReleaseDispatch();
	curRange.ReleaseDispatch();
}

void CMyExcel::_Excel_Find_(LPCTSTR lpszFind)
{
	//LookIn
#define xlComments COleVariant( -4144L )
#define xlFormulas COleVariant( -4123L ) // will find value in any cell
#define xlValues COleVariant( -4163L ) // ignores hidden cells

	//LookAt
#define xlWhole COleVariant( 1L ) // whole word search
#define xlPart COleVariant( 2L ) // partial word search

	//SearchOrder (vOpt works here)
#define xlByRows COleVariant( 1L )
#define xlByColumns COleVariant( 2L )

	//SearchDirection (required but usually has no effect)
#define xlNext 1L
#define xlPrev 2L

	// MatchCase
#define xlMatchCase COleVariant( 1L )
#define xlIgnoreCase COleVariant( 0L )

	// MatchByte
	// ignored, use vOpt

	_variant_t vOpt(DISP_E_PARAMNOTFOUND, VT_ERROR);
	LPDISPATCH lpDisp = NULL;
	
	lpDisp =MyRange.Find(COleVariant(_T("A")), vOpt, xlFormulas, xlPart,
		xlByRows, xlNext, xlIgnoreCase, vOpt);
	CString strPrompt;
	if(lpDisp)
	{
		//first
		Range iFirst = MyRange; 
		iFirst.AttachDispatch(lpDisp);  
		//iFirst.Select();  
		//iFirst.Activate();
		strPrompt.Format(_T("row=%ld col=%ld\n"), iFirst.GetRow(), iFirst.GetColumn());
		TRACE( _T("row=%ld col=%ld\n"), iFirst.GetRow(), iFirst.GetColumn());

		Range iNext = MyRange.FindNext(vOpt);
		do 
		{
			TRACE( _T("row=%ld col=%ld\n"), 
				iNext.GetRow(), iNext.GetColumn());

			iNext = MyRange.FindNext( _variant_t(iNext) );
		} while (iNext.GetRow() != iFirst.GetRow() 
			|| iNext.GetColumn() !=iFirst.GetColumn() );
	}
	
}

bool CMyExcel::setVisible(bool bIsVisible)
{
	MyApp.SetVisible(bIsVisible);
	return true;
}

bool CMyExcel::selectRange(long Row, long Col)
{
	MySheet.Activate();
	VARIANT lpDisp=MyRange.GetItem(_variant_t(Row), _variant_t(Col)); 
	Range rgRgeValue; 
	rgRgeValue.AttachDispatch(lpDisp.pdispVal, TRUE); 
	
	lpDisp = rgRgeValue.Select();
	rgRgeValue.Activate();

	rgRgeValue.ReleaseDispatch();
	return true;
}

BOOL CMyExcel::openSpecailFile(CString strFile)
{
	LPDISPATCH   lpDisp=NULL; 
	CoInitialize(NULL); 
	CloseExcelFile();
	CLSID clsid;
	HRESULT hr;
	hr=::CLSIDFromProgID(_T("Excel.Application"),&clsid); //ͨ��ProgIDȡ��CLSID
	if(FAILED(hr))
	{
		AfxMessageBox(_T("����ɣ���Ȼû�а�װOFFICE"));
		return FALSE;
	}

	IUnknown *pUnknown=NULL;
	IDispatch *pDispatch=NULL;
	hr=::GetActiveObject(clsid,NULL,&pUnknown); //�����Ƿ���WORD����������
	if(FAILED(hr))
	{
		if (!MyApp.CreateDispatch(_T("Excel.Application"),NULL)) 
		{ 
			AfxMessageBox(_T("EXCEL��ʼ��ʱ����!"),MB_OK|MB_ICONERROR); 
			return FALSE; 
		} 
		try
		{
			lpDisp=MyApp.GetWorkbooks(); 
			MyBooks.AttachDispatch(lpDisp,TRUE); 

			lpDisp = MyBooks.Open(strFile, vtMissing, vtMissing, vtMissing,  
				vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,vtMissing); 
			MyBook.AttachDispatch(lpDisp,TRUE); 

			lpDisp=MyBook.GetWorksheets();  
			MySheets.AttachDispatch(lpDisp,TRUE); 
			MyApp.SetVisible(TRUE);
		}
		catch (CException* e)
		{
			//AfxMessageBox(_T("�ļ���ʧ�ܣ�����δ����"));
			return FALSE;
		}
	}
	else
	{
		try
		{
			hr=pUnknown->QueryInterface(IID_IDispatch,(LPVOID *)&MyApp);
			if(FAILED(hr)) 
				throw(_T("û��ȡ��IDispatchPtr"));
			pUnknown->Release(); 
			pUnknown=NULL;

			lpDisp=MyApp.GetWorkbooks(); 
			MyBooks.AttachDispatch(lpDisp,TRUE); 
			int nLen = MyBooks.GetCount();
			CString strName;
			bool bIsFind = false;
			for (int i=1; i<=nLen; i++)
			{
				lpDisp = MyBooks.GetItem(_variant_t(i));
				MyBook.AttachDispatch(lpDisp, TRUE);
				strName = MyBook.GetFullName();
				if (strName.CompareNoCase(strFile) == 0)
				{
					bIsFind = true;
					break;
				}
			}
			if (!bIsFind)
			{
				lpDisp = MyBooks.Open(strFile, vtMissing, vtMissing, vtMissing,  
					vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,vtMissing); 
				MyBook.AttachDispatch(lpDisp,TRUE); 
			}

			lpDisp=MyBook.GetWorksheets();  
			MySheets.AttachDispatch(lpDisp,TRUE); 
			MyApp.SetVisible(TRUE);
		}
		catch (CException* e)
		{
			//AfxMessageBox(_T("�ļ���ʧ�ܣ�����δ����"));
			return FALSE;
		}
	}
	

	strFilePath=strFile; 
	return TRUE; 
}


void CMyExcel::CloseExcelFile(BOOL if_save)
{
	//����Ѿ��򿪣��ر��ļ�
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR); 
	if (strFilePath.IsEmpty() == FALSE)
	{
		//�������,�����û�����,���û��Լ��棬����Լ�SAVE�������Ī���ĵȴ�
			//
		MyBook.Close(COleVariant(short(FALSE)),COleVariant(strFilePath),covOptional);
		MyBooks.Close();
		//���ļ����������
		strFilePath.Empty();
	}


	MyRange.ReleaseDispatch(); 
	MySheet.ReleaseDispatch(); 
	MySheets.ReleaseDispatch(); 
	MyBook.Close(COleVariant(short(FALSE)),_variant_t(strFilePath),covOptional); 
	MyBook.ReleaseDispatch(); 
	MyBooks.Close(); 
	MyBooks.ReleaseDispatch(); 
}
