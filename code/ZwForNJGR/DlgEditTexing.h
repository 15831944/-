#pragma once
#include "resource.h"
#include "BaseUtils.h"

// CDlgEditTexing �Ի���

class CDlgEditTexing : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDlgEditTexing)

public:
	CDlgEditTexing(CString row,CString col,CString sheetname,int type,CString duanzi,CString xianhao,
		CString quxiang,CString texing,AcDbObjectId idEdit,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgEditTexing();

// �Ի�������
	enum { IDD = IDD_DLG_EDITTX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_sCol;
	CString m_sDuanzi;
	CString m_sQxiang;
	CString m_sRow;
	CString m_sSheetName;
	CString m_sTexing;
	CString m_sTexingYuanyou;
	CString m_sXianhao;
	CString m_sType;
	AcDbObjectId m_idEdit;
	bool m_bEdit;
	bool m_bDelete;
	bool m_bAdd;
	void editTx();
	void deleteTx();
	void addTx();

	void init();

	double m_dTableLength;//��񳤶�

	double m_dFirstTableLength;//��һ������
	double m_dSecondTableLength;//�ڶ�������
	double m_dThirdTableLength;//����������

	double m_dRfSecondTableLength;//RF�ڶ��ڳ���
	double m_dRfThirdTableLength;//RF�����ڳ���
public:
	afx_msg void OnBnClickedOk();
};
