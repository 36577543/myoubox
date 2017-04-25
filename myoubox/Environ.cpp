#include "stdafx.h"
#include "Environ.h"
#include "Logger.h"

#include <ws2tcpip.h>
#include <boost/locale.hpp>				// boost::locale::conv::from_utf

static src::severity_channel_logger_mt<SeverityLevel>& logger = global_logger::get();

std::string Environ::_mac;
std::string Environ::_ip;
std::string Environ::_pcname;
std::string Environ::_osVersion;
int Environ::_osType = 0;

std::string Environ::_processName;
std::string Environ::_processPath;

std::string gb2312ToUTF8(const std::string &str)
{
	return boost::locale::conv::to_utf<char>(str, "gb2312");
}

std::string utf8ToGB2312(const std::string &str)
{
	return boost::locale::conv::from_utf(str, "gb2312");
}

Environ::Environ()
{
}

Environ::~Environ()
{
}

void Environ::init()
{
	getProcessNamePath(_processName, _processPath);
	initLogger(debug, _processName);

	getMac(_mac);
	getIpPcname(_ip, _pcname);
	getWindowsVersion(_osVersion);

	RaisePrivileges();
	SetIECoreVersion();
}

void Environ::getProcessNamePath(std::string &processName, std::string &processPath)
{
	TCHAR szFullPath[MAX_PATH];
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);
	std::string path = CT2A(szFullPath);
	auto pos1 = path.rfind('\\');
	auto pos2 = path.rfind(".exe");
	if (pos1 != std::string::npos)
		processPath = path.substr(0, pos1 + 1);
	if (pos1 != std::string::npos && pos2 != std::string::npos)
		processName = path.substr(pos1 + 1, pos2 - pos1 - 1);
}

void Environ::getMac(std::string &mac)
{
	if (!GetMacAddressByAdaptersAddresses(mac))
	{
		LOG_ERROR(logger) << "GetMacAddressByAdaptersAddresses failed";
		if (!GetMacAddressByAdaptersInfo(mac))
			LOG_ERROR(logger) << "GetMacAddressByAdaptersInfo failed";
	}
}

void Environ::getIpPcname(std::string &ip, std::string &pcname)
{
	char buf[MAX_PATH] = { 0 };
	int  ret = 0;
	struct addrinfo hints;
	struct addrinfo *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;

	if ((ret = getaddrinfo("", NULL, &hints, &res)) != 0)
		LOG_ERROR(logger) << "getaddrinfo:" << ret;
	else
	{
		struct sockaddr_in *sa = (struct sockaddr_in *)res->ai_addr;
		ip = inet_ntop(AF_INET, &sa->sin_addr.s_addr, buf, sizeof(buf));
		pcname = res->ai_canonname;
	}
}

bool Environ::GetMacAddressByAdaptersAddresses(std::string &mac)
{
	BOOST_LOG_FUNCTION();
	bool ret = false;

	ULONG out_buf_len = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buf_len);
	if (addresses == NULL)
	{
		LOG_ERROR(logger) << "malloc";
		return false;
	}

	if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, addresses, &out_buf_len) == ERROR_BUFFER_OVERFLOW)
	{
		free(addresses);
		addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buf_len);
		if (addresses == NULL)
		{
			LOG_ERROR(logger) << "malloc";
			return false;
		}
	}

	if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, addresses, &out_buf_len) == NO_ERROR)
	{
		PIP_ADAPTER_ADDRESSES curr_addresses = addresses;
		for (; curr_addresses != NULL; curr_addresses = curr_addresses->Next)
		{
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if (curr_addresses->PhysicalAddressLength != 6)
				continue;

			std::stringstream ss;
			ss << std::hex << std::uppercase
				<< int(curr_addresses->PhysicalAddress[0]) //<< "-"
				<< int(curr_addresses->PhysicalAddress[1]) //<< "-"
				<< int(curr_addresses->PhysicalAddress[2]) //<< "-"
				<< int(curr_addresses->PhysicalAddress[3]) //<< "-"
				<< int(curr_addresses->PhysicalAddress[4]) //<< "-"
				<< int(curr_addresses->PhysicalAddress[5]);
			mac = ss.str();
			ret = true;
			break;
		}
	}

	free(addresses);
	return ret;
}

bool Environ::GetMacAddressByAdaptersInfo(std::string &mac)
{
	BOOST_LOG_FUNCTION();
	bool ret = false;

	ULONG out_buf_len = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO adapter_info = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if (adapter_info == NULL)
		return false;

	if (GetAdaptersInfo(adapter_info, &out_buf_len) == ERROR_BUFFER_OVERFLOW)
	{
		free(adapter_info);
		adapter_info = (IP_ADAPTER_INFO *)malloc(out_buf_len);
		if (adapter_info == NULL)
			return false;
	}

	if (GetAdaptersInfo(adapter_info, &out_buf_len) == NO_ERROR)
	{
		PIP_ADAPTER_INFO adapter = adapter_info;
		for (; adapter != NULL; adapter = adapter->Next)
		{
			// 确保是以太网
			if (adapter->Type != MIB_IF_TYPE_ETHERNET)
				continue;
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if (adapter->AddressLength != 6)
				continue;

			std::stringstream ss;
			ss << std::hex << std::uppercase
				<< int(adapter->Address[0]) //<< "-"
				<< int(adapter->Address[1]) //<< "-"
				<< int(adapter->Address[2]) //<< "-"
				<< int(adapter->Address[3]) //<< "-"
				<< int(adapter->Address[4]) //<< "-"
				<< int(adapter->Address[5]);

			mac = ss.str();
			ret = true;
			break;
		}
	}

	free(adapter_info);
	return ret;
}

bool Environ::RaisePrivileges()
{
	BOOST_LOG_FUNCTION();
	HANDLE TokenHandle;
	TOKEN_PRIVILEGES t_privileges = { 0 };

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))
	{
		LOG_ERROR(logger) << "OpenProcessToken:" << GetLastError();
		return false;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &t_privileges.Privileges[0].Luid))
	{
		LOG_ERROR(logger) << "LookupPrivilegeValue:" << GetLastError();
		return true;
	}

	t_privileges.PrivilegeCount = 1;
	t_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(TokenHandle, false, &t_privileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		LOG_ERROR(logger) << "AdjustTokenPrivileges:" << GetLastError();
		CloseHandle(TokenHandle);
		return false;
	}
	else
	{
		return true;
	}
}

void Environ::SetIECoreVersion()
{
	BOOST_LOG_FUNCTION();
	REGSAM samDesiredOpen = KEY_ALL_ACCESS;
	TCHAR* path = _T("SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION");
	std::string valueName = _processName + ".exe";
	CA2T lpValueName(valueName.c_str());

	long version = 9999;
	HKEY hKey;
	DWORD dwDisposition;
	long ret = RegOpenKeyEx(HKEY_CURRENT_USER, path, 0, samDesiredOpen, &hKey);	// HKEY_LOCAL_MACHINE	KEY_ALL_ACCESS	REG_LEGAL_OPTION
	if (ret != ERROR_SUCCESS)
	{
		LOG_ERROR(logger) << "RegOpenKeyEx:" << ret;
		ret = RegCreateKeyEx(HKEY_CURRENT_USER, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
		if (ret != ERROR_SUCCESS)
		{
			LOG_ERROR(logger) << "RegCreateKeyEx:" << ret;
			return;
		}
	}

	ret = RegSetValueEx(hKey, lpValueName, NULL, REG_DWORD, (BYTE*)&version, sizeof(version));
	if (ret != ERROR_SUCCESS)
	{
		LOG_ERROR(logger) << "RegSetValueEx:" << ret;
		return;
	}
}

void Environ::getWindowsVersion(std::string &version)
{
	BOOST_LOG_FUNCTION();
	TBYTE buffer[MAX_PATH];
	DWORD cbData = MAX_PATH;
	HKEY hKey;
	DWORD ret;

	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\"), 0, KEY_QUERY_VALUE, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		LOG_ERROR(logger) << "RegOpenKeyEx:" << ret;
		return;
	}

	ret = RegQueryValueEx(hKey, _T("ProductName"), NULL, NULL, (LPBYTE)buffer, &cbData);
	if (ret != ERROR_SUCCESS)
	{
		LOG_ERROR(logger) << "RegQueryValueEx:" << ret;
		return;
	}
	version = CT2A(CString(buffer));
}
