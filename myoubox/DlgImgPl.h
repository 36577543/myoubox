#pragma once

#include "LKImage.h"

// CDlgImgPl �Ի���

class CDlgImgPl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImgPl)

public:
	CDlgImgPl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgImgPl();

// �Ի�������
	enum { IDD = IDD_DIALOG_IMGP };

public:
	void SetImage(CLKImage *pImg){ m_pImgPl = pImg; }
	virtual BOOL OnInitDialog();
	virtual void OnOK(){}
	virtual void OnCancel(){}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// �ɰ�ͼƬ
	CLKImage	 *m_pImgPl = 0;
	bool DoUpdateDummyDialog();
};
