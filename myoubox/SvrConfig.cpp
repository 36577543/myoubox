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
#include <boost/property_tree/ini_parser.hpp>
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
		pt::read_ini(_file, tree);

		_centerAddr = tree.get<std::string>("center.address", "");
		_centerPort = tree.get<std::string>("center.port", "");

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

	tree.put("center.address", _centerAddr);
	tree.put("center.port", _centerPort);

	tree.put("store.storeName", _storeName);
	tree.put("store.storeID", _storeID);
	tree.put("store.deviceID", _deviceID);

	tree.put("unlock.username", _username);
	tree.put("unlock.password", _password);

	pt::write_ini(_file, tree);
}