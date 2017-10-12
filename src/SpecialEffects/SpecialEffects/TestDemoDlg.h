#pragma once

#include "svn\DIB.h"
#include "svn\WaterEffect.h"

// CTestDemoDlg dialog

class CTestDemoDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDemoDlg)

public:
	CTestDemoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestDemoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TESTDEMO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:

protected:
	CWaterEffect m_waterEffect;
	CDib m_renderSrc;
	CDib m_renderDest;
public:
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};
