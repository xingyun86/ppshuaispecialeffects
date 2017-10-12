// SpecialEffectsDlg.h : header file
//

#if !defined(AFX_SPECIALEFFECTSDLG_H__A0F3BB40_1B46_4C4B_99ED_2C97AF5C65AF__INCLUDED_)
#define AFX_SPECIALEFFECTSDLG_H__A0F3BB40_1B46_4C4B_99ED_2C97AF5C65AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSpecialEffectsDlg dialog
#include "WaterRoutine.h"
#include "FireRoutine.h"
#include "PlasmaRoutine.h"
#include "DIBSectionLite.h"

class CSpecialEffectsDlg : public CDialog
{
// Construction
public:
	CSpecialEffectsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSpecialEffectsDlg)
	enum { IDD = IDD_SPECIALEFFECTS_DIALOG };
	CSliderCtrl	m_sldrPlasma;
	CSliderCtrl	m_sldrFire;
	CButton	m_chkWater;
	CButton	m_chkPlasma;
	CButton	m_chkFire;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecialEffectsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// member vars
	UINT			m_nTimer;
	CPictureHolder	m_pictureHolder;
	CDIBSectionLite	m_bmpRenderSource;
	CDIBSectionLite m_bmpRenderTarget;

	CFireRoutine	m_FireRoutine;
	CWaterRoutine	m_WaterRoutine;
	CPlasmaRoutine	m_PlasmaRoutine;

	BOOL			m_bRunFire;
	BOOL			m_bRunWater;
	BOOL			m_bRunPlasma;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSpecialEffectsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel(){
		KillTimer(m_nTimer);
		CDialog::OnCancel();
	}
	void OnReSizeWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonanimate();
	afx_msg void OnButtonstop();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCheckfire();
	afx_msg void OnCheckplasma();
	afx_msg void OnCheckwater();
	afx_msg void OnReleasedcaptureSliderplasmaalpha(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderfirealpha(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnExitSizeMove();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECIALEFFECTSDLG_H__A0F3BB40_1B46_4C4B_99ED_2C97AF5C65AF__INCLUDED_)
