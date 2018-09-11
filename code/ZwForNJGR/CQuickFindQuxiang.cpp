#include "StdAfx.h"
#include "CQuickFindQuxiang.h"
#include "BaseUtils.h"
#include "CGlobalQuery.h"
#include "DlgListError.h"
extern CGlobalQuery gQuery;

CQuickFindQuxiang::CQuickFindQuxiang(void)
{
}


CQuickFindQuxiang::~CQuickFindQuxiang(void)
{
}

bool CQuickFindQuxiang::sel()
{
	bool bRet = false;
	AcGePoint3d pt;
	ads_name ename;
	int nRet = acedEntSel(_T("\n��ѡ���������ݣ�"), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return false;
	}
	AcDbObjectId objId = AcDbObjectId::kNull;
	acdbGetObjectId(objId, ename);
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}

	if (pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText = AcDbText::cast(pEnt);
		m_strSheetName = pText->layer();
		m_strContent = pText->textString();
		pText->close();
	}
	else
	{
		AfxMessageBox(_T("û��ѡ��ȥ������"));
		pEnt->close();
		return false;
	}
	return true;
}

bool CQuickFindQuxiang::doIt()
{
	if (!CBaseUtils::jxbFlag())
	{
		AfxMessageBox(_T("��ͨ�������������ӱ���������CADͼ��"));
		return false;
	}
	else
	{
		if (!sel())
			return false;
		if (!AnalysisData())
			return false;
		if (!showDialog())
			return false;
#ifdef DEBUG
		print();
#endif
	}
	return true;
}

bool CQuickFindQuxiang::AnalysisData()
{
	COMVEC allVec = gQuery.getCellInfo();
	if (allVec.size() < 1)
	{
		return false;
	}
	CString strTexing;
	CCommonCell cell;
	for (COMVEC::iterator iter = allVec.begin(); iter != allVec.end(); ++iter)
	{
		cell = *iter;
		strTexing = cell.getRealTexing();
		if (strTexing.CompareNoCase(m_strContent) == 0)
		{
			m_VecInfo.push_back(cell);
			addDlgInfo(cell);
		}
	}
	if (m_VecInfo.size() < 1)
	{
		AfxMessageBox(_T("û���ҵ��������ݣ���ȷ���Ƿ�ѡ�������������"));
		return false;
	}

	return true;
}

void CQuickFindQuxiang::print()
{
	CCommonCell cell;
	for (COMVEC::iterator iter = m_VecInfo.begin(); iter != m_VecInfo.end(); ++iter)
	{
		cell = *iter;
		acutPrintf(_T("\n���ԣ�%s,ԭʼ��Ϣ��%s,sheet����%s-�У�%d�У�%d"), cell.getRealTexing(), cell.getTexing(), cell.getSheetName(), cell.getRow(), cell.getCol());
	}
}

bool CQuickFindQuxiang::showDialog()
{
	if (m_dlgInfo.size() < 1)
	{
		return false;
	}
	CAcModuleResourceOverride rs;
	CDlgListError dlg;
	CString strPrompt = _T("���ҵ�������ͬ����������");
	dlg.setErrorData(strPrompt, m_dlgInfo);
	dlg.setShowFlag(false);
	if (dlg.DoModal() != IDOK)
	{
		return false;
	}

	m_dlgInfo.clear();
	return true;
}

void CQuickFindQuxiang::addDlgInfo(CCommonCell curComCell)
{
	CString strType = _T("���ҵ�������ͬ����������");
	CIgnoreErrorCell ignoreCell;
	ignoreCell.set(curComCell.getSheetName(), MyTransFunc::intToStr(curComCell.getRow()), MyTransFunc::intToStr(curComCell.getCol()), curComCell.getTexing(), curComCell.getRealTexing(), strType);
	m_dlgInfo.push_back(ignoreCell);
}
