#pragma once

//����ͼ�������һЩ��Ҫ����
class CDrawBaseData
{
public:
	CDrawBaseData(void);
	~CDrawBaseData(void);
	void init();
	//************************************
	// Method:    frameLength
	// FullName:  CDrawBaseData::frameLength
	// Access:    public 
	// Returns:   double
	// Qualifier:ͼ�򳤶�
	//************************************
	double frameLength();
	//************************************
	// Method:    frameHeight
	// FullName:  CDrawBaseData::frameHeight
	// Access:    public 
	// Returns:   double
	// Qualifier:ͼ��߶�
	//************************************
	double frameHeight();
	//************************************
	// Method:    frameHorSpace
	// FullName:  CDrawBaseData::frameHorSpace
	// Access:    public 
	// Returns:   double
	// Qualifier:ͼ����߼��
	//************************************
	double frameHorSpace();
	//************************************
	// Method:    frameVerSpace
	// FullName:  CDrawBaseData::frameVerSpace
	// Access:    public 
	// Returns:   double
	// Qualifier:ͼ��������
	//************************************
	double frameVerSpace();

	//************************************
	// Method:    tableLength
	// FullName:  CDrawBaseData::tableLength
	// Access:    public 
	// Returns:   double
	// Qualifier:��񳤶�
	//************************************
	double tableLength();
	//************************************
	// Method:    tableHeight
	// FullName:  CDrawBaseData::tableHeight
	// Access:    public 
	// Returns:   double
	// Qualifier:���߶�
	//************************************
	double tableHeight();

	//************************************
	// Method:    horSpace
	// FullName:  CDrawBaseData::horSpace
	// Access:    public 
	// Returns:   double
	// Qualifier:������
	//************************************
	double horSpace();

	//************************************
	// Method:    verSpace
	// FullName:  CDrawBaseData::verSpace
	// Access:    public 
	// Returns:   double
	// Qualifier:������
	//************************************
	double verSpace();

	//************************************
	// Method:    lineLength
	// FullName:  CDrawBaseData::lineLength
	// Access:    public 
	// Returns:   double
	// Qualifier:�����߳�
	//************************************
	double lineLength();
	double spaceHorGap();
	double spaceVerGap();
	int HxFrameCount();

	double firstTableLength();
	double secondTableLength();
	double thirdTableLength();
	double rfSecondTableLength();
	double rfThirdTableLength();

	AcGePoint3d getInsertPt();
	AcGePoint3d getContentPt(AcGePoint3d insertPt);
	void setInsertPt(AcGePoint3d insertPt);
private:
	//��������
	double m_dFrameLength;//ͼ�򳤶�
	double m_dFrameHeight;//ͼ��߶�
	double m_dFrameHorSpace;//ͼ�������
	double m_dFrameVerSpace;//ͼ��������
	double m_dTableLength;//��񳤶�
	double m_dTableHeight;//���߶�
	double m_dHorSpace;//������
	double m_dVerSapce;//������
	double m_dLineLength;//�����߳�
	double m_dSpareHorGap;//���ú���
	double m_dSpareVerGap;//��������
	double m_dGap;//��ͼ������ߵļ�϶
	double m_dLeftVerGap;
	double m_dLeftHorGap;
	bool m_bIsDrawVerLine;//�Ƿ����������
	bool m_bIsDrawHorLine;//�Ƿ���ƺ��ŵ���
	int  m_nHxCount;//����ͼ����
	double m_dFirstTableLength;//��һ������
	double m_dSecondTableLength;//�ڶ�������
	double m_dThirdTableLength;//����������

	double m_dRfSecondTableLength;//RF�ڶ��ڳ���
	double m_dRfThirdTableLength;//RF�����ڳ���
};

