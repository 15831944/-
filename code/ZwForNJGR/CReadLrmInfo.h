#pragma once
#include "CReadInfo.h"
#include "CGlobalQuery.h"

class CReadLrmInfo: public CReadInfo
{
public:
	CReadLrmInfo(void);
	~CReadLrmInfo(void);

	bool doIt();
	//�������ݶ�ȡ
	bool readDuanzi(int nItem, int nNum);
	bool doCommaItem(CString strItem, int nRow, int nCol);
	void doColItem(CString strItem, int nRow, int nCol);
private:
	bool m_bIsMod;//�Ƿ�ģ��
	CSheetStoreData m_ModSingleData;

	//��¼RF����ͨ����
	COMVEC RFComVec;
	COMVEC RFModComVec;
	COMVEC comVec;
	COMVEC comModVec;
	vector<CString> tmpVec;
	CString strPreName;
	CString strMidName;

};