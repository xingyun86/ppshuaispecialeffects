// SpecialEffectsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecialEffects.h"
#include "SpecialEffectsDlg.h"
#include "TestDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUTTON_RUNNING	"开始"
#define BUTTON_STOP		"停止"

static int G_EDGE_WIDTH = 15;          //边界宽度
static int G_EDGE_HEIGHT = 20;          //边界高度
static int G_BMP_WIDTH = 400;          //位图宽度
static int G_BMP_HEIGHT = 300;          //位图高度

#define TIMER_START 1971         //定时器ID
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecialEffectsDlg dialog

CSpecialEffectsDlg::CSpecialEffectsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpecialEffectsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecialEffectsDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpecialEffectsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecialEffectsDlg)
	DDX_Control(pDX, IDC_SLIDERPLASMAALPHA, m_sldrPlasma);
	DDX_Control(pDX, IDC_SLIDERFIREALPHA, m_sldrFire);
	DDX_Control(pDX, IDC_CHECKWATER, m_chkWater);
	DDX_Control(pDX, IDC_CHECKPLASMA, m_chkPlasma);
	DDX_Control(pDX, IDC_CHECKFIRE, m_chkFire);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpecialEffectsDlg, CDialog)
	//{{AFX_MSG_MAP(CSpecialEffectsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTONANIMATE, OnButtonAnimate)
	ON_BN_CLICKED(IDC_BUTTONSTOP, OnButtonstop)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECKFIRE, OnCheckfire)
	ON_BN_CLICKED(IDC_CHECKPLASMA, OnCheckplasma)
	ON_BN_CLICKED(IDC_CHECKWATER, OnCheckwater)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERPLASMAALPHA, OnReleasedcaptureSliderplasmaalpha)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERFIREALPHA, OnReleasedcaptureSliderfirealpha)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecialEffectsDlg message handlers

BOOL CSpecialEffectsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	SetDlgItemText(IDC_BUTTONSTOP, _T("SVN水纹"));
	
	//装入位图
	m_pictureHolder.CreateFromBitmap(IDB_BITMAP_MAP);

	m_FireRoutine.m_iAlpha = 30;		// 渐变 30% alpha
	m_sldrFire.SetRange(1, 100, TRUE);
	m_sldrFire.SetPos(30);
	m_PlasmaRoutine.m_iAlpha = 30;// 渐变 30 % alpha
	m_sldrPlasma.SetRange(1, 100, TRUE);
	m_sldrPlasma.SetPos(30);

	//初始化变量
	m_bRunWater = TRUE;
	m_bRunFire = FALSE;
	m_bRunPlasma = FALSE;
	m_chkWater.SetCheck(TRUE);
	{
		CRect rect;
		RECT rcGroup = { 0 };
		RECT rcButtonAnimate = { 0 };
		RECT rcButtonStop = { 0 };
		int n_BMP_WIDTH = 0;
		int n_BMP_HEIGHT = 0;

		GetClientRect(&rect);
		GetDlgItem(IDC_STATIC_SETTING)->GetClientRect(&rcGroup);
		GetDlgItem(IDC_BUTTONANIMATE)->GetClientRect(&rcButtonAnimate);
		GetDlgItem(IDC_BUTTONSTOP)->GetClientRect(&rcButtonStop);

		rcGroup.right = rcButtonAnimate.right + rcButtonStop.right;

		n_BMP_WIDTH = rect.right - rect.left - rcGroup.right - G_EDGE_WIDTH * 2;
		n_BMP_HEIGHT = rect.bottom - rect.top - G_EDGE_HEIGHT * 2;
		n_BMP_WIDTH -= n_BMP_WIDTH % 10;
		n_BMP_HEIGHT -= n_BMP_HEIGHT % 10;

		m_bmpRenderSource.Create32BitFromPicture(&m_pictureHolder, n_BMP_WIDTH, n_BMP_HEIGHT);
		m_bmpRenderTarget.Create32BitFromPicture(&m_pictureHolder, n_BMP_WIDTH, n_BMP_HEIGHT);

		::SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE) & (~WS_MAXIMIZEBOX));
		std::thread([](void * p)->void {
			CSpecialEffectsDlg * pThis = (CSpecialEffectsDlg *)p;
			CRect rect;
			RECT rcGroup = { 0 };
			RECT rcButtonAnimate = { 0 };
			RECT rcButtonStop = { 0 };
			int n_BMP_WIDTH = 0;
			int n_BMP_HEIGHT = 0;

			pThis->GetDlgItem(IDC_STATIC_SETTING)->GetClientRect(&rcGroup);
			pThis->GetDlgItem(IDC_BUTTONANIMATE)->GetClientRect(&rcButtonAnimate);
			pThis->GetDlgItem(IDC_BUTTONSTOP)->GetClientRect(&rcButtonStop);

			rcGroup.right = rcButtonAnimate.right + rcButtonStop.right;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, SPIF_SENDCHANGE);//获取屏幕客户区大小
			n_BMP_WIDTH = rect.right - rect.left - rcGroup.right - G_EDGE_WIDTH * 2;
			n_BMP_HEIGHT = rect.bottom - GetSystemMetrics(SM_CYCAPTION) - rect.top - G_EDGE_HEIGHT * 2;
			n_BMP_WIDTH -= n_BMP_WIDTH % 10;
			n_BMP_HEIGHT -= n_BMP_HEIGHT % 10;

			pThis->m_bmpRenderSourceMax.Create32BitFromPicture(&pThis->m_pictureHolder, n_BMP_WIDTH, n_BMP_HEIGHT);
			pThis->m_bmpRenderTargetMax.Create32BitFromPicture(&pThis->m_pictureHolder, n_BMP_WIDTH, n_BMP_HEIGHT);

			::SetWindowLong(pThis->m_hWnd, GWL_STYLE, GetWindowLong(pThis->m_hWnd, GWL_STYLE) | WS_MAXIMIZEBOX);

		}, this).detach();		
	}

	OnResizeWindow(SC_RESTORE);

	OnButtonAnimate();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpecialEffectsDlg::OnResizeWindow(UINT nID)
{
	CRect rect;
	CClientDC dc(this);

	RECT rcTemp = { 0 };
	RECT rcGroup = { 0 };
	RECT rcButtonAnimate = { 0 };
	RECT rcButtonStop = { 0 };
	RECT rcCheckWater = { 0 };
	RECT rcCheckFire = { 0 };
	RECT rcCheckPlasma = { 0 };
	RECT rcSliderFireAlpha = { 0 };
	RECT rcSliderPlasmaAlpha = { 0 };

	BOOL bRepaintNow = FALSE;
		
	GetClientRect(&rect);

	GetDlgItem(IDC_STATIC_SETTING)->GetClientRect(&rcGroup);
	GetDlgItem(IDC_BUTTONANIMATE)->GetClientRect(&rcButtonAnimate);
	GetDlgItem(IDC_BUTTONSTOP)->GetClientRect(&rcButtonStop);
	GetDlgItem(IDC_CHECKWATER)->GetClientRect(&rcCheckWater);
	GetDlgItem(IDC_CHECKFIRE)->GetClientRect(&rcCheckFire);
	GetDlgItem(IDC_CHECKPLASMA)->GetClientRect(&rcCheckPlasma);
	GetDlgItem(IDC_SLIDERFIREALPHA)->GetClientRect(&rcSliderFireAlpha);
	GetDlgItem(IDC_SLIDERPLASMAALPHA)->GetClientRect(&rcSliderPlasmaAlpha);

	rcGroup.right = rcButtonAnimate.right + rcButtonStop.right;

	G_BMP_WIDTH = rect.right - rect.left - rcGroup.right - G_EDGE_WIDTH * 2;
	G_BMP_HEIGHT = rect.bottom - rect.top - G_EDGE_HEIGHT * 2;
	G_BMP_WIDTH -= G_BMP_WIDTH % 10;
	G_BMP_HEIGHT -= G_BMP_HEIGHT % 10;
	
	if (((nID & SC_MAXIMIZE) == SC_MAXIMIZE))
	{
		m_pBmpRenderSource = &m_bmpRenderSourceMax;
		m_pBmpRenderTarget = &m_bmpRenderTargetMax;
	}
	else// if (((nID & SC_RESTORE) == SC_RESTORE))
	{
		m_pBmpRenderSource = &m_bmpRenderSource;
		m_pBmpRenderTarget = &m_bmpRenderTarget;
	}

	//创建水纹对象
	m_WaterRoutine.Create(G_BMP_WIDTH, G_BMP_HEIGHT);

	//创建火焰对象
	m_FireRoutine.Create(G_BMP_WIDTH, G_BMP_HEIGHT);

	//创建岩浆对象
	m_PlasmaRoutine.Create(G_BMP_WIDTH, G_BMP_HEIGHT);
	
	SetRect(&rcTemp, rect.right - rcGroup.right, 0, rect.right, rect.bottom);
	GetDlgItem(IDC_STATIC_SETTING)->MoveWindow(&rcTemp, bRepaintNow);
	GetDlgItem(IDC_STATIC_SETTING)->GetClientRect(&rcGroup);

	SetRect(&rcTemp, rect.right - rcGroup.right, 0, rect.right - rcGroup.right + rcButtonAnimate.right, rcButtonAnimate.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += G_EDGE_HEIGHT;
	rcTemp.bottom += G_EDGE_HEIGHT;
	GetDlgItem(IDC_BUTTONANIMATE)->MoveWindow(&rcTemp, bRepaintNow);

	SetRect(&rcTemp, rect.right - rcGroup.right + rcButtonAnimate.right, 0, rect.right - rcGroup.right + rcButtonAnimate.right + rcButtonStop.right, rcButtonStop.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += G_EDGE_HEIGHT;
	rcTemp.bottom += G_EDGE_HEIGHT;
	GetDlgItem(IDC_BUTTONSTOP)->MoveWindow(&rcTemp, bRepaintNow);

	SetRect(&rcTemp, rect.right - rcGroup.right, rcButtonAnimate.bottom, rect.right - rcGroup.right + rcCheckWater.right, rcButtonAnimate.bottom + rcCheckWater.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += G_EDGE_HEIGHT;
	rcTemp.bottom += G_EDGE_HEIGHT;
	GetDlgItem(IDC_CHECKWATER)->MoveWindow(&rcTemp, bRepaintNow);

	SetRect(&rcTemp, rect.right - rcGroup.right + rcCheckWater.right, rcButtonAnimate.bottom, rect.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right, rcButtonAnimate.bottom + rcCheckFire.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += G_EDGE_HEIGHT;
	rcTemp.bottom += G_EDGE_HEIGHT;
	GetDlgItem(IDC_CHECKFIRE)->MoveWindow(&rcTemp, bRepaintNow);

	SetRect(&rcTemp, rect.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right, rcButtonAnimate.bottom, rect.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right + rcCheckPlasma.right, rcButtonAnimate.bottom + rcCheckPlasma.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += G_EDGE_HEIGHT;
	rcTemp.bottom += G_EDGE_HEIGHT;
	GetDlgItem(IDC_CHECKPLASMA)->MoveWindow(&rcTemp, bRepaintNow);

	SetRect(&rcTemp, rect.right - rcGroup.right + rcCheckWater.right, rcButtonAnimate.bottom + rcCheckFire.bottom, rect.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right, rcButtonAnimate.bottom + rcCheckFire.bottom + rcSliderFireAlpha.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += G_EDGE_HEIGHT;
	rcTemp.bottom += G_EDGE_HEIGHT;
	GetDlgItem(IDC_SLIDERFIREALPHA)->MoveWindow(&rcTemp, bRepaintNow);

	SetRect(&rcTemp, rect.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right, rcButtonAnimate.bottom + rcCheckFire.bottom, rect.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right + rcCheckPlasma.right, rcButtonAnimate.bottom + rcCheckPlasma.bottom + rcSliderPlasmaAlpha.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += G_EDGE_HEIGHT;
	rcTemp.bottom += G_EDGE_HEIGHT;
	GetDlgItem(IDC_SLIDERPLASMAALPHA)->MoveWindow(&rcTemp, bRepaintNow);

	RedrawWindow(&rect, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_ERASE);
}

void CSpecialEffectsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
		if (((nID & SC_MAXIMIZE) == SC_MAXIMIZE) || ((nID & SC_RESTORE) == SC_RESTORE))
		{
			OnResizeWindow(nID);
		}
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpecialEffectsDlg::OnPaint()
{
	CRect rect;
	CPaintDC dc(this); // device context for painting
		
	GetClientRect(&rect);

	if (IsIconic())
	{		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);

		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();

		//在此重绘位图
		/*CDC mDC;
		CRect rect;
		CBitmap mBmp;

		GetClientRect(&rect);
		mDC.CreateCompatibleDC(&dc);
		mBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		mDC.SelectObject(&mBmp)->DeleteObject();
		mDC.FillSolidRect(rect, dc.GetBkColor());
		m_pBmpRenderTarget->Draw(&mDC, CPoint(G_EDGE_WIDTH, G_EDGE_HEIGHT));

		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &mDC, 0, 0, SRCCOPY);
		//dc.BitBlt(G_EDGE_WIDTH, G_EDGE_HEIGHT, G_BMP_WIDTH, G_BMP_HEIGHT, &mDC, G_EDGE_WIDTH, G_EDGE_HEIGHT, SRCCOPY);
		//dc.SetStretchBltMode(COLORONCOLOR);
		//dc.StretchBlt(G_EDGE_WIDTH, G_EDGE_HEIGHT, G_BMP_WIDTH, G_BMP_HEIGHT, &mDC, G_EDGE_WIDTH, G_EDGE_HEIGHT, G_BMP_WIDTH, G_BMP_HEIGHT, SRCCOPY);

		mBmp.DeleteObject();
		mDC.DeleteDC();*/
	}
}

BOOL CSpecialEffectsDlg::OnEraseBkgnd(CDC* pDC)
{
	//CDialog::OnEraseBkgnd(pDC);
	
	CDC mDC;
	CRect rect;
	CBitmap mBmp;

	GetClientRect(&rect);
	mDC.CreateCompatibleDC(NULL);
	mBmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	mDC.SelectObject(&mBmp)->DeleteObject();
	mDC.FillSolidRect(rect, pDC->GetDCBrushColor());

	m_pBmpRenderTarget->Draw(&mDC, CPoint(G_EDGE_WIDTH, G_EDGE_HEIGHT));
	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &mDC, 0, 0, SRCCOPY);
	//pDC->BitBlt(G_EDGE_WIDTH, G_EDGE_HEIGHT, G_BMP_WIDTH, G_BMP_HEIGHT, &mDC, G_EDGE_WIDTH, G_EDGE_HEIGHT, SRCCOPY);
	//pDC->SetStretchBltMode(COLORONCOLOR);
	//pDC->StretchBlt(G_EDGE_WIDTH, G_EDGE_HEIGHT, G_BMP_WIDTH, G_BMP_HEIGHT, &mDC, G_EDGE_WIDTH, G_EDGE_HEIGHT, G_BMP_WIDTH, G_BMP_HEIGHT, SRCCOPY);

	mBmp.DeleteObject();
	mDC.DeleteDC();

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpecialEffectsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSpecialEffectsDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	if (nIDEvent == TIMER_START)
	{
		if (!m_bRunWater)
		{
			//浮动它，以便消失
			m_WaterRoutine.FlattenWater();
		}

		m_WaterRoutine.Render((DWORD*)m_pBmpRenderSource->GetDIBits(), (DWORD*)m_pBmpRenderTarget->GetDIBits());

		if (m_bRunFire)
		{
			m_FireRoutine.Render((DWORD*)m_pBmpRenderTarget->GetDIBits(), G_BMP_WIDTH, G_BMP_HEIGHT);
		}
		if (m_bRunPlasma)
		{
			m_PlasmaRoutine.Render((DWORD*)m_pBmpRenderTarget->GetDIBits(), G_BMP_WIDTH, G_BMP_HEIGHT, G_BMP_WIDTH);
		}

		CRect rect(G_EDGE_WIDTH, G_EDGE_HEIGHT, G_BMP_WIDTH, G_BMP_HEIGHT);
		RedrawWindow(&rect, NULL, RDW_UPDATENOW | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_ERASE);
	}
}

void CSpecialEffectsDlg::OnButtonAnimate()
{
	_TCHAR tButtonText[MAX_PATH] = { 0 };

	GetDlgItemText(IDC_BUTTONANIMATE, tButtonText, sizeof(tButtonText) / sizeof(*tButtonText));
	if (*tButtonText && !lstrcmp(tButtonText, _T(BUTTON_RUNNING)))
	{
		m_nTimer = SetTimer(TIMER_START, 10, NULL);
		SetDlgItemText(IDC_BUTTONANIMATE, _T(BUTTON_STOP));
	}
	else
	{
		KillTimer(m_nTimer);
		SetDlgItemText(IDC_BUTTONANIMATE, _T(BUTTON_RUNNING));
	}
}

void CSpecialEffectsDlg::OnButtonstop() 
{
	CTestDemoDlg dlgTestDemo;
	dlgTestDemo.DoModal();
}

void CSpecialEffectsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);
	CRect rcPicture(G_EDGE_WIDTH, G_EDGE_HEIGHT, G_EDGE_WIDTH + m_pBmpRenderSource->GetWidth(), G_EDGE_HEIGHT + m_pBmpRenderSource->GetHeight());

	if (rcPicture.PtInRect(point))
	{
		//偏移到位图左上角，以便产生水纹效果
		point.y -= G_EDGE_HEIGHT;
		point.y = G_BMP_HEIGHT - point.y;

		m_WaterRoutine.HeightBlob(point.x - G_EDGE_WIDTH, point.y, 5, G_EDGE_HEIGHT * 3, m_WaterRoutine.m_iHpage);

		if (!m_bRunWater)
		{
			//浮动它，以便消失
			m_WaterRoutine.FlattenWater();
		}
		m_WaterRoutine.Render((DWORD*)m_pBmpRenderSource->GetDIBits(), (DWORD*)m_pBmpRenderTarget->GetDIBits());

		if (m_bRunFire)
		{
			m_FireRoutine.Render((DWORD*)m_pBmpRenderTarget->GetDIBits(), G_BMP_WIDTH, G_BMP_HEIGHT);
		}

		if (m_bRunPlasma)
		{
			m_PlasmaRoutine.Render((DWORD*)m_pBmpRenderTarget->GetDIBits(), G_BMP_WIDTH, G_BMP_HEIGHT, G_BMP_WIDTH);
		}

		CRect rect(G_EDGE_WIDTH, G_EDGE_HEIGHT, G_BMP_WIDTH, G_BMP_HEIGHT);
		RedrawWindow(&rect, NULL, RDW_UPDATENOW | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_ERASE);
	}
}

void CSpecialEffectsDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	CRect rcPicture(G_EDGE_WIDTH, G_EDGE_HEIGHT, G_EDGE_WIDTH + m_pBmpRenderSource->GetWidth(), G_EDGE_HEIGHT + m_pBmpRenderSource->GetHeight());

	if (rcPicture.PtInRect(point))
	{
		//偏移到位图左上角，以便产生水纹效果
		point.y = G_BMP_HEIGHT - point.y + G_EDGE_HEIGHT;

		m_WaterRoutine.HeightBlob(point.x - G_EDGE_WIDTH, point.y, 30, G_EDGE_HEIGHT * 9, m_WaterRoutine.m_iHpage);
	}
}

void CSpecialEffectsDlg::OnCheckfire() 
{
	m_bRunFire = m_chkFire.GetCheck();	
}

void CSpecialEffectsDlg::OnCheckplasma() 
{
	m_bRunPlasma = m_chkPlasma.GetCheck();	
}

void CSpecialEffectsDlg::OnCheckwater() 
{
	m_bRunWater = m_chkWater.GetCheck();
}

void CSpecialEffectsDlg::OnReleasedcaptureSliderplasmaalpha(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_PlasmaRoutine.m_iAlpha = m_sldrPlasma.GetPos();
	
	*pResult = 0;
}

void CSpecialEffectsDlg::OnReleasedcaptureSliderfirealpha(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_FireRoutine.m_iAlpha = m_sldrFire.GetPos();
	
	*pResult = 0;
}
