#pragma once
//ͷ����������
#include "CNameCell.h"
#include <map>
#include <vector>
#include "CCommonCell.h"
using namespace std;

class CLrmvpSheetInfo
{
public:
	CLrmvpSheetInfo(void);
	~CLrmvpSheetInfo(void);
	void setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow);

	bool doIt();

	bool prompt();
	void readVpxData();

	bool writeExcel();
	vector<vector<CString> > getModData() const;
	CString getModSheetName() const;
	CString getSheetName() const;
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
	vector<CString> doSpecial(vector<CString> tmpCurVec, CNameCell cell, int nRow);

private:
	CString m_strSheetName;//sheetName;
	CString m_strModSheetName;//��ͷsheetname
	CString m_strPlug;//����
	CString m_strModPlug;//��ͷ
	CString m_strName;
	vector<vector<CString> > m_Data;//ԭʼ��������
	vector<vector<CString> > m_modData;//��ͷ����
	map<int, CNameCell > m_MapName;
	int m_nType;//lrmv��������
	int m_nColNum;
	int m_nRowNum;
	int m_nItem;
	bool m_bIsCheck;
};
