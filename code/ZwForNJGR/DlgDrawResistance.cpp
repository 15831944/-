//-----------------------------------------------------------------------------
//----- DlgDrawResistance.cpp : Implementation of CDlgDrawResistance
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgDrawResistance.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgDrawResistance, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgDrawResistance, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgDrawResistance::CDlgDrawResistance (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgDrawResistance::IDD, pParent, hInstance) 
{
	readIniFile();
}

//-----------------------------------------------------------------------------
void CDlgDrawResistance::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_QUXIANG, m_strQuxiang);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgDrawResistance::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

// -----------------------------------------------------------------------------
BOOL CDlgDrawResistance::OnInitDialog(void)
{
	BOOL retCode =CAcUiDialog::OnInitDialog () ;
	CenterWindow(acedGetAcadDwgView());

	return (retCode) ;
}

// -----------------------------------------------------------------------------
void CDlgDrawResistance::OnOK(void)
{
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
	GetDlgItem(IDC_EDIT_QUXIANG)->GetWindowText(m_strQuxiang);
	//////////////////////////////////////////////////////////////////////////
	CString strFile = MyBaseUtils::GetAppPath() + _T("���\\dz.dwg");
	if (!gGlobal.IsFileExist(strFile))
	{
		AfxMessageBox(_T("û�е����ļ�"));
		return;
	}

	//ѡ��ʵ��
	AcGePoint3d pt = selEnt();
	//���Ƶ���
	AcDbObjectId objId = MyDrawEntity::InsertBlkRef(strFile, pt);
	objId = MyEditEntity::openEntChangeLayer(objId, m_layerId);
	//���Ƶ�������
	drawDzText(objId);

	//����ȥ������
	drawQuxiangText(pt);

	writeIniFile();
	CAcUiDialog::OnOK () ;
}

void CDlgDrawResistance::writeIniFile()
{
	gGlobal.SetIniValue(_T("���"), _T("��������"), m_strName);
	gGlobal.SetIniValue(_T("���"), _T("ȥ��"), m_strQuxiang);
	gGlobal.WriteIniFile();
}

void CDlgDrawResistance::readIniFile()
{
	m_strName = gGlobal.GetIniValue(_T("���"), _T("��������"));
	m_strQuxiang = gGlobal.GetIniValue(_T("���"), _T("ȥ��"));
}

AcGePoint3d CDlgDrawResistance::selEnt()
{
	AcGePoint3d pt;
	BeginEditorCommand();

	ads_name ename;
	int nRet = acedEntSel(_T("\n��ѡ��һ����Ҫ���ӽӵص�ͼ��:"), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return pt;
	}
	AcDbEntity* pEnt = NULL;
	AcDbObjectId objId = AcDbObjectId::kNull;
	acdbGetObjectId(objId, ename);
	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
	{
		return pt;
	}
	AcDbExtents exts;
	CString strBlockName;
	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
		pBlkRef->getGeomExtents(exts);
		MyBaseUtils::symbolIdToName(pBlkRef->blockTableRecord(), strBlockName);
		m_layerId = pBlkRef->layerId();
		pBlkRef->close();
	}
	else
	{
		pEnt->close();
	}
	if (strBlockName.CompareNoCase(_T("ZW_TABLE")) == 0)
	{
		pt.x = exts.maxPoint().x;
		pt.y = (exts.maxPoint().y + exts.minPoint().y)/2;
		pt.z = 0;
	}

	CompleteEditorCommand();
	return pt;
}

void CDlgDrawResistance::drawQuxiangText(AcGePoint3d pt)
{
	double dThirdTableLength  = gGlobal.GetIniValue(_T("��������"), _T("�������γ���"), 0);
	pt.x -= dThirdTableLength/2;

	AcDbObjectId textId = MyDrawEntity::DrawText(pt, m_strQuxiang, 3.0, AcDbObjectId::kNull, AcDb::kTextMid);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
}

void CDlgDrawResistance::drawDzText(AcDbObjectId objId)
{
	
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		return;
	}	
	AcDbExtents exts;

	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
		pBlkRef->getGeomExtents(exts);
		pBlkRef->close();
	}
	else
	{
		pEnt->close();
	}
	
	AcGePoint3d pt;
	pt.x = exts.maxPoint().x;
	pt.y = (exts.maxPoint().y + exts.minPoint().y)/2;
	pt.z = 0;

	AcDbObjectId textId = MyDrawEntity::DrawText(pt, m_strName, 3.0, AcDbObjectId::kNull, AcDb::kTextLeft, AcDb::kTextVertMid);
	textId = MyEditEntity::openEntChangeLayer(textId, m_layerId);
}
