#pragma once

class CBaseUtils
{
public:
	CBaseUtils(void);
	~CBaseUtils(void);
	static AcGePoint3d startPt();
	static void setStartPt(AcGePoint3d startPt);

	static AcGePoint3d endPt();
	static void setEndPt(AcGePoint3d endPt);

	static CString frameType();
	static void setFrameType(CString strType);
	//ͼ��������
	static double verFrameSpace();
	static void setVerFrameSpace(double dVerFrameSpace);
	//ͼ�������
	static double horFrameSpace();
	static void setHorFrameSpace(double dHorFrameSpace);

	static bool jxbFlag();
	static void setJxbFlag(bool bFlag);

	static double hxFrameCount();
	static void setHxFrameCount(double nCount);

	static double frameLength();
	static void setFrameLength(double dFrameLen);

	static double frameHeight();
	static void setFrameHeight(double dFrameHeight);

	static double leftVerGap();
	static void setLeftVerGap(double dFrameHeight);

	static double leftHorGap();
	static void setLeftHorGap(double dGap);

	static double spaceVerGap();
	static void setSpaceVerGap(double dGap);
	
	static double spaceHorGap();
	static void setSpaceHorGap(double dGap);

	static double HorSpace();
	static void setHorSpace(double dGap);

	static double VerSpace();
	static void setVerSpace(double dGap);

	static int  frameCount();
	static void setFrameCount(int nFrameCount);

	static int rfCount();
	static void setRfCount(int nRfCount);

	static int dpCount();
	static void setDpCount(int nDpCount);

	static int firstCount();
	static void setFirstCount(int nCount);

	static int secondCount();
	static void setSecondCount(int nCount);

	static CString filePath();
	static void setFilePath(CString strFilePath);

	//����int ��type ��ȡ���Ե�����[dp]����Ϣ
	static CString GetTexingType(int nType);

	static int GetTypeTexing(CString sType);
	
	//����ͼֽ�����к���xdatatag ��չ���ݵ�ʵ��
	static AcDbObjectIdArray GetObjectIdArrayForXdata(CString xdataTag);

	static AcDbExtents GetExtentsForEntity(AcDbObjectId idEnt);

	static AcDbObjectIdArray GetObjectIdArrayForRange(AcDbExtents ext);

	static AcGePoint3d GetBlockRefPostion(AcDbObjectId idBlock);

	static AcDbObjectId GetEntityLayer(AcDbObjectId idEnt);
};
