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
	int64_t _storeID;
	std::string _deviceID;

	std::string _centerAddr;
	std::string _centerPort;
	std::string _file = "SvrConfig.ini";
private:
	SvrConfig();
	~SvrConfig();
};
