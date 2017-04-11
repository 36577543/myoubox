// DlgWeb.cpp : 实现文件
//

#include "stdafx.h"
#include "myoubox.h"
#include "DlgWeb.h"
#include "StoreConfig.h"
#include "Logger.h"

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

#define CHECK_HR(hr)			\
	if (FAILED(hr))				\
	{							\
		BOOST_LOG_FUNCTION();	\
		LOG_ERROR(logger) << std::showbase << std::uppercase << std::hex << hr << std::noshowbase << std::nouppercase << std::dec; \
		goto done;				\
	}
#define CHECK_NULL(point)		\
	if (!point)					\
	{							\
		BOOST_LOG_FUNCTION();	\
		LOG_ERROR(logger) << "point is NULL"; \
		goto done;				\
	}

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
	Navigate(L"http://139.224.61.179/vr/manage/login");
	//OnOK();
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
	TCHAR* path = _T("SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION");
	TCHAR* valueName = _T("myoubox.exe");
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



void CDlgWeb::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);

}


void CDlgWeb::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
	src::severity_channel_logger_mt<SeverityLevel>& logger = global_logger::get();

	HRESULT hr;
	IHTMLDocument2 *pDoc2 = NULL;
	IHTMLDocument3 *pDoc3 = NULL;
	IHTMLElement *pElementDeviceNumber = NULL;
	IHTMLElement *pElementInput = NULL;
	IHTMLElement *pElementTr = NULL;
	IHTMLElement2 *pElement2Tr = NULL;
	IHTMLElementCollection * pCollectionTr = NULL;
	IHTMLElementCollection *pCollectionInput = NULL;
	IDispatch *pDispatchTr = NULL;
	IDispatch *pDispatchInput = NULL;

	int64_t storeId = 0;
	int64_t deviceID = 0;
	CString url = szUrl;
	if (StoreConfig::getInstance()._storeId != 0 && StoreConfig::getInstance()._deviceID != 0)
		return;
	else
	{
		if (url.Find(_T("/manage/index")) != -1)
			Navigate(_T("http://139.224.61.179/vr/manage/device/list"));
	}

	if (url.Find(_T("/device/add")) != -1 || url.Find(_T("/device/list")) != -1)
	{
		hr = GetDHtmlDocument(&pDoc2);
		CHECK_HR(hr);
		CHECK_NULL(pDoc2);

		hr = pDoc2->QueryInterface(IID_IHTMLDocument3, (void**)&pDoc3);
		CHECK_HR(hr);
		CHECK_NULL(pDoc3);
	}

	if (url.Find(_T("/device/add")) != -1)
	{
		hr = pDoc3->getElementById(_bstr_t("deviceNumber"), &pElementDeviceNumber);
		CHECK_HR(hr);
		CHECK_NULL(pElementDeviceNumber);

		hr = pElementDeviceNumber->put_innerText(_bstr_t(StoreConfig::getInstance()._mac.c_str()));
		CHECK_HR(hr);
	}

	if (url.Find(_T("/device/list")) != -1)
	{
		hr = pDoc3->getElementsByTagName(_bstr_t("tr"), &pCollectionTr);
		long length;
		pCollectionTr->get_length(&length);
		for (long i = 0; i < length; i++)
		{
			_variant_t index = i;
			hr = pCollectionTr->item(index, index, &pDispatchTr);
			if FAILED(hr)
			{
				LOG_ERROR(logger) << "pCollectionTr->item:" << hr;
				continue;
			}

			hr = pDispatchTr->QueryInterface(IID_IHTMLElement, (void **)&pElementTr);
			CHECK_HR(hr);
			hr = pDispatchTr->QueryInterface(IID_IHTMLElement2, (void **)&pElement2Tr);
			CHECK_HR(hr);

			_bstr_t bstr;
			hr = pElementTr->get_innerText(&bstr.GetBSTR());
			CHECK_HR(hr);

			CString text = bstr;
			auto &mac = StoreConfig::getInstance()._mac;
			if (text.Find(CString(mac.c_str())) != -1)
			{
				hr = pElement2Tr->getElementsByTagName(_bstr_t("input"), &pCollectionInput);
				CHECK_HR(hr);
				long len;
				pCollectionInput->get_length(&len);
				for (long j = 0; j < length; j++)
				{
					_variant_t index = j;
					hr = pCollectionInput->item(index, index, &pDispatchInput);
					if FAILED(hr)
					{
						LOG_ERROR(logger) << "pCollectionInput->item:" << hr;
						continue;
					}
					hr = pDispatchInput->QueryInterface(IID_IHTMLElement, (void **)&pElementInput);
					CHECK_HR(hr);

					_variant_t varName, varValue;
					hr = pElementInput->getAttribute(_bstr_t("name"), 0, &varName);
					CHECK_HR(hr);
					hr = pElementInput->getAttribute(_bstr_t("value"), 0, &varValue);
					CHECK_HR(hr);

					varValue.ChangeType(VT_I4);
					if (varName == _variant_t("storeId"))
						storeId = varValue.iVal;
					if (varName == _variant_t("id"))
						deviceID = varValue.iVal;
				}
				break;
			}
		}
		if (storeId == 0 || deviceID == 0)
		{
			AfxMessageBox(_T("未添加营业点，或未添加当前设备！"));
			Navigate(_T("http://139.224.61.179/vr/manage/device/add"));
		}
		else
		{
			if (StoreConfig::getInstance()._storeId == 0 || StoreConfig::getInstance()._deviceID == 0)
			{
				auto &cfg = StoreConfig::getInstance();
				cfg._storeId = storeId;
				cfg._deviceID = deviceID;
				cfg.save();
				AfxMessageBox(_T("绑定设备成功！"));
			}
		}
	}

done:
	SafeRelease(&pDispatchTr);
	SafeRelease(&pDispatchInput);
	SafeRelease(&pElementDeviceNumber);
	SafeRelease(&pElementInput);
	SafeRelease(&pElementTr);
	SafeRelease(&pElement2Tr);
	SafeRelease(&pDoc2);
	SafeRelease(&pDoc3);
}
