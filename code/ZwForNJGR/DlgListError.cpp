//-----------------------------------------------------------------------------
//----- DlgListError.cpp : Implementation of CDlgListError
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgListError.h"
#include "CGlobalQuery.h"
#include "MyExcel.h"
#include "BaseUtils.h"
extern CGlobalQuery gQuery;
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgListError, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgListError, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgListError::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgListError::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgListError::OnNMDblclkList1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgListError::CDlgListError (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgListError::IDD, pParent, hInstance) 
{
	m_ErrorVec.clear();
	m_bFirst = true;
	m_bIsShowFlag = true;
}


//-----------------------------------------------------------------------------
void CDlgListError::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgListError::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

static int Text_List_Error_Edit_Init( int col )
{
	return CEditList::eLast;
}

BOOL CDlgListError::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	GetClientRect(&m_rect);  
	SetWindowText(m_strTitle);
	// TODO:  Add extra initialization here
	CenterWindow(acedGetAcadDwgView());
	
	m_ListCtrl.SetExtendedStyle( m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT/* | LVS_EX_CHECKBOXES*/);

	m_ListCtrl.InsertColumn( 0, _T("Sheet����"));
	m_ListCtrl.InsertColumn( 1, _T("��"));
	m_ListCtrl.InsertColumn( 2, _T("��"));
	m_ListCtrl.InsertColumn( 3, _T("Excel����"));
	m_ListCtrl.InsertColumn( 4, _T("����"));
	m_ListCtrl.InsertColumn( 5, _T("��ע"));

	RECT rect;   
	m_ListCtrl.GetWindowRect(&rect); 
	long width = rect.right - rect.left;   
	double dUnit = width/15;
	m_ListCtrl.SetColumnWidth( 0, 2*dUnit);
	m_ListCtrl.SetColumnWidth( 1, dUnit);
	m_ListCtrl.SetColumnWidth( 2, dUnit);
	m_ListCtrl.SetColumnWidth( 3, 3*dUnit);
	m_ListCtrl.SetColumnWidth( 4, 3*dUnit);
	m_ListCtrl.SetColumnWidth( 5, 4*dUnit);

	// set functionality of list according to column
	//m_ListCtrl.SetColumnType ( (fGetType)Text_List_Error_Edit_Init);	

	for (vector<CIgnoreErrorCell>::iterator iter = m_ErrorVec.begin(); iter != m_ErrorVec.end(); iter++)
	{
		setInfoToListCtrl(*iter);
	}
	m_ErrorVec.clear();
	if (m_bIsShowFlag)
	{
		GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgListError::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	gQuery.clearErrorLog();
	CAcUiDialog::OnOK();
}

void CDlgListError::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CAcUiDialog::OnCancel();
}

void CDlgListError::setErrorData(CString strTitle, vector<CIgnoreErrorCell> errorVec)
{
	m_strTitle = strTitle;
	m_ErrorVec.insert(m_ErrorVec.end(), errorVec.begin(), errorVec.end());
	errorVec.clear();
}

void CDlgListError::setInfoToListCtrl(CIgnoreErrorCell comCell)
{
	int nItem = m_ListCtrl.GetItemCount();
	nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );

	CString strCol;
	m_ListCtrl.SetItemText(nItem, 0, comCell.sheetName());
	m_ListCtrl.SetItemText(nItem, 1, comCell.row());
	m_ListCtrl.SetItemText(nItem, 2, comCell.col());
	m_ListCtrl.SetItemText(nItem, 3, comCell.texing());
	m_ListCtrl.SetItemText(nItem, 4, comCell.realTexing());
	m_ListCtrl.SetItemText(nItem, 5, comCell.prompt());
}

void CDlgListError::OnSize(UINT nType, int cx, int cy)
{
	CAcUiDialog::OnSize(nType, cx, cy);
	CWnd *pWnd;     
	pWnd = GetDlgItem(IDC_LIST1);     //��ȡ�ؼ���� 

	if(nType==1) 
		return;   //����Ǵ�����С����ʲô������    

	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����    
	{      
		CRect rect;   //��ȡ�ؼ��仯ǰ��С      
		LONG cWidth,cHeight; //��¼�ؼ����Ҳ��������Ҳ��ľ��룬��¼�ؼ��ĵײ�������ײ��ľ���     
		pWnd->GetWindowRect(&rect);      

		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������      
		rect.left  = rect.left * cx / m_rect.Width();    // �����ؼ���С  
		rect.top  = rect.top * cy / m_rect.Height(); 
		rect.bottom = cy - rect.top - 27; 
		rect.right = cx - rect.top - rect.left;  
		pWnd->MoveWindow(rect);//���ÿؼ���С   

		//�ƶ����ԺͲ����԰�ť

		CWnd *pOkWnd, *pCancelWnd;
		pOkWnd = GetDlgItem(IDOK);
		pCancelWnd = GetDlgItem(IDCANCEL);

		CRect okRect,cancelRect;
		pOkWnd->GetWindowRect(&okRect);
		ScreenToClient(&okRect);
		pOkWnd->SetWindowPos(NULL, okRect.left, rect.bottom + 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE );

		pCancelWnd->GetWindowRect(&cancelRect);
		ScreenToClient(&cancelRect);
		pCancelWnd->SetWindowPos(NULL, rect.right - cancelRect.right + cancelRect.left, rect.bottom + 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
	}    
	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С
	// TODO: Add your message handler code here
}


void CDlgListError::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	int nItem = pNMListView->iItem;//���ѡ�е���
	//int m_Col = pNMListView->iSubItem;//���ѡ����
	
	CString strSheetName,strRow,strCol,strTexing,strRealTexing,strPrompt;
	strSheetName = m_ListCtrl.GetItemText(nItem, 0);
	strRow = m_ListCtrl.GetItemText(nItem, 1);
	strCol = m_ListCtrl.GetItemText(nItem, 2);
	strTexing = m_ListCtrl.GetItemText(nItem, 3);
	strRealTexing = m_ListCtrl.GetItemText(nItem, 4);
	strPrompt = m_ListCtrl.GetItemText(nItem, 5);

	int nRow = MyTransFunc::StringToInt(strRow);
	int nCol = MyTransFunc::StringToInt(strCol);
	if (m_bFirst)
	{
		acutPrintf(_T("\n��һ�����У���ȴ�����Excel,���ݵ��Բ�ͬ�������Ҫ1~3s"));
		m_bFirst = false;
	}
	CMyExcel myExcel;
	CString strFile = CBaseUtils::filePath();	
	BOOL bRet = myExcel.openSpecailFile(strFile);
	if (!bRet)
	{
		return;
	}		
	HWND hwndAcad = adsw_acadMainWnd();
	::SendMessage(hwndAcad, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	//��ȡ�ļ���
	CString strtemp1, strTemp;
	strTemp = strFile;
	strtemp1 = strTemp.Right(strTemp.GetLength() - strTemp.ReverseFind('\\')-1);
	strtemp1 += _T(" - Excel");
	HWND pWnd = ::FindWindow(NULL, strtemp1);
	::SendMessage(pWnd, WM_ACTIVATETOPLEVEL, TRUE, 0);
	::SendMessage(pWnd, WM_SHOWWINDOW, TRUE, 0);
	::SendMessage(pWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

	::SendMessage(this->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	bRet = myExcel.OpenSheet(strSheetName);
	myExcel.selectRange(nRow, nCol);
	*pResult = 0;
}


void CDlgListError::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CAcUiDialog::OnClose();
}


void CDlgListError::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	CAcUiDialog::OnCancel();
}

void CDlgListError::setShowFlag(bool bIsShowFlag)
{
	m_bIsShowFlag = bIsShowFlag;
}
