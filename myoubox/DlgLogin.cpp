// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "myoubox.h"
#include "DlgLogin.h"
#include "afxdialogex.h"
#include "myouboxConst.h"

#include "LKFontMgr.h"
#include "LKImageMgr.h"

// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CLKDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
: CLKDialog(CDlgLogin::IDD, pParent)
//, m_BtnOK(2)
//, m_BtnCancel(2)
{
	SetTopOffset(22);
	SetRightOffset(10);
	InitImageList();
	CRect rt(2, 2, 2, 2);
	SetNCClientRect(rt);
}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// 初始化图片列表
void CDlgLogin::InitImageList()
{
	// 初始化图片列表
	CLKDialog::InitImageList();
	//m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_CLOSE] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MIN] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_RESTORE] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SELF] = 0;
}

// 初始化窗口前景图片
void CDlgLogin::OnInitMemImg()
{
	//// 头部背景
	//CLKImage *pImgHBG = CLKImageMgr::GetImageS(LKIMAMYOUBOX_DIALOG_HEAD);
	CLKImage *pImg = GetMemImage();
	//if (pImgHBG && pImg)
	//{
	//	CRect rtHBG(2, 2, m_rtWnd.Width() - 4, pImgHBG->GetHeight());
	//	pImgHBG->DrawToImage(pImg, rtHBG);
	//}
	// 头部标签
	CLKImage *pImgHL = CLKImageMgr::GetImageS(LKIMAMYOUBOX_DIALOG_HEADLABEL);
	if (pImgHL && pImg)
	{
		CRect rtHL(0, 0, pImgHL->GetWidth(), pImgHL->GetHeight());
		rtHL.MoveToXY(20, 10);
		pImgHL->DrawToImage(pImg, rtHL);
	}
	//// 分隔条
	//CLKImage *pImgSplit = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_SPLIT);
	//if (pImgSplit && pImg)
	//{
	//	CRect rtS(0, 67, m_rtWnd.Width(), pImgSplit->GetHeight() + 67);
	//	//pImgSplit->DrawToImageLR(pImg, rtS);
	//	//CLKImage img(rtS.Width(), rtS.Height());
	//	//pImgSplit->StretchBilinear(&img, rtS.Width(), rtS.Height(), pImgSplit->GetWidth(), pImgSplit->GetHeight());
	//	pImgSplit->DrawToImage(pImg, rtS);
	//}
}

// 处理窗口wm_paint消息
void CDlgLogin::OnPaint(CDC *pDC)
{
	CRect rt(20, 120, 0, 0);
	rt.right = rt.left + 160;
	rt.bottom = rt.top + 30;
	CString strText = L"用户名：";
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	CFont *pFt = pDC->SelectObject(CLKFontMgr::GetSTPoint22());
	pDC->DrawText(strText, rt, DT_RIGHT | DT_VCENTER);
	strText = L"密  码：";
	rt.OffsetRect(0, 60);
	pDC->DrawText(strText, rt, DT_RIGHT | DT_VCENTER);
	pDC->SelectObject(pFt);
}

BEGIN_MESSAGE_MAP(CDlgLogin, CLKDialog)
	ON_BN_CLICKED(103, &CDlgLogin::OnBnClickedButtonOk)
	ON_BN_CLICKED(104, &CDlgLogin::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序


BOOL CDlgLogin::OnInitDialog()
{
	CLKDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rt(180, 120, 0, 0);
	rt.right = rt.left + 160;
	rt.bottom = rt.top + 30;
	m_ctlUserName.SetClientEdge(CRect(0, 0, 0, 0));
	m_ctlUserName.SetTextFont(CLKFontMgr::GetSTPoint16());
	m_ctlUserName.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | ES_WANTRETURN, rt, this, 100);
	rt.OffsetRect(0, 60);
	m_ctlUserPass.SetClientEdge(CRect(0, 0, 0, 0));
	m_ctlUserPass.CreatePass(rt, this, 101);

	GetClientRect(&rt);
	CRect rtBtn(rt.right - 160, rt.bottom - 40, 0, 0);
	rtBtn.right = rtBtn.left + 72;
	rtBtn.bottom = rtBtn.top + 30;
	m_BtnOK.SetTextFont(CLKFontMgr::GetSTPoint16());
	m_BtnOK.SetLeftMargin(16);
	m_BtnOK.Create(L"确定", rtBtn, this, 103);
	rtBtn.OffsetRect(80, 0);
	m_BtnCancel.SetTextFont(CLKFontMgr::GetSTPoint16());
	m_BtnCancel.SetLeftMargin(16);
	m_BtnCancel.Create(L"取消", rtBtn, this, 104);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgLogin::OnBnClickedButtonOk()
{
	OnOK();
}

void CDlgLogin::OnBnClickedButtonCancel()
{
	OnCancel();
}
