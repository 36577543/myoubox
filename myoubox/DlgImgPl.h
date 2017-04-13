#pragma once

#include "LKImage.h"

// CDlgImgPl 对话框

class CDlgImgPl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImgPl)

public:
	CDlgImgPl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImgPl();

// 对话框数据
	enum { IDD = IDD_DIALOG_IMGP };

public:
	void SetImage(CLKImage *pImg){ m_pImgPl = pImg; }
	virtual BOOL OnInitDialog();
	virtual void OnOK(){}
	virtual void OnCancel(){}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 蒙板图片
	CLKImage	 *m_pImgPl = 0;
	bool DoUpdateDummyDialog();
};
