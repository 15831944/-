#pragma once
//����������
//���ڣ�2018��7��26��11:55:07

#include "map"
using namespace std;

class CDrawOutLine
{
public:
	CDrawOutLine(void);
	~CDrawOutLine(void);
	//��������
	bool doIt();

private:
	//��ȡ����ͼ����
	bool getAllLayerName();
	//��ȡͼ��ĵ���Ϣ
	void getLayerPoints(CString strName);
	//����ͼ�������Ϣ
	void doLayerNameInfo(CString strName);
	//���������ǵ���ƶ����
	void drawPlineByTwoPoints(AcGePoint3d startPt, AcGePoint3d endPt);
	void EraseAllGroup();
private:
	map<CString, vector<CString> > m_MapName;
	AcDbObjectIdArray m_idArrs;
	map<double, vector<AcGePoint3d> > m_pointsMap;
	int m_nColor;
};

