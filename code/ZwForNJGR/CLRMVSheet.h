#pragma once
#include "CNameCell.h"
#include <map>
#include <vector>
#include "CCommonCell.h"
#include "CLrmvTitleInfo.h"
#include "SheetStoreData.h"
#include "WriteLog.h"
using namespace std;
//��ȡ����LRMV����

class CLRMVSheet
{
public:
	CLRMVSheet(void);
	~CLRMVSheet(void);

	void setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, CString strSheetName, int nCol, int nRow);

	bool doIt();

	bool prompt();
	void readVpxData();

	bool writeExcel();
	map<CNameCell, vector<CCommonCell> > getMapInfo() const;
	map<CNameCell, vector<CCommonCell> > getMapModInfo() const;
	vector<vector<CString> > getModExcelData() const;
	CString getModSheetName();
	CString getSheetName() const;

	CSheetStoreData getData();
	CSheetStoreData getModData();
private:
	CString modSheetName(CString strSheetName);
	void	getPlug();
	vector<CCommonCell> readVpx(int nItem, int m_nRowNum, CNameCell cell);

	CString GetNameByIndex(int j, int nColNum);
	vector<CString> doEvenRelation(vector<CString> tmpCurVec, CNameCell cell);
	vector<CString> doOddRelation(vector<CString> tmpCurVec, CNameCell cell);
	vector<CString> doSingleRelation(vector<CString> tmpCurVec, CNameCell cell);
	vector<CString> doPowerRelation(vector<CString> tmpCurVec, CNameCell cell);
	vector<CString> doEmptyRelation(vector<CString> tmpCurVec, CNameCell cell);
	CCommonCell readEvenData(CString strDuanzi, CString strTexing, CString strQuxiang = _T(""));
	//��������
	void doLrmvTitle(vector<CLrmvTitleInfo> vec);
	//����Item
	CCommonCell doTableItem(CString strItem);
	CCommonCell doRfItem(CString strItem);
	CCommonCell doGLItem(CString strItem);

	int GetGLType(int nSize);
	
	CString getRealSheetName();
	//����ģ������
	void doModData(int nStart, int nEnd, CNameCell nameCell);
	//
	bool doMod();
private:
	CString m_strSheetName;//sheetName;
	CString m_strModSheetName;//��ͷsheetname
	CString m_strPlug;//����
	CString m_strModPlug;//��ͷ
	CString m_strName;
	vector<vector<CString> > m_Data;//ԭʼ��������
	vector<vector<CString> > m_modData;//��ͷ����
	map<int, CNameCell > m_MapName;
	map<CNameCell, vector<CCommonCell> > m_MapInfo;
	map<CNameCell, vector<CCommonCell> > m_MapModInfo;
	vector<CCommonCell> m_ComVec;
	map<CNameCell, vector<CCommonCell> > m_ComModVec;
	int m_nType;//lrmv��������
	int m_nColNum;
	int m_nRowNum;
	vector<CCommonCell> m_tmpVec;

	
	CSheetStoreData m_singleData;

	CSheetStoreData m_ModSingleData;
	CNameCell m_nameCell;
	CWriteLog m_log;
};
