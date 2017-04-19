#pragma once
#include <string>

class Environ
{
public:
	Environ();
	~Environ();
	static void init();

	static std::string _mac;
	static std::string _ip;
	static std::string _pcname;
	static std::string _osVersion;
	static int _osType;

	static std::string _processName;
	static std::string _processPath;
	static void getProcessNamePath(std::string &processName, std::string &processPath);

	static void getMac(std::string &mac);
	static void getIpPcname(std::string &ip, std::string &pcname);
	static void getWindowsVersion(std::string &version);
	static bool GetMacAddressByAdaptersInfo(std::string &mac);
	static bool GetMacAddressByAdaptersAddresses(std::string &mac);

	static bool RaisePrivileges();
	static void SetIECoreVersion();
};

std::string gb2312ToUTF8(const std::string &str);
std::string utf8ToGB2312(const std::string &str);
