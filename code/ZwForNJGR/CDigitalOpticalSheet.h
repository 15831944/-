#pragma once
#include "SheetStoreData.h"
#include "CCommonCell.h"
#include "CNameCell.h"
#include "GLData.h"
#include <algorithm>
using namespace std;
//���ֹ����ݶ�ȡ
class CDigitalOpticalSheet
{
public:
	CDigitalOpticalSheet(void);
	~CDigitalOpticalSheet(void);
	void setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, CString strSheetName, int nCol, int nRow, int nType = 0);
	
	CString modSheetName(CString strSheetName);
	bool doIt();

	void readCommon();
	CSheetStoreData getData();

private:
	int GetGLType(int nSize);
	vector<CCommonCell> readDuanziData(int nItem, int param2);
	CString getRealSheetName();
private:
	CSheetStoreData m_singleData;

	CString m_strSheetName;//sheetName;
	CString m_strModSheetName;//��ͷsheetname
	CString m_strPlug;//����
	CString m_strModPlug;//��ͷ
	CString m_strName;
	vector<vector<CString> > m_Data;//ԭʼ��������
	int m_nType;//0��ʾ���ֹ⣬1��ʾģ���
	int m_nColNum;
	int m_nRowNum;
	vector<CCommonCell> m_tmpVec;
	map<int, CNameCell > m_MapName;
	CGLData m_glData;
	CNameCell m_nameCell;

};

