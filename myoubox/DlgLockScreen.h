#pragma once

#include "LKDialog.h"
#include "LKButton.h"
#include "LKContainer.h"

// CDlgLockScreen �Ի���

class CDlgLockScreen : public CLKDialog
{
	DECLARE_DYNAMIC(CDlgLockScreen)
public:
	static CDlgLockScreen *GetInstance(){ return m_pDlgLock; }
	// ����
	static void LockScreen(bool bLock);
	// �Ƿ�����
	static bool IsLockScreen();
	// �Ƿ�����������
	static void HideTaskbar(bool bHide);
	// ��С������
	static void MinmizeZDlg(HWND hSelf);
public:
	CDlgLockScreen(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLockScreen();

// �Ի�������
	enum { IDD = IDD_LOCK_DIALOG };
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// ��ʼ������ǰ��ͼƬ
	virtual void OnInitMemImg();
	// ������wm_paint��Ϣ
	virtual void OnPaint(CDC *pDC);

	virtual LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	static CDlgLockScreen *m_pDlgLock;
	// ��ά������
	CRect m_rtCode;
	// ��¼��ť
	CLKButton	m_BtnLogin;
	afx_msg void OnBnClickedButtonLogin();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
