#pragma once
#include "SheetStoreData.h"
#include "TableData.h"
#include "GLData.h"
#include "RFData.h"
#include "CommonData.h"
#include "CFrameInfo.h"

//���������LRMVͼ��
class CDrawLrmvEnt
{
public:
	CDrawLrmvEnt(void);
	~CDrawLrmvEnt(void);

	void setData(CSheetStoreData data);
	void doIt();
	//��A0��A1��ͼֽ����A0����A0���棬A1����A1����
	void sortData();
	bool draw();
	void setModFlag(bool bIsMod);
	void setCount(int nCount);
	void setLastDrawFlag(bool bIsLast);

private:
	bool drawTable(CTableData tblData);
	bool drawGLData(CGLData glData);
	bool drawRfData(CRFData rfData);
	void drawComData(CCommonData comData);
	//�ڸõ���Ʊ��ͼ��
	void drawTableContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec);
	void drawContent(AcGePoint3d tmpPt, CString strDuanzi, CString strTexing, CString strQuxiang, CString strXianhao, bool bIsDrawLine = true);
	void drawRF(AcGePoint3d tmpPt, CCommonCell comCell, bool bIsRF = true);
	void drawK(AcGePoint3d tmpPt, CCommonCell comCell);
	void drawGLEnt(AcGePoint3d tmpPt, CString strNumber, vector<CCommonCell> tmpVec);
	void drawGlContent(AcGePoint3d tmpPt, vector<CCommonCell> tmpVec, bool param3);
	void drawGlExts(CString strText);
	void drawGL(AcGePoint3d tmpPt, CCommonCell param2);
private:
	int m_nFrameType;//ͼ�����ͣ�Ĭ��Ϊ0��0��ʾA0��1��ʾA1
	CSheetStoreData m_sheetData;//sheet������
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
	CNameCell m_nameCell;
	bool m_bIsDrawMod;//�Ƿ����ģ��
	int m_nCount;//-1ΪĬ��
	bool m_bIsLastDraw;//�Ƿ����һ������
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

};

