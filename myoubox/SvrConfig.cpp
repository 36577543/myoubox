#include "stdafx.h"
#include "SvrConfig.h"
#include "Logger.h"
#include "Environ.h"

#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#include <string>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>   

namespace pt = boost::property_tree;
using std::once_flag;
using std::call_once;
using std::mutex;
using std::unique_lock;

static src::severity_channel_logger_mt<SeverityLevel>& logger = global_logger::get();

SvrConfig::SvrConfig()
{
}

SvrConfig::~SvrConfig()
{
}

SvrConfig& SvrConfig::getInstance()
{
	static once_flag instanceFlag;
	static SvrConfig* pInstance;

	call_once(instanceFlag, []()
	{
		static SvrConfig instance;
		pInstance = &instance;
	});
	return *pInstance;
}

bool SvrConfig::init()
{
	Environ::init();

	return load();
}

bool SvrConfig::load()
{
	try
	{
		pt::ptree tree;
		pt::read_json(_file, tree);

		_peerPort = tree.get<std::string>("peerAddr.port", "19880");
		_peerAddress = tree.get<std::string>("peerAddr.address", "");

		_username = tree.get<std::string>("unlock.username", "");
		_password = tree.get<std::string>("unlock.password", "");

		_storeName = tree.get<std::string>("store.storeName", "");
		_storeID = tree.get<int64_t>("store.storeID", 0);
		_deviceID = tree.get<std::string>("store.deviceID", "");

		return true;
	}
	catch (const boost::exception& ex)
	{
		LOG_ERROR(logger) << boost::diagnostic_information(ex);
	}
	catch (const std::exception& ex)
	{
		LOG_ERROR(logger) << ex.what();
	}
	return false;
}

void SvrConfig::save()
{
	pt::ptree tree;

	tree.put("peerAddr.address", _peerAddress);	// 202.109.114.115	hn.wuyunjk.com
	tree.put("peerAddr.port", _peerPort);

	tree.put("store.storeName", gb2312ToUTF8(_storeName));
	tree.put("store.storeID", _storeID);
	tree.put("store.deviceID", _deviceID);

	tree.put("unlock.username", _username);
	tree.put("unlock.password", _password);

	pt::write_json(_file, tree);
}

bool GetMacAddressByAdaptersAddresses(std::string &mac, int64_t *pID)
{
	bool ret = false;

	ULONG out_buf_len = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buf_len);
	if (addresses == NULL)
		return false;

	if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, addresses, &out_buf_len) == ERROR_BUFFER_OVERFLOW)
	{
		free(addresses);
		addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buf_len);
		if (addresses == NULL)
			return false;
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
			*pID = 0;
			memcpy(pID, curr_addresses->PhysicalAddress, 6);
			ret = true;
			break;
		}
	}

	free(addresses);
	return ret;
}

bool GetMacAddressByAdaptersInfo(std::string &mac, int64_t *pID)
{
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
			*pID = 0;
			memcpy(pID, adapter->Address, 6);
			ret = true;
			break;
		}
	}

	free(adapter_info);
	return ret;
}

