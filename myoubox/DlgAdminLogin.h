#pragma once

#include "LKDialog.h"
#include "LKButton.h"
#include "LKEdit.h"

// CDlgAdminLogin �Ի���

class CDlgAdminLogin : public CLKDialog
{
	DECLARE_DYNAMIC(CDlgAdminLogin)

public:
	CDlgAdminLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAdminLogin();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// ��ʼ������ǰ��ͼƬ
	virtual void OnInitMemImg();
	// ������wm_paint��Ϣ
	virtual void OnPaint(CDC *pDC);
	// ��ȡ�ṩ����DC�Ĵ��ھ��
	virtual HWND GetProvideBGHwnd(){ return ::AfxGetApp()->GetMainWnd()->GetSafeHwnd(); }

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
private:
	// �û�����
	CLKEdit		m_ctlUserName;
	// �û������
	CLKEdit		m_ctlUserPass;
	CLKButton	m_BtnOK;
	CLKButton	m_BtnCancel;

	CString	m_tip;
};
