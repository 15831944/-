#include "StdAfx.h"
#include "CGlobalQuery.h"
#include "DlgProress.h"
#include "Utility.h"
#include "MyExcel.h"
#include "CDrawLrmvEnt.h"
#include "CDrawDigitalOptical.h"
#include "Markup.h"
#include "CDrawEnt.h"
#include "DlgListError.h"
#include "CVerifyData.h"

extern CDlgProress* modelessDlg;
extern Adesk::Boolean startDlg();
extern Adesk::Boolean endDlg();

CGlobalQuery::CGlobalQuery(void)
{
	m_bSignalFlag = false;
	m_bFatalError = false;
	m_bIsPreview = false;
	m_RDplevel = 0;
	m_RDPRealData.clear();
	m_IgnoreVec.clear();
	m_dpNumInfo.clear();
	m_InfoVec.clear();
	m_ExcelInfo.clear();
	m_dpNumVec.clear();
	m_rfNumVec.clear();
	m_rfNumInfo.clear();
	m_DpInfo.clear();
	m_RfInfo.clear();
	m_DpInfo.setType(ICD_PROPERTY_DP);
	m_RfInfo.setType(ICD_PROPERTY_RF);
}


CGlobalQuery::~CGlobalQuery(void)
{
	m_bSignalFlag = false;
	m_bFatalError = false;
	m_bIsPreview = false;
	m_RDplevel = 0;
	m_RDPRealData.clear();
	m_IgnoreVec.clear();
	m_dpNumInfo.clear();
	m_InfoVec.clear();
	m_ExcelInfo.clear();
	m_dpNumVec.clear();
	m_rfNumVec.clear();
	m_rfNumInfo.clear();
	m_DpInfo.clear();
	m_RfInfo.clear();
}

void CGlobalQuery::clear()
{
	m_RFData.clear();
	m_DPData.clear();
	m_SData.clear();
	m_MData.clear();
	m_MTData.clear();
	m_CData.clear();

	m_RData.clear();
	m_RSData.clear();
	m_RMData.clear();

	m_MMData.clear();
	m_SSData.clear();
	m_RRData.clear();
	m_RRMTData.clear();
	m_RRMTJxData.clear();
	m_RMTData.clear();

	m_ErrorVec.clear();

	m_LrmvSheetData.clear();
	m_DigitalSheetData.clear();
	m_LrmvModSheetData.clear();

	m_blkNameVec.clear();
	m_nameVec.clear();
	m_modNameVec.clear();

	m_datainfo.clear();
	m_ComvecError.clear();
	m_TComVec.clear();
	m_RRDpData.clear();
	m_RDPRealData.clear();
	m_IgnoreVec.clear();

	m_bSignalFlag = false;
	m_bFatalError = false;
	m_bIsPreview = false;
	m_RDplevel = 0;
	m_dpNumInfo.clear();
	m_InfoVec.clear();
	m_dpNumVec.clear();

	m_rfNumVec.clear();
	m_rfNumInfo.clear();
	m_DpInfo.clear();
	m_RfInfo.clear();
	m_DpInfo.setType(ICD_PROPERTY_DP);
	m_RfInfo.setType(ICD_PROPERTY_RF);
}

void CGlobalQuery::clearVec()
{
	m_ErrorVec.clear();
	m_LrmvSheetData.clear();
	m_LrmvModSheetData.clear();
	m_DigitalSheetData.clear();
	m_blkNameVec.clear();
	m_nameVec.clear();
	m_modNameVec.clear();
	m_IgnoreVec.clear();
	m_DPData.clear();
	m_dpNumInfo.clear();
	m_InfoVec.clear();
	m_rfNumVec.clear();
	m_rfNumInfo.clear();
}

void CGlobalQuery::clearErrorLog()
{
	m_ErrorVec.clear();
}

void CGlobalQuery::doCommonCell(CCommonCell& cell)
{
	adddifferSignal(cell);

	bool bIsAdded = false;
	int nIndex = 0;
	CString strTexing,strTmp;
	int nTmp1,nTmp2;

	strTexing = cell.getTexing();
	strTexing.Trim();//ȥ���� ������

	CString strRealTexing = cell.getRealTexing();
	strRealTexing.Trim();

	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	//��[D]�����ȼ��ᵽǰ�棬�����D�Ͳ��������
	nTmp1 = strTexing.Find(ICD_PROPERTY_D);
	if (nTmp1 >=0)
	{
		cell.setType(-1);
		return;
	}
	nTmp1 = strTexing.Find(ICD_PROPERTY_RF);
	if (nTmp1 >= 0)
	{
		m_RfInfo.AddData(cell);

		cell.setType(2);
		int nFind = strTexing.Find(ICD_PROPERTY_RF_EQUAL);
		if (nFind >= 0)
		{
			strTexing = CUtility::RemoveString(strTexing, ICD_PROPERTY_RF_EQUAL);
		}
		else
			strTexing = CUtility::RemoveString(strTexing, ICD_PROPERTY_RF);
		doRFNumber(cell);
		if (!strRealTexing.IsEmpty())
		{
			tmpVec.push_back(cell);
			pair<map<CString, vector<CCommonCell> >::iterator, bool> ret = m_RFData.insert(make_pair(strRealTexing, tmpVec));
			if (!ret.second)
			{
				ret.first->second.push_back(cell);
			}
		}			
		bIsAdded = true;
	}
	nTmp1 = strTexing.Find(ICD_PROPERTY_GL);
	if (nTmp1 >= 0)
	{
		cell.setType(11);
		if (!strRealTexing.IsEmpty())
		{
			strTexing = CUtility::RemoveString(strTexing, ICD_PROPERTY_GL);
			tmpVec.push_back(cell);
			pair<map<CString, vector<CCommonCell> >::iterator, bool> ret = m_MTData.insert(make_pair(strRealTexing, tmpVec));
			if (!ret.second)
			{
				ret.first->second.push_back(cell);
			}
		}	
		bIsAdded = true;
	}


	nTmp1 = strTexing.Find(ICD_PROPERTY_R);
	if (nTmp1 >= 0)
	{
		doRLevel(strTexing, cell);
		bIsAdded = true;
	}
	nTmp1 = strTexing.Find(_T("[M]"));
	nTmp2 = strTexing.Find(_T("[S]"));
	if (nTmp1 >=0)
	{
		cell.setType(8);
		tmpVec.push_back(cell);

		pair<map<CString, vector<CCommonCell> >::iterator, bool> ret = m_MData.insert(make_pair(strRealTexing, tmpVec));
		if (!ret.second)
		{
			ret.first->second.push_back(cell);
		}
		bIsAdded = true;
	}
	if (nTmp2 >=0)
	{
		cell.setType(7);
		tmpVec.push_back(cell);
		pair<map<CString, vector<CCommonCell> >::iterator, bool> ret = m_SData.insert(make_pair(strRealTexing, tmpVec));
		if (!ret.second)
		{
			ret.first->second.push_back(cell);
		}
		bIsAdded = true;
	}

	nTmp1 = strTexing.Find(ICD_PROPERTY_DP);
	if (nTmp1 >= 0)
	{
		m_DpInfo.AddData(cell);

		int nFind = strTexing.Find(ICD_PROPERTY_DP_EQUAL);
		if (nFind >= 0)
		{
			strTexing = CUtility::RemoveString(strTexing, ICD_PROPERTY_DP_EQUAL);
		}
		else
			strTexing = CUtility::RemoveString(strTexing, ICD_PROPERTY_DP);
		doDpNumber(cell);

		tmpVec.clear();
		cell.setType(1);
		tmpVec.push_back(cell);
		pair<map<CString, vector<CCommonCell> >::iterator, bool> ret = m_DPData.insert(make_pair(strRealTexing, tmpVec));
		if (!ret.second)
		{
			ret.first->second.push_back(cell);
		}
		bIsAdded = true;
	}

	if (!bIsAdded)
	{
		if (strTexing.IsEmpty())
		{
			cell.setType(-1);
		}
		else
		{
			if (strRealTexing.IsEmpty())
			{
				return;
			}
			cell.setType(0);
			tmpVec.push_back(cell);
			pair<map<CString, vector<CCommonCell> >::iterator, bool> ret = m_CData.insert(make_pair(strRealTexing, tmpVec));
			if (!ret.second)
			{
				ret.first->second.push_back(cell);
			}
		}
	}
	tmpVec.clear();
}

void CGlobalQuery::doMTCommonCell(CCommonCell& cell)
{
	adddifferSignal(cell);
	//MT���������ź�

	CString strTexing,strTmp;
	int nTmp1;
	bool bIsAdded = false;

	strTexing = cell.getTexing();
	strTexing.Trim();//ȥ���� ������
	CString strRealTexing = cell.getRealTexing();
	if ((strRealTexing.IsEmpty()) || (strRealTexing.CompareNoCase(_T("NA")) == 0))
	{
		return;
	}
	cell.setType(11);
	nTmp1 = strTexing.Find(_T("[R"));
	if (nTmp1 >= 0)
	{
		cell.setType(3);
		doMTRLevel(strTexing, cell);
		bIsAdded = true;
	}
	if (!bIsAdded)
	{
		vector<CCommonCell> tmpVec;
		tmpVec.clear();
		tmpVec.push_back(cell);
		pair<map<CString, vector<CCommonCell> >::iterator, bool> ret = m_MTData.insert(make_pair(strRealTexing, tmpVec));
		if (!ret.second)
		{
			ret.first->second.push_back(cell);
		}
		tmpVec.clear();
	}
}

void CGlobalQuery::doRFCommonCell(CCommonCell& cell)
{
	adddifferSignal(cell, true);

	CString strRealTexing = cell.getRealTexing();
	strRealTexing.Trim();
	CString strTexing = cell.getTexing();
	int nFind = strTexing.Find(ICD_PROPERTY_D);
	if (nFind >= 0)
	{
		return;
	}
	if ((strRealTexing.IsEmpty()) || (strRealTexing.CompareNoCase(_T("NA")) == 0))
	{
		cell.setType(-1);
		return;
	}
	m_RfInfo.AddData(cell);
	//////////////////////////////////////////////////////////////////////////
	doRFNumber(cell);
	//////////////////////////////////////////////////////////////////////////
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	tmpVec.push_back(cell);
	pair<map<CString, vector<CCommonCell> >::iterator, bool> ret = m_RFData.insert(make_pair(strRealTexing, tmpVec));
	if (!ret.second)
	{
		ret.first->second.push_back(cell);
	}
	tmpVec.clear();
}

bool CGlobalQuery::doIt()
{
	if (m_bFatalError)
	{
		return false;
	}

	m_strErrorType = _T("���ݲ���һһƥ��");
	if (m_bSignalFlag)
	{
		/*if (!CallIgnoreDlg(_T("����ź�У�����")))
		{
			return false;
		}*/
	}

	AddRSToRData();
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(51);
	}	

	m_strErrorType = _T("M S һһƥ��У��");
	if (!doMData())
	{
		/*if (!CallIgnoreDlg(_T("M S һһƥ��У��")))
		{
			return false;
		}*/
		
	}
	if (!doSData())
	{
		return false;
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(60);
	}
	//acedSetStatusBarProgressMeterPos(20);

	m_strErrorType = _T("RF��Ƶһһƥ��У��");
	if (!doRFData())
	{
		/*if (!CallIgnoreDlg(_T("RF��Ƶһһƥ��У��")))
		{
			return false;
		}*/
	}	

	m_strErrorType = _T("DP��Ƶһһƥ��У��");
	if (!doDPData())
	{
		/*if (!CallIgnoreDlg(_T("DP��Ƶһһƥ��У��")))
		{
			return false;
		}*/
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(80);
	}

	m_strErrorType = _T("R�㼶һһƥ��У��");
	if (!doRData())
	{
		/*if (!CallIgnoreDlg(_T("R�㼶һһƥ��У��")))
		{
			return false;
		}*/
	}

	m_strErrorType = _T("R + MTһһƥ��У��");
	if (!doRMTData())
	{
		/*if (!CallIgnoreDlg(_T("R + MTһһƥ��У��")))
		{
			return false;
		}*/
	}

	m_strErrorType = _T("MTһһƥ��У��");
	if (!doMTData())
	{
		/*if (!CallIgnoreDlg(_T("MTһһƥ��У��")))
		{
			return false;
		}*/
	}

	m_strErrorType = _T("��ͨ����һһƥ��У��");
	if (!doCData())
	{/*
		if (!CallIgnoreDlg(_T("��ͨ����һһƥ��У��")))
		{
			return false;
		}*/
	}
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(90);
	}

	m_strErrorType = _T("R + Mһһƥ��У��");
	if (!doRMData())
	{
		/*if (!CallIgnoreDlg(_T("R + Mһһƥ��У��")))
		{
			return false;
		}*/
	}
	
	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(100);
	}

	if (!CallIgnoreDlg(_T("����У����Ϣ")))
	{
		return false;
	}
	
	//����̶����
	//EraseFixedNum();

	m_RfInfo.EraseFixedInfo();
	m_DpInfo.EraseFixedInfo();

	return true;
}

CString CGlobalQuery::getQuxiang(CCommonCell& cell)
{
	CString strOut;
	int nType = cell.getType();
	//0Ϊ��ͨ���ݣ�1ΪDp���ݣ�2ΪRF���ݣ�3Ϊ���ֹ����ݣ�
	//4Ϊģ������ݣ�5Ϊ���������ݣ�6ΪR�㼶���ݣ�
	//7ΪS���ݣ�8ΪM���ݣ�9ΪR+S����
	//10ΪR+M����,11Ϊ����������,-1Ϊ��[D]������
	CString strTexing = cell.getRealTexing();
	if (strTexing.IsEmpty())
	{
		return strOut;
	}
	CString strTempQuxiang = cell.getTempQuxiang();
	int nNumber = 1;
	if (nType == 0)
	{
		strOut = getSimpleResult(m_CData, strTexing, strTempQuxiang);
	}
	else if (nType == 1)
	{
		//strOut = getSpecialResult(m_DPData, strTexing, strTempQuxiang, nNumber);
		//strOut = getDpSpecialResult(cell);

		strOut = m_DpInfo.getQuxiang(cell);
		//cell.setCount(MyTransFunc::intToStr(nNumber));
		//�����R+S+DP����R+M+DP
		if (strOut.IsEmpty())//����ǿյģ���Ҫ��R+DP������ͨ��DP�����в���ȥ��
		{
			CString strTemp = cell.getTexing();
			int nIndex = strTemp.Find(_T("[R"));
			if (nIndex >= 0)//�����R�㼶����R�㼶���߼�
			{
				strOut = getRLevelQuxiang(cell);
			}
		}
	}
	else if (nType == 2)
	{
		//strOut = getSpecialResult(m_RFData, cell, strTexing, strTempQuxiang, nNumber);
		strOut = m_RfInfo.getQuxiang(cell);

		CString strTemp = cell.getTexing();
		int nFind = strTemp.Find(ICD_PROPERTY_D);
		if (nFind >= 0)
		{
			cell.setCount(_T(""));
		}
	}
	else if ((nType == 3)||(nType == 4))
	{
		strOut = getRMTLevelQuxiang(cell);
	}
	else if ((nType == 11))
	{
		strOut = getSimpleResult(m_MTData, strTexing, strTempQuxiang);
	}
	//else if (nType == 5)//���ΪLRMV�еļ������ˣ���ô�Ͳ���Ҫȥ����ȥ��
	//{
	//	strOut = cell.getQuxiang();
	//}
	else if (nType == 6)//�㼶����
	{
		strOut = getRLevelQuxiang(cell);
	}
	else if (nType == 7)//s����
	{
		map<CString, CString>::iterator iter = m_SSData.find(strTexing);
		if (iter == m_SSData.end())
		{
			return strOut;
		}
		strOut = iter->second;
	}
	else if (nType == 8)//M����
	{
		map<CString, CString>::iterator iter = m_MMData.find(strTexing);
		if (iter == m_MMData.end())
		{
			return strOut;
		}
		strOut = iter->second;
	}
	else if (nType == 12)
	{
		strOut = cell.getQuxiang();
		return strOut;
	}
	cell.setQuxiang(strOut);
	return strOut;
}

void CGlobalQuery::addCellInfo(CCommonCell cell)
{
	m_InfoVec.push_back(cell);
}

bool CGlobalQuery::writeCellInfoToFile()
{
	CVerifyData verData;
	verData.setData(m_InfoVec);
	bool bRet = verData.doIt();	
	return bRet;
}
COMVEC CGlobalQuery::getCellInfo()
{
	return m_InfoVec;
}
//bool CGlobalQuery::writeJxb()
//{
//	map<int, pair<CCommonCell, CCommonCell> > dpoutPutMap = getDpOutPutData();
//	//��ȡRF����
//	map<int, pair<CCommonCell, CCommonCell> > rfoutPutMap = getRFOutPutData();
//	//��ȡ�����Ϣ
//	map<CString, vector<pair<CCommonCell, CCommonCell> > > mtOutPutMap = getMTOutPutData();
//	
//	if ((dpoutPutMap.size() == 0) && (rfoutPutMap.size() == 0) && (mtOutPutMap.size() == 0))
//	{
//		acutPrintf(_T("\nû�е�Ƶ����Ƶ���ݼ���������"));
//		return false;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	//д�ļ�	
//	//���������
//	startDlg();
//	Sleep(100);
//	if (modelessDlg != NULL)
//	{
//		modelessDlg->SetWindowText(_T("���ɽ��߱�"));
//		modelessDlg->setPos(0);
//	}
//	CString strPre = MyBaseUtils::GetAppPath() + _T("support\\���߱�.xls");
//	CString strCurFile = CUtility::getFilePath() + _T("���߱�.xls");
//	::CopyFile(strPre, strCurFile, FALSE);
//
//	CMyExcel Excl;
//	if (!Excl.Open(strCurFile))
//	{
//		CString strPrompt;
//		if (gGlobal.IsFileExist(strCurFile))
//		{
//			strPrompt.Format(_T("%s�ļ�����д"), strPre);
//		}
//		else
//		{
//			strPrompt.Format(_T("%s�ļ�δ�ҵ�,��鿴�����ļ��Ƿ�����"), strPre);
//		}
//		AfxMessageBox(strPrompt);
//		modelessDlg->setPos(100);
//		endDlg();
//		return false;
//	}
//
//	Excl.OpenSheet(1);
//
//	CString strBmNo = gGlobal.GetIniValue(_T("ͼ������"), _T("BM"));
//	Excl.SetItemText(1, 2, strBmNo);
//	int nRow = 7;	
//	CString strNo;
//	int nSize = dpoutPutMap.size() + rfoutPutMap.size();//����
//	for (map<int, pair<CCommonCell, CCommonCell> >::iterator iter = dpoutPutMap.begin(); iter != dpoutPutMap.end(); ++iter)
//	{
//		strNo.Format(_T("%d"), iter->first);
//		Excl.SetItemText(nRow, 2, strNo);
//		Excl.SetItemText(nRow, 3, iter->second.first.getTempQuxiang());
//		Excl.SetItemText(nRow, 4, iter->second.second.getTempQuxiang());
//		nRow++;
//		if (modelessDlg != NULL)
//		{
//			modelessDlg->setPos((nRow-8)* 40/nSize);
//		}
//	}
//	dpoutPutMap.clear();
//
//	for (map<int, pair<CCommonCell, CCommonCell> >::iterator iter = rfoutPutMap.begin(); iter != rfoutPutMap.end(); ++iter)
//	{
//		strNo.Format(_T("W%d"), iter->first);
//		Excl.SetItemText(nRow, 2, strNo);
//		Excl.SetItemText(nRow, 3, iter->second.first.getTempQuxiang());
//		Excl.SetItemText(nRow, 4, iter->second.second.getTempQuxiang());
//		nRow++;
//		if (modelessDlg != NULL)
//		{
//			modelessDlg->setPos((nRow-8)* 40/nSize);
//		}
//	}
//	rfoutPutMap.clear();
//
//	vector<pair<CString, CString> > tmpVec;
//	tmpVec.clear();
//	nSize = mtOutPutMap.size();
//	int j=0;
//	for (map<CString, vector<pair<CString, CString> > >::iterator iter = mtOutPutMap.begin(); iter != mtOutPutMap.end(); ++iter)
//	{
//		strNo = iter->first;
//		tmpVec = iter->second;
//		int i=0;
//		for (vector<pair<CString, CString> >::iterator iTr = tmpVec.begin(); iTr != tmpVec.end(); ++iTr)
//		{
//			Excl.SetItemText(nRow, 2, strNo);
//			Excl.SetItemText(nRow, 3, iTr->first);
//			Excl.SetItemText(nRow, 4, iTr->second);
//			nRow++;
//			i++;
//			if (modelessDlg != NULL)
//			{
//				modelessDlg->setPos(40 + i*j*20/(nSize*tmpVec.size()));
//			}
//		}
//		j++;
//		tmpVec.clear();
//	}
//	mtOutPutMap.clear();
//	//���㼶��MT����
//	int nCount = 1;
//	CCommonCell preComCell,curComCell;
//	pair<CCommonCell, CCommonCell> jxPair;
//	for (vector<pair<CCommonCell, CCommonCell> >::iterator iter = m_RRMTJxData.begin(); iter != m_RRMTJxData.end(); ++iter)
//	{
//		jxPair = *iter;
//		preComCell = jxPair.first;
//		curComCell = jxPair.second;
//		if (preComCell.getCount().CompareNoCase(curComCell.getCount()) == 0)
//		{
//			strNo.Format(_T("%s"), preComCell.getCount());
//		}
//		else
//		{
//			strNo.Format(_T("%s-%s"), preComCell.getCount(), curComCell.getCount());
//		}
//		
//		Excl.SetItemText(nRow, 2, strNo);
//		Excl.SetItemText(nRow, 3, preComCell.getTempQuxiang());
//		Excl.SetItemText(nRow, 4, curComCell.getTempQuxiang());
//		nCount++;
//		nRow++;
//		if (modelessDlg != NULL)
//		{
//			modelessDlg->setPos(60 + 30*nCount/(m_RRMTJxData.size()));
//		}
//	}
//	Excl.Save();
//	Excl.Clear();
//
//	if (modelessDlg != NULL)
//	{
//		modelessDlg->setPos(100);
//	}
//	endDlg();
//	ShellExecute(NULL,_T("open"), strCurFile, NULL, NULL, SW_SHOW); 
//	return true;
//}

bool CGlobalQuery::writeJxljgxb()
{
	//////////////////////////////////////////////////////////////////////////
	//д�ļ�	
	//���������
	if ((m_RRMTJxData.size() == 0) 
		&& (m_MTData.size() == 0) 
		&& (m_RDPRealData.size() == 0)
		&& (m_RFData.size() == 0))
	{
		acutPrintf(_T("\nû�е�Ƶ����Ƶ���ݼ���������"));
		return false;
	}
	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("���ɵ��½��߹�ϵ��"));
		modelessDlg->setPos(0);
	}
	CString strPre = MyBaseUtils::GetAppPath() + _T("support\\���½��߹�ϵ��.xls");
	CString strCurFile = CUtility::getFilePath() + _T("���½��߹�ϵ��.xls");
	::CopyFile(strPre, strCurFile, FALSE);

	CMyExcel Excl;
	if (!Excl.Open(strCurFile))
	{
		CString strPrompt;
		if (gGlobal.IsFileExist(strCurFile))
		{
			strPrompt.Format(_T("%s�ļ�����д"), strPre);
		}
		else
		{
			strPrompt.Format(_T("%s�ļ�δ�ҵ�,��鿴�����ļ��Ƿ�����"), strPre);
		}
		AfxMessageBox(strPrompt);
		modelessDlg->setPos(100);
		endDlg();
		return false;
	}

	Excl.OpenSheet(1);

	CString strNo;
	int nRow = 2;
	COMVEC comVec;
	comVec.clear();
	CCommonCell preComCell,curComCell;	
	CString strPrintSymbol;
	int nCount = 1;
	int nIndex = 1;
	map<int, pair<CCommonCell, CCommonCell> > dpoutPutMap = getDpOutPutData();
	//��ȡRF����
	map<int, pair<CCommonCell, CCommonCell> > rfoutPutMap = getRFOutPutData();
	int nSize = dpoutPutMap.size() + rfoutPutMap.size();
	for (map<int, pair<CCommonCell, CCommonCell> >::iterator iter = dpoutPutMap.begin(); iter != dpoutPutMap.end(); ++iter)
	{
		pair<CCommonCell, CCommonCell> comPair = iter->second;
		nCount = iter->first;
		preComCell = comPair.first;
		curComCell = comPair.second;
		strNo.Format(_T("%d"), nCount);
		Excl.SetItemText(nRow, 1, strNo);
		Excl.SetItemText(nRow, 4, preComCell.getNameCell().getWzNo());//���ʴ���
		Excl.SetItemText(nRow, 5, preComCell.getShortName());//λ��
		strPrintSymbol = strNo + _T("-") + preComCell.getTempQuxiang();
		Excl.SetItemText(nRow, 6, strPrintSymbol);//��ӡ��ʶ
		Excl.SetItemText(nRow, 7, preComCell.getDuanzi());//����
		Excl.SetItemText(nRow, 8, preComCell.getRealTexing());//����

		//��һ��
		Excl.SetItemText(nRow, 10, curComCell.getNameCell().getWzNo());
		Excl.SetItemText(nRow, 5+6, curComCell.getShortName());//λ��
		strPrintSymbol = strNo + _T("-") + curComCell.getTempQuxiang();
		Excl.SetItemText(nRow, 6+6, strPrintSymbol);//��ӡ��ʶ
		Excl.SetItemText(nRow, 7+6, curComCell.getDuanzi());//����
		Excl.SetItemText(nRow, 8+6, curComCell.getRealTexing());//����
		Excl.SetItemText(nRow, 8+10, _T("��Ƶ"));
		nRow++;
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(nIndex* 40/nSize);
		}
	}

	for (map<int, pair<CCommonCell, CCommonCell> >::iterator iter = rfoutPutMap.begin(); iter != rfoutPutMap.end(); ++iter)
	{
		pair<CCommonCell, CCommonCell> comPair = iter->second;
		nCount = iter->first;
		preComCell = comPair.first;
		curComCell = comPair.second;
		strNo.Format(_T("W%d"), nCount);
		Excl.SetItemText(nRow, 1, strNo);
		Excl.SetItemText(nRow, 4, preComCell.getNameCell().getWzNo());//���ʴ���
		Excl.SetItemText(nRow, 5, preComCell.getShortName());//λ��
		strPrintSymbol = strNo + _T("-") + preComCell.getTempQuxiang();
		Excl.SetItemText(nRow, 6, strPrintSymbol);//��ӡ��ʶ
		Excl.SetItemText(nRow, 7, preComCell.getDuanzi());//����
		Excl.SetItemText(nRow, 8, preComCell.getRealTexing());//����

		//��һ��
		Excl.SetItemText(nRow, 10, curComCell.getNameCell().getWzNo());
		Excl.SetItemText(nRow, 5+6, curComCell.getShortName());//λ��
		strPrintSymbol = strNo + _T("-") + curComCell.getTempQuxiang();
		Excl.SetItemText(nRow, 6+6, strPrintSymbol);//��ӡ��ʶ
		Excl.SetItemText(nRow, 7+6, curComCell.getDuanzi());//����
		Excl.SetItemText(nRow, 8+6, curComCell.getRealTexing());//����
		Excl.SetItemText(nRow, 8+10, _T("��Ƶ"));
		nRow++;
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos((nRow-8)* 40/nSize);
		}
	}
	

	map<CString, vector<pair<CCommonCell, CCommonCell> > > mapInfo = doMTInfoToMap();
	nCount = 1;
	nSize = mapInfo.size();
	pair<CCommonCell, CCommonCell> jxPair;
	vector<pair<CCommonCell, CCommonCell> > tmpPairVec;
	int nStartRow,nEndRow;
	 
	for (map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator iter = mapInfo.begin(); iter != mapInfo.end(); ++iter)
	{
		tmpPairVec = iter->second;
		bool bFirst = true;
		nStartRow = nRow;
		nEndRow = nRow +tmpPairVec.size();	
		Excl.MergeCells(nRow, 1, nEndRow, 1);

		for (vector<pair<CCommonCell, CCommonCell> >::iterator Itr = tmpPairVec.begin(); Itr != tmpPairVec.end(); ++Itr)
		{
			jxPair = *Itr;
			preComCell = jxPair.first;
			curComCell = jxPair.second;
			if (bFirst)
			{
				strNo.Format(_T("%s"), preComCell.getCount());
				Excl.SetItemText(nRow, 1, strNo);
				bFirst = false;
			}
			Excl.SetItemText(nRow, 4, preComCell.getNameCell().getWzNo());
			Excl.SetItemText(nRow, 5, preComCell.getRealSheetName() + preComCell.getShortName());//λ��
			Excl.SetItemText(nRow, 6, preComCell.getTempQuxiang());//��ӡ��ʶ
			Excl.SetItemText(nRow, 7, preComCell.getDuanzi());//����
			Excl.SetItemText(nRow, 8, preComCell.getRealTexing());//����

			//��һ��
			Excl.SetItemText(nRow, 10, curComCell.getNameCell().getWzNo());
			Excl.SetItemText(nRow, 5+6, curComCell.getRealSheetName() + curComCell.getShortName());//λ��
			Excl.SetItemText(nRow, 6+6, curComCell.getTempQuxiang());//��ӡ��ʶ
			Excl.SetItemText(nRow, 7+6, curComCell.getDuanzi());//����
			Excl.SetItemText(nRow, 8+6, curComCell.getRealTexing());//����
			Excl.SetItemText(nRow, 8+10, _T("��������"));
			nRow++;
		}

		tmpPairVec.clear();

		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(40 + nCount* 50/nSize);
		}
		nCount++;
	}
	mapInfo.clear();

	//����[T]��������
	for (vector<CCommonCell>::iterator iter = m_TComVec.begin(); iter != m_TComVec.end(); ++iter)
	{
		preComCell = *iter;
		Excl.SetItemText(nRow, 5, preComCell.getRealSheetName() + preComCell.getShortName());//λ��
		Excl.SetItemText(nRow, 6, preComCell.getTempQuxiang());//��ӡ��ʶ
		Excl.SetItemText(nRow, 7, preComCell.getDuanzi());//����
		Excl.SetItemText(nRow, 8, preComCell.getRealTexing());//����
		nRow++;
	}
	Excl.Save();
	Excl.Clear();

	if (modelessDlg != NULL)
	{
		modelessDlg->setPos(100);
	}
	endDlg();
	ShellExecute(NULL,_T("open"), strCurFile, NULL, NULL, SW_SHOW); 
	return true;
}

void CGlobalQuery::addErrorString(CString strError)
{
	m_ErrorVec.push_back(strError);
}

bool CGlobalQuery::doSData()
{
	vector<CCommonCell> sVec;
	vector<CCommonCell> mVec;
	CCommonCell sComCell,mComCell;
	CString strSTexing,strMTexing;
	for (MDMap::iterator Itr = m_MData.begin(); Itr != m_MData.end(); ++Itr)
	{	
		strMTexing = Itr->first;
		for (MDMap::iterator iter = m_SData.begin(); iter != m_SData.end(); ++iter)
		{
			strSTexing = iter->first;
			if (strMTexing.CompareNoCase(strSTexing) == 0)
			{
				mVec = Itr->second;
				mComCell = mVec.at(0);

				sVec = iter->second;
				sComCell = sVec.at(0);
				//��¼M����
				m_MMData.insert(make_pair(strMTexing, sComCell.getTempQuxiang()));
				m_SSData.insert(make_pair(strMTexing, mComCell.getTempQuxiang()));

				mVec.clear();
				sVec.clear();
			}
		}
	}

	for (MDMap::iterator iter = m_SData.begin(); iter != m_SData.end(); ++iter)
	{
		strMTexing = iter->first;
		MDMap::iterator IR = m_MData.find(strMTexing);
		if (IR == m_MData.end())
		{
			COMVEC tmpVec;
			tmpVec = iter->second;
			for (COMVEC::iterator IIIR = tmpVec.begin(); IIIR != tmpVec.end(); ++IIIR)
			{
				m_strErrorType = _T("M S һһƥ��У��");
				CString strError = getErrorString(*IIIR);
				addErrorString(strError);
			}
			tmpVec.clear();
		}
	}
	return true;
}

bool CGlobalQuery::doCData()
{
	bool bRet = true;
	CCommonCell curComCell;
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	CString strLog;
	for (MDMap::iterator iter = m_CData.begin(); iter != m_CData.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() != 2)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				strLog = getErrorString(curComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}
		tmpVec.clear();
	}

	return bRet;
}

bool CGlobalQuery::doRData()
{
	bool bRet = true;
	CCommonCell preComCell, curComCell;
	vector<CCommonCell> tmpVec,preVec,curVec;
	tmpVec.clear();
	preVec.clear();
	curVec.clear();
	CString strLog;
	pair<int, CString> tmpPair;
	map<int, vector<CCommonCell> > tmpMap;
	for (map<pair<int, CString>, vector<CCommonCell> >::iterator iter = m_RData.begin(); iter != m_RData.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() > 1)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				strLog = getErrorString(curComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}
		else
		{
			pair<map<int, vector<CCommonCell> >::iterator, bool> bRt = tmpMap.insert(make_pair(iter->first.first, tmpVec));
			if (!bRt.second)
			{
				bRt.first->second.insert(bRt.first->second.end(), tmpVec.begin(), tmpVec.end());
			}
		}
		tmpVec.clear();
	}
	int nlen = 0;

	//������R2��R1
	/*for (map<int, vector<CCommonCell> >::iterator iter = tmpMap.begin(); iter != tmpMap.end(); ++iter)
	{
		nlen = iter->first;
		tmpVec = iter->second;
		preVec = iter->second;
		nlen++;
		map<int, vector<CCommonCell> >::iterator IIR = tmpMap.find(nlen);
		if (IIR == tmpMap.end())
		{
			continue;
		}
		else
		{
			curVec = IIR->second;
			for (vector<CCommonCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
			{
				preComCell = *preIter;
				CString strPreTexing = preComCell.getRealTexing();
				if (strPreTexing.IsEmpty())
				{
					continue;
				}
				for (vector<CCommonCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
				{
					curComCell = *curIter;
					CString strCurTexing = curComCell.getRealTexing();
					if (strCurTexing.IsEmpty())
					{
						continue;
					}
					if (strCurTexing.CompareNoCase(strPreTexing) == 0)
					{
						tmpPair.first = nlen;
						tmpPair.second = strCurTexing;
						//��¼��һ����ȥ��
						m_RRData.insert(make_pair(tmpPair, preComCell.getTempQuxiang()));
					}
				}
			}
		}
	}


	//��β������һ��ͷ��
	if (tmpMap.size() >= 2)//���ݱ��벻С��2��������
	{
		preVec.clear();
		curVec.clear();

		map<int, vector<CCommonCell> >::reverse_iterator bg = tmpMap.rbegin();//β��
		map<int, vector<CCommonCell> >::iterator iter = tmpMap.begin();//ͷ��

		nlen = iter->first;

		preVec = bg->second;
		curVec = iter->second;

		for (vector<CCommonCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
		{
			preComCell = *preIter;
			CString strPreTexing = preComCell.getRealTexing();
			if (strPreTexing.IsEmpty())
			{
				continue;
			}
			for (vector<CCommonCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
			{
				curComCell = *curIter;
				CString strCurTexing = curComCell.getRealTexing();
				if (strCurTexing.IsEmpty())
				{
					continue;
				}
				if (strCurTexing.CompareNoCase(strPreTexing) == 0)
				{
					tmpPair.first = nlen;
					tmpPair.second = strCurTexing;
					//��¼��һ����ȥ��
					m_RRData.insert(make_pair(tmpPair, preComCell.getTempQuxiang()));
				}
			}
		}
	}*/
	

	//������R1��R2
	bool bIsFind = false;
	for (map<int, vector<CCommonCell> >::reverse_iterator iter = tmpMap.rbegin(); iter != tmpMap.rend(); ++iter)
	{
		nlen = iter->first;
		preVec = iter->second;
		nlen--;
		map<int, vector<CCommonCell> >::iterator IIR = tmpMap.find(nlen);
		if (IIR == tmpMap.end())
		{
			//���nlen���ڵ���1
			if (nlen >= 1)
			{
				for (COMVEC::iterator iiir = preVec.begin(); iiir != preVec.end(); ++iiir)
				{
					CString strError = getErrorString(*iiir);
					addErrorString(strError);
					bRet = false;
				}
				preVec.clear();
			}
			continue;
		}
		else
		{
			curVec = IIR->second;
			for (vector<CCommonCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
			{
				preComCell = *preIter;
				CString strPreTexing = preComCell.getRealTexing();
				if (strPreTexing.IsEmpty())
				{
					continue;
				}
				bIsFind = false;
				for (vector<CCommonCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
				{
					curComCell = *curIter;
					CString strCurTexing = curComCell.getRealTexing();
					if (strCurTexing.IsEmpty())
					{
						continue;
					}
					if (strCurTexing.CompareNoCase(strPreTexing) == 0)
					{
						tmpPair.first = nlen;
						tmpPair.second = strCurTexing;
						curComCell.setQuxiang(preComCell.getTempQuxiang());
						//��¼��һ����ȥ��
						m_RRData.insert(make_pair(tmpPair, curComCell));
						bIsFind = true;
					}
				}
				if (!bIsFind)
				{
					CString strError = getErrorString(preComCell);
					addErrorString(strError);
					bRet = false;
				}
			}
		}
		preVec.clear();
		curVec.clear();
	}


	//����һ��ͷ���Ƿ����ҵ���һ�˵�����
	if (tmpMap.size() < 2)//���С��2˵��ֻ��1�����û�У���ô��Ȼ�������ݶ��Ǵ��
	{
		map<int, vector<CCommonCell> >::iterator bg = tmpMap.begin();
		if (bg == tmpMap.end())
		{
			return true;
		}
		tmpVec = bg->second;
		for (COMVEC::iterator itr = tmpVec.begin(); itr	!= tmpVec.end(); ++itr)
		{
			CString strError = getErrorString(*itr);
			addErrorString(strError);
			bRet = false;
		}
		tmpVec.clear();
	}
	else
	{
		map<int, vector<CCommonCell> >::iterator bg = tmpMap.begin();
		tmpVec = bg->second;
		for (COMVEC::iterator itr = tmpVec.begin(); itr	!= tmpVec.end(); ++itr)
		{
			preComCell = *itr;
			tmpPair.first = 2;
			tmpPair.second = preComCell.getRealTexing();

			map<pair<int, CString>, COMVEC >::iterator IIIR = m_RData.find(tmpPair);
			if (IIIR == m_RData.end())
			{
				CString strError = getErrorString(preComCell);
				addErrorString(strError);
				bRet = false;
			}
		}
		tmpVec.clear();
	}

	//��β������һ����һ��
	if (tmpMap.size() >= 2)//���ݱ��벻С��2��������
	{
		preVec.clear();
		curVec.clear();

		map<int, vector<CCommonCell> >::reverse_iterator iter = tmpMap.rbegin();//ͷ��

		nlen = iter->first;
		nlen--;	
		curVec = iter->second;

		map<int, vector<CCommonCell> >::iterator bg = tmpMap.find(nlen);//��һ��
		if (bg == tmpMap.end())
		{
			for (COMVEC::iterator iiir = curVec.begin(); iiir != curVec.end(); ++iiir)
			{
				CString strError = getErrorString(*iiir);
				addErrorString(strError);
				bRet = false;
			}
			return bRet;
		}
		nlen = iter->first;
		preVec = bg->second;
		for (vector<CCommonCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
		{
			bool bIsFind = false;

			curComCell = *curIter;
			CString strCurTexing = curComCell.getRealTexing();
			if (strCurTexing.IsEmpty())
			{
				continue;
			}
			for (vector<CCommonCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
			{
				preComCell = *preIter;
				CString strPreTexing = preComCell.getRealTexing();
				if (strPreTexing.IsEmpty())
				{
					continue;
				}
				if (strCurTexing.CompareNoCase(strPreTexing) == 0)
				{
					tmpPair.first = nlen;
					tmpPair.second = strCurTexing;
					//��¼��һ����ȥ��
					curComCell.setTempQuxiang(preComCell.getTempQuxiang());
					m_RRData.insert(make_pair(tmpPair, curComCell));
					bIsFind = true;
				}
			}
			if (!bIsFind)
			{
				CString strError = getErrorString(curComCell);
				addErrorString(strError);
				bRet = false;
			}
		}
		curVec.clear();
		preVec.clear();
	}
	
	//�����м�㼶����
	for (map<pair<int, CString>, CCommonCell>::iterator iier = m_RRData.begin(); iier != m_RRData.end(); ++iier)
	{
		tmpPair = iier->first;
		nlen = tmpPair.first;
		curComCell = iier->second;
		nlen--;
		if (nlen <=0)
		{
			continue;
		}
		tmpPair.first = nlen;
		map<pair<int, CString>, CCommonCell>::iterator iRter = m_RRData.find(tmpPair);
		if (iRter != m_RRData.end())
		{
			preComCell = iRter->second;
			curComCell.setPreQuxiang(preComCell.getTempQuxiang());
			iier->second = curComCell;
		}
		else
		{

		}
	}
	tmpMap.clear();
	return bRet	;
}

bool CGlobalQuery::doRDpData()
{
	bool bRet = true;
	CCommonCell preComCell, curComCell;
	vector<CCommonCell> tmpVec,preVec,curVec;
	tmpVec.clear();
	preVec.clear();
	curVec.clear();
	CString strLog;
	pair<CCommonCell, CCommonCell> tmpPair;
	map<int, vector<CCommonCell> > tmpMap;
	for (map<pair<int, CString>, vector<CCommonCell> >::iterator iter = m_RRDpData.begin(); iter != m_RRDpData.end(); ++iter)
	{
		tmpVec = iter->second;
		
		pair<map<int, vector<CCommonCell> >::iterator, bool> bRt = tmpMap.insert(make_pair(iter->first.first, tmpVec));
		if (!bRt.second)
		{
			bRt.first->second.insert(bRt.first->second.end(), tmpVec.begin(), tmpVec.end());
		}
		
		tmpVec.clear();
	}
	int nlen = 0;

	bool bIsFind = false;
	CString strTemp;
	if (tmpMap.size() > 0)
	{
		m_RDplevel = tmpMap.rbegin()->first;
	}
	for (map<int, vector<CCommonCell> >::reverse_iterator iter = tmpMap.rbegin(); iter != tmpMap.rend(); ++iter)
	{
		nlen = iter->first;
		preVec = iter->second;
		nlen--;
		map<int, vector<CCommonCell> >::iterator IIR = tmpMap.find(nlen);
		if (IIR == tmpMap.end())
		{
			for (vector<CCommonCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
			{
				preComCell = *preIter;
				CString strPreTexing = preComCell.getRealTexing();
				strTemp.Format(_T("%d%s"), nlen+1, strPreTexing);//��¼��ǰ���ݣ���������һ�������в�ѯ
				map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator IIIr = m_RDPRealData.find(strTemp);
				if (IIIr == m_RDPRealData.end())//���û�ҵ���˵��û���γɹ�������ô��������
				{
					m_strErrorType = _T("�㼶��DP����û��һһƥ��");
					strLog = getErrorString(preComCell);
					m_ErrorVec.push_back(strLog);
					bRet = false;
				}
			}
			preVec.clear();
			continue;
		}
		else
		{
			curVec = IIR->second;
			for (vector<CCommonCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
			{
				preComCell = *preIter;
				CString strPreTexing = preComCell.getRealTexing();
				//��¼��һ����ȥ��
				strTemp.Format(_T("%d%s"), nlen, strPreTexing);
				if (strPreTexing.IsEmpty())
				{
					continue;
				}
				bIsFind = false;
				for (vector<CCommonCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
				{
					curComCell = *curIter;
					CString strCurTexing = curComCell.getRealTexing();
					if (strCurTexing.IsEmpty())
					{
						continue;
					}
					if (strCurTexing.CompareNoCase(strPreTexing) == 0)
					{
						curComCell.setQuxiang(preComCell.getTempQuxiang());
						tmpPair.first = curComCell;
						tmpPair.second = preComCell;
					
						vector<pair<CCommonCell, CCommonCell> > tmpVec;
						tmpVec.push_back(tmpPair);
						pair<map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator, bool> ret = m_RDPRealData.insert(make_pair(strTemp, tmpVec));
						if (!ret.second)
						{
							ret.first->second.push_back(tmpPair);
						}
						tmpVec.clear();
						bIsFind = true;
					}
				}
				//���û���ҵ�����ô��Ҫ�����ݵ�ǰ���������ٲ���һ��
				if (!bIsFind)
				{
					strTemp.Format(_T("%d%s"), nlen+1, strPreTexing);//��¼��ǰ���ݣ���������һ�������в�ѯ
					map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator IIIr = m_RDPRealData.find(strTemp);
					if (IIIr == m_RDPRealData.end())//���û�ҵ���˵��û���γɹ�������ô��������
					{
						m_strErrorType = _T("�㼶��DP����û��һһƥ��");
						strLog = getErrorString(preComCell);
						m_ErrorVec.push_back(strLog);
						bRet = false;
					}
				}
			}	
		}	
		curVec.clear();
		preVec.clear();
	}
	tmpMap.clear();
	return bRet	;
}

bool CGlobalQuery::doRMTData()
{
	bool bRet = true;
	CCommonCell preComCell, curComCell;
	vector<CCommonCell> tmpVec,preVec,curVec;
	tmpVec.clear();
	preVec.clear();
	curVec.clear();
	CString strLog;
	pair<int, CString> tmpPair;
	pair<CCommonCell, CCommonCell> jxPair;
	map<int, vector<CCommonCell> > tmpMap;
	for (map<pair<int, CString>, vector<CCommonCell> >::iterator iter = m_RMTData.begin(); iter != m_RMTData.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() > 1)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				strLog = getErrorString(curComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}
		else
		{
			pair<map<int, vector<CCommonCell> >::iterator, bool> bRt = tmpMap.insert(make_pair(iter->first.first, tmpVec));
			if (!bRt.second)
			{
				bRt.first->second.insert(bRt.first->second.end(), tmpVec.begin(), tmpVec.end());
			}
		}
		tmpVec.clear();
	}
	int nlen = 0;

	bool bIsFind = false;
	for (map<int, vector<CCommonCell> >::reverse_iterator iter = tmpMap.rbegin(); iter != tmpMap.rend(); ++iter)
	{
		nlen = iter->first;
		preVec = iter->second;
		nlen--;
		map<int, vector<CCommonCell> >::iterator IIR = tmpMap.find(nlen);
		if (IIR == tmpMap.end())
		{
			//���nlen���ڵ���1
			if (nlen >= 1)
			{
				for (COMVEC::iterator iiir = preVec.begin(); iiir != preVec.end(); ++iiir)
				{
					CString strError = getErrorString(*iiir);
					addErrorString(strError);
					bRet = false;
				}
			}
			continue;
		}
		else
		{
			curVec = IIR->second;
			for (vector<CCommonCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
			{
				preComCell = *preIter;
				CString strPreTexing = preComCell.getRealTexing();
				if (strPreTexing.IsEmpty())
				{
					continue;
				}
				bIsFind = false;
				for (vector<CCommonCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
				{
					curComCell = *curIter;
					CString strCurTexing = curComCell.getRealTexing();
					if (strCurTexing.IsEmpty())
					{
						continue;
					}
					if (strCurTexing.CompareNoCase(strPreTexing) == 0)
					{
						tmpPair.first = nlen;
						tmpPair.second = strCurTexing;

						curComCell.setQuxiang(preComCell.getTempQuxiang());
						//��¼��һ����ȥ��
						m_RRMTData.insert(make_pair(tmpPair, curComCell));

						jxPair.first = curComCell;
						jxPair.second = preComCell;
						m_RRMTJxData.push_back(jxPair);
						bIsFind = true;
					}
				}
			
				if (!bIsFind)
				{
					CString strError = getErrorString(preComCell);
					addErrorString(strError);
					bRet = false;
				}
			}
		}
		preVec.clear();	
		curVec.clear();
	}


	//����һ��ͷ���Ƿ����ҵ���һ�˵�����
	if (tmpMap.size() < 2)//���С��2˵��ֻ��1�����û�У���ô��Ȼ�������ݶ��Ǵ��
	{
		map<int, vector<CCommonCell> >::iterator bg = tmpMap.begin();
		if (bg == tmpMap.end())
		{
			return true;
		}
		tmpVec = bg->second;
		for (COMVEC::iterator itr = tmpVec.begin(); itr	!= tmpVec.end(); ++itr)
		{
			CString strError = getErrorString(*itr);
			addErrorString(strError);
			bRet = false;
		}
		tmpVec.clear();
	}
	else
	{
		map<int, vector<CCommonCell> >::iterator bg = tmpMap.begin();
		tmpVec = bg->second;
		for (COMVEC::iterator itr = tmpVec.begin(); itr	!= tmpVec.end(); ++itr)
		{
			preComCell = *itr;
			tmpPair.first = 2;
			tmpPair.second = preComCell.getRealTexing();

			map<pair<int, CString>, COMVEC >::iterator IIIR = m_RMTData.find(tmpPair);
			if (IIIR == m_RMTData.end())
			{
				CString strError = getErrorString(preComCell);
				addErrorString(strError);
				bRet = false;
			}
		}
		tmpVec.clear();
	}

	//��β������һ����һ��
	if (tmpMap.size() >= 2)//���ݱ��벻С��2��������
	{
		preVec.clear();
		curVec.clear();

		map<int, vector<CCommonCell> >::reverse_iterator iter = tmpMap.rbegin();//ͷ��

		nlen = iter->first;
		nlen--;	
		curVec = iter->second;

		map<int, vector<CCommonCell> >::iterator bg = tmpMap.find(nlen);//��һ��
		if (bg == tmpMap.end())
		{
			for (COMVEC::iterator iiir = curVec.begin(); iiir != curVec.end(); ++iiir)
			{
				CString strError = getErrorString(*iiir);
				addErrorString(strError);
				bRet = false;
			}
			return bRet;
		}
		nlen = iter->first;
		preVec = bg->second;

		for (vector<CCommonCell>::iterator preIter = preVec.begin(); preIter != preVec.end(); ++preIter)
		{
			preComCell = *preIter;
			CString strPreTexing = preComCell.getRealTexing();
			if (strPreTexing.IsEmpty())
			{
				continue;
			}
			bool bIsFind = false;
			for (vector<CCommonCell>::iterator curIter = curVec.begin(); curIter != curVec.end(); ++curIter)
			{
				curComCell = *curIter;
				CString strCurTexing = curComCell.getRealTexing();
				if (strCurTexing.IsEmpty())
				{
					continue;
				}
				if (strCurTexing.CompareNoCase(strPreTexing) == 0)
				{
					tmpPair.first = nlen;
					tmpPair.second = strCurTexing;
					//��¼��һ����ȥ��
					curComCell.setQuxiang(preComCell.getTempQuxiang());
					m_RRMTData.insert(make_pair(tmpPair, curComCell));
					jxPair.first = curComCell;
					jxPair.second = preComCell;
					m_RRMTJxData.push_back(jxPair);
					bIsFind = true;
				}
			}
			if (!bIsFind)
			{
				CString strError = getErrorString(preComCell);
				addErrorString(strError);
				bRet = false;
			}
		}
		preVec.clear();
		curVec.clear();
	}
	
	//�����м�㼶����
	for (map<pair<int, CString>, CCommonCell>::iterator iier = m_RRMTData.begin(); iier != m_RRMTData.end(); ++iier)
	{
		tmpPair = iier->first;
		nlen = tmpPair.first;
		curComCell = iier->second;
		nlen--;
		if (nlen <=0)
		{
			continue;
		}
		tmpPair.first = nlen;
		map<pair<int, CString>, CCommonCell>::iterator iRter = m_RRMTData.find(tmpPair);
		if (iRter != m_RRMTData.end())
		{
			preComCell = iRter->second;
			curComCell.setPreQuxiang(preComCell.getTempQuxiang());
			iier->second = curComCell;
		}
		else
		{

		}
	}
	return bRet	;
}

bool CGlobalQuery::doRFData()
{
	bool bRet = true;
	CCommonCell curComCell;
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	CString strLog;
	for (MDMap::iterator iter = m_RFData.begin(); iter != m_RFData.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() != 2)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				m_strErrorType = _T("RF���ݲ���һһƥ��");
				strLog = getErrorString(curComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}
		tmpVec.clear();
	}

	return bRet;
}

bool CGlobalQuery::doDPData()
{
	bool bRet = true;
	CCommonCell curComCell;
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	CString strLog;
	if (!doRDpData())
	{
		return false;
	}
	CString strTemp;
	vector<pair<CCommonCell, CCommonCell> > tmpPairVec;
	pair<CCommonCell, CCommonCell> tmpPair;
	CCommonCell curCell, preCell;

	for (MDMap::iterator iter = m_DPData.begin(); iter != m_DPData.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() < 2)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				m_strErrorType = _T("DP���ݲ���һһƥ��");
				strLog = getErrorString(curComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}
		else if (tmpVec.size() == 2)
		{
			tmpPairVec.clear();
			preCell = tmpVec[0];
			curCell = tmpVec[1];
			preCell.setQuxiang(curCell.getTempQuxiang());
			curCell.setQuxiang(preCell.getTempQuxiang());
			tmpPair.first = preCell;
			tmpPair.second = curCell;
			tmpPairVec.push_back(tmpPair);
			strTemp = preCell.getRealTexing();

			pair<map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator, bool> ret = m_RDPRealData.insert(make_pair(strTemp, tmpPairVec));
			if (!ret.second)
			{
				ret.first->second.push_back(tmpPair);
			}
			tmpPairVec.clear();
		}
		else
		{
			
			vector<CCommonCell> tmpV;//��¼��ʱ��S����
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				int nIndex = curComCell.getTexing().Find(_T("[S]"));
				if (nIndex >= 0)
				{
					tmpV.push_back(curComCell);
				}
				else
				{
					curCell = curComCell;
				}
			}
			tmpPairVec.clear();
			for (vector<CCommonCell>::iterator IIr = tmpV.begin(); IIr != tmpV.end(); ++IIr)
			{
				preCell = *IIr;	
				curCell.setQuxiang(preCell.getTempQuxiang());
				tmpPair.first = curCell;//M��ǰ��
				preCell.setQuxiang(curCell.getTempQuxiang());
				tmpPair.second = preCell;//S�ں���
				tmpPairVec.push_back(tmpPair);
			}
			tmpV.clear();
			strTemp = curCell.getRealTexing();
			m_RDPRealData.insert(make_pair(strTemp, tmpPairVec));
			tmpPairVec.clear();
		}
		tmpVec.clear();
	}

	return bRet;
}

bool CGlobalQuery::doMTData()
{
	bool bRet = true;
	CCommonCell curComCell;
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	CString strLog;
	for (MDMap::iterator iter = m_MTData.begin(); iter != m_MTData.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() != 2)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				m_strErrorType = _T("MT���ݲ���һһƥ��");
				strLog = getErrorString(curComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}
		tmpVec.clear();
	}

	return bRet;
}

bool CGlobalQuery::doMData()
{
	bool bRet = true;
	CCommonCell curComCell;
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	CString strLog;
	for (MDMap::iterator iter = m_MData.begin(); iter != m_MData.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() > 1)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				strLog = getErrorString(curComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}
		tmpVec.clear();
	}
	//////////////////////////////////////////////////////////////////////////
	//����һ�Զ��ϵ[M]��[S]�Ĵ���У��
	if (m_MData.size()>=m_SData.size())
	{
		for (MDMap::iterator iTer = m_MData.begin(); iTer != m_MData.end(); ++iTer)
		{
			CString strTemp = iTer->first;
			MDMap::iterator iier = m_SData.find(strTemp);
			if (iier == m_SData.end())
			{
				tmpVec = iTer->second;
				if (tmpVec.size() > 1)
				{
					for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
					{
						curComCell = *Itr;
						strLog = getErrorString(curComCell);
						m_ErrorVec.push_back(strLog);
						bRet = false;
					}
				}
				tmpVec.clear();
			}
			else	
			{
				//�ҵ�����
				vector<CCommonCell> mComVec = iTer->second;
				vector<CCommonCell> sComVec = iier->second;
				if (!GetMDpError(mComVec, sComVec))
				{
					bRet = false;
				}
				mComVec.clear();
				sComVec.clear();
			}
		}
	}
	else
	{
		for (MDMap::iterator iTer = m_SData.begin(); iTer != m_SData.end(); ++iTer)
		{
			CString strTemp = iTer->first;
			MDMap::iterator iier = m_MData.find(strTemp);
			if (iier == m_MData.end())
			{
				tmpVec = iTer->second;
				if (tmpVec.size() > 1)
				{
					for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
					{
						curComCell = *Itr;
						strLog = getErrorString(curComCell);
						m_ErrorVec.push_back(strLog);
						bRet = false;
					}
				}
				tmpVec.clear();
			}
		}
	}
	return bRet;
}

bool CGlobalQuery::doRMData()
{
	bool bRet = true;
	CCommonCell curComCell;
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	CString strLog;
	map<int, vector<CCommonCell> > mMap, sMap;
	for (MRMAP::iterator iter = m_RMData.begin(); iter != m_RMData.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() > 1)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				strLog = getErrorString(curComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}
		else
		{
			pair<map<int, vector<CCommonCell> >::iterator, bool> bRt = mMap.insert(make_pair(iter->first.first, tmpVec));
			if (!bRt.second)
			{
				bRt.first->second.insert(bRt.first->second.end(), tmpVec.begin(), tmpVec.end());
			}
		}
		tmpVec.clear();
	}

	int nSize = m_RMData.size();
	//�洢RS����
	for (MRMAP::iterator iter = m_RSData.begin(); iter != m_RSData.end(); ++iter)
	{
		tmpVec = iter->second;
		pair<map<int, vector<CCommonCell> >::iterator, bool> bRt = sMap.insert(make_pair(iter->first.first, tmpVec));
		if (!bRt.second)
		{
			bRt.first->second.insert(bRt.first->second.end(), tmpVec.begin(), tmpVec.end());
		}
		tmpVec.clear();
	}


	//��ͷ����������
	int nlen = 0;
	vector<CCommonCell> mComVec;
	vector<CCommonCell> sComVec;
	CCommonCell mComCell;
	CCommonCell sComCell;
	CString strSTexing,strMTexing;
	//R1[M] R2[S]
	//for (map<int, vector<CCommonCell> >::iterator iter = mMap.begin(); iter != mMap.end(); ++iter)
	//{
	//	mComVec = iter->second;
	//	nlen = iter->first;
	//	nlen++;
	//	map<int, vector<CCommonCell> >::iterator SIter = sMap.find(nlen);
	//	if (SIter == sMap.end())
	//	{
	//		//û���ҵ�ȥ������������
	//		continue;
	//	}
	//	sComVec = SIter->second;
	//	for (vector<CCommonCell>::iterator ITR = mComVec.begin(); ITR != mComVec.end(); ++ITR)
	//	{
	//		mComCell = *ITR;
	//		strMTexing = mComCell.getRealTexing();
	//		for (vector<CCommonCell>::iterator ISter = sComVec.begin(); ISter != sComVec.end(); ++ISter)
	//		{
	//			sComCell = *ISter;
	//			strSTexing = sComCell.getRealTexing();
	//			if (strMTexing.CompareNoCase(strSTexing) == 0)
	//			{
	//				m_RMMData.insert(make_pair(strSTexing, sComCell.getTempQuxiang()));
	//				m_RSSData.insert(make_pair(strSTexing, mComCell.getTempQuxiang()));
	//			}
	//		}
	//	}
	//}

	for (map<int, vector<CCommonCell> >::iterator iter = sMap.begin(); iter != sMap.end(); ++iter)
	{
		sComVec = iter->second;
		nlen = iter->first;
		nlen++;
		map<int, vector<CCommonCell> >::iterator MIter = mMap.find(nlen);
		if (MIter == mMap.end())
		{
			//û���ҵ�ȥ������������
			nlen = iter->first;
			nlen--;
			MIter = mMap.find(nlen);
			if (MIter == mMap.end())
			{
				//��ô˵��ǰ��û�����ݣ�ȷʵ�Ҳ���
				for (vector<CCommonCell>::iterator IIer = sComVec.begin();  IIer != sComVec.end(); ++IIer)
				{
					curComCell = *IIer;
					strLog = getErrorString(curComCell);
					m_ErrorVec.push_back(strLog);
					bRet = false;
				}
				continue;
			}
		}
		mComVec = MIter->second;
		if (!GetMDpError(mComVec, sComVec))
		{
			bRet = false;
		}
		mComVec.clear();
		sComVec.clear();

		/*for (vector<CCommonCell>::iterator ITR = mComVec.begin(); ITR != mComVec.end(); ++ITR)
		{
			mComCell = *ITR;
			strMTexing = mComCell.getRealTexing();
			bool bIsFind = false;
			for (vector<CCommonCell>::iterator ISter = sComVec.begin(); ISter != sComVec.end(); ++ISter)
			{
				sComCell = *ISter;
				strSTexing = sComCell.getRealTexing();
				if (strMTexing.CompareNoCase(strSTexing) == 0)
				{
					bIsFind = true;
				}
			}
			if (!bIsFind)
			{
				strLog = getErrorString(mComCell);
				m_ErrorVec.push_back(strLog);
				bRet = false;
			}
		}*/
	}
	sMap.clear();
	mMap.clear();
	return bRet;
}

CString CGlobalQuery::getSimpleResult(MDMap dData, CString strTexing, CString strTempQuxiang)
{
	vector<CCommonCell> tmpVec;
	CString strOut;
	CCommonCell preCell,curCell;
	MDMap::iterator iter = dData.find(strTexing);
	if (iter == dData.end())
	{
		return strOut;
	}
	tmpVec = iter->second;
	if (tmpVec.size() != 2)
	{
		return strOut;
	}
	preCell = tmpVec.at(0);
	curCell = tmpVec.at(1);
	if (preCell.getTempQuxiang().CompareNoCase(strTempQuxiang) == 0)
	{
		strOut = curCell.getTempQuxiang();
	}
	else
	{
		strOut = preCell.getTempQuxiang();
	}
	tmpVec.clear();
	return strOut;
}

CString CGlobalQuery::getSpecialResult(MDMap dData, CCommonCell cell, CString strTexing, CString strTempQuxiang, int& nCount)
{
	vector<CCommonCell> tmpVec;
	CString strOut,strTemp;
	CCommonCell preCell,curCell;
	nCount = 1;
	CString strCount = cell.getCount();
	if (!strCount.IsEmpty())
	{
		strOut = getQuxiangByRfNum(cell);
		nCount = MyTransFunc::StringToInt(cell.getCount());
		return strOut;
	}
	for (MDMap::iterator iter = dData.begin(); iter != dData.end(); ++iter)
	{
		strTemp = iter->first;	
		while (m_rfNumVec.find(nCount) != m_rfNumVec.end())
		{
			nCount++;
		}

		if (strTemp.CompareNoCase(strTexing) == 0)
		{
			tmpVec = iter->second;
			if (tmpVec.size() != 2)
			{
				nCount = 0;
				tmpVec.clear();
				break;
			}
			preCell = tmpVec.at(0);
			curCell = tmpVec.at(1);
			if (preCell.getTempQuxiang().CompareNoCase(strTempQuxiang) == 0)
			{
				strOut = curCell.getTempQuxiang();
			}
			else
			{
				strOut = preCell.getTempQuxiang();
			}
			tmpVec.clear();
			break;
		}
		else
		{
			nCount++;
		}
	}
	return strOut;
}

bool CGlobalQuery::doRLevel(CString& strTexing, CCommonCell& cell)
{
	int nTmp2,nTmp1;
	CString strRealTexing = cell.getRealTexing();
	CString strTmp;
	nTmp2 = strTexing.Find(_T("[R"));
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	if (nTmp2 >= 0)
	{
		nTmp1 = strTexing.Find(_T("]"), nTmp2);
		if (nTmp1 > nTmp2)
		{
			strTmp = strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
			int nLen = MyTransFunc::StringToInt(strTmp);
			pair<int, CString> tmpPair;
			tmpPair.first = nLen;
			tmpPair.second = strRealTexing;
			//
			nTmp1 = strTexing.Find(_T("[M]"));
			if (nTmp1 >= 0)
			{
				strTexing.Replace(_T("[M]"), _T(""));
				cell.setType(6);
				tmpVec.push_back(cell);

				pair<map<pair<int, CString>, vector<CCommonCell> >::iterator, bool> ret = m_RMData.insert(make_pair(tmpPair, tmpVec));
				if (!ret.second)
				{
					ret.first->second.push_back(cell);
				}

				//cell.setType(6);
				ret = m_RData.insert(make_pair(tmpPair, tmpVec));
				if (!ret.second)
				{
					ret.first->second.push_back(cell);
				}
			}
			else 
			{
				nTmp1 = strTexing.Find(_T("[S]"));
				if (nTmp1 >= 0)
				{
					strTexing.Replace(_T("[S]"), _T(""));
					cell.setType(6);
					tmpVec.push_back(cell);

					pair<map<pair<int, CString>, vector<CCommonCell> >::iterator, bool> ret = m_RSData.insert(make_pair(tmpPair, tmpVec));
					if (!ret.second)
					{
						ret.first->second.push_back(cell);
					}
				}
				else
				{
					cell.setType(6);
					tmpVec.push_back(cell);

					pair<map<pair<int, CString>, vector<CCommonCell> >::iterator, bool> ret = m_RData.insert(make_pair(tmpPair, tmpVec));
					if (!ret.second)
					{
						ret.first->second.push_back(cell);
					}
				}
			}
			nTmp1 = strTexing.Find(ICD_PROPERTY_DP);
			if (nTmp1 >= 0)
			{
				m_DpInfo.AddData(cell);

				cell.setType(1);
				doDpNumber(cell);
				tmpVec.clear();
				tmpVec.push_back(cell);
				strTexing.Replace(ICD_PROPERTY_DP, _T(""));
				pair<map<pair<int, CString>, vector<CCommonCell> >::iterator, bool> ret = m_RRDpData.insert(make_pair(tmpPair, tmpVec));
				if (!ret.second)
				{
					ret.first->second.push_back(cell);
				}
			}
		}
		else
		{
			nTmp1 = strTexing.Find(_T("[M]"));
			if (nTmp1 >= 0)
			{
				strTexing.Replace(_T("[M]"), _T(""));
				nTmp2 = strTexing.Find(_T("[R"));
				nTmp1 = strTexing.Find(_T("]"), nTmp2);
				strTmp = strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
				int nLen = MyTransFunc::StringToInt(strTmp);
				pair<int, CString> tmpPair;
				tmpPair.first = nLen;
				tmpPair.second = strRealTexing;
				cell.setType(6);
				tmpVec.push_back(cell);

				pair<map<pair<int, CString>, vector<CCommonCell> >::iterator, bool> ret = m_RMData.insert(make_pair(tmpPair, tmpVec));
				if (!ret.second)
				{
					ret.first->second.push_back(cell);
				}

				ret = m_RData.insert(make_pair(tmpPair, tmpVec));
				if (!ret.second)
				{
					ret.first->second.push_back(cell);
				}

				nTmp1 = strTexing.Find(ICD_PROPERTY_DP);
				if (nTmp1 >= 0)
				{
					m_DpInfo.AddData(cell);

					cell.setType(1);
					doDpNumber(cell);
					tmpVec.clear();
					tmpVec.push_back(cell);
					strTexing.Replace(ICD_PROPERTY_DP, _T(""));
					ret = m_RRDpData.insert(make_pair(tmpPair, tmpVec));
					if (!ret.second)
					{
						ret.first->second.push_back(cell);
					}
				}
			}
			else 
			{
				nTmp1 = strTexing.Find(_T("[S]"));
				if (nTmp1 >= 0)
				{
					strTexing.Replace(_T("[S]"), _T(""));
					nTmp2 = strTexing.Find(_T("[R"));
					nTmp1 = strTexing.Find(_T("]"), nTmp2);
					strTmp = strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
					int nLen = MyTransFunc::StringToInt(strTmp);
					pair<int, CString> tmpPair;
					tmpPair.first = nLen;
					tmpPair.second = strRealTexing;
					cell.setType(6);
					tmpVec.push_back(cell);

					pair<map<pair<int, CString>, vector<CCommonCell> >::iterator, bool> ret = m_RSData.insert(make_pair(tmpPair, tmpVec));
					if (!ret.second)
					{
						ret.first->second.push_back(cell);
					}
					
					nTmp1 = strTexing.Find(ICD_PROPERTY_DP);
					if (nTmp1 >= 0)
					{
						m_DpInfo.AddData(cell);

						cell.setType(1);
						doDpNumber(cell);
						tmpVec.clear();
						tmpVec.push_back(cell);
						int nFind = strTexing.Find(ICD_PROPERTY_DP_EQUAL);
						if (nFind >= 0)
						{
							strTexing = CUtility::RemoveString(strTexing, ICD_PROPERTY_DP_EQUAL);
						}
						else
							strTexing = CUtility::RemoveString(strTexing, ICD_PROPERTY_DP);
						ret = m_RRDpData.insert(make_pair(tmpPair, tmpVec));
						if (!ret.second)
						{
							ret.first->second.push_back(cell);
						}
					}
				}
			}
		}
	}
	tmpVec.clear();
	return true;
}

bool CGlobalQuery::doMTRLevel(CString& strTexing, CCommonCell& cell)
{
	int nTmp2,nTmp1;
	CString strRealTexing = cell.getRealTexing();
	CString strTmp;
	nTmp2 = strTexing.Find(_T("[R"));
	vector<CCommonCell> tmpVec;
	tmpVec.clear();
	if (nTmp2 >= 0)
	{
		nTmp1 = strTexing.Find(_T("]"), nTmp2);
		if (nTmp1 > nTmp2)
		{
			strTmp = strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
			int nLen = MyTransFunc::StringToInt(strTmp);
			pair<int, CString> tmpPair;
			tmpPair.first = nLen;
			tmpPair.second = strRealTexing;

			cell.setType(3);
			tmpVec.push_back(cell);
			pair<map<pair<int, CString>, vector<CCommonCell> >::iterator, bool> ret = m_RMTData.insert(make_pair(tmpPair, tmpVec));
			if (!ret.second)
			{
				ret.first->second.push_back(cell);
			}
		}
	}
	tmpVec.clear();
	return true;
}

map<int, pair<CCommonCell, CCommonCell> > CGlobalQuery::getDpOutPutData()
{
	map<int, pair<CCommonCell, CCommonCell> > dpoutPutMap;
	dpoutPutMap.clear();
	pair<CCommonCell, CCommonCell> tmpPair;
	int nIndex = 0;
	COMVEC comVec;
	comVec.clear();
	//��ȡDP����
	CCommonCell preCell,curCell;
	for (map<CString, COMVEC>::iterator iter = m_dpNumInfo.begin(); iter != m_dpNumInfo.end(); ++iter)
	{
		comVec = iter->second;

		if (comVec.size() < 2)
		{
			comVec.clear();
			continue;
		}
		else
		{
			preCell = comVec.at(0);
			curCell = comVec.at(1);
			tmpPair.first = preCell;
			tmpPair.second = curCell;

			nIndex = MyTransFunc::StringToInt(preCell.getCount());
			dpoutPutMap.insert(make_pair(nIndex, tmpPair));
			comVec.clear();
		}
	}
	
	nIndex = 1;
	for(map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator iter = m_RDPRealData.begin(); iter != m_RDPRealData.end(); ++iter)
	{
		vector<pair<CCommonCell, CCommonCell> > tmpVec;
		tmpVec = iter->second;
		while (m_dpNumVec.find(nIndex) != m_dpNumVec.end())
		{
			nIndex++;
		}
		for (vector<pair<CCommonCell, CCommonCell> >::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
		{
			tmpPair = *Itr;
			dpoutPutMap.insert(make_pair(nIndex, tmpPair));
			nIndex++;
		}
		tmpVec.clear();
	}
	return dpoutPutMap;

}

map<int, pair<CCommonCell, CCommonCell> > CGlobalQuery::getRFOutPutData()
{
	map<int, pair<CCommonCell, CCommonCell> > dpoutPutMap;
	dpoutPutMap.clear();
	pair<CCommonCell, CCommonCell> tmpPair;
	int nIndex = 1;
	COMVEC comVec;
	comVec.clear();
	//��ȡDP����
	CCommonCell preCell,curCell;
	for (map<CString, COMVEC>::iterator iter = m_rfNumInfo.begin(); iter != m_rfNumInfo.end(); ++iter)
	{
		comVec = iter->second;

		if (comVec.size() < 2)
		{
			comVec.clear();
			continue;
		}
		else
		{
			preCell = comVec.at(0);
			curCell = comVec.at(1);
			tmpPair.first = preCell;
			tmpPair.second = curCell;

			nIndex = MyTransFunc::StringToInt(preCell.getCount());
			dpoutPutMap.insert(make_pair(nIndex, tmpPair));
			comVec.clear();
		}
	}

	nIndex = 1;
	for (MDMap::iterator iter = m_RFData.begin(); iter != m_RFData.end(); ++iter)
	{
		while (m_rfNumVec.find(nIndex) != m_rfNumVec.end())
		{
			nIndex++;
		}
		comVec = iter->second;

		if (comVec.size() < 2)
		{
			comVec.clear();
			continue;
		}
		else
		{
			preCell = comVec.at(0);
			curCell = comVec.at(1);
			tmpPair.first = preCell;
			tmpPair.second = curCell;

			dpoutPutMap.insert(make_pair(nIndex, tmpPair));
			comVec.clear();
			nIndex++;
		}
	}
	return dpoutPutMap;
}

map<CString, vector<pair<CCommonCell, CCommonCell> > > CGlobalQuery::getMTOutPutData()
{
	map<CString, vector<pair<CCommonCell, CCommonCell> > > mtOutPtdata;
	mtOutPtdata.clear();
	vector<pair<CCommonCell, CCommonCell> > tmpVec;
	tmpVec.clear();
	pair<CCommonCell, CCommonCell> tmpPair;

	COMVEC tmpComVec;
	tmpComVec.clear();
	CCommonCell preComCell,curComCell;
	CString strCount;
	for (MDMap::iterator iter = m_MTData.begin(); iter != m_MTData.end(); ++iter)
	{
		tmpComVec = iter->second;
		if (tmpComVec.size() != 2)
		{
			continue;
		}
		preComCell = tmpComVec.at(0);
		curComCell = tmpComVec.at(1);

		tmpPair.first = preComCell;
		tmpPair.second = curComCell;

		tmpVec.push_back(tmpPair);
		if (preComCell.getCount().CompareNoCase(curComCell.getCount()) == 0)
		{
			strCount = preComCell.getCount();
		}
		else
		{
			strCount = preComCell.getCount() + _T("-") + curComCell.getCount();
		}
		pair<map<CString, vector<pair<CCommonCell, CCommonCell> > >::iterator, bool> bRet = mtOutPtdata.insert(make_pair(strCount, tmpVec));
		if (!bRet.second)
		{
			bRet.first->second.push_back(tmpPair);
		}
		tmpVec.clear();
	}

	return mtOutPtdata;
}

CString CGlobalQuery::getErrorString(CCommonCell curComCell)
{
	CString strCol,strLog;
	m_ComvecError.push_back(curComCell);
	//strCol.Format(_T("%c"), 64+ curComCell.getCol());
	strCol.Format(_T("%d"), curComCell.getCol());
	strLog.Format(_T("%s,%d,%s,%s,%s,%s"), curComCell.getSheetName(),curComCell.getRow(), strCol, curComCell.getTexing(), curComCell.getRealTexing(), m_strErrorType);

	CIgnoreErrorCell ignoreCell;
	ignoreCell.set(curComCell.getSheetName(), MyTransFunc::intToStr(curComCell.getRow()), strCol, curComCell.getTexing(), curComCell.getRealTexing(), m_strErrorType);
	m_IgnoreVec.push_back(ignoreCell);

	return strLog;
}

void CGlobalQuery::setErrorType(CString strErrorType)
{
	m_strErrorType = strErrorType;
}

bool CGlobalQuery::drawLrmvModData()
{
	int nSize = m_LrmvModSheetData.size();
	int i =0;
	for (vector<CSheetStoreData>::iterator iter = m_LrmvModSheetData.begin(); iter != m_LrmvModSheetData.end(); ++iter)
	{
		CSheetStoreData storeData = *iter;
		CDrawLrmvEnt drawEnt;
		drawEnt.setModFlag(true);
		drawEnt.setData(storeData);
		if (i == m_LrmvModSheetData.size() - 1)
		{
			drawEnt.setLastDrawFlag(true);
		}
		drawEnt.draw();
		i++;
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(70+15*i/nSize);
		}
		storeData.clear();
	}
	return true;
}

bool CGlobalQuery::drawLrmvData()
{
	int nSize = m_LrmvSheetData.size();
	int i =0;
	for (vector<CSheetStoreData>::iterator iter = m_LrmvSheetData.begin(); iter != m_LrmvSheetData.end(); ++iter)
	{
		CSheetStoreData storeData = *iter;
		CDrawLrmvEnt drawEnt;
		drawEnt.setData(storeData);

		drawEnt.draw();
		i++;
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(20+30*i/nSize);
		}
		storeData.clear();
	}
	return true;
}

void CGlobalQuery::drawDigitalData()
{
	//�������ֹ⣬ģ���
	int nSize = m_DigitalSheetData.size();
	int i =0;
	for (vector<CSheetStoreData>::iterator iter = m_DigitalSheetData.begin(); iter != m_DigitalSheetData.end(); ++iter)
	{
		CSheetStoreData storeData = *iter;
		CDrawDigitalOptical drawEnt;
		drawEnt.setData(storeData);
		drawEnt.draw();
		i++;
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(50+20*i/nSize);
		}
		storeData.clear();
	}
}

bool CGlobalQuery::drawCommonData()
{
	int nSize = m_nameVec.size();
	int i=0;
	CDrawEntity drawEnt;
	vector<CGLData> glVecData;
	for (vector<CString>::iterator iter = m_nameVec.begin(); iter != m_nameVec.end(); ++iter)
	{
		CString strName;
		strName = *iter;
		map<CString, CSingleSheetData>::iterator Itr = m_datainfo.find(strName);
		if (Itr != m_datainfo.end())
		{
			CSingleSheetData singleData = Itr->second;

			drawEnt.Init();
			drawEnt.setSheetName(strName);
			if (singleData.getMapInfo().size() > 0)
			{
				drawEnt.drawFrame();	
				drawEnt.draw(singleData.getMapInfo());
			}
			glVecData = singleData.getSheetStoreData().getGLData();
			if (glVecData.size() > 0)
			{
				CDrawDigitalOptical drawDg;
				drawDg.setData(singleData.getSheetStoreData());
				drawDg.draw();
			}
			glVecData.clear();
			singleData.clear();
		}
		//acedSetStatusBarProgressMeterPos(30+i*40/nSize);
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(i*20/nSize);
		}
		i++;
	}

	return true;
}


bool CGlobalQuery::exchangeTexingCommonData()
{
	//���������Ҫ�޸ĵ� ��� 
	AcDbObjectIdArray idArr = CBaseUtils::GetObjectIdArrayForXdata(ICD_XDATA_XIUGAI);

	int nSize = m_nameVec.size();
	int i = 0;
	for (vector<CString>::iterator iter = m_nameVec.begin(); iter != m_nameVec.end(); ++iter)
	{
		CString strName;
		strName = *iter;
		map<CString, CSingleSheetData>::iterator Itr = m_datainfo.find(strName);
		//�ҵ�Ҫexcel����Ϣ
		if (Itr != m_datainfo.end())
		{
			CSingleSheetData singleData = Itr->second;
			map<CNameCell,vector<CCommonCell>> mapCell=singleData.getMapInfo();
			vector<CCommonCell> comVec;
			for (map<CNameCell, vector<CCommonCell>>::iterator iter = mapCell.begin(); iter != mapCell.end();++iter)
			{
				comVec = iter->second;
				for (int j = 0; j < comVec.size();j++)
				{
					CCommonCell temp = comVec[j];
					getQuxiang(temp);
					CString comSheet,comQuxiang;
					int comRow, comCol;
					comSheet = temp.getSheetName();
					comQuxiang = temp.getQuxiang();
					comRow = temp.getRow();
					comCol = temp.getCol();
					//ͨ����Ϣ����id
					for (int k = 0; k < idArr.length();k++)
					{
						CString sheet;
						int nRow, nCol;
						MyBaseAppXdata::getXdata(ICD_XDATA_SHEET, sheet, idArr[k]);
						MyBaseAppXdata::getXdata(ICD_XDATA_ROW, nRow, idArr[k]);
						MyBaseAppXdata::getXdata(ICD_XDATA_COL, nCol, idArr[k]);
						if (comSheet.CompareNoCase(sheet)==0&&comRow==nRow&&comCol==nCol)
						{
							bool bEdit = false;
							
							AcDbObjectIdArray idArrTemp = CBaseUtils::GetObjectIdArrayForRange(CBaseUtils::GetExtentsForEntity(idArr[k]));
							for (int l = 0; l < idArrTemp.length();l++)
							{
								CString sTag;
								MyBaseAppXdata::getXdata(ICD_XDATA_QUXIANGTAG, sTag, idArrTemp[l]);
								if (sTag != _T("��"))
								{
									continue;
								}
								AcDbEntity *pEnt = NULL;
								Acad::ErrorStatus es;
								es = acdbOpenAcDbEntity(pEnt, idArrTemp[l], AcDb::kForWrite);
								if (es != eOk)
								{
									continue;
								}
								if (!pEnt->isKindOf(AcDbText::desc()))
								{
									pEnt->close();
									continue;
								}
								AcDbText *pText = static_cast<AcDbText*>(pEnt);
								pText->setTextString(comQuxiang);
								pText->close();
								MyBaseAppXdata::setXdata(ICD_XDATA_TEXING, comQuxiang, idArrTemp[l]);
								bEdit = true;
								break;
							}
							//���û���޸� ��ȥ��Ϊ�� ����Ҫ���ȥ��
							if (!bEdit&&comQuxiang!=_T(""))
							{
								AcGePoint3d textPt3;
								AcGePoint3d m_basePt = CBaseUtils::GetBlockRefPostion(idArr[k]);
								AcDbObjectId m_layerId = CBaseUtils::GetEntityLayer(idArr[k]);

								double m_dFirstTableLength, m_dSecondTableLength, m_dThirdTableLength, m_dTableHeight;
								m_dTableHeight = gGlobal.GetIniValue(_T("��������"), _T("���߶�"), 0);
								m_dFirstTableLength = gGlobal.GetIniValue(_T("��������"), _T("����һ�γ���"), 0);
								m_dSecondTableLength = gGlobal.GetIniValue(_T("��������"), _T("���ڶ��γ���"), 0);
								m_dThirdTableLength = gGlobal.GetIniValue(_T("��������"), _T("�������γ���"), 0);

								textPt3.x = m_basePt.x + m_dFirstTableLength + m_dSecondTableLength + m_dThirdTableLength / 2;
								textPt3.y = m_basePt.y + m_dTableHeight / 2;
								textPt3.z = 0;

								AcDbObjectId textId;

								textId = MyDrawEntity::DrawText(textPt3, comQuxiang, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
								//xzh ȥ��text��д��ȥ����
								MyBaseAppXdata::setXdata(ICD_XDATA_QUXIANGTAG, _T("��"), textId);
								MyBaseAppXdata::setXdata(ICD_XDATA_QUXIANG, comQuxiang, textId);

								double dLen = MyEditEntity::OpenObjAndGetLength(textId);
								if (dLen > m_dThirdTableLength)
								{
									double dScale = 3.0*m_dThirdTableLength / (dLen + 1);
									textId = MyEditEntity::openTextChangeTextHeight(textId, dScale);
								}
								textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
							}
						}
					}
				}
			}
		}
		//acedSetStatusBarProgressMeterPos(30+i*40/nSize);
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(i * 20 / nSize);
		}
		i++;
	}

	return true;
}

bool CGlobalQuery::drawTu()
{
	CDrawEntity drawEnt;
	if (m_blkNameVec.size() > 0)
	{
		drawEnt.Init();
		drawEnt.drawTu(m_blkNameVec);
		modelessDlg->setPos(88);
	}
	return true;
}

bool CGlobalQuery::writeErrorLog()
{
	bool bIsFirst = true;
	m_log.setPath(CUtility::getLogPath());
	for (vector<CString>::iterator iter = m_ErrorVec.begin(); iter != m_ErrorVec.end(); ++iter)
	{	
		if (bIsFirst)
		{
			m_log.writeCsvLog(_T("sheet����,��,��,Excel������,����,��ע"));
			bIsFirst = false;
		}
		m_log.writeCsvLog(*iter);
	}
	return true;
}

void CGlobalQuery::addLrmvSheetData(CSheetStoreData data)
{
	m_LrmvSheetData.push_back(data);
}

vector<CSheetStoreData> CGlobalQuery::getLRMVSheetData()
{
	return m_LrmvSheetData;
}

void CGlobalQuery::addLrmvModSheetData(CSheetStoreData data)
{
	m_LrmvModSheetData.push_back(data);
}

vector<CSheetStoreData> CGlobalQuery::getLRMVModSheetData()
{
	return m_LrmvModSheetData;
}

void CGlobalQuery::addDigitalSheetData(CSheetStoreData data)
{
	m_DigitalSheetData.push_back(data);
}

vector<CSheetStoreData> CGlobalQuery::getDigitalSheetData()
{
	return m_DigitalSheetData;
}

void CGlobalQuery::addCommonSheetData(CString strSheet, CSingleSheetData data)
{
	m_datainfo.insert(make_pair(strSheet, data));
}

map<CString, CSingleSheetData> CGlobalQuery::getCommonData()
{
	return m_datainfo;
}

void CGlobalQuery::addModNameItem(CString str)
{
	m_modNameVec.push_back(str);
}

void CGlobalQuery::addSheetNameItem(CString str)
{
	m_nameVec.push_back(str);
}

void CGlobalQuery::addBlockName(CString str)
{
	m_blkNameVec.push_back(str);
}

bool CGlobalQuery::draw()
{
	//0~20	
	drawCommonData();
	//20~50
	drawLrmvData();
	//50~70
	drawDigitalData();
	//70~85
	drawLrmvModData();
	//85~90
	drawTu();
	return true;
}


void CGlobalQuery::AddRSToRData()
{
	//��R+S������R�д���һ��
	pair<int, CString> tmpPair;
	COMVEC comVec,tmpVec;
	CCommonCell comCell;
	for (MRMAP::iterator iter = m_RSData.begin(); iter != m_RSData.end(); ++iter)
	{
		tmpPair = iter->first;
		comVec = iter->second;
		if (comVec.size() > 0)
		{
			comCell = comVec.at(0);
			tmpVec.clear();
			tmpVec.push_back(comCell);
			pair<MRMAP::iterator, bool> bRet = m_RData.insert(make_pair(tmpPair, tmpVec));
			if (!bRet.second)
			{
				bRet.first->second.push_back(comCell);
			}
		}
	}
}

map<CString, vector<pair<CCommonCell, CCommonCell> > > CGlobalQuery::doMTInfoToMap()
{
	//����MT����
	map<CString, vector<pair<CCommonCell, CCommonCell> > > mapInfo;
	vector<pair<CCommonCell, CCommonCell> > tmpVec;
	tmpVec.clear();
	pair<CCommonCell, CCommonCell> tmpPairVec;
	COMVEC comVec;
	comVec.clear();
	CCommonCell preComCell,curComCell;
	vector<pair<CCommonCell, CCommonCell> > TempVec;

	for (MDMap::iterator iter = m_MTData.begin(); iter != m_MTData.end(); ++iter)
	{
		comVec = iter->second;
		if (comVec.size() < 2)
		{
			continue;
		}
		preComCell = comVec[0];
		curComCell = comVec[1];
		comVec.clear();

		tmpPairVec.first = preComCell;
		tmpPairVec.second = curComCell;
		tmpVec.clear();
		tmpVec.push_back(tmpPairVec);
		pair<map<CString,  vector<pair<CCommonCell, CCommonCell> > >::iterator, bool> ret = mapInfo.insert(make_pair(preComCell.getCount(), tmpVec));
		if (!ret.second)
		{
			TempVec.clear();
			TempVec = ret.first->second;
			bool bIsFind = false;
			pair<CCommonCell, CCommonCell> pairTemp;
			for (vector<pair<CCommonCell, CCommonCell> >::iterator IIIIR = TempVec.begin(); IIIIR != TempVec.end(); ++IIIIR)
			{
				pairTemp = *IIIIR;
				if (((pairTemp.first.IsEqual(tmpPairVec.first)) &&(pairTemp.second.IsEqual(tmpPairVec.second)))
					||((pairTemp.second.IsEqual(tmpPairVec.first)) &&(pairTemp.first.IsEqual(tmpPairVec.second))))
				{
					bIsFind = true;
					break;
				}
			}
			if (!bIsFind)
			{
				ret.first->second.push_back(tmpPairVec);
			}
			TempVec.clear();
		}
		
		tmpPairVec.first = curComCell;
		tmpPairVec.second = preComCell;
		tmpVec.clear();
		tmpVec.push_back(tmpPairVec);
		ret = mapInfo.insert(make_pair(curComCell.getCount(), tmpVec));
		if (!ret.second)
		{
			TempVec.clear();
			TempVec = ret.first->second;
			bool bIsFind = false;
			pair<CCommonCell, CCommonCell> pairTemp;
			for (vector<pair<CCommonCell, CCommonCell> >::iterator IIIIR = TempVec.begin(); IIIIR != TempVec.end(); ++IIIIR)
			{
				pairTemp = *IIIIR;
				if (((pairTemp.first.IsEqual(tmpPairVec.first)) &&(pairTemp.second.IsEqual(tmpPairVec.second)))
					||((pairTemp.second.IsEqual(tmpPairVec.first)) &&(pairTemp.first.IsEqual(tmpPairVec.second))))
				{
					bIsFind = true;
					break;
				}
			}
			if (!bIsFind)
			{
				ret.first->second.push_back(tmpPairVec);
			}
			TempVec.clear();
		}
		tmpVec.clear();
	}
	//���㼶��MT����
	pair<CCommonCell, CCommonCell> jxPair;
	for (vector<pair<CCommonCell, CCommonCell> >::iterator iter = m_RRMTJxData.begin(); iter != m_RRMTJxData.end(); ++iter)
	{
		jxPair = *iter;
		preComCell = jxPair.first;
		curComCell = jxPair.second;

		tmpPairVec.first = preComCell;
		tmpPairVec.second = curComCell;
		tmpVec.clear();
		tmpVec.push_back(tmpPairVec);
		pair<map<CString,  vector<pair<CCommonCell, CCommonCell> > >::iterator, bool> ret = mapInfo.insert(make_pair(preComCell.getCount(), tmpVec));
		if (!ret.second)
		{
			TempVec.clear();
			TempVec = ret.first->second;
			bool bIsFind = false;
			pair<CCommonCell, CCommonCell> pairTemp;
			for (vector<pair<CCommonCell, CCommonCell> >::iterator IIIIR = TempVec.begin(); IIIIR != TempVec.end(); ++IIIIR)
			{
				pairTemp = *IIIIR;
				if (((pairTemp.first.IsEqual(tmpPairVec.first)) &&(pairTemp.second.IsEqual(tmpPairVec.second)))
					||((pairTemp.second.IsEqual(tmpPairVec.first)) &&(pairTemp.first.IsEqual(tmpPairVec.second))))
				{
					bIsFind = true;
					break;
				}
			}
			if (!bIsFind)
			{
				ret.first->second.push_back(tmpPairVec);
			}
			TempVec.clear();
		}

		tmpPairVec.first = curComCell;
		tmpPairVec.second = preComCell;
		tmpVec.clear();
		tmpVec.push_back(tmpPairVec);
		ret = mapInfo.insert(make_pair(curComCell.getCount(), tmpVec));
		if (!ret.second)
		{
			TempVec.clear();
			TempVec = ret.first->second;
			bool bIsFind = false;
			pair<CCommonCell, CCommonCell> pairTemp;
			for (vector<pair<CCommonCell, CCommonCell> >::iterator IIIIR = TempVec.begin(); IIIIR != TempVec.end(); ++IIIIR)
			{
				pairTemp = *IIIIR;
				if (((pairTemp.first.IsEqual(tmpPairVec.first)) &&(pairTemp.second.IsEqual(tmpPairVec.second)))
					||((pairTemp.second.IsEqual(tmpPairVec.first)) &&(pairTemp.first.IsEqual(tmpPairVec.second))))
				{
					bIsFind = true;
					break;
				}
			}
			if (!bIsFind)
			{
				ret.first->second.push_back(tmpPairVec);
			}
			TempVec.clear();
		}
		tmpVec.clear();
	}

	return mapInfo;
}

bool CGlobalQuery::CallIgnoreDlg(CString strPrompt)
{	
	if (m_IgnoreVec.size() < 1)
	{
		return true;
	}
	endDlg();
	CAcModuleResourceOverride rs;
	CDlgListError dlg;
	dlg.setErrorData(strPrompt, m_IgnoreVec);
	if (dlg.DoModal() != IDOK)
	{
		return false;
	}
	m_IgnoreVec.clear();
	return true;
}

CString CGlobalQuery::getRLevelQuxiang(CCommonCell cell)
{
	CString strOut;
	CString strTexing;
	strTexing = cell.getTexing();
	int nTmp2 = strTexing.Find(_T("[R"));
	int nTmp1 = strTexing.Find(_T("]"), nTmp2);
	CString strTmp = strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
	int nLen = MyTransFunc::StringToInt(strTmp);
	pair<int, CString> tmpPair;
	tmpPair.first = nLen;
	tmpPair.second = cell.getRealTexing();

	map<pair<int, CString>, CCommonCell>::iterator iter = m_RRData.find(tmpPair);
	if (iter == m_RRData.end())
	{
		return strOut;
	}
	CCommonCell curComCell;
	CString strFix = gGlobal.GetIniValue(_T("��������"), _T("�㼶�ָ���"));
	curComCell = iter->second;
	if (curComCell.getPreQuxiang().IsEmpty())
	{
		strOut = curComCell.getQuxiang();
	}
	else
	{
		if (curComCell.getQuxiang().IsEmpty())
		{
			strOut = curComCell.getPreQuxiang();
		}
		else
		{
			if (curComCell.getPreQuxiang().CompareNoCase(curComCell.getQuxiang()) == 0)
			{
				strOut = curComCell.getQuxiang();
			}
			else
			{
				strOut = curComCell.getPreQuxiang() + strFix + curComCell.getQuxiang();
			}
		}
	}
	return strOut;
}

CString CGlobalQuery::getRMTLevelQuxiang(CCommonCell cell)
{
	CString strOut;
	CString strTexing;
	strTexing = cell.getTexing();
	int nTmp2 = strTexing.Find(_T("[R"));
	int nTmp1 = strTexing.Find(_T("]"), nTmp2);
	CString strTmp = strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
	int nLen = MyTransFunc::StringToInt(strTmp);
	pair<int, CString> tmpPair;
	tmpPair.first = nLen;
	tmpPair.second = cell.getRealTexing();

	map<pair<int, CString>, CCommonCell>::iterator iter = m_RRMTData.find(tmpPair);
	if (iter == m_RRMTData.end())
	{
		return strOut;
	}
	CCommonCell curComCell;
	CString strFix = gGlobal.GetIniValue(_T("��������"), _T("�㼶�ָ���"));
	curComCell = iter->second;
	if (curComCell.getPreQuxiang().IsEmpty())
	{
		strOut = curComCell.getQuxiang();
	}
	else
	{
		if (curComCell.getQuxiang().IsEmpty())
		{
			strOut = curComCell.getPreQuxiang();
		}
		else
		{
			if (curComCell.getPreQuxiang().CompareNoCase(curComCell.getQuxiang()) == 0)
			{
				strOut = curComCell.getQuxiang();
			}
			else
			{
				strOut = curComCell.getPreQuxiang() + strFix + curComCell.getQuxiang();
			}
		}
	}
	return strOut;
}

CString CGlobalQuery::getDpSpecialResult(CCommonCell& cell)
{
	CString strNum,strOut,strTmp,strPre;
	bool bIsLevel;
	strTmp = getDpCmpString(cell, strPre, bIsLevel);
	if (strPre.CompareNoCase(strTmp) == 0)
	{
		//�����ͬ��ʾ����Ҫ����ǰ��������в�������
		strOut = FindDataFromRDpData(cell, strTmp);
	}
	else
	{	
		//�ȴ�ǰ��������в���һ��
		FindDataFromRDpData(cell, strPre);

		strOut = FindDataFromRDpData(cell, strTmp);
	}


	
	return strOut;
}

CString CGlobalQuery::getDpCmpString(CCommonCell cell, CString& strPre, bool& bIsLevel)
{
	bIsLevel = false;
	CString strTmp;
	CString strTexing = cell.getTexing();
	int nTmp2 = strTexing.Find(_T("[R"));
	if (nTmp2 >= 0)
	{
		int nTmp1 = strTexing.Find(_T("]"), nTmp2);
		strTmp  = strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
		int nLevel = MyTransFunc::StringToInt(strTmp);
		if (nLevel == m_RDplevel)//������Ϊ���һ�������ʱ������һ���㼶�в�������
		{
			nLevel--;
			strTmp.Format(_T("%d"), nLevel);
		}	
		else if (nLevel == 1)
		{
			//����ǵ�һ�㲻��Ҫ����
			strTmp.Format(_T("%d"), nLevel);
			strPre = strTmp;
		}
		else
		{
			//������м�㣬��Ҫ
			strTmp.Format(_T("%d"), nLevel);
			strPre.Format(_T("%d"), nLevel-1);
			
		}
		bIsLevel = true;
	}
	else
	{
		//���û��R�㼶��������ͨ���ݣ�����Ҫ��ǰ׺
		strTmp = _T("");
		strPre = _T("");
	}
	CString strRealTexing = cell.getRealTexing();

	strTmp += strRealTexing;
	strPre += strRealTexing;
	return strTmp;
}

CString CGlobalQuery::FindDataFromRDpData(CCommonCell& cell, CString strTmp)
{
	CString strOut,strNum,strCurTexing, strTempNum;
	int nCount = 1;//��1��ʼ����
	vector<pair<CCommonCell, CCommonCell> > tmpVec;
	CCommonCell preCell, curCell;
	bool biscontinued = true;
	CString strFix = gGlobal.GetIniValue(_T("��������"), _T("�㼶�ָ���"));
	CString strCount = cell.getCount();
	if (!strCount.IsEmpty())
	{
		strOut = getQuxiangByDpNum(cell);
		return strOut;
	}

	for (map<CString, vector<pair<CCommonCell, CCommonCell> >>::iterator iter = m_RDPRealData.begin();
		iter != m_RDPRealData.end(); ++iter)
	{
		strCurTexing = iter->first;
		tmpVec = iter->second;	
		int nSize = tmpVec.size();
		while (m_dpNumVec.find(nCount) != m_dpNumVec.end())
		{
			nCount++;
		}

		if (strTmp.CompareNoCase(strCurTexing) == 0)
		{
			for (vector<pair<CCommonCell, CCommonCell> >::iterator Ir = tmpVec.begin(); Ir != tmpVec.end(); ++Ir)
			{
				pair<CCommonCell, CCommonCell> tmpPair = *Ir;
				preCell = tmpPair.first;
				curCell = tmpPair.second;

					if (cell.IsEqual(preCell))//˵�����vector�����е�����������cell
					{
						CString strTemp;
						CString strTexing = cell.getTexing();
						int nFind = strTexing.Find(_T("[M]"));
						if (nFind >= 0)
						{
							for (int i=0; i<nSize; i++)
							{
								strTemp.Format(_T("%d"), i+nCount);
								strNum+= strTemp;
								if (i!= nSize-1)
								{
									//��ӷָ�����Ĭ��Ϊ���Ժ����ʵ��ʹ������޸�
									strNum += _T(",");
								}
							}
						}
						else
							strNum.Format(_T("%d"), nCount);

						if (cell.getQuxiang().IsEmpty())
						{
							strOut = preCell.getQuxiang();
							cell.setQuxiang(strOut);
						}
						else
						{
							if (preCell.getQuxiang().IsEmpty())
							{
								strOut = cell.getQuxiang();
							}
							else
							{
								if (cell.getQuxiang().CompareNoCase(preCell.getQuxiang()) == 0)
								{
									strOut = preCell.getQuxiang();
								}
								else
								{
									strOut = cell.getQuxiang() + strFix + preCell.getQuxiang();
								}
							}
						}

						strTempNum = cell.getCount();
						if (strTempNum.IsEmpty())//���Ϊ��ֱ�����ã������Ϊ�գ���Ҫ��������ӵ�����
						{
							cell.setCount(strNum);
						}
						else
						{
							strTempNum+= _T(",");
							strTempNum+= strNum;
							cell.setCount(strTempNum);
						}
						biscontinued = false;
						break;
					}
					else
					{
						//��ô��Ҫ�ҵ������curcell��Ӧ������
						if (cell.IsEqual(curCell))
						{
							while (m_dpNumVec.find(nCount) != m_dpNumVec.end())
							{
								nCount++;
							}
							strNum.Format(_T("%d"), nCount);

							if (cell.getQuxiang().IsEmpty())
							{
								strOut = preCell.getTempQuxiang();
								cell.setQuxiang(strOut);
							}
							else
							{
								if (preCell.getTempQuxiang().IsEmpty())
								{
									strOut = cell.getQuxiang();
								}
								else
								{
									if (preCell.getTempQuxiang().CompareNoCase(cell.getQuxiang()) == 0)
									{
										strOut = preCell.getTempQuxiang();
									}
									else
									{
										strOut = preCell.getTempQuxiang() + strFix + cell.getQuxiang();
									}
								}
							}

							strTempNum = cell.getCount();
							if (strTempNum.IsEmpty())//���Ϊ��ֱ�����ã������Ϊ�գ���Ҫ��������ӵ�����
							{
								cell.setCount(strNum);
							}
							else
							{
								strTempNum+= _T(",");
								strTempNum+= strNum;
								cell.setCount(strTempNum);
							}
							biscontinued = false;
							//break;
						}
					}

				nCount++;
			}	
			if (!biscontinued)
			{
				tmpVec.clear();
				return strOut;
			}
		}
		else
		{
			if (biscontinued)
			{
				nCount += nSize;
			}
		}	
		tmpVec.clear();
	}
	return strOut;
}

bool CGlobalQuery::GetMDpError(vector<CCommonCell> mComVec, vector<CCommonCell> sComVec)
{
	bool bRet = true;
	CCommonCell mComCell,SComCell;
	CString strLog;
	bool bIsFind = false;
	COMVEC tmpErroVec, tmpNoFindErrorVec;
	CString strMRealTexing,strSRealTexing;
	tmpErroVec.clear();
	tmpNoFindErrorVec.clear();
	for (COMVEC::iterator Itr = mComVec.begin(); Itr != mComVec.end(); ++Itr)
	{
		mComCell = *Itr;//����ж����˵����ǰ���Ѿ���ʾ����
		int nFind = mComCell.getTexing().Find(ICD_PROPERTY_DP);
		strMRealTexing = mComCell.getRealTexing();
		if (nFind >= 0)
		{
			//�����DP��˵����Ӧ������S�Ա�����DP
			for (vector<CCommonCell>::iterator iter = sComVec.begin(); iter != sComVec.end(); ++iter)
			{
				SComCell = *iter;
				strSRealTexing = SComCell.getRealTexing();
				if (strMRealTexing.CompareNoCase(strSRealTexing) == 0)
				{
					nFind = SComCell.getTexing().Find(ICD_PROPERTY_DP);
					if (nFind >= 0)
					{
						//����У�˵����ȷ
						bIsFind = true;
						tmpNoFindErrorVec.push_back(SComCell);
					}
					else
					{
						//���û�У�˵����Ҫ��ʾ����
						tmpErroVec.push_back(SComCell);		
						bIsFind = false;
					}
				}
				else
				{
					continue;
				}
			}

			//Ҫôȫ��DP
			if (bIsFind)
			{
				if (tmpNoFindErrorVec.size() > 0)
				{
					for (COMVEC::iterator iter = tmpErroVec.begin(); iter != tmpErroVec.end(); ++iter)
					{
						SComCell = *iter;
						m_strErrorType = _T("û�н�M��Ӧ������S���ݶ�����DP");
						strLog = getErrorString(SComCell);
						m_ErrorVec.push_back(strLog);
						bRet = false;
					}
				}		
			}
			else//Ҫôȫ����dp
			{
				if (tmpErroVec.size() > 0)
				{
					for (COMVEC::iterator iter = tmpNoFindErrorVec.begin(); iter != tmpNoFindErrorVec.end(); ++iter)
					{
						SComCell = *iter;
						m_strErrorType = _T("û�н�M��Ӧ������S���ݶ�����DP");
						strLog = getErrorString(SComCell);
						m_ErrorVec.push_back(strLog);
						bRet = false;
					}
				}	
			}	
			tmpErroVec.clear();
			tmpNoFindErrorVec.clear();
		}
	}
	
	mComVec.clear();
	sComVec.clear();
	return bRet;
}

void CGlobalQuery::doDpNumber(CCommonCell& cell)
{
	CString strTexing = cell.getTexing();
	CString strTemp;
	CString strPre;
	cell.setCount(_T(""));//��Ϊ��
	int nFind = strTexing.Find(ICD_PROPERTY_DP_EQUAL);
	if (nFind >= 0)
	{
		//����ҵ���		
		int nTemp = strTexing.Find(_T("]"), nFind);
		strTemp = strTexing.Mid(nFind+4, nTemp-nFind - 4);
		cell.setCount(strTemp);

		int nCount = MyParserString::GetPileLength(strTemp, strPre);
		COMVEC tmpVec;
		tmpVec.push_back(cell);

		m_dpNumVec.insert(make_pair(nCount, nCount));

		strTemp = cell.getRealTexing();
		pair<map<CString, COMVEC>::iterator, bool> ret = m_dpNumInfo.insert(make_pair(strTemp, tmpVec));
		if (!ret.second)
		{
			ret.first->second.push_back(cell);
		}
	}
}

CString CGlobalQuery::getQuxiangByDpNum(CCommonCell& cell)
{
	CString strTexing = cell.getRealTexing();
	COMVEC tmpVec;
	CString strOut;
	map<CString, COMVEC>::iterator iter = m_dpNumInfo.find(strTexing);
	if (iter == m_dpNumInfo.end())
	{
		return strOut;
	}
	tmpVec = iter->second;
	if (tmpVec.size() != 2)
	{
		//����������
		return strOut;
	}

	CCommonCell preCell, curCell;
	preCell = tmpVec.at(0);
	curCell = tmpVec.at(1);
	if (cell.IsEqual(preCell))
	{
		strOut = curCell.getTempQuxiang();
	}
	else if (cell.IsEqual(curCell))
	{
		strOut = preCell.getTempQuxiang();
	}

	return strOut;
}

CString CGlobalQuery::getQuxiangByRfNum(CCommonCell& cell)
{
	CString strTexing = cell.getRealTexing();
	COMVEC tmpVec;
	CString strOut;
	map<CString, COMVEC>::iterator iter = m_rfNumInfo.find(strTexing);
	if (iter == m_rfNumInfo.end())
	{
		return strOut;
	}
	tmpVec = iter->second;
	if (tmpVec.size() != 2)
	{
		//����������
		return strOut;
	}

	CCommonCell preCell, curCell;
	preCell = tmpVec.at(0);
	curCell = tmpVec.at(1);
	if (cell.IsEqual(preCell))
	{
		strOut = curCell.getTempQuxiang();
	}
	else if (cell.IsEqual(curCell))
	{
		strOut = preCell.getTempQuxiang();
	}

	return strOut;
}

bool CGlobalQuery::getEditFrameFlag()
{
	if ((m_LrmvSheetData.size() == 0)
		&&(m_datainfo.size() == 0)
		&&(m_LrmvModSheetData.size() == 0))
	{
		AfxMessageBox(_T("�����򿪵�dwg�ļ��������޸�ͼ����������excel�ļ����ɵ�·ͼ"));
		return false;
	}
	return true;
}

void CGlobalQuery::addTDataToVec(CCommonCell comCell)
{
	m_TComVec.push_back(comCell);
}

void CGlobalQuery::clearDifferentialSignal()
{
	m_PSignal.clear();
	m_NSignal.clear();
	m_PlusSignal.clear();
	m_MinusSignal.clear();
}

void CGlobalQuery::adddifferSignal(CCommonCell comCell, bool bIsDeFaultRf)
{
	CString strTemp = comCell.getTexing();
	
	CString strItem = comCell.getRealTexing();
	CString strInput = strItem;
	int nFind =0;
	nFind = strItem.ReverseFind(_T('_'));	
	if (nFind >= 0)
	{
		strTemp = strItem.Mid(nFind+1, 1);
		CString strLast = strItem.Right(strItem.GetLength()-nFind - 2);
		if (strTemp.CompareNoCase(_T("N")) == 0)
		{
			if ((strLast.SpanIncluding(_T("0123456789")) == strLast) || strLast.IsEmpty())
			{
				strItem.Replace(_T("_N"), _T("_P"));//��N�ĸ�ΪP
				strInput = strItem; 
				addNSignal(strInput, comCell);		
				return;
			}
		}
		else if (strTemp.CompareNoCase(_T("P")) == 0)
		{
			if ((strLast.SpanIncluding(_T("0123456789")) == strLast) || strLast.IsEmpty())
			{
				addPSignal(strInput, comCell);
				return;
			}
		}
		else if (strTemp.CompareNoCase(_T("+")) == 0)
		{
			if ((strLast.SpanIncluding(_T("0123456789")) == strLast) || strLast.IsEmpty())
			{
				strItem.Replace(_T("_+"), _T("_-"));//��N�ĸ�ΪP
				addPlusSignal(strInput, comCell);
				return;
			}
		}
		else if (strTemp.CompareNoCase(_T("-")) == 0)
		{
			if ((strLast.SpanIncluding(_T("0123456789")) == strLast) || strLast.IsEmpty())
			{
				addMinusSignal(strInput, comCell);
				return;
			}
		}
	}

	return;
}

void CGlobalQuery::addPlusSignal(CString strItem, CCommonCell comCell)
{
	m_PlusSignal.insert(make_pair(strItem, comCell));
}

void CGlobalQuery::addMinusSignal(CString strItem, CCommonCell comCell)
{
	m_MinusSignal.insert(make_pair(strItem, comCell));
}

void CGlobalQuery::addPSignal(CString strItem, CCommonCell comCell)
{
	m_PSignal.insert(make_pair(strItem, comCell));
}

void CGlobalQuery::addNSignal(CString strItem, CCommonCell comCell)
{
	m_NSignal.insert(make_pair(strItem, comCell));
}

void CGlobalQuery::doDifferSignal()
{
	//����plus minus�ź�
	if (m_PlusSignal.size() >= m_MinusSignal.size())
	{
		//P�źű�M�źŶ�
		TraversingDifferData(m_PlusSignal, m_MinusSignal);
	}
	else
	{
		TraversingDifferData(m_MinusSignal, m_PlusSignal);
	}

	//����P N�ź�
	if (m_PSignal.size() >= m_NSignal.size())
	{
		TraversingDifferData(m_PSignal, m_NSignal);
	}
	else 
	{
		TraversingDifferData(m_NSignal, m_PSignal);
	}
}

void CGlobalQuery::TraversingDifferData(map<CString, CCommonCell> data1, map<CString, CCommonCell> data2)
{
	CString strItem,strLog;
	for (map<CString, CCommonCell>::iterator iter = data1.begin(); iter != data1.end(); ++iter)
	{
		strItem = iter->first;
		map<CString, CCommonCell>::iterator Itr = data2.find(strItem);
		if (Itr == data2.end())
		{
			m_strErrorType = _T("����źţ�δ�ɶԳ���");
			strLog = getErrorString(iter->second);
			m_ErrorVec.push_back(strLog);
			m_bSignalFlag = true;
		}
	}
}

void CGlobalQuery::setFatalErrorFlag()
{
	m_bFatalError = true;
}

void CGlobalQuery::addExcelInfo(CString strSheetName, vector<vector<CString> > tmpVec)
{
	m_ExcelInfo.insert(make_pair(strSheetName, tmpVec));
	tmpVec.clear();
}

map<CString, vector<vector<CString> > > CGlobalQuery::getExcelInfo() const
{
	return m_ExcelInfo;
}

void CGlobalQuery::EraseFixedNum()
{
	CString strTexting;
	CString strCurTexing;

	for (map<CString, COMVEC>::iterator iter = m_dpNumInfo.begin(); iter != m_dpNumInfo.end(); ++iter)
	{
		strTexting = iter->first;

		map<CString, vector<pair<CCommonCell, CCommonCell> > > ::iterator IIR = m_RDPRealData.find(strTexting);
		if (IIR != m_RDPRealData.end())
		{
			m_RDPRealData.erase(IIR);
		}
	}

	for (map<CString, COMVEC>::iterator iter = m_rfNumInfo.begin(); iter != m_rfNumInfo.end(); ++iter)
	{
		strTexting = iter->first;

		MDMap ::iterator IIR = m_RFData.find(strTexting);
		if (IIR != m_RFData.end())
		{
			m_RFData.erase(IIR);
		}
	}
}

void CGlobalQuery::doRFNumber(CCommonCell& cell)
{
	CString strTexing = cell.getTexing();
	CString strTemp;
	CString strPre;
	cell.setCount(_T(""));//��Ϊ��
	int nFind = strTexing.Find(ICD_PROPERTY_RF_EQUAL);
	if (nFind >= 0)
	{
		//����ҵ���		
		int nTemp = strTexing.Find(_T("]"), nFind);
		strTemp = strTexing.Mid(nFind+4, nTemp-nFind - 4);
		cell.setCount(strTemp);

		int nCount = MyParserString::GetPileLength(strTemp, strPre);
		COMVEC tmpVec;
		tmpVec.push_back(cell);

		m_rfNumVec.insert(make_pair(nCount, nCount));

		strTemp = cell.getRealTexing();
		pair<map<CString, COMVEC>::iterator, bool> ret = m_rfNumInfo.insert(make_pair(strTemp, tmpVec));
		if (!ret.second)
		{
			ret.first->second.push_back(cell);
		}
	}
}


CSingalNo CGlobalQuery::getDpInfo() const
{
	return m_DpInfo;
}

CSingalNo CGlobalQuery::getRfInfo() const
{
	return m_RfInfo;
}

COMVEC CGlobalQuery::getTInfo() const
{
	return m_TComVec;
}

vector<pair<CCommonCell, CCommonCell> > CGlobalQuery::getRMTInfo() const
{
	return m_RRMTJxData;
}

void CGlobalQuery::addFixNumAreaToStore(CString strType, int nStart, int nEnd)
{	
	if (strType.CompareNoCase(ICD_PROPERTY_RF_EQUAL) == 0)
	{
		m_RfInfo.addFixNumAreaToStore(nStart, nEnd);
	}
	else if (strType.CompareNoCase(ICD_PROPERTY_DP_EQUAL) == 0)
	{
		m_DpInfo.addFixNumAreaToStore(nStart, nEnd);
	}
}

bool CGlobalQuery::getPreviewFlag()
{
	return m_bIsPreview;
}

void CGlobalQuery::setPreviewFlag(bool bIsFlag)
{
	m_bIsPreview = bIsFlag;
}
