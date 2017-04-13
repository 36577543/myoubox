
// myouboxDlg.h : 头文件
//

#pragma once

#include "LKMainDialog.h"
#include "LKButton.h"
#include "LKEdit.h"
#include "LKTreeCtrl.h"
#include "LKTab.h"
#include "LKContainer.h"


#include "DlgImgPl.h"

// CDlgGameMenu 对话框
class CDlgGameMenu : public CLKMainDialog
{
// 构造
public:
	CDlgGameMenu(CWnd* pParent = NULL);	// 标准构造函数
	~CDlgGameMenu();

// 对话框数据
	enum { IDD = IDD_MYOUBOX_DIALOG };

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnEnChangeEdit();
	afx_msg void OnEBnClickedButtonSearch(); 
	DECLARE_MESSAGE_MAP()
protected:
	// 初始化图片列表
	virtual void InitImageList();
	// 初始化窗口前景图片
	virtual void OnInitMemImg();
	// 自定义菜单按钮被按下
	virtual void OnMenuClick();
	// 判断鼠标是否在其它自定义区域范围内
	virtual bool IsInOther(CPoint point);
	// 左键按下
	virtual void OnTrayLButtonDown();
	// 右键处理
	virtual void OnTrayRButtonUp();
	// 关闭按钮被按下
	virtual void OnCloseClick(){ ShowWindow(SW_HIDE); }
	// 最小化按钮被按下
	virtual void OnMinClick();
	//// 托盘双击消息
	//virtual void OnTrayLDBClick();
	// 事件
	virtual BOOL OnEnterPress(HWND hCrtl, LPARAM lparam);
	// 修改窗口区域
	virtual void ModiDialogRectRgn(CRgn &rgn);
private:
	// 启动器句柄
	HANDLE		m_hStartProcess = 0;
	// 主程序全路径
	CString		m_strMainProcess;
	HICON		m_hSmallIcon = 0;
	// 搜索框
	CLKEdit		m_ctlSearch;
	// 搜索按钮
	CLKButton	m_BtnSearch;
	//// 皮肤按钮
	//CLKButton	m_BtnSkin;
	//// 最小化按钮
	//CLKButton	m_BtnSkin;
	//// 最大化按钮
	//CLKButton	m_BtnSkin;
	// 退出/结帐按钮
	CLKButton	m_BtnQuitOrCheckout;
	// 热门游戏分类
	CLKTreeCtrl m_treeMain;
	// 顶部主分面标签
	CLKTab		m_tabMain;
	// 游戏列表
	CLKContainer m_Container;
	// 游戏蒙板 
	CDlgImgPl	*m_pDialog = 0;
	//CDynDialogEx    *m_pDialog = 0;
	// 蒙板图片
	CLKImage	*m_pImgPl = 0;
private:
	// 调整子控件的位置
	void AdjustChildCtrl();
	// 加载游戏数据
	void LoadGameInfo();
	// 加载游戏类型数据
	void LoadGameTypeInfo();
	// 处理树形控件选择项改变
	void OnTreeCtrlSelectedChanged(PLKTREEITEM pOldItem);
	// 处理TAB控件选择项改变
	void OnTabCtrlSelectedChanged();
	// 更新其它窗口
	void UpdateAllDlgBGImg(); 
	void OnLockClient();
	void OnOpenMainPanel();
	void OnEBnClickedButtonSearchI();
	bool GetProcessidFromName(CString &strName);
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
