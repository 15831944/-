#pragma once
#include "CCommonCell.h"
#include <list>
using namespace  std;

//�����ߺ���أ�����DP��RF
class CSingalNo
{
public:
	CSingalNo(void);
	~CSingalNo(void);
	void clear();
	//��������
	void setType(CString strType);
	//������
	void AddData(CCommonCell& cell);
	//��ӿ������ݵ�������
	void addFixNumAreaToStore(int nStart, int nEnd);
	//��ȡȥ������
	CString getQuxiang(CCommonCell& cell);
	//ɾ�����������
	void EraseFixedInfo();	
	//���ɽ��߱�ͽ��߹�ϵ����Ҫʹ�õ�����
	MPMAP getOutPutData() const;

	//У��dp����
	bool doDPData();

private:
	//���й̶���ŵ����ݼ�����ҹ̶���Ŷ�Ӧ��ȥ��
	CString getQuxiangByNum(CCommonCell& cell);
	//���ù̶���ŵ�����
	void doNumber(CCommonCell& cell);
	bool doSpecailData(COMVEC tmpVec, int nCount);
	bool doRDpData();
	CString doMSNumAndQuxiang(COMVEC tmpVec, int nCount, CCommonCell& cell);
	CString doRMSNumAndQuxiang(COMVEC tmpVec, int nCount, CCommonCell& cell);
	map<int, COMVEC> doRLevelData(COMVEC tmpVec);

	bool FindMultiData(pair<MDMap::iterator, bool> ret, CCommonCell cell);
	map<int,COMVEC> doFixDataToMap(COMVEC tmpVec);

	//�����ڲ�������
	CString doTwoData(COMVEC tmpVec, CCommonCell cell);
	//�����ڲ�����
	CString getFixNumFromFixMap(map<int, COMVEC> mapInf, int nCount, CCommonCell cell);
	void addDataToSpecailMData(int nCount, COMPAIR tmpPair);
	void doRMSAndSetCount(CCommonCell& cell);
private:
	list<CString> m_ListInfo;
	map<int, int> m_NumVec;
	MDMap m_NumInfo;
	MDMap m_Data;
	MDMap m_RData;
	MPMAP m_tmpRMSInfo;//��¼��ʱ��R��MS����
	MPMAP m_dpoutPutMap;

	MVMAP m_RDPRealData;//M������ǰ�棬S�ں���
	MRMAP m_RRDpData;

	CString m_strType;//���ͣ�RF��DP
	CString m_strTypeEqual;//���͵Ⱥ�
};

