
// myouboxDlg.h : ͷ�ļ�
//

#pragma once

#include "LKMainDialog.h"
#include "LKButton.h"
#include "LKEdit.h"
#include "LKTreeCtrl.h"
#include "LKTab.h"
#include "LKContainer.h"


#include "DlgImgPl.h"

// CDlgGameMenu �Ի���
class CDlgGameMenu : public CLKMainDialog
{
// ����
public:
	CDlgGameMenu(CWnd* pParent = NULL);	// ��׼���캯��
	~CDlgGameMenu();

// �Ի�������
	enum { IDD = IDD_MYOUBOX_DIALOG };

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// ��ʼ������ǰ��ͼƬ
	virtual void OnInitMemImg();
	// �Զ���˵���ť������
	virtual void OnMenuClick();
	// �ж�����Ƿ��������Զ�������Χ��
	virtual bool IsInOther(CPoint point);
	// �������
	virtual void OnTrayLButtonDown();
	// �Ҽ�����
	virtual void OnTrayRButtonUp();
	// �رհ�ť������
	virtual void OnCloseClick(){ ShowWindow(SW_HIDE); }
	// ��С����ť������
	virtual void OnMinClick();
	//// ����˫����Ϣ
	//virtual void OnTrayLDBClick();
	// �¼�
	virtual BOOL OnEnterPress(HWND hCrtl, LPARAM lparam);
	// �޸Ĵ�������
	virtual void ModiDialogRectRgn(CRgn &rgn);
private:
	// ���������
	HANDLE		m_hStartProcess = 0;
	// ������ȫ·��
	CString		m_strMainProcess;
	HICON		m_hSmallIcon = 0;
	// ������
	CLKEdit		m_ctlSearch;
	// ������ť
	CLKButton	m_BtnSearch;
	//// Ƥ����ť
	//CLKButton	m_BtnSkin;
	//// ��С����ť
	//CLKButton	m_BtnSkin;
	//// ��󻯰�ť
	//CLKButton	m_BtnSkin;
	// �˳�/���ʰ�ť
	CLKButton	m_BtnQuitOrCheckout;
	// ������Ϸ����
	CLKTreeCtrl m_treeMain;
	// �����������ǩ
	CLKTab		m_tabMain;
	// ��Ϸ�б�
	CLKContainer m_Container;
	// ��Ϸ�ɰ� 
	CDlgImgPl	*m_pDialog = 0;
	//CDynDialogEx    *m_pDialog = 0;
	// �ɰ�ͼƬ
	CLKImage	*m_pImgPl = 0;
private:
	// �����ӿؼ���λ��
	void AdjustChildCtrl();
	// ������Ϸ����
	void LoadGameInfo();
	// ������Ϸ��������
	void LoadGameTypeInfo();
	// �������οؼ�ѡ����ı�
	void OnTreeCtrlSelectedChanged(PLKTREEITEM pOldItem);
	// ����TAB�ؼ�ѡ����ı�
	void OnTabCtrlSelectedChanged();
	// ������������
	void UpdateAllDlgBGImg(); 
	void OnLockClient();
	void OnOpenMainPanel();
	void OnEBnClickedButtonSearchI();
	bool GetProcessidFromName(CString &strName);
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
