#pragma once
#include "SheetStoreData.h"
#include "CFrameInfo.h"

//�������ݹ�
class CDrawDigitalOptical
{
public:
	CDrawDigitalOptical(void);
	~CDrawDigitalOptical(void);

	void setData(CSheetStoreData data);
	void doIt();
	//��A0��A1��ͼֽ����A0����A0���棬A1����A1����
	void sortData();
	bool draw();
	void setFrameCount(int nFrameCount);
private:
	bool drawGLData(CGLData glData);
	void drawGLEnt(AcGePoint3d tmpPt, CString strNumber, vector<CCommonCell> tmpVec);
	void drawGlContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec);
	void drawContent(AcGePoint3d tmpPt, CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao, bool bIsDrawLine = true);
	void drawGlExts(CString strText);
private:
	int m_nFrameType;//ͼ�����ͣ�Ĭ��Ϊ0��0��ʾA0��1��ʾA1
	CSheetStoreData m_sheetData;//sheet������
	//CDrawBaseData m_drawBaseData;//������������
	CFrameInfo m_FrameInfo;//��������
	CString m_strRealName;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	AcGePoint3d m_basePt;//������
	AcGePoint3d m_InsertPt;//ͼ��������
	AcDbObjectId m_layerId;//ͼ��id
	AcDbObjectIdArray m_objIdArr;
	AcDbObjectIdArray m_ignoreErrorArr;
	AcGePoint3d m_glMinPt;
	AcGePoint3d m_glMaxPt;


	bool m_bIsDrawHorLine;
	bool m_bIsDrawVerLine;
	bool m_bIsUp;
	CNameCell m_nameCell;

	double m_dYPos;//��¼��ʼyλ��
	int m_nFrameCount;//��������
	int m_nCount;
	int m_nGlCount;//Gl����
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
};

