#pragma once
#include "resource.h"
#include "BaseUtils.h"
#include "MyExcel.h"
#include <algorithm>
#include "DlgProress.h"
#include "CGlobalQuery.h"
#include "Utility.h"

// CDlgEditExcel �Ի���
struct ModExcelInfo
{
	CString sheetName;
	int row;
	int col;
	CString texing;
	CString type;
	bool operator<(const ModExcelInfo&b)
	{
		if (this->sheetName<b.sheetName)
		{
			return true;
		}
		else if (this->sheetName>b.sheetName)
		{
			return false;
		}
		else
		{
			if (this->row<b.row)
			{
				return true;
			}
			else if (this->row>b.row)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
};

class CDlgEditExcel : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDlgEditExcel)

public:
	CDlgEditExcel(bool refreshExcel,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgEditExcel();

// �Ի�������
	enum { IDD = IDD_DLG_EDITEXCEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_sExcelPath;
private:
	bool m_bRefExcel;//���Ϊtrue ���dwgˢ����Ϣ��excel ���Ϊfalse ���excel�����޸�����

	double m_dTableLength;//��񳤶�

	double m_dFirstTableLength;//��һ������
	double m_dSecondTableLength;//�ڶ�������
	double m_dThirdTableLength;//����������

	double m_dRfSecondTableLength;//RF�ڶ��ڳ���
	double m_dRfThirdTableLength;//RF�����ڳ���

	std::vector<ModExcelInfo> m_vecModInfo;
	bool GetModInfoForDwg();//��ͼֽ�л�ȡ�޸���Ϣ
	void EditExcelForModInfo();//ͨ���޸���Ϣˢ��excel

	bool GetModInfoForExcel();//�����޸Ĵ�excel�л�ȡˢ��ȥ����Ϣ
	void EditDwgForModInfo();//ͨ���޸���Ϣˢ��dwg
public:
	void init();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
