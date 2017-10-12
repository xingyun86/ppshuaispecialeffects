// TestDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "svn/DIB.h"
#include "SpecialEffects.h"
#include "TestDemoDlg.h"
#include "afxdialogex.h"


// CTestDemoDlg dialog

IMPLEMENT_DYNAMIC(CTestDemoDlg, CDialog)

CTestDemoDlg::CTestDemoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestDemoDlg::IDD, pParent)
{

}

CTestDemoDlg::~CTestDemoDlg()
{
}

void CTestDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDemoDlg, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTestDemoDlg message handlers

#define ID_EFFECTTIMER	1999
#define ID_DROPTIMER	2000
BOOL CTestDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	CPictureHolder tmpPic;
	tmpPic.CreateFromBitmap(IDB_BITMAP_SVN);
	m_renderSrc.Create32BitFromPicture(&tmpPic, 468, 64);
	m_renderDest.Create32BitFromPicture(&tmpPic, 468, 64);

	m_waterEffect.Create(468, 64);
	SetTimer(ID_EFFECTTIMER, 40, nullptr);
	SetTimer(ID_DROPTIMER, 300, nullptr);

	SetWindowText(_T("SVNË®ÎÆÌØÐ§"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTestDemoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect r;
	r.left = 15;
	r.top = 20;
	r.right = r.left + m_renderSrc.GetWidth();
	r.bottom = r.top + m_renderSrc.GetHeight();

	if (r.PtInRect(point) != FALSE)
	{
		// dibs are drawn upside down...
		point.y -= 20;
		point.y = 64 - point.y;

		if (nFlags & MK_LBUTTON)
			m_waterEffect.Blob(point.x - 15, point.y, 5, 1600, m_waterEffect.m_iHpage);
		else
			m_waterEffect.Blob(point.x - 15, point.y, 2, 50, m_waterEffect.m_iHpage);

	}

	CDialog::OnMouseMove(nFlags, point);
}


void CTestDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_EFFECTTIMER)
	{
		m_waterEffect.Render((DWORD*)m_renderSrc.GetDIBits(), (DWORD*)m_renderDest.GetDIBits());
		CClientDC dc(this);
		CPoint ptOrigin(15, 20);
		m_renderDest.Draw(&dc, ptOrigin);
	}
	if (nIDEvent == ID_DROPTIMER)
	{
		CRect r;
		r.left = 15;
		r.top = 20;
		r.right = r.left + m_renderSrc.GetWidth();
		r.bottom = r.top + m_renderSrc.GetHeight();
		m_waterEffect.Blob(random(r.left, r.right), random(r.top, r.bottom), 2, 400, m_waterEffect.m_iHpage);
	}
	CDialog::OnTimer(nIDEvent);
}


void CTestDemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(ID_EFFECTTIMER);
	KillTimer(ID_DROPTIMER);
	CDialog::OnClose();
}
