#pragma once
#include "TableData.h"
#include "RFData.h"
#include "CommonData.h"
#include "GLData.h"
#include <vector>
using namespace std;

class CSheetStoreData
{
public:
	CSheetStoreData(void);
	~CSheetStoreData(void);

	void setSheetName(CString strSheetName);
	void setTableData(vector<CTableData> tableData);
	void setRFData(vector<CRFData> rfTableData);
	void setCommonData(vector<CCommonData> comData);
	void setGLData(vector<CGLData> glData);
	void setType(int nType);
	void setSheetType(int nType);
	void setLrmName(CString strLrmName);
	void setModLrmName(CString strModLrmName);
	//���ʵ��
	//************************************
	// Method:    addTableItem
	// FullName:  CSheetStoreData::addTableItem
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CTableData tblData
	//************************************
	void addTableItem(CTableData tblData);
	//************************************
	// Method:    addRfDataItem
	// FullName:  CSheetStoreData::addRfDataItem
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CRFData rfData
	//************************************
	void addRfDataItem(CRFData rfData);
	//************************************
	// Method:    addCommonDataItem
	// FullName:  CSheetStoreData::addCommonDataItem
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CCommonData comData
	//************************************
	void addCommonDataItem(CCommonData comData);
	//************************************
	// Method:    addGlDataItem
	// FullName:  CSheetStoreData::addGlDataItem
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CGLData glData
	//************************************
	void addGlDataItem(CGLData glData);

	CString getSheetName() const;
	vector<CTableData> getTableData() const;
	vector<CRFData> getRfData() const;
	vector<CCommonData> getCommonData() const;
	vector<CGLData> getGLData() const;
	int getType() const;
	int getSheetType() const;

	CString getLrmName() const;
	CString getModLrmName() const;

	void clear();
	CSheetStoreData& operator=(const CSheetStoreData& node);
private:
	vector<CTableData> m_tableData;//�������
	vector<CRFData> m_RfData;//��Ƶ����
	vector<CCommonData> m_CommonData;//��ͨ����
	vector<CGLData> m_glData;//GL����
	int m_nType;//sheet���ͣ������Ϊ0����ʾA0,Ϊ1��ʾA1
	int m_nSheetType;//0Ϊ��ͨ���ͣ�1ΪLrm���ͣ�2ΪLRMV���ͣ�3Ϊ���ֹ�����
	CString m_strSheetName;
	CString m_strLrmName;
	CString m_strModLrmName;
};
