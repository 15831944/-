//-----------------------------------------------------------------------------
//- StdAfx.h : include file for standard system include files,
//-      or project specific include files that are used frequently,
//-      but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once

#pragma pack (push, 8)
#pragma warning(disable: 4786 4996)
//#pragma warning(disable: 4098)

//-----------------------------------------------------------------------------
#define STRICT

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			//- Exclude rarely-used stuff from Windows headers
#endif

#define ICD_COUNT								_T("ICD_FRAME_COUNT")
#define ICD_FRAMETYPE							_T("ICD_ͼ������")
#define ICD_LRM_NAME							_T("ICD_LRM����")
#define ICD_SHEETNAME							_T("ICD_SHEET����")
//ͼ������
#define ICD_HORSPACE							_T("ICD_��������")
#define ICD_VerSapce							_T("ICD_���������")
#define ICD_LINELENGTH							_T("ICD_LINELENGTH")
#define ICD_SPAREHORGAP							_T("ICD_���ú���")
#define ICD_SPAREVERGAP							_T("ICD_��������")
#define ICD_LEFTVERGAP							_T("ICD_��������")
#define ICD_LEFTHORGAP							_T("ICD_���Ϻ���")

#define ICD_PROPERTY_DP							_T("[DP")
#define ICD_PROPERTY_DP_EQUAL					_T("[DP=")
#define ICD_PROPERTY_RF							_T("[RF")
#define ICD_PROPERTY_RF_EQUAL					_T("[RF=")
#define ICD_PROPERTY_R							_T("[R")
#define ICD_PROPERTY_MT							_T("[MT]")
#define ICD_PROPERTY_M							_T("[M]")
#define ICD_PROPERTY_S							_T("[S]")
#define ICD_PROPERTY_GL							_T("[GL")
#define ICD_PROPERTY_D							_T("[D]")
#define ICD_PROPERTY_CH							_T("[CH=")

//�洢xdata��ͷ
#define ICD_XDATA_TYPE                          _T("����")
#define ICD_XDATA_SHEET                         _T("ҳ��")
#define ICD_XDATA_ROW                           _T("����")
#define ICD_XDATA_COL                           _T("����")
#define ICD_XDATA_DUANZI                        _T("����")
#define ICD_XDATA_TEXING                        _T("����")
#define ICD_XDATA_QUXIANG                       _T("ȥ��")
#define ICD_XDATA_XIANHAO                       _T("�ߺ�")
#define ICD_XDATA_XIUGAI                        _T("���޸ı��")

#define ICD_XDATA_ZUOBIAO                       _T("��������")

#define ICD_XDATA_TEXINGTAG                     _T("���Ա��")
#define ICD_XDATA_QUXIANGTAG                    _T("ȥ����")

//- ObjectARX and OMF headers needs this
#include <map>

//-----------------------------------------------------------------------------
#include <afxwin.h>				//- MFC core and standard components
#include <afxext.h>				//- MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				//- MFC OLE classes
#include <afxodlgs.h>			//- MFC OLE dialog classes
#include <afxdisp.h>			//- MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>				//- MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>				//- MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>			//- MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				//- MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//-----------------------------------------------------------------------------
//- Include ObjectDBX/ObjectARX headers
//- Uncomment one of the following lines to bring a given library in your project.
//#define _BREP_SUPPORT_			//- Support for the BRep API
//#define _HLR_SUPPORT_				//- Support for the Hidden Line Removal API
//#define _AMODELER_SUPPORT_		//- Support for the AModeler API
//#define _ASE_SUPPORT_				//- Support for the ASI/ASE API
//#define _RENDER_SUPPORT_			//- Support for the AutoCAD Render API
//#define _ARX_CUSTOM_DRAG_N_DROP_	//- Support for the ObjectARX Drag'n Drop API
//#define _INC_LEAGACY_HEADERS_		//- Include legacy headers in this project
#include "arxHeaders.h"

//-----------------------------------------------------------------------------
#include "DocData.h" //- Your document specific data class holder

#include "MyBase.h"
#include <afxcontrolbars.h>
//- Declare it as an extern here so that it becomes available in all modules
extern AcApDataManager<CDocData> DocVars ;


#define JSWZ _T("ZW_DICT_ICD_JSYQ_")
#pragma pack (pop)

