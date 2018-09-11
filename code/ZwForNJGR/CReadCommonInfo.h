#pragma once
#include "CReadInfo.h"
#include "SingleSheetData.h"

class CReadCommonInfo:public CReadInfo
{
public:
	CReadCommonInfo(void);
	~CReadCommonInfo(void);
	//��ʼ��������
	bool doIt();
	//�������ݶ�ȡ
	bool readDuanzi(int nItem, int nNum);

	CSingleSheetData getdata() const;
private:
	CSingleSheetData m_sheetData;
	map<CNameCell, vector<CCommonCell> > m_MapInfo;
};

