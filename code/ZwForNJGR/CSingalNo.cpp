#include "StdAfx.h"
#include "CSingalNo.h"
#include "Utility.h"
#include "CGlobalQuery.h"

extern CGlobalQuery gQuery;

CSingalNo::CSingalNo(void)
{
	m_NumInfo.clear();
	m_NumVec.clear();
	m_ListInfo.clear();
	m_Data.clear();
	m_dpoutPutMap.clear();
	m_RDPRealData.clear();
	m_RRDpData.clear();

	m_RData.clear();
	m_tmpRMSInfo.clear();
}


CSingalNo::~CSingalNo(void)
{

}

void CSingalNo::clear()
{
	m_NumInfo.clear();
	m_NumVec.clear();
	m_ListInfo.clear();
	m_Data.clear();
	m_dpoutPutMap.clear();
	m_RDPRealData.clear();
	m_RRDpData.clear();

	m_RData.clear();
	m_tmpRMSInfo.clear();
}

void CSingalNo::setType(CString strType)
{
	m_strType = strType;
	m_strTypeEqual = strType + _T("=");
}

void CSingalNo::AddData(CCommonCell& cell)
{
	CString strTexing = cell.getTexing();
	CString strRealTexing = cell.getRealTexing();
	
	cell.setType(2);
	doNumber(cell);
	int nFind = strTexing.Find(m_strTypeEqual);
	if (nFind >= 0)
		strTexing = CUtility::RemoveString(strTexing, m_strTypeEqual);
	else
		strTexing = CUtility::RemoveString(strTexing, m_strType);

	COMVEC tmpVec;
	int	nTmp2 = strTexing.Find(_T("[R"));
	if (nTmp2 >= 0)
	{
		int nTmp1 = strTexing.Find(_T("]"), nTmp2);
		CString strTmp = strTexing.Mid(nTmp2+2, nTmp1-nTmp2-2);
		int nLen = MyTransFunc::StringToInt(strTmp);
		pair<int, CString> tmpPair;
		tmpPair.first = nLen;
		tmpPair.second = strRealTexing;
		cell.setType(6);
		tmpVec.push_back(cell);

		pair<map<pair<int, CString>, vector<CCommonCell> >::iterator, bool> ret = m_RRDpData.insert(make_pair(tmpPair, tmpVec));
		if (!ret.second)
		{
			ret.first->second.push_back(cell);
		}
		tmpVec.clear();

		tmpVec.push_back(cell);
		pair<MDMap::iterator, bool> Ret = m_RData.insert(make_pair(strRealTexing, tmpVec));
		if (!Ret.second)
		{
			bool bIsFind = FindMultiData(Ret, cell);
			if (!bIsFind)
			{
				Ret.first->second.push_back(cell);
			}
		}
		m_ListInfo.push_back(strRealTexing);
		tmpVec.clear();
	}
	
	if (!strRealTexing.IsEmpty())
	{
		tmpVec.push_back(cell);
		pair<MDMap::iterator, bool> ret = m_Data.insert(make_pair(strRealTexing, tmpVec));
		if (!ret.second)
		{
			bool bIsFind = FindMultiData(ret, cell);
			if (!bIsFind)
				ret.first->second.push_back(cell);
		}

		m_ListInfo.push_back(strRealTexing);
	}		

	tmpVec.clear();
}


void CSingalNo::doNumber(CCommonCell& cell)
{
	CString strTexing = cell.getTexing();
	CString strTemp;
	CString strPre;
	cell.setCount(_T(""));//��Ϊ��
	int nFind = strTexing.Find(m_strTypeEqual);
	if (nFind >= 0)
	{
		//����ҵ���		
		int nTemp = strTexing.Find(_T("]"), nFind);
		strTemp = strTexing.Mid(nFind+4, nTemp-nFind - 4);
		cell.setCount(strTemp);

		//�����,��
		int nCount = 0;
		nFind = strTemp.Find(_T(","));
		if (nFind > 0)
		{
			CString strCurTemp = strTemp;
			while (nFind > 0)
			{
				strPre = strCurTemp.Left(nFind);
				nCount = MyTransFunc::StringToInt(strPre);
				m_NumVec.insert(make_pair(nCount, nCount));

				strCurTemp = strCurTemp.Right(strCurTemp.GetLength() - nFind -1);
				strPre = strCurTemp;
				nFind = strPre.Find(_T(","));
			}
			nCount = MyTransFunc::StringToInt(strPre);
			if (nCount > 0)
			{
				m_NumVec.insert(make_pair(nCount, nCount));
			}
		}
		else
		{
			nCount = MyParserString::GetPileLength(strTemp, strPre);
			m_NumVec.insert(make_pair(nCount, nCount));
		}
		COMVEC tmpVec;
		tmpVec.push_back(cell);

		strTemp = cell.getRealTexing();
		pair<MDMap::iterator, bool> ret = m_NumInfo.insert(make_pair(strTemp, tmpVec));
		if (!ret.second)
		{
			bool bIsFind = FindMultiData(ret, cell);
			if (!bIsFind)
			{
				ret.first->second.push_back(cell);
			}
		}
	}
}

bool CSingalNo::doSpecailData(COMVEC tmpVec, int nCount)
{
	bool bRet = true;
	//R�㼶 
	//M S
	int nTemp = 0;
	CCommonCell comCell;
	CString strTexing;
	bool bIsRLevel = false;
	for (COMVEC::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		comCell = *iter;
		strTexing = comCell.getTexing();
		nTemp = strTexing.Find(_T("[R"));
		if (nTemp >= 0)
		{
			bIsRLevel = true;
		}
	}
	return bRet;
}

bool CSingalNo::doRDpData()
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
					gQuery.setErrorType(_T("�㼶��DP����û��һһƥ��"));
					strLog = gQuery.getErrorString(preComCell);
					gQuery.addErrorString(strLog);
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
						gQuery.setErrorType(_T("�㼶��DP����û��һһƥ��"));
						strLog = gQuery.getErrorString(preComCell);
						gQuery.addErrorString(strLog);
						bRet = false;
					}
				}
			}	
			curVec.clear();
			preVec.clear();
		}
	}
	tmpMap.clear();
	return bRet	;
}



CString CSingalNo::doMSNumAndQuxiang(COMVEC tmpVec, int nCount, CCommonCell& cell)
{
	CString strOut;
	CString strCount;
	CString strNo;
	int nSize = tmpVec.size();
	bool bIsMFlag = cell.getMFlag();
	CCommonCell curCell,preCell;

	COMPAIR tmpPair;
	COMVEC tempVec;
	tempVec.clear();
	for (COMVEC::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		curCell = *iter;
		if (curCell.getMFlag())
		{
			tmpPair.first = curCell;
		}
		else
		{
			tempVec.push_back(curCell);
		}
	}
	tmpVec.clear();
	int nTemp=0;
	int i=nCount;
	CString strTempOut;
	CString strFix = gGlobal.GetIniValue(_T("��������"), _T("�㼶�ָ���"));
	for (COMVEC::iterator iter = tempVec.begin(); iter != tempVec.end(); ++iter)
	{
		preCell = *iter;
		tmpPair.second = preCell;
		nTemp = i;
		while (m_NumVec.find(nTemp) != m_NumVec.end())
		{
			nTemp++;
		}
		strNo.Format(_T("%d"), nTemp);
		strCount += strNo;
		strTempOut += preCell.getTempQuxiang();
		if (i!=(nSize + nCount - 2))
		{
			strCount += _T(",");
			strTempOut += strFix;
		}

		if (preCell.IsEqual(cell))
		{
			cell.setCount(strNo);
			strOut = tmpPair.first.getTempQuxiang();
		}

		m_dpoutPutMap.insert(make_pair(nTemp, tmpPair));
		addDataToSpecailMData(nCount, tmpPair);
		i++;
	}
	tempVec.clear();

	if (bIsMFlag)//�����M
	{
		cell.setCount(strCount);
		strOut = strTempOut;
	}
	return strOut;
}

CString CSingalNo::doRMSNumAndQuxiang(COMVEC tmpVec, int nCount, CCommonCell& cell)
{
	//���ȥ���ڴ˴����ã��ڲ㼶�еõ�
	CString strTexing;
	CString strOut;
	CCommonCell curCell, preCell;
	COMPAIR tmpPair;
	int nLen=0;
	int curLen=0;
	int nSize=0;
	map<int, COMVEC> tmpMap = doRLevelData(tmpVec);
	tmpVec.clear();

	COMVEC tempVec;
	tempVec.clear();

	bool bIsMFlag = cell.getMFlag();
	map<int, COMVEC>::iterator iter = tmpMap.begin();
	if (iter == tmpMap.end())
	{
		return strOut;
	}
	
	for (; iter != tmpMap.end(); ++iter)
	{
		tmpVec = iter->second;
		nLen = iter->first;
		curLen = nLen+1;
		map<int, COMVEC>::iterator IIIR = tmpMap.find(curLen);
		if (IIIR == tmpMap.end())//δ�ҵ������治�ÿ���
		{
			;
		}
		else//�ҵ��ˣ�Ҫ���Ǻ�������
		{
			tempVec = IIIR->second;
			nSize = tmpVec.size();
			if (tmpVec.size() > 1)//��ΪS
			{
				if (tempVec.size() == 1)//��ΪM
				{
					curCell = tempVec.at(0);	
					tmpPair.second = curCell;
					for (COMVEC::iterator ITr = tmpVec.begin(); ITr != tmpVec.end(); ++ITr)
					{
						preCell = *ITr;
						while (m_NumVec.find(nCount) != m_NumVec.end())	
						{
							nCount++;
						}
						tmpPair.first = preCell;
						
						m_dpoutPutMap.insert(make_pair(nCount, tmpPair));//�������
						addDataToSpecailMData(nCount, tmpPair);
						
						nCount++;
					}
				}
			}
			else	
			{
				//��ΪM����
				curCell = tmpVec.at(0);
				tmpPair.first = curCell;
				for (COMVEC::iterator ITr = tempVec.begin(); ITr != tempVec.end(); ++ITr)
				{
					preCell = *ITr;
					while (m_NumVec.find(nCount) != m_NumVec.end())
					{
						nCount++;
					}
					tmpPair.second = preCell;		
					/*if (preCell.getMFlag())
					{
						tmpPair.first = preCell;
						tmpPair.second = curCell;
					}*/
					m_dpoutPutMap.insert(make_pair(nCount, tmpPair));//�������
					addDataToSpecailMData(nCount, tmpPair);
					nCount++;
				}	
			}
			tempVec.clear();
		}
	
		tmpVec.clear();
	}
	tmpMap.clear();
	//ȫ��R�㼶������


	//����R�㼶�����ݣ�����R+M������


	//ȫ��R+MS������
	return strOut;

}

map<int, COMVEC> CSingalNo::doRLevelData(COMVEC tmpVec)
{
	CString strTexing;
	CString strTmp;
	map<int, COMVEC> tmpMap;
	tmpMap.clear();
	COMVEC tempVec;
	CCommonCell curCell;
	int nLen = 0;

	for (COMVEC::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		curCell = *iter;
		strTexing = curCell.getTexing();
		nLen = curCell.getRlevel();
		tempVec.push_back(curCell);
		pair<map<int, COMVEC>::iterator, bool> bRet = tmpMap.insert(make_pair(nLen, tempVec));
		if (!bRet.second)
		{
			bRet.first->second.push_back(curCell);
		}
		tempVec.clear();
	}	
	tmpVec.clear();
	return tmpMap;
}

CString CSingalNo::getQuxiang(CCommonCell& cell)
{
	COMVEC tmpVec;
	CString strOut,strTemp,strNo;
	CCommonCell preCell,curCell;
	int nCount = 1;
	CString strTexing = cell.getRealTexing();
	CString strTempQuxiang = cell.getTempQuxiang();
	CString strCount = cell.getCount();
	if (!strCount.IsEmpty())
	{
		strOut = getQuxiangByNum(cell);
		nCount = MyTransFunc::StringToInt(cell.getCount());
		return strOut;
	}
	int nSize = 0;
	
	for (list<CString>::iterator iter = m_ListInfo.begin(); iter != m_ListInfo.end(); ++iter)
	{
		while (m_NumVec.find(nCount) != m_NumVec.end())
		{
			nCount++;
		}
		tmpVec.clear();
		strTemp = *iter;
		MDMap::iterator IIR = m_Data.find(strTemp);
		if (IIR == m_Data.end())
		{
			continue;
		}
		tmpVec = IIR->second;
		nSize = tmpVec.size();
		if (strTemp.CompareNoCase(strTexing) == 0)
		{
			if (tmpVec.size() < 2)
			{
				nCount = 0;
				tmpVec.clear();
				break;
			}
			else if (tmpVec.size() == 2)
			{
				preCell = tmpVec.at(0);
				curCell = tmpVec.at(1);
				COMPAIR tmpPair;
				tmpPair.first = preCell;
				tmpPair.second = curCell;
				if (preCell.getTempQuxiang().CompareNoCase(strTempQuxiang) == 0)
				{
					strOut = curCell.getTempQuxiang();
				}
				else
				{
					strOut = preCell.getTempQuxiang();
				}
				strNo.Format(_T("%d"), nCount);
				cell.setCount(strNo);

				m_dpoutPutMap.insert(make_pair(nCount, tmpPair));
				tmpVec.clear();
			}
			else 
			{
				//����2,��ô����M S ����R + MS�������
				MDMap::iterator IIR = m_RData.find(strTemp);
				if (IIR == m_RData.end())//��ô����M S�߼�
				{
					strOut = doMSNumAndQuxiang(tmpVec, nCount, cell);
				}
				else
				{
					m_tmpRMSInfo.clear();
					strOut = doRMSNumAndQuxiang(tmpVec, nCount, cell);
					
					doRMSAndSetCount(cell);
				}
			}
			break;
		}
		else	
		{
			for (int i=1; i<nSize; i++)
			{
				nCount++;
				while (m_NumVec.find(nCount) != m_NumVec.end())
				{
					nCount++;
				}
			}
		}
	}
	return strOut;
}

void CSingalNo::EraseFixedInfo()
{
	CString strTexing;
	CString strCurTexing;
	//ɾ���ظ���Ԫ��
	map<CString, CString> tmpMap;
	for (list<CString>::iterator IIR = m_ListInfo.begin(); IIR != m_ListInfo.end();)
	{
		strCurTexing = *IIR;
		pair<map<CString, CString>::iterator, bool> bRet = tmpMap.insert(make_pair(strCurTexing, strCurTexing));
		if (!bRet.second)
		{
			m_ListInfo.erase(IIR++);
		}
		else
		{
			++IIR;
		}
	}
	tmpMap.clear();

	for (MDMap::iterator iter = m_NumInfo.begin(); iter != m_NumInfo.end(); ++iter)
	{
		strTexing = iter->first;
		//ɾ�����������
		for (list<CString>::iterator IIR = m_ListInfo.begin(); IIR != m_ListInfo.end();)
		{
			strCurTexing = *IIR;
			if (strTexing.CompareNoCase(strCurTexing) == 0)
			{
				m_ListInfo.erase(IIR++);
			}
			else
			{
				++IIR;
			}
		}
	}

	//������ͳһ
}

CString CSingalNo::getQuxiangByNum(CCommonCell& cell)
{
	CString strTexing = cell.getRealTexing();
	COMVEC tmpVec;
	CString strOut;	
	CString strQuxiang;
	CString strCurCount;
	CString strFix = gGlobal.GetIniValue(_T("��������"), _T("�㼶�ָ���"));
	int nCount = 0;

	MDMap::iterator iter = m_NumInfo.find(strTexing);
	if (iter == m_NumInfo.end())
	{
		return strOut;
	}
	tmpVec = iter->second;
	if (tmpVec.size() < 2)
	{
		//����������
		return strOut;
	}
	else if (tmpVec.size() == 2)
	{
		strOut = doTwoData(tmpVec, cell);
	}
	else
	{	
		strCurCount = cell.getCount();//�п�����1,2������1��������
		//��ô���ǲ㼶������ͨMS���ݣ�����R+MS����
		map<int, COMVEC> tmpMap = doFixDataToMap(tmpVec);
		tmpVec.clear();
		int nFind = strCurCount.Find(_T(","));
		if (nFind > 0)//��ôҪô��MS��Ҫô��R�㼶��Ҫô��R�㼶��MS
		{
			CString strTemp;
			int nCount =0;
			while (nFind >0)
			{
				strTemp = strCurCount.Left(nFind);
				strCurCount = strCurCount.Right(strCurCount.GetLength() - nFind -1);
				nCount = MyTransFunc::StringToInt(strTemp);
				strQuxiang = getFixNumFromFixMap(tmpMap, nCount, cell);

				strOut += strQuxiang;
				strOut += strFix;
				nFind = strCurCount.Find(_T(","));
			}
			nCount = MyTransFunc::StringToDouble(strCurCount);
			if (nCount > 0)
			{
				strQuxiang = getFixNumFromFixMap(tmpMap, nCount, cell);
				strOut += strQuxiang;
			}
		}
		else
		{
			nCount = MyTransFunc::StringToInt(strCurCount);
			strOut = getFixNumFromFixMap(tmpMap, nCount, cell);
		}
	}	
	return strOut;
}

MPMAP CSingalNo::getOutPutData() const
{
	return m_dpoutPutMap;
}

bool CSingalNo::doDPData()
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

	for (MDMap::iterator iter = m_Data.begin(); iter != m_Data.end(); ++iter)
	{
		tmpVec = iter->second;
		if (tmpVec.size() < 2)
		{
			for (vector<CCommonCell>::iterator Itr = tmpVec.begin(); Itr != tmpVec.end(); ++Itr)
			{
				curComCell = *Itr;
				gQuery.setErrorType(_T("DP���ݲ���һһƥ��"));
				strLog = gQuery.getErrorString(curComCell);
				gQuery.addErrorString(strLog);
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


bool CSingalNo::FindMultiData(pair<MDMap::iterator, bool> ret, CCommonCell cell)
{
	COMVEC findVec = ret.first->second;
	bool bIsFind = false;
	CCommonCell tempCell;
	for (COMVEC::iterator IIIIR = findVec.begin(); IIIIR != findVec.end(); ++IIIIR)
	{
		tempCell = *IIIIR;
		if (tempCell.IsEqual(cell))
		{
			bIsFind = true;
			break;
		}
	}

	return bIsFind;
}

map<int, COMVEC> CSingalNo::doFixDataToMap(COMVEC tmpVec)
{
	map<int, COMVEC> tmpMap;
	COMVEC tempVec;
	CCommonCell comCell;
	CString strPre;
	CString strTemp;
	int nCount;
	pair<map<int, COMVEC>::iterator, bool> bret;
	for (COMVEC::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
	{
		comCell = *iter;
		strTemp = comCell.getCount();
		//�����,��
		int nCount = 0;
		int nFind = strTemp.Find(_T(","));
		if (nFind > 0)
		{
			CString strCurTemp = strTemp;
			while (nFind > 0)
			{
				strPre = strCurTemp.Left(nFind);
				nCount = MyTransFunc::StringToInt(strPre);
				tempVec.clear();
				tempVec.push_back(comCell);
				bret = tmpMap.insert(make_pair(nCount, tempVec));
				if (!bret.second)
				{
					bret.first->second.push_back(comCell);
				}

				strCurTemp = strCurTemp.Right(strCurTemp.GetLength() - nFind -1);
				strPre = strCurTemp;
				nFind = strPre.Find(_T(","));
			}
			nCount = MyTransFunc::StringToInt(strPre);
			if (nCount > 0)
			{
				tempVec.clear();
				tempVec.push_back(comCell);
				bret = tmpMap.insert(make_pair(nCount, tempVec));
				if (!bret.second)
				{
					bret.first->second.push_back(comCell);
				}
			}
		}
		else
		{
			nCount = MyTransFunc::StringToInt(strTemp);
			if (nCount > 0)
			{
				tempVec.clear();
				tempVec.push_back(comCell);
				bret = tmpMap.insert(make_pair(nCount, tempVec));
				if (!bret.second)
				{
					bret.first->second.push_back(comCell);
				}
			}
		}
	}
	return tmpMap;
}

void CSingalNo::addFixNumAreaToStore(int nStart, int nEnd)
{
	for (int i= nStart; i<=nEnd; i++)
	{
		m_NumVec.insert(make_pair(i, i));
	}
}

CString CSingalNo::doTwoData(COMVEC tmpVec, CCommonCell cell)
{
	CString strOut;
	CString strCount;
	COMPAIR tmpPair;
	CCommonCell preCell, curCell;
	int nCount = 0;
	int nFind = 0;
	preCell = tmpVec.at(0);
	curCell = tmpVec.at(1);

	strCount = preCell.getCount();
	nFind = strCount.Find(_T(","));//�ؼ��֣��ָ���
	if (nFind > 0)
	{
		strCount = curCell.getCount();
	}
	nCount = MyTransFunc::StringToDouble(strCount);
	if (cell.IsEqual(preCell))
	{
		strOut = curCell.getTempQuxiang();
		tmpPair.second = curCell;
	}
	else if (cell.IsEqual(curCell))
	{
		strOut = preCell.getTempQuxiang();
		tmpPair.second = preCell;
	}	
	tmpPair.first = cell;

	m_dpoutPutMap.insert(make_pair(nCount, tmpPair));
	return strOut;
}

CString CSingalNo::getFixNumFromFixMap(map<int, COMVEC> tmpMap, int nCount, CCommonCell cell)
{
	CString strOut;
	COMVEC tmpVec;
	map<int, COMVEC>::iterator IIIR = tmpMap.find(nCount);
	if (IIIR == tmpMap.end())
	{
		return strOut;
	}
	tmpVec = IIIR->second;
	if (tmpVec.size() != 2)
	{
		CString strPrompt;
		for (COMVEC::iterator iR = tmpVec.begin(); iR != tmpVec.end(); ++iR)
		{
			CCommonCell tempCell;
			tempCell = *iR;
			strPrompt += tempCell.getTexing();
			strPrompt += _T("\n");
		}
		acutPrintf(strPrompt);
	}
	else if (tmpVec.size() == 2)
	{
		strOut = doTwoData(tmpVec, cell);
	}
	else
	{
		CString strPrompt;
		for (COMVEC::iterator iR = tmpVec.begin(); iR != tmpVec.end(); ++iR)
		{
			CCommonCell tempCell;
			tempCell = *iR;
			strPrompt += tempCell.getTexing();
			strPrompt += _T("\n");
		}
		acutPrintf(strPrompt);
	}
	tmpVec.clear();
	tmpMap.clear();
	return strOut;
}

void CSingalNo::addDataToSpecailMData(int nCount, COMPAIR tmpPair)
{
	m_tmpRMSInfo.insert(make_pair(nCount, tmpPair));
}

void CSingalNo::doRMSAndSetCount(CCommonCell& cell)
{
	CString strNo;
	CCommonCell preCell, curCell;
	
	COMPAIR tmpPair;
	CString strTempCount;
	for (MPMAP::iterator IIT = m_tmpRMSInfo.begin(); IIT != m_tmpRMSInfo.end(); ++IIT)
	{
		tmpPair = IIT->second;
		preCell = tmpPair.first;
		curCell = tmpPair.second;
		if (preCell.IsEqual(cell))
		{
			strNo.Format(_T("%d"), IIT->first);
			strNo += _T(",");
			strTempCount += strNo;
		}
		else if (curCell.IsEqual(cell))
		{
			strNo.Format(_T("%d"), IIT->first);
			strNo += _T(",");
			strTempCount += strNo;
		}
	}
	strTempCount = strTempCount.Left(strTempCount.GetLength()-1);//ȥ�����һ����
	cell.setCount(strTempCount);
}
