#pragma once
#include "CNameCell.h"
#include <vector>
#include <map>
#include "CCommonCell.h"
#include "SheetStoreData.h"
#include "CGlobalQuery.h"
using namespace std;

//����
const int ColQHCol = 0;//ǻ����������

class CReadInfo
{
public:
	CReadInfo(void);
	~CReadInfo(void);

	void setData(vector<vector<CString> > vData,map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow);
	virtual bool doIt();
protected:
	CString getRealSheetName();
	int GetGLType(int nSize);
	CString getSheetMod() const;
	CString getType();
	CSheetStoreData getSheetData() const;
	vector<int> getColInt(int nStart, int nNum);
	//��ȡnamecell��״̬

	bool AddRange(int nRow, int nEndRow);
	//��ȡcommoncell��״̬
	void getCellStatus(CString strItem);
	CCommonCell addCell(CString strItem, CString strDuanzi, CString strTexing, int nRow, int nCol);
	//add����comcomcell
	void addToDataCell(CCommonCell comCell);
public:
	CNameCell m_NameCell;
	CString m_strSheetName;
	CString m_strName;
	vector<vector<CString> > m_Data;
	map<int, CNameCell > m_MapName;
	CSheetStoreData m_sheetGL;
	//���ݴ洢
	COMVEC RFComVec;
	COMVEC RFModComVec;
	COMVEC comVec;
	COMVEC comModVec;

	int m_nItem;
	int m_nColNum;//����
	int m_nRowNum;//����
	bool m_bIsMod;
	bool m_bIsRF;//�Ƿ���Ƶ
	bool m_bIsGl;//�Ƿ����
	bool m_bIsAddData;//�Ƿ��T,�����T�Ͳ���ĸ������ʾ����
};

