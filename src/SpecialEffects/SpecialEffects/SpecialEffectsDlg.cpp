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

//#define BMP_WIDTH   708          //位图宽度
//#define BMP_HEIGHT  378          //位图高度

#define BUTTON_RUNNING	"开始"
#define BUTTON_STOP		"停止"

volatile int EDGE_WIDTH = 15;          //边界宽度
volatile int EDGE_HEIGHT = 20;          //边界高度
volatile int BMP_WIDTH = 400;          //位图宽度
volatile int BMP_HEIGHT = 300;          //位图高度

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
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTONANIMATE, OnButtonanimate)
	ON_BN_CLICKED(IDC_BUTTONSTOP, OnButtonstop)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECKFIRE, OnCheckfire)
	ON_BN_CLICKED(IDC_CHECKPLASMA, OnCheckplasma)
	ON_BN_CLICKED(IDC_CHECKWATER, OnCheckwater)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERPLASMAALPHA, OnReleasedcaptureSliderplasmaalpha)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERFIREALPHA, OnReleasedcaptureSliderfirealpha)
	//}}AFX_MSG_MAP
	ON_WM_EXITSIZEMOVE()
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
	
	//装入位图
	m_pictureHolder.CreateFromBitmap(IDB_BITMAP_MAP);

	m_FireRoutine.m_iAlpha = 30;		// 渐变 30% alpha
	m_sldrFire.SetRange(1, 100, TRUE);
	m_sldrFire.SetPos(30);
	m_PlasmaRoutine.m_iAlpha = 30;// 渐变 30 % alpha
	m_sldrPlasma.SetRange(1, 100, TRUE);
	m_sldrPlasma.SetPos(30);

/*	m_bmpRenderSource.Create32BitFromPicture(&m_pictureHolder, BMP_WIDTH, BMP_HEIGHT);
	m_bmpRenderTarget.Create32BitFromPicture(&m_pictureHolder, BMP_WIDTH, BMP_HEIGHT);

	//创建水纹对象
	m_WaterRoutine.Create(BMP_WIDTH, BMP_HEIGHT);
	
	//创建火焰对象
	m_FireRoutine.m_iAlpha = 30;		// 渐变 30% alpha
	m_FireRoutine.m_iHeight = BMP_HEIGHT;
	m_FireRoutine.m_iWidth = BMP_WIDTH;
	m_FireRoutine.InitFire();
	m_sldrFire.SetRange(1,100,TRUE);
	m_sldrFire.SetPos(30);
	
	//创建岩浆对象
	m_PlasmaRoutine.Create(BMP_WIDTH,BMP_HEIGHT);
	m_PlasmaRoutine.m_iAlpha = 30;// 渐变 30 % alpha
	m_sldrPlasma.SetRange(1,100,TRUE);
	m_sldrPlasma.SetPos(30);
	*/
	//初始化变量
	m_bRunWater = TRUE;
	m_bRunFire = FALSE;
	m_bRunPlasma = FALSE;
	m_chkWater.SetCheck(TRUE);
	
	//RECT rcClientWorker = { 0 };
	//SystemParametersInfo(SPI_GETWORKAREA, 0, &rcClientWorker, 0);
	//SetWindowPos(&CWnd::wndNoTopMost, rcClientWorker.left, rcClientWorker.top, rcClientWorker.right, rcClientWorker.bottom, SWP_SHOWWINDOW);
	OnReSizeWindow();

	SetDlgItemText(IDC_BUTTONSTOP, _T("SVN水纹"));
	OnButtonanimate();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpecialEffectsDlg::OnReSizeWindow()
{
	RECT rcTemp = { 0 };
	RECT rcWindow = { 0 };
	RECT rcGroup = { 0 };
	RECT rcButtonAnimate = { 0 };
	RECT rcButtonStop = { 0 };
	RECT rcCheckWater = { 0 };
	RECT rcCheckFire = { 0 };
	RECT rcCheckPlasma = { 0 };
	RECT rcSliderFireAlpha = { 0 };
	RECT rcSliderPlasmaAlpha = { 0 };

	GetClientRect(&rcWindow);
	GetDlgItem(IDC_STATIC_SETTING)->GetClientRect(&rcGroup);

	GetDlgItem(IDC_BUTTONANIMATE)->GetClientRect(&rcButtonAnimate);
	GetDlgItem(IDC_BUTTONSTOP)->GetClientRect(&rcButtonStop);
	GetDlgItem(IDC_CHECKWATER)->GetClientRect(&rcCheckWater);
	GetDlgItem(IDC_CHECKFIRE)->GetClientRect(&rcCheckFire);
	GetDlgItem(IDC_CHECKPLASMA)->GetClientRect(&rcCheckPlasma);
	GetDlgItem(IDC_SLIDERFIREALPHA)->GetClientRect(&rcSliderFireAlpha);
	GetDlgItem(IDC_SLIDERPLASMAALPHA)->GetClientRect(&rcSliderPlasmaAlpha);

	rcGroup.right = rcButtonAnimate.right + rcButtonStop.right;

	BMP_WIDTH = rcWindow.right - rcWindow.left - rcGroup.right - EDGE_WIDTH * 2;
	BMP_WIDTH -= BMP_WIDTH % 10;
	BMP_HEIGHT = rcWindow.bottom - rcWindow.top - EDGE_HEIGHT * 2;
	BMP_HEIGHT -= BMP_HEIGHT % 10;

	m_bmpRenderSource.Create32BitFromPicture(&m_pictureHolder, BMP_WIDTH, BMP_HEIGHT);
	m_bmpRenderTarget.Create32BitFromPicture(&m_pictureHolder, BMP_WIDTH, BMP_HEIGHT);

	//创建水纹对象
	m_WaterRoutine.Create(BMP_WIDTH, BMP_HEIGHT);

	//创建火焰对象
	m_FireRoutine.m_iHeight = BMP_HEIGHT;
	m_FireRoutine.m_iWidth = BMP_WIDTH;
	m_FireRoutine.InitFire();

	//创建岩浆对象
	m_PlasmaRoutine.Create(BMP_WIDTH, BMP_HEIGHT);
		
	SetRect(&rcTemp, rcWindow.right - rcGroup.right, 0, rcWindow.right, rcWindow.bottom);
	GetDlgItem(IDC_STATIC_SETTING)->MoveWindow(&rcTemp);
	GetDlgItem(IDC_STATIC_SETTING)->GetClientRect(&rcGroup);

	SetRect(&rcTemp, rcWindow.right - rcGroup.right, 0, rcWindow.right - rcGroup.right + rcButtonAnimate.right, rcButtonAnimate.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += EDGE_HEIGHT;
	rcTemp.bottom += EDGE_HEIGHT;
	GetDlgItem(IDC_BUTTONANIMATE)->MoveWindow(&rcTemp);

	SetRect(&rcTemp, rcWindow.right - rcGroup.right + rcButtonAnimate.right, 0, rcWindow.right - rcGroup.right + rcButtonAnimate.right + rcButtonStop.right, rcButtonStop.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += EDGE_HEIGHT;
	rcTemp.bottom += EDGE_HEIGHT;
	GetDlgItem(IDC_BUTTONSTOP)->MoveWindow(&rcTemp);

	SetRect(&rcTemp, rcWindow.right - rcGroup.right, rcButtonAnimate.bottom, rcWindow.right - rcGroup.right + rcCheckWater.right, rcButtonAnimate.bottom + rcCheckWater.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += EDGE_HEIGHT;
	rcTemp.bottom += EDGE_HEIGHT;
	GetDlgItem(IDC_CHECKWATER)->MoveWindow(&rcTemp);

	SetRect(&rcTemp, rcWindow.right - rcGroup.right + rcCheckWater.right, rcButtonAnimate.bottom, rcWindow.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right, rcButtonAnimate.bottom + rcCheckFire.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += EDGE_HEIGHT;
	rcTemp.bottom += EDGE_HEIGHT;
	GetDlgItem(IDC_CHECKFIRE)->MoveWindow(&rcTemp);

	SetRect(&rcTemp, rcWindow.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right, rcButtonAnimate.bottom, rcWindow.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right + rcCheckPlasma.right, rcButtonAnimate.bottom + rcCheckPlasma.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += EDGE_HEIGHT;
	rcTemp.bottom += EDGE_HEIGHT;
	GetDlgItem(IDC_CHECKPLASMA)->MoveWindow(&rcTemp);

	SetRect(&rcTemp, rcWindow.right - rcGroup.right + rcCheckWater.right, rcButtonAnimate.bottom + rcCheckFire.bottom, rcWindow.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right, rcButtonAnimate.bottom + rcCheckFire.bottom + rcSliderFireAlpha.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += EDGE_HEIGHT;
	rcTemp.bottom += EDGE_HEIGHT;
	GetDlgItem(IDC_SLIDERFIREALPHA)->MoveWindow(&rcTemp);

	SetRect(&rcTemp, rcWindow.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right, rcButtonAnimate.bottom + rcCheckFire.bottom, rcWindow.right - rcGroup.right + rcCheckWater.right + rcCheckFire.right + rcCheckPlasma.right, rcButtonAnimate.bottom + rcCheckPlasma.bottom + rcSliderPlasmaAlpha.bottom);
	rcTemp.left += 1;
	rcTemp.right += 1;
	rcTemp.top += EDGE_HEIGHT;
	rcTemp.bottom += EDGE_HEIGHT;
	GetDlgItem(IDC_SLIDERPLASMAALPHA)->MoveWindow(&rcTemp);

}

void CSpecialEffectsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & SC_MAXIMIZE) == SC_MAXIMIZE)
	{		
		CDialog::OnSysCommand(nID, lParam);
		OnReSizeWindow();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpecialEffectsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//在此重绘位图
		CPaintDC dc(this);
		CPoint ptOrigin(EDGE_WIDTH, EDGE_HEIGHT);
		m_bmpRenderTarget.Draw(&dc,ptOrigin);

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpecialEffectsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSpecialEffectsDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == TIMER_START)
	{		
		if(!m_bRunWater)
			m_WaterRoutine.FlattenWater();  //浮动它，以便消失

		m_WaterRoutine.Render((DWORD*)m_bmpRenderSource.GetDIBits(), (DWORD*)m_bmpRenderTarget.GetDIBits());

		if(m_bRunFire)
			m_FireRoutine.Render((DWORD*)m_bmpRenderTarget.GetDIBits(),BMP_WIDTH,BMP_HEIGHT);

		if(m_bRunPlasma)
			m_PlasmaRoutine.Render((DWORD*)m_bmpRenderTarget.GetDIBits(),BMP_WIDTH,BMP_HEIGHT,BMP_WIDTH);

		CClientDC dc(this);
		CPoint ptOrigin(EDGE_WIDTH, EDGE_HEIGHT);
		m_bmpRenderTarget.Draw(&dc, ptOrigin);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CSpecialEffectsDlg::OnButtonanimate() 
{
	_TCHAR tButtonText[MAX_PATH] = { 0 };

	GetDlgItemText(IDC_BUTTONANIMATE, tButtonText, sizeof(tButtonText) / sizeof(*tButtonText));
	if (*tButtonText && !lstrcmp(tButtonText, _T(BUTTON_RUNNING)))
	{
		m_nTimer = SetTimer(TIMER_START, 30, NULL);
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
	CRect rcPicture;

	rcPicture.left = EDGE_WIDTH;
	rcPicture.top = EDGE_HEIGHT;
	rcPicture.right = rcPicture.left + m_bmpRenderSource.GetWidth();
	rcPicture.bottom = rcPicture.top + m_bmpRenderSource.GetHeight();

	if (rcPicture.PtInRect(point) == TRUE)
	{
		//偏移到位图左上角，以便产生水纹效果
		point.y -= EDGE_HEIGHT;
		point.y = BMP_HEIGHT - point.y;

		m_WaterRoutine.HeightBlob(point.x - EDGE_WIDTH, point.y, 5, EDGE_HEIGHT * 3, m_WaterRoutine.m_iHpage);

		if (!m_bRunWater)
			m_WaterRoutine.FlattenWater();	//浮动它，以便消失

		m_WaterRoutine.Render((DWORD*)m_bmpRenderSource.GetDIBits(), (DWORD*)m_bmpRenderTarget.GetDIBits());

		if (m_bRunFire)
			m_FireRoutine.Render((DWORD*)m_bmpRenderTarget.GetDIBits(), BMP_WIDTH, BMP_HEIGHT);

		if (m_bRunPlasma)
			m_PlasmaRoutine.Render((DWORD*)m_bmpRenderTarget.GetDIBits(), BMP_WIDTH, BMP_HEIGHT, BMP_WIDTH);

		CClientDC dc(this);
		CPoint ptOrigin(EDGE_WIDTH, EDGE_HEIGHT);
		m_bmpRenderTarget.Draw(&dc, ptOrigin);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CSpecialEffectsDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcPicture;

	rcPicture.left = EDGE_WIDTH;
	rcPicture.top = EDGE_HEIGHT;
	rcPicture.right = rcPicture.left + m_bmpRenderSource.GetWidth();
	rcPicture.bottom = rcPicture.top + m_bmpRenderSource.GetHeight();

	if(rcPicture.PtInRect(point) == TRUE)
	{
		//偏移到位图左上角，以便产生水纹效果
		point.y -= EDGE_HEIGHT;
		point.y = BMP_HEIGHT-point.y;

		m_WaterRoutine.HeightBlob(point.x - EDGE_WIDTH, point.y, 30, EDGE_HEIGHT * 9, m_WaterRoutine.m_iHpage);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
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

void CSpecialEffectsDlg::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnExitSizeMove();
	
	OnReSizeWindow();
}
