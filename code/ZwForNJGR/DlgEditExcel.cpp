// DlgEditExcel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgEditExcel.h"
#include "afxdialogex.h"


// CDlgEditExcel �Ի���
extern Adesk::Boolean startDlg();
extern Adesk::Boolean endDlg();
extern CGlobalQuery gQuery;
extern CDlgProress *modelessDlg;

IMPLEMENT_DYNAMIC(CDlgEditExcel, CDialogEx)

CDlgEditExcel::CDlgEditExcel(bool refreshExcel, CWnd* pParent /*=NULL*/)
: CAcUiDialog(CDlgEditExcel::IDD, pParent)
{
	//��ȡexcel��ַ
	m_sExcelPath = gGlobal.GetIniValue(_T("ͼ������"), _T("·��"));
	m_bRefExcel = refreshExcel;
	init();
}

CDlgEditExcel::~CDlgEditExcel()
{
}

void CDlgEditExcel::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EXCELPATH, m_sExcelPath);
}


BEGIN_MESSAGE_MAP(CDlgEditExcel, CAcUiDialog)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CDlgEditExcel::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDOK, &CDlgEditExcel::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEditExcel ��Ϣ�������


void CDlgEditExcel::init()
{
	m_dFirstTableLength = gGlobal.GetIniValue(_T("��������"), _T("����һ�γ���"), 0);
	m_dSecondTableLength = gGlobal.GetIniValue(_T("��������"), _T("���ڶ��γ���"), 0);
	m_dThirdTableLength = gGlobal.GetIniValue(_T("��������"), _T("�������γ���"), 0);
	m_dTableLength = m_dFirstTableLength + m_dSecondTableLength + m_dThirdTableLength;

	m_dRfSecondTableLength = gGlobal.GetIniValue(_T("��������"), _T("RF���ڶ��γ���"), 0);
	m_dRfThirdTableLength = gGlobal.GetIniValue(_T("��������"), _T("RF�������γ���"), 0);
}

void CDlgEditExcel::OnBnClickedBtnSelect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CFileDialog importDlg(TRUE, _T(".xls"), NULL, 0,
		_T("excel��ʽ(*.xls;*.xlsx) |*.xls;*.xlsx|"), acedGetAcadFrame());
	if (importDlg.DoModal() == IDOK)
	{
		m_sExcelPath = importDlg.GetPathName();
	}
	else
	{
		return;
	}
	UpdateData(FALSE);
}

void CDlgEditExcel::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_bRefExcel)
	{
		if (GetModInfoForDwg())
		{
			EditExcelForModInfo();
		}
	}
	else
	{
		if (GetModInfoForExcel())
		{
			EditDwgForModInfo();
		}
	}
	CAcUiDialog::OnOK();
}

bool CDlgEditExcel::GetModInfoForDwg()
{
	m_vecModInfo.clear();
	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("��������"));
	}
	AcDbObjectIdArray idArr = CBaseUtils::GetObjectIdArrayForXdata(ICD_XDATA_XIUGAI);
	for (int i = 0; i < idArr.length();i++)
	{
		ModExcelInfo modTemp;
		MyBaseAppXdata::getXdata(ICD_XDATA_SHEET, modTemp.sheetName, idArr[i]);
		MyBaseAppXdata::getXdata(ICD_XDATA_ROW, modTemp.row, idArr[i]);
		MyBaseAppXdata::getXdata(ICD_XDATA_COL, modTemp.col, idArr[i]);
		MyBaseAppXdata::getXdata(ICD_XDATA_TEXING, modTemp.texing, idArr[i]);
		int nType;
		MyBaseAppXdata::getXdata(ICD_XDATA_TYPE, nType, idArr[i]);
		modTemp.type = CBaseUtils::GetTexingType(nType);
		m_vecModInfo.push_back(modTemp);
		if (modelessDlg!=NULL)
		{
			modelessDlg->setPos((i+1) * 100 / idArr.length());
			Sleep(50);
		}
	}
	endDlg();
	if (m_vecModInfo.size()==0)
	{
		return false;
	}
	sort(m_vecModInfo.begin(), m_vecModInfo.end());
	return true;
}

void CDlgEditExcel::EditExcelForModInfo()
{
	CMyExcel excel;
	BOOL bRet = excel.Open(m_sExcelPath);
	if (!bRet)
	{
		AfxMessageBox(_T("excel��ʧ��!����excel�ļ���"));
		return;
	}
	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("�����޸�excel"));
	}
	for (int i = 0; i < m_vecModInfo.size(); i++)
	{
		ModExcelInfo modTemp = m_vecModInfo[i];
		if (!excel.OpenSheet(modTemp.sheetName))
		{
			continue;
		}
		excel.SetItemText(modTemp.row, modTemp.col, modTemp.type + modTemp.texing);
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos((i+1) * 100 / m_vecModInfo.size());
			Sleep(50);
		}
	}
	endDlg();
	excel.Save();
	excel.Exit();
}

bool CDlgEditExcel::GetModInfoForExcel()
{
	UpdateData(TRUE);

	CMyExcel myExcel;
	BOOL bRet = myExcel.Open(m_sExcelPath);
	if (!bRet)
	{
		AfxMessageBox(_T("�ļ����ܴ򿪣�����·���Ƿ���Ч"));
		return false;
	}

	//�ڳ�ʼ��ʱ���������ÿ�
	gQuery.clear();
	startDlg();
	Sleep(100);
	if (modelessDlg != NULL)
	{
		modelessDlg->SetWindowText(_T("�Զ�У��"));
	}

	int nCount = myExcel.GetSheetCount();
	for (int i = 1; i <= nCount; i++)
	{
		bRet = myExcel.OpenSheet(i);
		myExcel.GetArea();
		myExcel.readSpecailCol();
		if (modelessDlg != NULL)
		{
			modelessDlg->setPos(i * 50 / nCount);
			Sleep(50);
		}
	}
	myExcel.Clear();
	endDlg();

	return true;
}

void CDlgEditExcel::EditDwgForModInfo()
{
	gQuery.exchangeTexingCommonData();
}

BOOL CDlgEditExcel::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	if (!m_bRefExcel)
	{
		this->SetWindowText(_T("��excel�ֲ�ˢ��ͼֽ"));
	}
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
