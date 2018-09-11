#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include "CFrameInfo.h"

class CDrawEntity
{
public:
	CDrawEntity(void);
	~CDrawEntity(void);
	//��ȡ��������
	void Init();
	bool drawFrame();
	void setNewFrameFlag(bool bIsAdded = true);

	void setCount(int nCount);
	bool draw(map<CNameCell, vector<CCommonCell> > mapInfo);
	AcGePoint3d getContentPt(AcGePoint3d insertPt);

	bool drawTable(CNameCell nameCell, vector<CCommonCell> comVec);
	void setSheetName(CString strSheetName);
	//�������
	void doTablePoint(double dDist);
	void getTablePt(bool bIsDrawText = false);

	void doRFPoint();

	void doGapPoint();
	void drawContent(CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao = _T(""), bool bDrawLine = true,int nRow=-1,int nCol=-1,int nType=-1);

	//����RFͼ��
	void drawRF(CCommonCell, bool bisRF = true);

	//���û���mod�ı�־
	void setModFlag(bool bDrawModFlag);
	//����ͼ��
	void drawTu(vector<CString> tuVec);
private:
	int getCount();
	AcGePoint3d getInsertPt();
	bool drawGL(CCommonCell comCell);
	void drawK(CCommonCell comCell);
	bool IsStartPt();
	bool getBlkPt(double dLen, double dHeight);
	AcDbObjectId drawblkRef(CString strFile);
	void moveToPt(AcDbObjectId objId);

	AcGePoint3d getblkStartPt(int nCount);
private:
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
	AcGePoint3d m_basePt;//������
	AcGePoint3d m_InsertPt;//ͼ��������
	CString m_strSheetName;//sheet����
	AcDbObjectId m_layerId;
	AcDbObjectIdArray m_objIdArr;
	AcDbObjectIdArray m_ignoreErrorArr;
	AcDbObjectId m_contentId;//���id
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

	bool m_bIsDrawMod;//�Ƿ����ģ��
	bool m_bIsDrawXxk;//�Ƿ�������߿�
	bool m_bFirstDrawRf;//�Ƿ��һ�λ���RF
	double m_dBlkLen;
	double m_dBlkHeight;
	bool m_bIsMuban;//�Ƿ�ĸ��
	double m_dOffsetDist;//ƫ�Ʊ���
	map<CString, int> m_dictGroup;
	CString m_strRealName;
	int m_nCount;
	bool m_bIsNewFrame;//�������µ�ͼ��

	CFrameInfo m_FrameInfo;
	CString m_strPreName;
	bool m_bIsHor;//�������

};
