#pragma once

#include "LKDialog.h"
#include "LKButton.h"
#include "LKEdit.h"

// CDlgAdminLogin 对话框

class CDlgAdminLogin : public CLKDialog
{
	DECLARE_DYNAMIC(CDlgAdminLogin)

public:
	CDlgAdminLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAdminLogin();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 初始化图片列表
	virtual void InitImageList();
	// 初始化窗口前景图片
	virtual void OnInitMemImg();
	// 处理窗口wm_paint消息
	virtual void OnPaint(CDC *pDC);
	// 获取提供背景DC的窗口句柄
	virtual HWND GetProvideBGHwnd(){ return ::AfxGetApp()->GetMainWnd()->GetSafeHwnd(); }

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
private:
	// 用户名框
	CLKEdit		m_ctlUserName;
	// 用户密码框
	CLKEdit		m_ctlUserPass;
	CLKButton	m_BtnOK;
	CLKButton	m_BtnCancel;

	CString	m_tip;
};
