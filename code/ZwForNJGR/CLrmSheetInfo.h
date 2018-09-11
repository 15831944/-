#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include "SingleSheetData.h"
#include "SheetStoreData.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//��ȡLrm����
class CLrmSheetInfo
{
public:
	CLrmSheetInfo(void);
	~CLrmSheetInfo(void);
	void setData(vector<vector<CString> > vData,map<int, CNameCell > tmpVec, int nItem, CString strSheetName, int nCol, int nRow);
	
	bool prompt();
	CString getSheetMod() const;
	bool doIt();

	CSheetStoreData getData() const;
	CSheetStoreData getModData();
	bool IsMod();
private:
	CString getType();
	void readSpecial();
	CString GetNameByIndex(int j, int nNum);

	CString getRealSheetName();
	//��ȡLRM��׼����
	vector<CCommonCell> readLrmData(int nItem, int nNum);

	CCommonCell doTableItem(CString strItem);
	CCommonCell doRfItem(CString strItem);
	CCommonCell doItem(CString strItem);
	vector<int> getColInt(int nStart, int nNum);
	CString modSheetName(CString strSheetName) const;
private:
	CString m_strType;//sheet���ͣ���ͨ��LRM��VPX
	CString m_strSheetName;//sheetName;
	vector<vector<CString> > m_Data;
	//vector<int> m_NameIndex;
	map<int, CNameCell > m_MapName;

	CSheetStoreData m_singleData;
	CSheetStoreData m_ModSingleData;

	int m_nItem;
	int m_nColNum;
	int m_nRowNum;
	int m_nType;//0Ϊ��ͨ��1Ϊ���⣬2ΪVPX
	bool m_bIsMod;//�Ƿ�ģ��
	bool m_bIsContinued;
	CString m_strModSheetName;
	CString m_strName;
	bool m_bIsSplit;
	bool m_bIsLrmv;
	CNameCell m_NameCell;
};
