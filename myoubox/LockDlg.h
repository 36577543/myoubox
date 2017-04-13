#pragma once

#include "LKDialog.h"
#include "LKButton.h"
#include "LKContainer.h"

// CLockDlg 对话框

class CLockDlg : public CLKDialog
{
	DECLARE_DYNAMIC(CLockDlg)
public:
	static CLockDlg *GetInstance(){ return m_pDlgLock; }
	// 锁屏
	static void LockScreen(bool bLock);
	// 是否锁屏
	static bool IsLockScreen();
	// 是否隐藏任务栏
	static void HideTaskbar(bool bHide);
	// 最小化窗口
	static void MinmizeZDlg(HWND hSelf);
public:
	CLockDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLockDlg();

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
	static CLockDlg *m_pDlgLock;
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
