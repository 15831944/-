#pragma once
#include "CNameCell.h"
#include <map>
#include <vector>
#include "CCommonCell.h"
using namespace std;

class CLrmvSheetInfo
{
public:
	CLrmvSheetInfo(void);
	~CLrmvSheetInfo(void);
	void setData(vector<vector<CString> > vData, map<int, CNameCell > tmpVec, CString strSheetName, int nCol, int nRow);

	bool doIt();

	bool prompt();
	void readVpxData();

	bool writeExcel();
	map<CNameCell, vector<CCommonCell> > getMapInfo() const;
	map<CNameCell, vector<CCommonCell> > getMapModInfo() const;
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
	vector<CString> doSpecial(vector<CString> tmpCurVec, CNameCell cell);

private:
	CString m_strSheetName;//sheetName;
	CString m_strModSheetName;//插头sheetname
	CString m_strPlug;//插座
	CString m_strModPlug;//插头
	CString m_strName;
	vector<vector<CString> > m_Data;//原始插座数据
	vector<vector<CString> > m_modData;//插头数据
	map<int, CNameCell > m_MapName;
	map<CNameCell, vector<CCommonCell> > m_MapInfo;
	map<CNameCell, vector<CCommonCell> > m_MapModInfo;
	vector<CCommonCell> m_ComVec;
	vector<CCommonCell> m_ComModVec;
	int m_nType;//lrmv数据类型
	int m_nColNum;
	int m_nRowNum;
	vector<CCommonCell> m_tmpVec;
};

