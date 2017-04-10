// DlgWeb.cpp : 实现文件
//

#include "stdafx.h"
#include "myoubox.h"
#include "DlgWeb.h"


// CDlgWeb 对话框
BOOL RaisePrivileges();
void SetIECoreVersion();

IMPLEMENT_DYNCREATE(CDlgWeb, CDHtmlDialog)

CDlgWeb::CDlgWeb(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(IDD_DIALOG_WEB, IDR_HTML_DIALOG_WEB, pParent)
{
	RaisePrivileges();
	SetIECoreVersion();
}

CDlgWeb::~CDlgWeb()
{
}

void CDlgWeb::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDlgWeb::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CDlgWeb, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgWeb)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CDlgWeb 消息处理程序

HRESULT CDlgWeb::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDlgWeb::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

BOOL RaisePrivileges()
{
	HANDLE TokenHandle;
	TOKEN_PRIVILEGES t_privileges = { 0 };

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &t_privileges.Privileges[0].Luid))
	{
		return TRUE;
	}

	t_privileges.PrivilegeCount = 1;
	t_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(TokenHandle, FALSE, &t_privileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		CloseHandle(TokenHandle);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void SetIECoreVersion()
{
	REGSAM samDesiredOpen = KEY_ALL_ACCESS;
	TCHAR* path = L"SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION";
	TCHAR* valueName = L"DHTMLDialog.exe";
	long version = 9999;
	TCHAR err[1024];
	HKEY hKey;
	DWORD dwDisposition;
	long ret = RegOpenKeyEx(HKEY_CURRENT_USER, path, 0, samDesiredOpen, &hKey);	// HKEY_LOCAL_MACHINE	KEY_ALL_ACCESS	REG_LEGAL_OPTION
	if (ret != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ret, NULL, err, sizeof(err), NULL);
		ret = RegCreateKeyEx(HKEY_CURRENT_USER, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
		if (ret != ERROR_SUCCESS)
		{
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ret, NULL, err, sizeof(err), NULL);
			return;
		}
	}

	ret = RegSetValueEx(hKey, valueName, NULL, REG_DWORD, (BYTE*)&version, sizeof(version));
	if (ret != ERROR_SUCCESS)
		return;
}

