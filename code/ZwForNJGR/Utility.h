#pragma once
#include "CCommonCell.h"

class CUtility
{
public:
	CUtility(void);
	~CUtility(void);

	static AcDbObjectId getStyleIdByiniFile(CString strHead, CString strNo);
	static AcDbObjectId createBlkRef();
	static AcDbObjectId getContentBlkRef();
	static int getRowByPt(AcGePoint3d pt);
	static int getRowAndColByPt(AcGePoint3d pt, int& nRow, int& nCol);
	static double getAddData(AcGePoint3d pt);
	static CString GetLocalUserDocPath();
	static CString getFileNameByFullName(CString strFullName);	
	//excel��dwg�ļ�·��
	static CString getFilePath();
	//log��־·��
	static CString getLogPath();
	static CString getBmPath();
	static CString getDate();
	static CString getExcelPos(int nRow, int nCol);

	static CString getFrameIniFile();

	static void UpdatePageNo(int nBegin);
	//��������:
	//************************************
	// Method:    ParserStringComma
	// FullName:  CUtility::ParserStringComma
	// Access:    public static 
	// Returns:   int ���أ���index
	// Qualifier:
	// Parameter: CString strInput ������������
	// Parameter: CString & strDuanzi�����������
	// Parameter: CString & strTexing��������
	//************************************
	static int ParserStringComma(CString strInput, CString& strTexing);

	//************************************
	// Method:    IsIgnoreErrorFlag
	// FullName:  CUtility::IsIgnoreErrorFlag
	// Access:    public static 
	// Returns:   bool
	// Qualifier: �����Ƿ��������
	// Parameter: CCommonCell cell
	//************************************
	static bool IsIgnoreErrorFlag(CCommonCell cell);

	//************************************
	// Method:    EditignoreError
	// FullName:  CUtility::EditignoreError
	// Access:    public static 
	// Returns:   void
	// Qualifier:�޸���Ҫ�༭�ĺ�������
	// Parameter: AcDbObjectIdArray objIdArr
	//************************************
	static void EditignoreError(AcDbObjectIdArray objIdArr);
	static void DwgZoomExtent( AcDbDatabase *pDb );
	static AcDbExtents GetModelSpaceExtent( AcDbDatabase *pDb );
	static AcDbObjectIdArray GetAllEntityIds( const TCHAR* layerName, AcDbDatabase *pDb );
	static void ZoomExtent();
	static void GetCurrentView( AcDbViewTableRecord &view );
	static AcGePoint3d WcsToDcsPoint( const AcGePoint3d &point );
	static CString RemoveString(CString strInput, CString strTemp);
	static BOOL CreateDirectoryWithUserFullControlACL(LPCTSTR lpPath);

	//************************************
	// Method:    getIniFromFile
	// FullName:  CUtility::getIniFromFile
	// Access:    public static 
	// Returns:   bool
	// Qualifier:��ȡͼ��������Ϣ����Ϣ
	// Parameter: CString strProperty
	// Parameter: double & dX
	// Parameter: double & dY
	// Parameter: double & dSize
	//************************************
	static void getIniFromFile(CString strProperty, double& dX, double& dY, double& dSize);

	//************************************
	// Method:    ParserStringAndGetStartAndEnd
	// FullName:  CUtility::ParserStringAndGetStartAndEnd
	// Access:    public static 
	// Returns:   void
	// Qualifier:�����ַ�����������ʼ����ֹ
	// Parameter: CString strInput
	// Parameter: int & nStart
	// Parameter: int & nEnd
	//************************************
	static bool ParserStringAndGetStartAndEnd(CString strInput, int& nStart, int& nEnd);

	static void doIt();
	static void preView();
	static void UpdatePageNo();
	//��֤�Ƿ�dp��rf��һһƥ���ϵ
	static bool verifyOther();
};
