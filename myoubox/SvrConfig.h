#pragma once
#include <string>
#include <mutex>
#include <thread>

class SvrConfig
{
public:
	static SvrConfig& getInstance();
	bool init();
	bool load();
	void save();

	std::string _username;
	std::string _password;
	std::string _storeName;
	std::string _mac;
	int64_t _storeID;
	std::string _deviceID;

	std::string _peerAddress;
	std::string _peerPort;
	std::string _file = "SvrConfig.json";
private:
	SvrConfig();
	~SvrConfig();
};
