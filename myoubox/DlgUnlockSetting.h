#pragma once

#include "LKDialog.h"
#include "LKButton.h"
#include "LKEdit.h"
#include "LKStatic.h"

// CDlgUnlockSetting �Ի���

class CDlgUnlockSetting : public CLKDialog
{
	DECLARE_DYNAMIC(CDlgUnlockSetting)

public:
	CDlgUnlockSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUnlockSetting();

// �Ի�������
	enum { IDD = IDD_DIALOG_UNLOCK_SETTING };

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
	
	CLKEdit		m_ctlUserName;	// �û�����	
	CLKEdit		m_ctlUserPass;	// �û������
	CLKButton	m_BtnOK;
	CLKButton	m_BtnCancel;
	CString	m_tip1;
};
