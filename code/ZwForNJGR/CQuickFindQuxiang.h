#pragma once
#include "CCommonCell.h"
#include "CIgnoreErrorCell.h"
//���ٲ���ȥ�����ڵ�ԭʼexcelλ�ã������޸�

class CQuickFindQuxiang
{
public:
	CQuickFindQuxiang(void);
	~CQuickFindQuxiang(void);
	bool doIt();
	//��ѡ
	bool sel();
	//��������
	bool AnalysisData();
	//��ӡ��Ϣ
	void print();
	//��ʾ�Ի���
	bool showDialog();
private:
	void addDlgInfo(CCommonCell curComcell);
private:
	CString m_strContent;//����
	CString m_strSheetName;
	COMVEC m_VecInfo;//����
	vector<CIgnoreErrorCell> m_dlgInfo;
};

