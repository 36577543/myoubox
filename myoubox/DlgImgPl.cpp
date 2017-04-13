// DlgImgPl.cpp : 实现文件
//

#include "stdafx.h"
#include "myoubox.h"
#include "DlgImgPl.h"
#include "afxdialogex.h"


// CDlgImgPl 对话框

IMPLEMENT_DYNAMIC(CDlgImgPl, CDialogEx)

CDlgImgPl::CDlgImgPl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImgPl::IDD, pParent)
{

}

CDlgImgPl::~CDlgImgPl()
{
}

void CDlgImgPl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImgPl, CDialogEx)
END_MESSAGE_MAP()


// CDlgImgPl 消息处理程序

BOOL CDlgImgPl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DoUpdateDummyDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

bool CDlgImgPl::DoUpdateDummyDialog()
{
	if (!m_pImgPl)
	{
		return false;
	}
	if (!m_pImgPl->IsValid())
	{
		return false;
	}
	BYTE SourceConstantAlpha = 255;
	// make sure the window has the WS_EX_LAYERED style
	ModifyStyleEx(0, WS_EX_LAYERED);
	// get the window rectangule (we are only interested in the top left position)
	CRect rectDlg(0, 0, m_pImgPl->GetWidth(), m_pImgPl->GetHeight());
	//GetWindowRect(rectDlg);
	// Create/setup the DC's
	CDC dcScreen;
	CDC dcMemory;
	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);
	CBitmap bmpx;
	bmpx.CreateCompatibleBitmap(&dcScreen, rectDlg.Width(), rectDlg.Height());
	CBitmap *pOldBitmap = dcMemory.SelectObject(&bmpx);
	CRect rtWnd(rectDlg);
	rtWnd.MoveToXY(0, 0);
	//OnDraw(&dcMemory, rtWnd);
	m_pImgPl->DrawToDC(&dcMemory, 0, 0, 0);

	// calculate the new window position/size based on the bitmap size
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(rectDlg.Width(), rectDlg.Height());
	// setup the blend function
	BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0, 0); // start point of the copy from dcMemory to dcScreen
	// perform the alpha blend
	BOOL bRet = ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
	if (bRet == 0)
	{
		TRACE(_T("UpdateLayeredWindow %d\r\n"), ::GetLastError());
		return false;
	}
	//ASSERT(bRet); // ops... something bad just occured....
	// clean up
	dcMemory.SelectObject(pOldBitmap);
	//OnDrawItems(&dcScreen, rectDlg);
	return true;
}
