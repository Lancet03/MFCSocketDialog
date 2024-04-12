
// ClientDlg.h : header file
//

#pragma once


// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Message from server
	CString m_message;
	afx_msg void OnEnChangeEditMessage();
	HANDLE hEvent;
	HANDLE hMemMap;
	void* mmap;
	virtual void OnCancel();
	void WaitForEvent();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_timerID;
	// Timer of checking updates for text field
	CEdit m_editTimer;
	void InitializeServerConnection();
};
