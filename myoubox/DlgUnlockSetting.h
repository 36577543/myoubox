#pragma once

#include "LKDialog.h"
#include "LKButton.h"
#include "LKEdit.h"
#include "LKStatic.h"

// CDlgUnlockSetting 对话框

class CDlgUnlockSetting : public CLKDialog
{
	DECLARE_DYNAMIC(CDlgUnlockSetting)

public:
	CDlgUnlockSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUnlockSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_UNLOCK_SETTING };

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
	
	CLKEdit		m_ctlUserName;	// 用户名框	
	CLKEdit		m_ctlUserPass;	// 用户密码框
	CLKButton	m_BtnOK;
	CLKButton	m_BtnCancel;
	CString	m_tip1;
};
