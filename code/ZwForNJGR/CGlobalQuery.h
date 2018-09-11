#pragma once
#include "CCommonCell.h"
#include "WriteLog.h"
#include "SheetStoreData.h"
#include "CFrameInfo.h"
#include "SingleSheetData.h"
#include "CIgnoreErrorCell.h"
#include "CSingalNo.h"
#include "BaseUtils.h"


//ȫ�ֲ�ѯ����
class CGlobalQuery
{
public:
	CGlobalQuery(void);
	~CGlobalQuery(void);

	void clear();
	//�����vector������
	void clearVec();
	//���Error����
	void clearErrorLog();

	void doCommonCell(CCommonCell& cell);
	void doMTCommonCell(CCommonCell& cell);
	//void doSzgMTCommonCell(CCommonCell cell);
	//void doMngMTCommonCell(CCommonCell cell);
	void doRFCommonCell(CCommonCell& cell);

	bool doIt();
	//ͨ�������ҵ����Զ�Ӧ��ȥ��
	CString getQuxiang(CCommonCell& cell);

	//��ӡ��Ϣ������ļ�����
	void addCellInfo(CCommonCell cell);
	bool writeCellInfoToFile();

	//////////////////////////////////////////////////////////////////////////
	//������߱�
	//bool writeJxb();
	//����������ӹ�ϵ��
	bool writeJxljgxb();
	//���Ӵ�����Ϣ
	void addErrorString(CString strError);
	//дerror log��־
	bool writeErrorLog();

	//��ȡLRMV����
	void addLrmvSheetData(CSheetStoreData data);
	vector<CSheetStoreData> getLRMVSheetData();
	//��ȡlrmv mod����
	void addLrmvModSheetData(CSheetStoreData data);
	vector<CSheetStoreData> getLRMVModSheetData();
	void addDigitalSheetData(CSheetStoreData data);
	//��ȡ���ֹ�����
	vector<CSheetStoreData> getDigitalSheetData();
	//����ۺ��������
	void addCommonSheetData(CString strSheet, CSingleSheetData data);
	map<CString, CSingleSheetData> getCommonData();
	//���modname
	void addModNameItem(CString str);
	//���name
	void addSheetNameItem(CString str);
	//��ӿ�����
	void addBlockName(CString str);

	//����
	bool draw();
	//��ͨͼ�ν�������
	bool exchangeTexingCommonData();

	//�Ƿ��޸�ͼ��
	bool getEditFrameFlag();
	//��T�������ݼ���vector��
	void addTDataToVec(CCommonCell comCell);
	//�������ź�
	void clearDifferentialSignal();
	void adddifferSignal(CCommonCell comCell, bool bIsDefaultRF = false);
	void addPlusSignal(CString strItem, CCommonCell comCell);
	void addMinusSignal(CString strItem, CCommonCell comCell);
	void addPSignal(CString strItem, CCommonCell comCell);
	void addNSignal(CString strItem, CCommonCell comCell);
	void doDifferSignal();
	void TraversingDifferData(map<CString, CCommonCell> data1, map<CString, CCommonCell> data2); 
	void setFatalErrorFlag();

	void addExcelInfo(CString strSheetName, vector<vector<CString> > tmpVec);
	map<CString, vector<vector<CString> > > getExcelInfo() const;	
	
	map<CString, vector<pair<CCommonCell, CCommonCell> > > getMTOutPutData();
	CSingalNo getDpInfo() const;
	CSingalNo getRfInfo() const;
	//��MT���ݴ���map��map<cstring, vector<ccomcell> >
	map<CString, vector<pair<CCommonCell, CCommonCell> > > doMTInfoToMap();
	COMVEC getTInfo() const;
	vector<pair<CCommonCell, CCommonCell> > getRMTInfo() const;	
	//����comcell�õ�������Ϣ
	CString getErrorString(CCommonCell comCell);
	void setErrorType(CString strErrorType);
	//��ӿ������ݵ�������
	void addFixNumAreaToStore(CString strType, int nStart, int nEnd);
	//����Ԥ���ı��
	void setPreviewFlag(bool bIsFlag);
	bool getPreviewFlag();
	COMVEC getCellInfo();

private:
	bool doSData();

	bool doCData();
	bool doRData();
	bool doRDpData();
	
	//MT -R
	bool doRMTData();

	bool doRFData();
	bool doDPData();
	bool doMTData();
	bool doMData();

	bool doRMData();

	CString getSimpleResult(MDMap dData, CString strTexing, CString strTempQuxiang);
	CString getSpecialResult(MDMap dData, CCommonCell cell, CString strTexing, CString strTempQuxiang, int& nCount);

	//������R�㼶����R+S�㼶������R+M�㼶
	bool doRLevel(CString& strTexing, CCommonCell& cell);
	//����MT R�㼶
	bool doMTRLevel(CString& strTexing, CCommonCell& cell);
	//������߱�
	//��ȡDP�������
	map<int, pair<CCommonCell, CCommonCell> > getDpOutPutData();
	//��ȡRF�������
	map<int, pair<CCommonCell, CCommonCell> > getRFOutPutData();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//����
	//����LRMV modͼ��
	bool drawLrmvModData();
	//����LRMVͼ��
	bool drawLrmvData();
	void drawDigitalData();
	//������ͨͼ��
	bool drawCommonData();

	//����ͼ��
	bool drawTu();
	//��R+s���ݴ���R��
	void AddRSToRData();

	//����ignoreerror�Ի���
	bool CallIgnoreDlg(CString strPrompt);
	//��ȡR�㼶Cell��ȥ��
	CString getRLevelQuxiang(CCommonCell cell);
	//��ȡR�㼶MT Cell��ȥ��
	CString getRMTLevelQuxiang(CCommonCell cell);
	//��ȡDP count����
	CString getDpSpecialResult(CCommonCell& cell);
	
	//************************************
	// Method:    getDpCmpString
	// FullName:  CGlobalQuery::getDpCmpString
	// Access:    private 
	// Returns:   CString
	// Qualifier:����cell���������ڲ������ݵ��ַ����������Ƿ���㼶�ı�ʶ
	// Parameter: CCommonCell cell
	// Parameter: bool & bIsLevel
	//************************************
	CString getDpCmpString(CCommonCell cell, CString& strPre, bool& bIsLevel);
	//************************************
	// Method:    FindDataFromRDpData
	// FullName:  CGlobalQuery::FindDataFromRDpData
	// Access:    private 
	// Returns:   CString
	// Qualifier:��DP�����ҵ�ȥ�����ݣ���������Ӧ�ı��
	// Parameter: CCommonCell & cell
	// Parameter: CString strTmp
	//************************************
	CString FindDataFromRDpData(CCommonCell& cell, CString strTmp);
	//************************************
	// Method:    GetMDpError
	// FullName:  CGlobalQuery::GetMDpError
	// Access:    private 
	// Returns:   bool
	// Qualifier:����M S DP������һһƥ����֤
	// Parameter: vector<CCommonCell> mComVec
	// Parameter: vector<CCommonCell> sComVec
	//************************************
	bool GetMDpError(vector<CCommonCell> mComVec, vector<CCommonCell> sComVec);
	//************************************
	// Method:    doDpNumber
	// FullName:  CGlobalQuery::doDpNumber
	// Access:    private 
	// Returns:   void
	// Qualifier:��ȡ�ֶ����õ�dp number�����õ�������
	// Parameter: CCommonCell & cell
	//************************************
	void doDpNumber(CCommonCell& cell);
	CString getQuxiangByDpNum(CCommonCell& cell);
	CString getQuxiangByRfNum(CCommonCell& cell);
	void EraseFixedNum();
	void doRFNumber(CCommonCell& cell);

	//int getDpNumbyCount(int nCount);
	
private:
	MDMap m_RFData;
	MDMap m_DPData;
	map<CString, vector<pair<CCommonCell, CCommonCell> > > m_RDPRealData;//M������ǰ�棬S�ں���
	MRMAP m_RRDpData;

	MDMap m_SData;
	MDMap m_MData;
	MDMap m_MTData;
	MDMap m_CData;//��ͨ����

	//MDMap m_SZGMTData;//���ֹ�����
	//MDMap m_MNGMTData;//ģ�������
	//////////////////////////////////////////////////////////////////////////
	MRMAP m_RData;
	MRMAP m_RSData;
	MRMAP m_RMData;
	MRMAP m_RMTData;

	map<pair<int, CString>, CCommonCell> m_RRMTData;
	//���ڹ������ݵĽ��߱�����
	vector<pair<CCommonCell, CCommonCell> > m_RRMTJxData;
	//////////////////////////////////////////////////////////////////////////

	map<CString, CString> m_MMData;
	map<CString, CString> m_SSData;
	map<pair<int, CString>, CCommonCell> m_RRData;

	CWriteLog m_log;

	vector<CString> m_ErrorVec;//��¼������Ϣ
	vector<CIgnoreErrorCell> m_IgnoreVec;//��¼���Դ�����Ϣ
	
	//��¼LRMV����
	vector<CSheetStoreData> m_LrmvSheetData;
	//��¼LRMV mod����
	vector<CSheetStoreData> m_LrmvModSheetData;
	//��¼���ֹ�����
	vector<CSheetStoreData> m_DigitalSheetData;

	//��¼����
	vector<CString> m_blkNameVec;
	vector<CString> m_nameVec;
	vector<CString> m_modNameVec;//ȫ��ģ�������
	map<CString, CSingleSheetData> m_datainfo;
	vector<CCommonCell> m_ComvecError;
	//��¼T��������
	vector<CCommonCell> m_TComVec;
	//��¼���+�ź�
	map<CString, CCommonCell> m_PlusSignal;
	map<CString, CCommonCell> m_MinusSignal;
	map<CString, CCommonCell> m_PSignal;
	map<CString, CCommonCell> m_NSignal;
	bool m_bSignalFlag;//��¼����źŵĴ���״̬
	bool m_bFatalError;//��������
	int m_RDplevel;//��¼DP��R�㼶
	
	CString m_strErrorType;//��������

	map<CString, COMVEC> m_dpNumInfo;//dp�ֶ����õ���ֵ
	map<CString, COMVEC> m_rfNumInfo;//RF�ֶ����õ���ֵ
	map<int, int> m_dpNumVec;
	map<int, int> m_rfNumVec;
	COMVEC m_InfoVec;//����Commoncell��Ϣ
	map<CString, vector<vector<CString> > > m_ExcelInfo;

	CSingalNo m_DpInfo;
	CSingalNo m_RfInfo;
	bool m_bIsPreview;//�Ƿ�Ԥ��
};

