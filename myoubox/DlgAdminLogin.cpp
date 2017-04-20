// DlgLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "myoubox.h"
#include "DlgAdminLogin.h"
#include "afxdialogex.h"
#include "myouboxConst.h"

#include "LKFontMgr.h"
#include "LKImageMgr.h"

#include "SvrConfig.h"
#include "md5.hh"
#include "AtlBase.h"
#include "AtlConv.h"

// CDlgAdminLogin �Ի���

IMPLEMENT_DYNAMIC(CDlgAdminLogin, CLKDialog)

CDlgAdminLogin::CDlgAdminLogin(CWnd* pParent /*=NULL*/)
: CLKDialog(CDlgAdminLogin::IDD, pParent)
{
	SetTopOffset(22);
	SetRightOffset(10);
	InitImageList();
	CRect rt(2, 2, 2, 2);
	SetNCClientRect(rt);
}

CDlgAdminLogin::~CDlgAdminLogin()
{
}

void CDlgAdminLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// ��ʼ��ͼƬ�б�
void CDlgAdminLogin::InitImageList()
{
	// ��ʼ��ͼƬ�б�
	CLKDialog::InitImageList();
	//m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_CLOSE] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MIN] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_RESTORE] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SELF] = 0;
}

// ��ʼ������ǰ��ͼƬ
void CDlgAdminLogin::OnInitMemImg()
{
	//// ͷ������
	//CLKImage *pImgHBG = CLKImageMgr::GetImageS(LKIMAMYOUBOX_DIALOG_HEAD);
	CLKImage *pImg = GetMemImage();
	//if (pImgHBG && pImg)
	//{
	//	CRect rtHBG(2, 2, m_rtWnd.Width() - 4, pImgHBG->GetHeight());
	//	pImgHBG->DrawToImage(pImg, rtHBG);
	//}
	// ͷ����ǩ
	CLKImage *pImgHL = CLKImageMgr::GetImageS(LKIMAMYOUBOX_DIALOG_HEADLABEL);
	if (pImgHL && pImg)
	{
		CRect rtHL(0, 0, pImgHL->GetWidth(), pImgHL->GetHeight());
		rtHL.MoveToXY(20, 10);
		pImgHL->DrawToImage(pImg, rtHL);
	}
	//// �ָ���
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

// ������wm_paint��Ϣ
void CDlgAdminLogin::OnPaint(CDC *pDC)
{
	CRect rt(20, 120, 0, 0);
	rt.right = rt.left + 160;
	rt.bottom = rt.top + 30;
	CString strText = L"�û�����";
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	CFont *pFt = pDC->SelectObject(CLKFontMgr::GetSTPoint22());
	pDC->DrawText(strText, rt, DT_RIGHT | DT_VCENTER);
	strText = L"��  �룺";
	rt.OffsetRect(0, 60);
	pDC->DrawText(strText, rt, DT_RIGHT | DT_VCENTER);
	pDC->SelectObject(pFt);

	CRect rect;
	GetClientRect(&rect);
	rt.left = 0;
	rt.right = rect.right;
	rt.OffsetRect(0, 60);
	pDC->SetTextColor(RGB(255, 0, 0));
	pFt = pDC->SelectObject(CLKFontMgr::GetSTPoint16());
	pDC->DrawText(m_tip, rt, DT_CENTER | DT_VCENTER);
	pDC->SelectObject(pFt);
}

BEGIN_MESSAGE_MAP(CDlgAdminLogin, CLKDialog)
	ON_BN_CLICKED(103, &CDlgAdminLogin::OnBnClickedButtonOk)
	ON_BN_CLICKED(104, &CDlgAdminLogin::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CDlgAdminLogin ��Ϣ�������


BOOL CDlgAdminLogin::OnInitDialog()
{
	CLKDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	m_BtnOK.Create(L"ȷ��", rtBtn, this, 103);
	rtBtn.OffsetRect(80, 0);
	m_BtnCancel.SetTextFont(CLKFontMgr::GetSTPoint16());
	m_BtnCancel.SetLeftMargin(16);
	m_BtnCancel.Create(L"ȡ��", rtBtn, this, 104);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CDlgAdminLogin::OnBnClickedButtonOk()
{
	std::string username, password;
	username = CW2A(m_ctlUserName.GetText());
	password = CW2A(m_ctlUserPass.GetPassword());

	auto & config = SvrConfig::getInstance();
	if (config._username != username)
	{
		static int n = 1;
		m_tip.Format(L"�û����������%d��", n++);
		Invalidate();
		return;
	}

	MD5 context;
	context.update((unsigned char *)password.c_str(), password.length());
	context.finalize();
	if (config._password != context.hex_digest())
	{
		static int n = 1;
		m_tip.Format(L"�����������%d��", n++);
		Invalidate();
		return;
	}

	OnOK();
}

void CDlgAdminLogin::OnBnClickedButtonCancel()
{
	OnCancel();
}
