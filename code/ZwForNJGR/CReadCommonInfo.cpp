#include "StdAfx.h"
#include "CReadCommonInfo.h"
#include "CGlobalQuery.h"

extern CGlobalQuery gQuery;

CReadCommonInfo::CReadCommonInfo(void)
{
	m_MapInfo.clear();
}


CReadCommonInfo::~CReadCommonInfo(void)
{
	m_MapInfo.clear();
}

bool CReadCommonInfo::doIt()
{
	int nItem = 1;
	for (map<int, CNameCell>::iterator itr = m_MapName.begin(); itr != m_MapName.end();)
	{
		vector<CCommonCell> comVec;
		comVec.clear();

		nItem = itr->first;
		m_NameCell = itr->second;
		//�������ź�
		gQuery.clearDifferentialSignal();

		m_strName = m_NameCell.getRealName();
		
		if (++itr == m_MapName.end())//������Χ
		{
			readDuanzi(nItem, m_nRowNum);
		}
		else//û�г�����Χ
		{
			int nRow = itr->first;
			readDuanzi(nItem, nRow-1);
		}
		gQuery.doDifferSignal();
	}
	//��������ӵ�sheet data��
	m_sheetData.setMapInfo(m_MapInfo);
	m_sheetData.setSheetStoreData(m_sheetGL);

	//��������ӵ�ȫ���д洢����
	gQuery.addCommonSheetData(m_strSheetName, m_sheetData);
	return true;
}

bool CReadCommonInfo::readDuanzi(int nItem, int nNum)
{
	CString strDuanzi,strTemp,strTexing, strTmp,strQuxiang,strLog;
	CNameCell cell = m_NameCell;
	CCommonCell commonCell;
	commonCell.setNameCell(cell);
	COMVEC comVec;
	int nType;
	int nMulriple = m_nColNum/2;
	bool bIsGl = false;

	CGLData glData;
	glData.setNamCell(cell);
	if (cell.getName().Find(_T("[MT]")) >= 0)
	{
		CString strTempName = cell.getName();
		strTempName.Replace(_T("[MT]"), _T(""));
		cell.setName(strTempName);
		glData.setNamCell(cell);
		bIsGl = true;
	}
	for (int j=0; j<nMulriple; j++)
	{
		strTemp = m_Data[nItem][2*j];
		if (strTemp.CompareNoCase(_T("����")) == 0)
		{
			comVec.clear();
			for (int i=nItem+1; i<nNum; i++)
			{
				strTexing = m_Data[i][1+2*j];
				strDuanzi = m_Data[i][2*j];
				if (strTexing.IsEmpty()&&strDuanzi.IsEmpty())
				{
					continue;
				}
				if ((strDuanzi.CompareNoCase(_T("ĸ������������")) == 0) ||
					(strDuanzi.CompareNoCase(_T("ģ������������")) == 0) ||
					(strDuanzi.CompareNoCase(_T("ĸ�������ʴ���")) == 0) ||
					(strDuanzi.CompareNoCase(_T("ģ�������ʴ���")) == 0))
				{
					continue;
				}
				strLog.Format(_T("sheetName = %s,��%d�У���%d��"),  m_strSheetName, i+1, 2*j+2);
				if ((m_NameCell.getRealName().CompareNoCase(_T("����")) == 0) ||
					(m_NameCell.getRealName().CompareNoCase(_T("����")) == 0))
				{
					strQuxiang = strDuanzi + _T(":1");
				}
				else
				{
					strQuxiang = getRealSheetName() + m_NameCell.getRealName() + _T(":") + strDuanzi;
				}
				commonCell.setTempQuxiang(strQuxiang);
				commonCell.setModQuxiang(strQuxiang);
				//������
				int nFind = strTexing.Find(_T("[No="));
				if (nFind >= 0)
				{
					int nTemp = strTexing.Find(_T("]"), nFind);
					strTemp = strTexing.Mid(nFind+4, nTemp-nFind - 4);
					commonCell.setCount(strTemp);
				}
				strLog = MyParserString::RemoveSymbol(_T("]"), strTexing);

				commonCell.setTexing(strTexing);
				commonCell.setRealTexing(strLog);

				commonCell.setRow(i+1);
				commonCell.setCol(2+2*j);
				commonCell.setSheetName(m_strSheetName);

				//��������ڴ˴���
				commonCell.setName(m_strName);
				commonCell.setDuanzi(strDuanzi);
				if (strDuanzi.IsEmpty() && strTexing.IsEmpty())
				{
					continue;
				}

				if ((!strLog.IsEmpty()) && (strLog.CompareNoCase(_T("NC")) != 0))
				{
					strTmp = getSheetMod() + m_strName +  _T(":") + strDuanzi;
					commonCell.setModQuxiang(strTmp);
				}
				else
				{
					commonCell.setModQuxiang(_T(""));
				}

				if (bIsGl)
				{
					if (!commonCell.getCount().IsEmpty())
					{
						glData.setNumber(commonCell.getCount());
					}
					gQuery.doMTCommonCell(commonCell);
				}
				else
				{
					gQuery.doCommonCell(commonCell);
				}
				comVec.push_back(commonCell);
			}
			if (bIsGl)
			{
				nType = GetGLType(comVec.size());
				glData.setCommonCellVec(comVec);
				glData.setType(nType);
				m_sheetGL.addGlDataItem(glData);
				comVec.clear();
				glData.clear();
			}
			else
			{
				cell.setItem(j+nItem);
				m_MapInfo.insert(make_pair(cell, comVec));
			}
		}	
	}
	return true;
}

CSingleSheetData CReadCommonInfo::getdata() const
{
	return m_sheetData;
}
