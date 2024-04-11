
// ClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <Windows.h>
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg dialog



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_message(_T("ќжидание сообщени€ с сервера..."))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_message);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editTimer);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_MESSAGE, &CClientDlg::OnEnChangeEditMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: Add extra initialization here
	


	//this->hEvent = hEvent;
	//this->hMemMap = hMemMap;
	//this->mmap = mmap;

	this->hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"TestEvent");
	if (this->hEvent == NULL)
	{
		SetDlgItemText(IDC_EDIT_MESSAGE, _T("Error opening event"));

		return 0;
	}

	this->hMemMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"TestMMap");
	this->mmap = MapViewOfFile(this->hMemMap, FILE_MAP_ALL_ACCESS, 0, 0, 4096);

	SetDlgItemText(IDC_EDIT_MESSAGE, _T("Waiting event"));

	this->m_timerID = SetTimer(1, 100, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnEnChangeEditMessage()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CClientDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	UnmapViewOfFile(this->mmap);
	CloseHandle(this->hMemMap);
	CloseHandle(this->hEvent);


	CDialogEx::OnCancel();
}

void CClientDlg::WaitForEvent() {

	std::string* pMessage = reinterpret_cast<std::string*>(this->mmap);
	std::string message = *pMessage;

	CString cstrMessage(message.c_str());
	SetDlgItemText(IDC_EDIT_MESSAGE, cstrMessage);
}


void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//WaitForSingleObject(this->hEvent, 100); 

	std::string* pMessage = reinterpret_cast<std::string*>(this->mmap);
	
	std::string message;
	if (pMessage == nullptr) {
		message = "";
	}
	else {
		message = *pMessage;
		CString cstrMessage(message.c_str());
		SetDlgItemText(IDC_EDIT_MESSAGE, cstrMessage);
	}
	//WaitForSingleObject(hEvent, 100);
	
	
	

	CDialogEx::OnTimer(nIDEvent);
}
