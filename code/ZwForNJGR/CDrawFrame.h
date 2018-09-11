#pragma once
#include "CFrameInfo.h"
//����ͼ��
//���ڱ������ǹ̶��Ĵ�С45*180��
//�����ǵ�����ͼ���������½ǵ���Ϊ�����㣬�ʶ���������������λ�þ͸������½ǵ�ƫ�ƣ�
//��������ƫ�������ǹ̶��ģ��ʶ�ȫ��ѡ�ù̶�����ֵ
class CDrawFrame
{
public:
	CDrawFrame(void);
	~CDrawFrame(void);

	//ͼ����ɫ
	//void setSheetName(CString strSheetName);
	//void setSheetCount(int nCount);
	void setFrameIni(CString strSheetName, CFrameInfo frameIni, int nCount = -1);

	bool draw();
	void setAddFlag(bool bIsAdded = true);
	AcGePoint3d getInsertPt();

	AcGePoint3d getContentPt();
private:
	AcGePoint3d getPoint();
	bool drawFrame(AcGePoint3d basePt);
	//BMUP BMDOWN
	bool drawBmText(AcGePoint3d basePt);
	//����
	bool drawCountText(AcGePoint3d basePt);
	//��Ʒ����
	void drawProName(AcGePoint3d basePt);
	//���
	void drawDesigner(AcGePoint3d basePt);
	//���
	void drawVerifier(AcGePoint3d basePt);
	//���Ƶ�һ���հ�����
	void drawFirstEmpty(AcGePoint3d basePt);
	//���Ʊ�׼��
	void drawStandard(AcGePoint3d basePt);
	//���Ƶڶ����հ�����
	void drawSecondEmpty(AcGePoint3d basePt);
	//������׼
	void drawApproval(AcGePoint3d basePt);
	//���ƹ��ؼ�
	void drawGzj(AcGePoint3d basePt);
	//������Ŀ�ȼ�
	void drawProGrade(AcGePoint3d basePt);
	//������Ŀ�׶�
	void drawProStage(AcGePoint3d basePt);

	//��������
	AcGePoint3d drawBasicText(CString strDescription, double& dSize);

private:
	int m_nCount;//�ڼ���ͼ��	
	//int m_nSheetCount;//sheet�ڵ�count��Ĭ��Ϊ1
	AcGePoint3d m_InsertPt;
	CString m_strSheetName;//sheet����
	CFrameInfo m_FrameIni;
	CFrameTitle m_frameTitle;
	AcDbObjectId m_TextStyleId;
	bool m_bIsAdded;//�Ƿ�����
};

