#include "StdAfx.h"
#include "CFrameInfo.h"
#include "BaseUtils.h"

CFrameInfo::CFrameInfo(void)
{	
	m_dHorSpace = CBaseUtils::HorSpace();//���������
	m_dVerSapce = CBaseUtils::VerSpace();//��������
	m_dSpareHorGap = CBaseUtils::spaceHorGap();//���ú���
	m_dSpareVerGap = CBaseUtils::spaceVerGap();//��������
	m_dLeftVerGap = CBaseUtils::leftVerGap();//���Ϻ���
	m_dLeftHorGap = CBaseUtils::leftHorGap();//���Ϻ���
	//�̶����ݣ��������޸�
	m_dTableHeight = gGlobal.GetIniValue(_T("��������"), _T("���߶�"), 0);
	m_dLineLength = gGlobal.GetIniValue(_T("��������"), _T("�����߳�"), 0);
	m_dFirstTableLength = gGlobal.GetIniValue(_T("��������"), _T("����һ�γ���"), 0);
	m_dSecondTableLength =  gGlobal.GetIniValue(_T("��������"), _T("���ڶ��γ���"), 0);
	m_dThirdTableLength  = gGlobal.GetIniValue(_T("��������"), _T("�������γ���"), 0);
	m_dTableLength = m_dFirstTableLength + m_dSecondTableLength + m_dThirdTableLength;
	m_dRfSecondTableLength = gGlobal.GetIniValue(_T("��������"), _T("RF���ڶ��γ���"), 0);
	m_dRfThirdTableLength = gGlobal.GetIniValue(_T("��������"), _T("RF�������γ���"), 0);
	m_dGap = gGlobal.GetIniValue(_T("��������"), _T("��϶"), 0);
	m_strFrameType =  gGlobal.GetIniValue(_T("ͼ������"), _T("ͼ������"));
}

CFrameInfo::~CFrameInfo(void)
{
}



double CFrameInfo::getTableLength() const
{
	return m_dTableLength;
}

double CFrameInfo::getTableHeight() const
{
	return m_dTableHeight;
}

double CFrameInfo::getLineLength() const
{
	return m_dLineLength;
}

double CFrameInfo::getFristTableLength() const
{
	return m_dFirstTableLength;
}

double CFrameInfo::getSecondTableLength() const
{
	return m_dSecondTableLength;
}

double CFrameInfo::getThirdTableLength() const
{
	return m_dThirdTableLength;
}

double CFrameInfo::getRfSecondTableLength() const
{
	return m_dRfSecondTableLength;
}

double CFrameInfo::getRfThirdTableLength() const
{
	return m_dRfThirdTableLength;
}

double CFrameInfo::getHorSpace() const
{
	return m_dHorSpace;
}

double CFrameInfo::getVerSpace() const
{
	return m_dVerSapce;
}

double CFrameInfo::getSpaceHorGap() const
{
	return m_dSpareHorGap;
}

double CFrameInfo::getSpaceVerGap() const
{
	return m_dSpareVerGap;
}

double CFrameInfo::getGap() const
{
	return m_dGap;
}

double CFrameInfo::getLeftVerGap() const
{
	return m_dLeftVerGap;
}

double CFrameInfo::getLeftHorGap() const
{
	return m_dLeftHorGap;
}


CString CFrameInfo::getFrameType() const
{
	return m_strFrameType;
}

CString CFrameInfo::getLrmName() const
{
	return m_strLrmName;
}

void CFrameInfo::setTableLength(double dTableLength)
{
	m_dTableLength = dTableLength;
}

void CFrameInfo::setTableHeight(double dTableHeight)
{
	m_dTableHeight = dTableHeight;
}

void CFrameInfo::setLineLength(double dLineLength)
{
	m_dLineLength = dLineLength;
}

void CFrameInfo::setFirstTableLength(double dFirstTableLength)
{
	m_dFirstTableLength = dFirstTableLength;
}

void CFrameInfo::setSecondTableLength(double dSecondTableLength)
{
	m_dSecondTableLength = dSecondTableLength;
}

void CFrameInfo::setThirdTableLength(double dThirdTableLength)
{
	m_dThirdTableLength = dThirdTableLength;
}

void CFrameInfo::setRfSecondTableLength(double dRfSecondTableLength)
{
	m_dRfSecondTableLength = dRfSecondTableLength;
}

void CFrameInfo::setRfThirdTableLength(double dRfThirdTableLength)
{
	m_dRfThirdTableLength = dRfThirdTableLength;
}

void CFrameInfo::setHorSpace(double dHorSpace)
{
	m_dHorSpace = dHorSpace;
}

void CFrameInfo::setVerSpace(double dVerSpace)
{
	m_dVerSapce = dVerSpace;
}

void CFrameInfo::setSpaceHorGap(double dSpaceHorGap)
{
	m_dSpareHorGap = dSpaceHorGap;
}

void CFrameInfo::setSpaceVerGap(double dSpaceVerGap)
{
	m_dSpareVerGap = dSpaceVerGap;
}

void CFrameInfo::setGap(double dGap)
{
	m_dGap = dGap;
}

void CFrameInfo::setLeftVerGap(double dLeftVerGap)
{
	m_dLeftVerGap = dLeftVerGap;
}

void CFrameInfo::setLeftHorGap(double dLeftHorGap)
{
	m_dLeftHorGap = dLeftHorGap;
}

void CFrameInfo::setFrameType(CString strFrameType)
{
	m_strFrameType = strFrameType;
}

void CFrameInfo::setLrmName(CString strLrmName)
{
	m_strLrmName = strLrmName;
}

CFrameInfo& CFrameInfo::operator=(const CFrameInfo& node)
{
	//��ʱ�̶����ݣ��������޸�
	m_dTableLength = node.getTableLength();//��񳤶�
	m_dTableHeight = node.getTableHeight();//���߶�
	m_dLineLength = node.getLineLength();//�����߳�
	m_dFirstTableLength = node.getFristTableLength();//��һ������
	m_dSecondTableLength = node.getSecondTableLength();//�ڶ�������
	m_dThirdTableLength = node.getThirdTableLength();//����������
	m_dRfSecondTableLength = node.getRfSecondTableLength();//RF�ڶ��ڳ���
	m_dRfThirdTableLength = node.getRfThirdTableLength();//RF�����ڳ���
	//���޸�
	m_dHorSpace = node.getHorSpace();//������
	m_dVerSapce = node.getVerSpace();//������
	m_dSpareHorGap = node.getSpaceHorGap();//���ú���
	m_dSpareVerGap = node.getSpaceVerGap();//��������
	m_dGap = node.getGap();//��ͼ������ߵļ�϶
	m_dLeftVerGap = node.getLeftVerGap();//��������
	m_dLeftHorGap = node.getLeftHorGap();//���Ϻ���
	m_strFrameType = node.getFrameType();
	m_strLrmName = node.getLrmName();
	return *this;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CFrameTitle::CFrameTitle(void)
{
	m_strBmText = gGlobal.GetIniValue(_T("ͼ������"), _T("BM"));
	m_strDesigner = gGlobal.GetIniValue(_T("ͼ������"), _T("���ʦ"));
	m_strProName = gGlobal.GetIniValue(_T("ͼ������"), _T("��Ʒ����"));
	m_strProGrade = gGlobal.GetIniValue(_T("ͼ������"), _T("��Ŀ�ȼ�"));
	m_strProStage = gGlobal.GetIniValue(_T("ͼ������"), _T("��Ŀ�׶�"));
	m_strGZJ = gGlobal.GetIniValue(_T("ͼ������"), _T("���ؼ�"));
	m_strVerifier = gGlobal.GetIniValue(_T("ͼ������"), _T("�����"));
	m_strFirstEmpty = gGlobal.GetIniValue(_T("ͼ������"), _T("���"));//��һ���հ״�
	m_strStandard = gGlobal.GetIniValue(_T("ͼ������"), _T("��׼��"));//��׼��
	m_strSecondEmpty = gGlobal.GetIniValue(_T("ͼ������"), _T("�ڶ����հ״�"));//�ڶ����հ״�
	m_strApproval = gGlobal.GetIniValue(_T("ͼ������"), _T("��׼"));//��׼
}

CFrameTitle::~CFrameTitle(void)
{

}
CString CFrameTitle::getBmText() const
{
	return m_strBmText;
}

CString CFrameTitle::getDesigner() const
{
	return m_strDesigner;
}

CString CFrameTitle::getProName() const
{
	return m_strProName;
}

CString CFrameTitle::getProGrade() const
{
	return m_strProGrade;
}

CString CFrameTitle::getProStage() const
{
	return m_strProStage;
}

CString CFrameTitle::getGZJ() const
{
	return m_strGZJ;
}

CString CFrameTitle::getVerifier() const
{
	return m_strVerifier;
}

CString CFrameTitle::getFirstEmpty() const
{
	return m_strFirstEmpty;
}	

CString CFrameTitle::getStandard() const
{
	return m_strStandard;
}

CString CFrameTitle::getSencondEmpty() const
{
	return m_strSecondEmpty;
}

CString CFrameTitle::getApproval() const
{
	return m_strApproval;
}

void CFrameTitle::setBmText(CString strBmText)
{
	m_strBmText = strBmText;
}

void CFrameTitle::setDesigner(CString strDesigner)
{
	m_strDesigner = strDesigner;
}

void CFrameTitle::setProName(CString strProName)
{
	m_strProName = strProName;
}

void CFrameTitle::setProGrade(CString strGrade)
{
	m_strProGrade = strGrade;
}

void CFrameTitle::setProStage(CString strProStage)
{
	m_strProGrade = strProStage;
}

void CFrameTitle::setGZJ(CString strGZJ)
{
	m_strGZJ = strGZJ;
}

void CFrameTitle::setVerifier(CString strVerifier)
{
	m_strVerifier = strVerifier;
}

void CFrameTitle::setFirstEmpty(CString strFirstEmpty)
{
	m_strFirstEmpty = strFirstEmpty;
}

void CFrameTitle::setStandard(CString strStandard)
{
	m_strStandard = strStandard;
}

void CFrameTitle::setSencondEmpty(CString strEmpty)
{
	m_strSecondEmpty = strEmpty;
}

void CFrameTitle::setApproval(CString strApproval)
{
	m_strApproval = strApproval;
}

CFrameTitle& CFrameTitle::operator=(const CFrameTitle& node)
{
	m_strBmText = node.getBmText();//bm����
	m_strDesigner = node.getDesigner();//���ʦ
	m_strProName = node.getProName();//��Ʒ����
	m_strProGrade = node.getProGrade();//��Ŀ�ȼ�
	m_strProStage = node.getProStage();//��Ŀ�׶�
	m_strGZJ = node.getGZJ();//���ؼ�
	m_strVerifier = node.getVerifier();//���
	m_strFirstEmpty = node.getFirstEmpty();//��һ���հ״�
	m_strStandard = node.getStandard();
	m_strSecondEmpty = node.getSencondEmpty();
	m_strApproval = node.getApproval();
	return *this;
}
