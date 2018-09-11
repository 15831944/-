//---------------------------------------------------------
#pragma once
#include "excel9.h" 
#include <comdef.h> 
#include <vector>
#include "CNameCell.h"
#include "SingleSheetData.h"
#include "CSheetInfo.h"
#include "SheetStoreData.h"
using namespace excel;
using namespace std;

class CMyExcel 
{ 
public: 
	//Excel��Ӧ�� 
	_Application   MyApp;   
	_Workbook   MyBook; 
	_Worksheet   MySheet;    
	Workbooks   MyBooks;    
	Worksheets   MySheets;    
	Range   MyRange;   
	CString strFilePath; 
	//���캯�� 
	CMyExcel(); 
	//�������� 
	~CMyExcel(); 
	//���µ�Excel�ļ� 
	BOOL Open(); 
	//��strFile�ļ� 
	BOOL Open(CString strFile); 
	BOOL openSpecailFile(CString strFile);

	int GetSheetCount();
	//����ΪstrSheet�ı� 
	BOOL OpenSheet(CString strSheet = _T(""), bool bIsAdded = true); 
	bool OpenSheet(int nCount);
	BOOL OpenExcelDoc();
	//����(ROW,COL)���ַ�strText 
	BOOL SetItemText(long Row,long Col,CString strText); 
	//ȡ��(ROW,COL)���ַ� 
	CString GetItemText(long Row,long Col); 
	bool selectRange(long Row, long Col);
	//��ȡborderֵ
	vector<int> GetBorderItem(long Row,long Col, bool& bAdd);
	//��ȡ���뷽ʽ 
	//�����п� 
	double GetColumnWidth(long Row,long Col); 
	//�����и� 
	double GetRowHeight(long Row,long Col); 
	//��ȡ��Ч����
	void GetArea();
	
	bool verifyData();

	//�˳�excel 
	void Exit(); 
	//��ʾexcel 
	void SetVisible(BOOL blnVisible); 
	//���Ҵ��ļ��Ƿ���� 
	BOOL IsFileExist(CString strFn, BOOL bDir); 
	//�Զ�����(��Դ��Ѿ����ڵ��ļ�) 
	void Save(); 
	//����ΪstrPath 
	void SaveAs(CString strPath); 
	//����µı� 
	void AddSheet(CString strSheet); 
	//�õ��µ�ѡ������ 
	void GetRange(CString strBegin,CString strEnd); 
	//�õ�Range��Χ
	void GetRangeArea(CString strBegin,CString strEnd);

	//���Զ�չ�� 
	void AutoColFit(); 
	//���Զ�չ�� 
	void AutoRowFit(); 
	//�����Զ����� 
	void SetWrapText(BOOL blnTrue); 
	//�õ��������� 
	void AutoRange(); 
	//�ϲ���Ԫ�� 
	void SetMergeCells(BOOL blnTrue); 
	//�ϲ���Ԫ����
	void MergeCells(long nRow, long nCol, long nEndRow, long nEndCol);
	//�õ���Ч���� 
	long GetRowS(); 
	//�õ���Ч���� 
	long GetColS(); 
	//�����п� 
	void SetColumnWidth(int intWidth); 
	//�����и� 
	void SetRowHeight(int intHeight); 
	//�����в�����
	void InsertRow(CString strBegin, CString strEnd, long nStart);
	//���ص�ǰ��������·�� 
	CString GetAppPath(); 
	//void UpdateData(AcDbObjectId blkRecId);

	void Clear();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//����Ϊ��Ŀ���ӵĺ���
	void readSpecailCol();
	//��ȡLRMV-p����
	void readLrmvpCol();
	//��ȡlrmv����
	void readLrmvCol();

	bool verifyItemLength(CString strItem);
	void readLrmvdata(int nLen);
	void writeFile();
	void writeLrmvFile();

	map<CString, vector<vector<CString> > > ReadAllData();
	//��ȡ��ǰ�ҳ�������
	vector<vector<CString> > ReadActiveSheetData(CString& strSheetName);
	void readDigitalData(int nLen, int nType = 0);
	void readCommonData(int nLen);
	void readLrmCommonData(int nLen);
	void readLrmvpData(int nLen);
	void readLrmvData(int nLen);

	//����������Ϣ
	vector<vector<CString> > doNameInfo(int nLen);
	void _Excel_Find_(LPCTSTR lpszFind);
	bool setVisible(bool bIsVisible);
	void CloseExcelFile(BOOL if_save = FALSE);
private:
	long iRowNum;
	long iColNum;
	long iStartRow;
	long iStartCol;

	int m_nItem;
	map<int, CNameCell > m_MapData;
	map<CString, CSingleSheetData> m_datainfo;
	vector<CString> m_nameVec;
	vector<CString> m_modNameVec;//ȫ��ģ�������

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//��¼lrmv����
	vector<pair<CString, vector<vector<CString> > > > m_ExcelVec;
	//��¼����
	vector<CString> m_blkNameVec;
};


