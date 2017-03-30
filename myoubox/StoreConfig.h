#pragma once
#include <mutex>
#include <thread>

class StoreConfig
{
public:
	static StoreConfig& getInstance();
	bool load();
	void save();

	std::string _username;
	std::string _password;
	std::string _storeName;
	std::string _storeId;
	std::string _mac;
	int64_t _deviceID;

private:
	StoreConfig();
	~StoreConfig();
};

bool GetMacAddressByAdaptersInfo(std::string &mac, int64_t *pID);
bool GetMacAddressByAdaptersAddresses(std::string &mac, int64_t *pID);
