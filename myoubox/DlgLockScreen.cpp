// DlgLockScreen.cpp : 实现文件
//

#include "stdafx.h"
#include "myoubox.h"
#include "DlgLockScreen.h"
#include "afxdialogex.h"
#include "LKFontMgr.h"
#include "ConfigFile.h"
#include "DlgAdminLogin.h"
#include "DlgGameMenu.h"
#include "TrayMainMenu.h"

#include "log.h"
#include "CenterComm.h"

// CDlgLockScreen 对话框
CDlgLockScreen *CDlgLockScreen::m_pDlgLock = 0;
// 是否锁屏
bool CDlgLockScreen::IsLockScreen()
{
	if (m_pDlgLock)
	{
		if (m_pDlgLock->GetSafeHwnd())
		{
			return m_pDlgLock->IsWindowVisible();
		}
	}
	return false;
}

// 锁屏
void CDlgLockScreen::LockScreen(bool bLock)
{
	CLogClient::WriteLog("锁屏: %d", bLock);
	if (bLock)
	{
		if (::AfxGetApp()->GetMainWnd())
		{
			if (::AfxGetApp()->GetMainWnd()->IsWindowVisible())
			{
				::AfxGetApp()->GetMainWnd()->ShowWindow(SW_HIDE);
			}
		}
		//if (::AfxGetApp()->GetMainWnd())
		//{
		//	::AfxGetApp()->GetMainWnd()->ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
		//	WINDOWPLACEMENT wp; //显示隐藏
		//	wp.length = sizeof(WINDOWPLACEMENT);
		//	wp.flags = WPF_RESTORETOMAXIMIZED;
		//	wp.showCmd = SW_HIDE;
		//	::AfxGetApp()->GetMainWnd()->SetWindowPlacement(&wp);
		//}
		if (!m_pDlgLock)
		{
			m_pDlgLock = new CDlgLockScreen();
		}
		//CLogClient::WriteLog("设置关机定时器");
		//m_pDlgLock->SetTimer(1, 5 * 60 * 1000, 0);
		if (m_pDlgLock)
		{
			if (!m_pDlgLock->GetSafeHwnd())
			{
				m_pDlgLock->Create(CDlgLockScreen::IDD);
			}
			CLogClient::WriteLog("显示锁屏界面1: %d", m_pDlgLock);
			if (m_pDlgLock->GetSafeHwnd())
			{
				CLogClient::WriteLog("显示锁屏界面2: %d", m_pDlgLock->GetSafeHwnd());
				m_pDlgLock->ShowWindow(SW_SHOW);
			}
			else
			{
				CLogClient::WriteLog("锁屏界面句柄无效");
			}
		}
		else
		{
			CLogClient::WriteLog("无法创建锁屏界面");
		}
	}
	else
	{
		CLogClient::WriteLog("解锁");
		if (m_pDlgLock->GetSafeHwnd())
		{
			m_pDlgLock->DestroyWindow();
		}
		if (m_pDlgLock)
		{
			delete m_pDlgLock;
			m_pDlgLock = 0;
		}
		//if (::AfxGetApp()->GetMainWnd())
		//{
		//	::AfxGetApp()->GetMainWnd()->ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
		//	WINDOWPLACEMENT wp; //显示隐藏
		//	wp.length = sizeof(WINDOWPLACEMENT);
		//	wp.flags = WPF_RESTORETOMAXIMIZED;
		//	wp.showCmd = SW_SHOW;
		//	wp.rcNormalPosition = CRect(0, 0, CURSCREEN_WIDTH, CURSCREEN_HEIGHT);
		//	::AfxGetApp()->GetMainWnd()->SetWindowPlacement(&wp);
		//}
		if (::AfxGetApp()->GetMainWnd())
		{
			if (!::AfxGetApp()->GetMainWnd()->IsWindowVisible())
			{
				::AfxGetApp()->GetMainWnd()->ShowWindow(SW_SHOW);
				::AfxGetApp()->GetMainWnd()->SetTimer(10001, 100, 0);
			}
		}
	}
}

IMPLEMENT_DYNAMIC(CDlgLockScreen, CDialogEx)

CDlgLockScreen::CDlgLockScreen(CWnd* pParent /*=NULL*/)
: CLKDialog(CDlgLockScreen::IDD, pParent)
{
#ifndef _DEBUG
	CDlgLockScreen::HideTaskbar(true);
#endif
	m_rtCode = CRect(0, 0, 0, 0);
	InitImageList();
	CRect rt(2, 2, 2, 2);
	SetNCClientRect(rt);
}

CDlgLockScreen::~CDlgLockScreen()
{
	//CDlgLockScreen::HideTaskbar(false);
}

void CDlgLockScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// 初始化图片列表
void CDlgLockScreen::InitImageList()
{
	// 初始化图片列表
	CLKDialog::InitImageList();
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_CLOSE] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MIN] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_RESTORE] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SELF] = 0;
}

// 初始化窗口前景图片
void CDlgLockScreen::OnInitMemImg()
{
	// 背景
	CRect rt(0, 0, CURSCREEN_WIDTH, CURSCREEN_HEIGHT);
	CStringA strFileA = CConfigFile::GetInstance()->GetLoginBgPathNameA().c_str();
	CString strFile;
	strFile = strFileA;
	CLKImage *pImg = GetMemImage();
	CLKImage img;
	if (img.LoadEx(strFile, false) && pImg)
	{
		img.StretchBilinear(pImg, rt.Width(), rt.Height(), img.GetWidth(), img.GetHeight());
		img.Destroy();
	}
	// 右标签1 
	strFileA = CConfigFile::GetInstance()->GetRight1PathNameA().c_str();
	strFile = strFileA;
	CRect rtRight(0, 0, 0, 0);
	if (img.LoadEx(strFile, false) && pImg)
	{
		rtRight = CRect(0, 0, img.GetWidth(), img.GetHeight());
		rtRight.MoveToXY(rt.right - rtRight.Width() - 10, 10);
		img.DrawToImage(pImg, rtRight);
		rtRight.OffsetRect(0, img.GetHeight() + 10);
		img.Destroy();
	}
	// 右标签2 
	strFileA = CConfigFile::GetInstance()->GetRight2PathNameA().c_str();
	strFile = strFileA;
	if (img.LoadEx(strFile, false) && pImg)
	{
		if (rtRight.IsRectEmpty()){ rtRight = CRect(0, 0, img.GetWidth(), img.GetHeight()); }
		img.DrawToImage(pImg, rtRight);
		rtRight.OffsetRect(0, img.GetHeight() + 10);
		img.Destroy();
	}
	// 右标签3 
	strFileA = CConfigFile::GetInstance()->GetRight3PathNameA().c_str();
	strFile = strFileA;
	if (img.LoadEx(strFile, false) && pImg)
	{
		if (rtRight.IsRectEmpty()){ rtRight = CRect(0, 0, img.GetWidth(), img.GetHeight()); }
		img.DrawToImage(pImg, rtRight);
		rtRight.OffsetRect(0, img.GetHeight() + 10);
		img.Destroy();
	}
	
	// 获取二维码图片 
	strFileA = CConfigFile::GetInstance()->GetCodePathNameA().c_str();
	strFile = strFileA;
	if (img.LoadEx(strFile, false) && pImg)
	{
		//if (rtRight.IsRectEmpty()){ rtRight = CRect(0, 0, img.GetWidth(), img.GetHeight()); }
		m_rtCode = CRect((rt.right - img.GetWidth()) / 2, (rt.bottom - img.GetHeight()) / 2, (rt.right + img.GetWidth()) / 2, (rt.bottom + img.GetHeight()) / 2);
		img.DrawToImage(pImg, m_rtCode);
	}
}

void CDlgLockScreen::OnPaint(CDC *pDC)
{
	if (!m_rtCode.IsRectEmpty())
	{
		CRect rt(m_rtCode);
		rt.OffsetRect(0, rt.Height());
		CString strText = L"请先扫码注册";
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,0,0));
		CFont *pFt = pDC->SelectObject(CLKFontMgr::GetSTPoint28());
		pDC->DrawText(strText, rt, DT_TOP | DT_CENTER);
		pDC->SelectObject(pFt);
	}
}

LRESULT CDlgLockScreen::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	//return ((CmyouboxDlg *)::AfxGetApp()->GetMainWnd())->OnCommonMsg(wParam, lParam);
	return CLKDialog::OnCommonMsg(wParam, lParam);
}

BEGIN_MESSAGE_MAP(CDlgLockScreen, CLKDialog)
	ON_BN_CLICKED(100, &CDlgLockScreen::OnBnClickedButtonLogin)
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlgLockScreen 消息处理程序

BOOL CDlgLockScreen::OnInitDialog()
{
	CRect rt(0, 0, CURSCREEN_WIDTH, CURSCREEN_HEIGHT);
#ifndef _DEBUG
	SetWindowPos(&wndTopMost, 0, 0, rt.Width(), rt.Height(), SWP_SHOWWINDOW);
#else
	SetWindowPos(0, 0, 0, rt.Width(), rt.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
#endif
	CLKDialog::OnInitDialog();
	//SetWindowText(L"");
	// 按钮
	CRect rtBtn(rt.right - 82, rt.bottom - 32, rt.right - 10, rt.bottom - 10);
	m_BtnLogin.SetTextFont(CLKFontMgr::GetSTB100());
	m_BtnLogin.SetLeftMargin(9);
	m_BtnLogin.Create(L"登   录", rtBtn, this, 100);

#ifndef _DEBUG
	SetTimer(1, 100, 0);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgLockScreen::OnBnClickedButtonLogin()
{
	CDlgAdminLogin dlg(this);
	if (IDOK == dlg.DoModal())
	{
		auto button = (CDlgGameMenu *)::AfxGetApp()->GetMainWnd()->GetDescendantWindow(TRAYMAINMENUITEM_ID_LOCKSYS);
		if (button)
		{
			button->SetDlgCtrlID(1);
			button->SetWindowText(_T("退   出"));
			button->Invalidate();
		}
		LockScreen(false);
	}
}

void CDlgLockScreen::HideTaskbar(bool bHide)
{
	CString s = _T("Shell_TrayWnd");
	HWND Htaskbar = ::FindWindow((LPCTSTR)s, NULL);
	if (Htaskbar)
	{
		if (bHide)
			::ShowWindow(Htaskbar, SW_HIDE);
		else
			::ShowWindow(Htaskbar, SW_SHOW);
	}
	else
	{
		CLogClient::WriteLog("任务栏不存在");
	}
}



void CDlgLockScreen::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	// TODO:  在此添加消息处理程序代码和/或调用默认值 
	if (1 == nIDEvent)
	{
		if (IsWindowVisible())
		{
			if (::AfxGetApp()->GetMainWnd())
			{
				if (::AfxGetApp()->GetMainWnd()->IsWindowVisible())
				{
					::AfxGetApp()->GetMainWnd()->ShowWindow(SW_HIDE);
				}
			}
			MinmizeZDlg(GetSafeHwnd());
			//HWND hTop = ::GetForegroundWindow();
			//if (hTop != GetSafeHwnd() && hTop != 0)
			//{
			//	DWORD dwProcessID(0);
			//	GetWindowThreadProcessId(hTop, &dwProcessID);
			//	DWORD dwCurProcessID = GetCurrentProcessId();
			//	if (dwProcessID != dwCurProcessID)
			//	{
			//		//::SetParent(GetSafeHwnd(), hTop);
			//		::SetWindowPos(hTop, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
			//		::ShowWindow(hTop, SW_MINIMIZE);
			//		::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
			//		::SetForegroundWindow(GetSafeHwnd());
			//		CLogClient::WriteLog("CDlgLock::OnTimer: %d -- %d", dwProcessID, dwCurProcessID);
			//	}
			//}
			////CLogClient::WriteLog("CDlgLock::OnTimer");
		}
		else
		{
			//if (::AfxGetApp()->GetMainWnd())
			//{
			//	if (!::AfxGetApp()->GetMainWnd()->IsWindowVisible())
			//	{
			//		::AfxGetApp()->GetMainWnd()->ShowWindow(SW_SHOW);
			//	}
			//}
			////KillTimer(nIDEvent);
		}
		SetTimer(1, 1000, 0);
	}
	CLKDialog::OnTimer(nIDEvent);
}

void CDlgLockScreen::MinmizeZDlg(HWND hSelf)
{
	HWND hTop = ::GetForegroundWindow();
	if (hTop != hSelf && hTop != 0)
	{
		DWORD dwProcessID(0);
		GetWindowThreadProcessId(hTop, &dwProcessID);
		DWORD dwCurProcessID = GetCurrentProcessId();
		if (dwProcessID != dwCurProcessID)
		{
			//::SetParent(GetSafeHwnd(), hTop);
			::SetWindowPos(hTop, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
			::ShowWindow(hTop, SW_MINIMIZE);
			::SetWindowPos(hSelf, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
			::SetForegroundWindow(hSelf);
			CLogClient::WriteLog("CDlgLock::OnTimer: %d -- %d", dwProcessID, dwCurProcessID);
		}
	}
}

BOOL CDlgLockScreen::PreTranslateMessage(MSG* pMsg)
{
	// 屏蔽ALT+F4
#ifndef _DEBUG
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}
#endif
	return CLKDialog::PreTranslateMessage(pMsg);
}


LRESULT CDlgLockScreen::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return HTOBJECT;
}

