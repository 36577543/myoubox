#pragma once

#include "LKDialog.h"
#include "LKButton.h"
#include "LKContainer.h"

// CLockDlg �Ի���

class CLockDlg : public CLKDialog
{
	DECLARE_DYNAMIC(CLockDlg)
public:
	static CLockDlg *GetInstance(){ return m_pDlgLock; }
	// ����
	static void LockScreen(bool bLock);
	// �Ƿ�����
	static bool IsLockScreen();
	// �Ƿ�����������
	static void HideTaskbar(bool bHide);
	// ��С������
	static void MinmizeZDlg(HWND hSelf);
public:
	CLockDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLockDlg();

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
	static CLockDlg *m_pDlgLock;
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
