#pragma once

#include "LKDialog.h"
#include "LKButton.h"
#include "LKContainer.h"

// CDlgLockScreen 对话框

class CDlgLockScreen : public CLKDialog
{
	DECLARE_DYNAMIC(CDlgLockScreen)
public:
	static CDlgLockScreen *GetInstance(){ return m_pDlgLock; }
	// 锁屏
	static void LockScreen(bool bLock);
	// 是否锁屏
	static bool IsLockScreen();
	// 是否隐藏任务栏
	static void HideTaskbar(bool bHide);
	// 最小化窗口
	static void MinmizeZDlg(HWND hSelf);
public:
	CDlgLockScreen(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLockScreen();

// 对话框数据
	enum { IDD = IDD_LOCK_DIALOG };
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 初始化图片列表
	virtual void InitImageList();
	// 初始化窗口前景图片
	virtual void OnInitMemImg();
	// 处理窗口wm_paint消息
	virtual void OnPaint(CDC *pDC);

	virtual LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	static CDlgLockScreen *m_pDlgLock;
	// 二维码区域
	CRect m_rtCode;
	// 登录按钮
	CLKButton	m_BtnLogin;
	afx_msg void OnBnClickedButtonLogin();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
