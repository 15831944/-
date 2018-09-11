//-----------------------------------------------------------------------------
//----- DlgExcelDataCmp.cpp : Implementation of CDlgExcelDataCmp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgExcelDataCmp.h"
#include "MyExcel.h"

static  int Excel_data_List_Type_Init( int col )
{
	if ( col == 0 )
		return CEditList::eLast;
	return CEditList::eEdit;
}
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgExcelDataCmp, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgExcelDataCmp, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgExcelDataCmp::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgExcelDataCmp::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRE, &CDlgExcelDataCmp::OnNMClickListPre)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CUR, &CDlgExcelDataCmp::OnNMClickListCur)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRE, &CDlgExcelDataCmp::OnNMDblclkListPre)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CUR, &CDlgExcelDataCmp::OnNMDblclkListCur)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRE, &CDlgExcelDataCmp::OnLvnItemchangedListPre)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CUR, &CDlgExcelDataCmp::OnLvnItemchangedListCur)
	ON_BN_CLICKED(IDOK, &CDlgExcelDataCmp::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_OTHER, &CDlgExcelDataCmp::OnEnKillfocusEditOther)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgExcelDataCmp::CDlgExcelDataCmp (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgExcelDataCmp::IDD, pParent, hInstance) 
{
	m_nType = 0;
}

//-----------------------------------------------------------------------------
void CDlgExcelDataCmp::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST_PRE, m_preListCtrl);
	DDX_Control(pDX, IDC_LIST_CUR, m_curListCtrl);
	DDX_Control(pDX, IDC_EDIT_OTHER, m_SubItemEdit);
	DDX_Text(pDX, IDC_EDIT_OTHER, m_strItemtextEdit);
	DDX_Control(pDX, IDC_BUTTON1, myBitmapButton1);
	DDX_Control(pDX, IDC_BUTTON2, m_myBtn2);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgExcelDataCmp::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgExcelDataCmp::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rct;
	GetDlgItem(IDC_LIST_CUR)->GetWindowRect(rct);
	double dWidth = rct.Width();
	m_preListCtrl.SetExtendedStyle( m_preListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_preListCtrl.InsertColumn( 0, _T("����"));
	m_preListCtrl.InsertColumn( 1, _T("ԭʼ����"));
	m_preListCtrl.InsertColumn(2, _T("�޸�����"));
	m_preListCtrl.SetColumnWidth( 0, dWidth/4);
	m_preListCtrl.SetColumnWidth( 1, 3*dWidth/8);
	m_preListCtrl.SetColumnWidth( 2, 3*dWidth/8 - 2);

	// set functionality of list according to column


	m_curListCtrl.SetExtendedStyle( m_curListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_curListCtrl.InsertColumn( 0, _T("����"));
	m_curListCtrl.InsertColumn( 1, _T("ԭʼ����"));
	m_curListCtrl.InsertColumn( 2, _T("�޸�����"));
	m_curListCtrl.SetColumnWidth( 0, dWidth/4);
	m_curListCtrl.SetColumnWidth( 1, 3*dWidth/8);
	m_curListCtrl.SetColumnWidth(2, 3*dWidth/8 - 2);
	// set functionality of list according to column
	Init();

	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	m_SubItemEdit.ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC_PREFILE)->SetWindowText(m_strPreFile);
	GetDlgItem(IDC_STATIC_CUR)->SetWindowText(m_strCurFile);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExcelDataCmp::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	int nCount = m_preListCtrl.GetItemCount();
	CString strValue;
	for (int i=0; i<nCount; i++)
	{
		if (m_preListCtrl.GetCheck(i))
		{
			strValue = m_preListCtrl.GetItemText(i,	1);
			m_curListCtrl.SetItemText(i, 2, strValue);
		}
	}
}

void CDlgExcelDataCmp::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	int nCount = m_curListCtrl.GetItemCount();
	CString strValue;
	for (int i=0; i<nCount; i++)
	{
		if (m_curListCtrl.GetCheck(i))
		{
			strValue = m_curListCtrl.GetItemText(i, 1);
			m_preListCtrl.SetItemText(i, 2, strValue);
		}
	}
}

void CDlgExcelDataCmp::setData(vector<CExcelDifferInfo> vecDif)
{
	m_VecInfo.clear();
	m_VecInfo.insert(m_VecInfo.end(), vecDif.begin(), vecDif.end());
}

bool CDlgExcelDataCmp::Init()
{
	CString strType,strValue;
	int nItem = 0;
	for (vector<CExcelDifferInfo>::iterator iter = m_VecInfo.begin(); iter != m_VecInfo.end(); ++iter)
	{
		CExcelDifferInfo excelDif = *iter;
		strType = excelDif.getRange();
		strValue = excelDif.getOldValue();

		nItem = m_preListCtrl.GetItemCount();
		nItem = m_preListCtrl.InsertItem( nItem+1, _T("") );
		m_preListCtrl.SetItemText(nItem, 0, strType);
		m_preListCtrl.SetItemText(nItem, 1, strValue);
		m_preListCtrl.SetItemText(nItem, 2, strValue);

		strValue = excelDif.getNewValue();
		nItem = m_curListCtrl.GetItemCount();
		nItem = m_curListCtrl.InsertItem( nItem+1, _T("") );
		m_curListCtrl.SetItemText(nItem, 0, strType);
		m_curListCtrl.SetItemText(nItem, 1, strValue);
		m_curListCtrl.SetItemText(nItem, 2, strValue);
	}
	
	return true;
}



void CDlgExcelDataCmp::OnNMClickListPre(NMHDR *pNMHDR, LRESULT *pResult)
{
	//��ȡ�������ڵ��к�
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_preListCtrl.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;

	int iItem = m_preListCtrl.HitTest(&lvinfo);
	if(-1 != iItem)
	{
		//�ж��Ƿ�����checkbox��
		m_npreItem = lvinfo.iItem;
		if (lvinfo.flags == LVHT_ONITEMSTATEICON)
		{
			m_bPreHit = TRUE;
		}
		else
		{
			if (m_preListCtrl.GetCheck(m_npreItem))
			{
				m_preListCtrl.SetCheck(m_npreItem, FALSE);
			}
			else
			{
				m_preListCtrl.SetCheck(m_npreItem, TRUE);
			}
		}

		//��������check��ϵ
		int nCount = m_curListCtrl.GetItemCount();
		for (int j=0; j<nCount; j++)
		{
			m_curListCtrl.SetCheck(j, FALSE);
		}
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}

	
	*pResult = 0;
}

void CDlgExcelDataCmp::OnNMClickListCur(NMHDR *pNMHDR, LRESULT *pResult)
{
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_curListCtrl.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;

	int iItem = m_curListCtrl.HitTest(&lvinfo);
	if(-1 != iItem)
	{
		//�ж��Ƿ�����checkbox��
		m_ncurItem = lvinfo.iItem;
		if (lvinfo.flags == LVHT_ONITEMSTATEICON)
		{
			m_bCurHit = TRUE;
		}
		else
		{
			if (m_curListCtrl.GetCheck(m_ncurItem))
			{
				m_curListCtrl.SetCheck(m_ncurItem, FALSE);
			}
			else
			{
				m_curListCtrl.SetCheck(m_ncurItem, TRUE);
			}
		}

		//��������check��ϵ
		int nCount = m_preListCtrl.GetItemCount();
		for (int j=0; j<nCount; j++)
		{
			m_preListCtrl.SetCheck(j, FALSE);
		}
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	}
	*pResult = 0;
}


void CDlgExcelDataCmp::OnNMDblclkListPre(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;  
	if(pNMListCtrl->iItem != -1)  
	{  
		CRect rect,dlgRect;  
		//��õ�ǰ�еĿ������Ӧ����û��������  
		//�˴����û�õ�����rect���ο������ÿ��  
		//�����һ��ʱ���صĿ�������еĿ�ȣ���Ҳ��֪��Ϊɶ  
		if (pNMListCtrl->iSubItem == 2)
		{
			int width = m_preListCtrl.GetColumnWidth(pNMListCtrl->iSubItem);  
			m_preListCtrl.GetSubItemRect(pNMListCtrl->iItem,pNMListCtrl->iSubItem,LVIR_BOUNDS,rect);  
			//����ѡ����б�������  
			//�����Ϊ�������������б�ؼ�һ��CEdit  
			//������Ҫ�����б������  
			//�Լ��������Ӧ�����к�����  
			m_nType = 1;
			m_npreItem = pNMListCtrl->iItem;
			m_npreSubItem = pNMListCtrl->iSubItem;
			//���listctrl���ο�  
			//��ø��Ի����λ���Ե���CEdit����ʾλ�ã�������������  
			m_preListCtrl.GetWindowRect(&dlgRect);  
			CString strText = m_preListCtrl.GetItemText(m_npreItem, m_npreSubItem);
			//�����븸���ڶ�Ӧ  
			ScreenToClient(&dlgRect);  
			int height = rect.Height();  
			rect.top += (dlgRect.top+1);  
			rect.left += (dlgRect.left+1);  
			rect.bottom = rect.top+height+2;  
			rect.right = rect.left+width+2;  
			m_SubItemEdit.MoveWindow(&rect);  
			m_SubItemEdit.ShowWindow(SW_SHOW);  
			m_SubItemEdit.SetWindowText(strText);
			m_SubItemEdit.SetSel(0, -1);
			m_SubItemEdit.SetFocus();  
		}
	}  
}


BOOL CDlgExcelDataCmp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->wParam == VK_RETURN)
	{
		if (GetDlgItem(IDC_EDIT_OTHER) == GetFocus())
		{
			if (m_nType == 1)
			{
				m_preListCtrl.SetFocus();
			}
			else if (m_nType == 2)
			{
				m_curListCtrl.SetFocus();
			}
		}
	}
	return CAcUiDialog::PreTranslateMessage(pMsg);
}




void CDlgExcelDataCmp::OnNMDblclkListCur(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;  
	if(pNMListCtrl->iItem != -1)  
	{  
		CRect rect,dlgRect;  
		//��õ�ǰ�еĿ������Ӧ����û��������  
		//�˴����û�õ�����rect���ο������ÿ��  
		//�����һ��ʱ���صĿ�������еĿ�ȣ���Ҳ��֪��Ϊɶ  
		if (pNMListCtrl->iSubItem == 2)
		{
			int width = m_curListCtrl.GetColumnWidth(pNMListCtrl->iSubItem);  
			m_curListCtrl.GetSubItemRect(pNMListCtrl->iItem,pNMListCtrl->iSubItem,LVIR_BOUNDS,rect);  
			//����ѡ����б�������  
			//�����Ϊ�������������б�ؼ�һ��CEdit  
			//������Ҫ�����б������  
			//�Լ��������Ӧ�����к�����  
			m_nType = 2;
			m_ncurItem = pNMListCtrl->iItem;
			m_ncurSubItem = pNMListCtrl->iSubItem;
			//���listctrl���ο�  
			//��ø��Ի����λ���Ե���CEdit����ʾλ�ã�������������  
			m_curListCtrl.GetWindowRect(&dlgRect);  
			CString strText = m_curListCtrl.GetItemText(m_ncurItem, m_ncurSubItem);
			//�����븸���ڶ�Ӧ  
			ScreenToClient(&dlgRect);  
			int height = rect.Height();  
			rect.top += (dlgRect.top+1);  
			rect.left += (dlgRect.left+1);  
			rect.bottom = rect.top+height+2;  
			rect.right = rect.left+width+2;  
			m_SubItemEdit.MoveWindow(&rect);  
			m_SubItemEdit.ShowWindow(SW_SHOW);  
			m_SubItemEdit.SetWindowText(strText);
			m_SubItemEdit.SetSel(0, -1);
			m_SubItemEdit.SetFocus();  

		}

	}  
}


void CDlgExcelDataCmp::OnLvnItemchangedListPre(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (m_bPreHit)
	{
		m_bPreHit = FALSE;
		if (m_preListCtrl.GetCheck(m_npreItem))
		{
			m_preListCtrl.SetCheck(m_npreItem, TRUE);
		}
		else
		{
			m_preListCtrl.SetCheck(m_npreItem, FALSE);
		}
	}
	*pResult = 0;
	
}


void CDlgExcelDataCmp::OnLvnItemchangedListCur(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (m_bCurHit)
	{
		m_bCurHit = FALSE;
		if (m_curListCtrl.GetCheck(m_ncurItem))
		{
			m_curListCtrl.SetCheck(m_ncurItem, TRUE);
		}
		else
		{
			m_curListCtrl.SetCheck(m_ncurItem, FALSE);
		}
	}
}


void CDlgExcelDataCmp::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAcUiDialog::OnOK();
	CString strPreFile = gGlobal.GetIniValue(_T("�ļ��ȶ�"), _T("��ʼ�ļ�"));
	CString strCurFile = gGlobal.GetIniValue(_T("�ļ��ȶ�"), _T("�����ļ�"));

	CMyExcel preExcel;
	preExcel.Open(strPreFile);
	preExcel.OpenSheet();
	int nCount = m_preListCtrl.GetItemCount();
	int nRow, nCol;
	CString strType,strValue;
	for (int i=0; i<nCount; i++)
	{
		strType = m_preListCtrl.GetItemText(i, 0);
		strValue = m_preListCtrl.GetItemText(i, 2);
		getRowAndColByString(strType, nRow, nCol);
		
		preExcel.SetItemText(nRow, nCol, strValue);
	}
	preExcel.Save();
	preExcel.Clear();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CMyExcel curExcel;
	curExcel.Open(strCurFile);
	curExcel.OpenSheet();
	for (int i=0; i<nCount; i++)
	{
		strType = m_curListCtrl.GetItemText(i, 0);
		strValue = m_curListCtrl.GetItemText(i, 2);
		getRowAndColByString(strType, nRow, nCol);

		curExcel.SetItemText(nRow, nCol, strValue);
	}
	curExcel.Save();
	curExcel.Clear();
}
void CDlgExcelDataCmp::getRowAndColByString(CString strValue, int& nRow, int& nCol)
{
	CString strTemp;
	int nFind = strValue.ReverseFind(_T('$'));
	if (nFind < 0)
	{
		nRow = 1;
		nCol = 1;
	}
	strTemp = strValue.Mid(1, nFind-1);
	
	nCol = titleToNumber(strTemp);

	strTemp = strValue.Mid(nFind+1, strValue.GetLength()-nFind-1);
	nRow = MyTransFunc::StringToInt(strTemp);

}


int CDlgExcelDataCmp::getindex(int l)
{  
	if(l==1)
		return 1;  
	else 
		return (getindex(l-1)+pow(26.0,(l-1)));
}  
int CDlgExcelDataCmp::titleToNumber(CString s)
{  
	int l=s.GetLength();  
	if(l==0)return 0;  
	int sum=0;  
	for(int i=0;i<l;i++){  
		sum*=26;  
		sum+=(s[i]-'A');}  
	return sum+getindex(l);  
}  

void CDlgExcelDataCmp::OnEnKillfocusEditOther()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(TRUE);  

	POINT pt;

	GetCursorPos(&pt);
	CRect rect;	
	
	if (m_nType == 1)
	{
		m_preListCtrl.ScreenToClient(&pt);
		m_preListCtrl.GetSubItemRect(m_npreItem,m_npreSubItem,LVIR_BOUNDS,rect);
		if(!rect.PtInRect(pt))//���������һ���ڵ��ı�������
		{
			CString text;
			m_SubItemEdit.GetWindowText(text);
			m_preListCtrl.SetItemText(m_npreItem,m_npreSubItem,text);
			m_SubItemEdit.ShowWindow(SW_HIDE);//����Ͽ�����
		}
	}
	else if (m_nType == 2)
	{
		m_curListCtrl.ScreenToClient(&pt);
		m_curListCtrl.GetSubItemRect(m_ncurItem,m_ncurSubItem,LVIR_BOUNDS,rect);
		if(!rect.PtInRect(pt))//���������һ���ڵ��ı�������
		{
			CString text;
			m_SubItemEdit.GetWindowText(text);
			m_curListCtrl.SetItemText(m_ncurItem,m_ncurSubItem,text);
			m_SubItemEdit.ShowWindow(SW_HIDE);//����Ͽ�����
		}
	}
}

void CDlgExcelDataCmp::setFile(CString strPreFile, CString strCurFile)
{
	m_strPreFile = strPreFile;
	m_strCurFile = strCurFile;
}
