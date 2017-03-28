#include "ConfigFile.h"
#include "gfunction.h"

//#include "diskseri.h"
//#include <atlutil.h>
//#include <iphlpapi.h>
//#pragma comment(lib, "IPHLPAPI.lib")
//#pragma comment(lib, "ws2_32")

// 服务器端配置文件名
#define CONFIG_FILENAME					"config.ini"
// 本地配置
#define CONFIG_LOCALAPP					"local"
// 本地配置
#define CONFIG_SKIN						"skin"
// 取出控制端版本
#define CONFIG_LOCAL_CONTROLVER			"1.0.0.1"
// 取出客户端版本
#define CONFIG_LOCAL_CLIENTVER			"1.0.0.1"
// tcp服务监听端口
#define CONFIG_LOCAL_TCPLISTENKEY		"tcpport"
// udp服务监听端口
#define CONFIG_LOCAL_UDPLISTENKEY		"udpport"
// 图片文件路径
#define CONFIG_LOCAL_IMAGEPATH		    "imagepath"
// 本地UDP监听端口
#define CONFIG_LOCAL_LOCALUDPLISTENPORT "udplistenport"
// 机器编号
#define CONFIG_LOCAL_MACHINENAME		"machinename"
//// 网吧注册号
//#define CONFIG_LOCAL_BARIDKEY			"barid"
//// 密码
//#define CONFIG_LOCAL_PASSKEY			"password"
//// 增值目录
//#define CONFIG_LOCAL_ADDVLAUEPATH		"addvaluepath"
//// 广告目录
//#define CONFIG_LOCAL_ADPATH 			"adpath"
//// 小程序目录
//#define CONFIG_LOCAL_MINPROGRAMPATH		"minprogrampath"
//// 升级目录
//#define CONFIG_LOCAL_UPDTAEPROGRAMPATH  "updatepath"
//// 客户端退出密码
//#define CONFIG_CENTER_CLIENTEXITPASSKEY	"clientexitpass"
// 获取二维码图片
#define CONFIG_GAMEMENU_CODEFILE	"code"
// 获取右边图片1
#define CONFIG_GAMEMENU_RIGHT1FILE	"right1"
// 获取右边图片2
#define CONFIG_GAMEMENU_RIGHT2FILE	"right2"
// 获取右边图片3
#define CONFIG_GAMEMENU_RIGHT3FILE	"right3"
// 登录背景图片
#define CONFIG_GAMEMENU_LOGINBGFILE	"loginbg"
// 游戏库
#define CONFIG_GAMEMENU_GAMEFILE	"gamedb"
// 游戏类型库
#define CONFIG_GAMETYPE_INDEXFILE	"typedb"
// 排位库
#define CONFIG_GAMEMENU_INDEXFILE	"indexdb"

// 皮肤个数
#define CONFIG_SKIN_COUNT			"count"
// 皮肤路径
#define CONFIG_SKIN_PATH			"path"

// 服务端版本
const char *g_constServerVer = "1.0.0.0";

template <> CConfigFile *CSingle<CConfigFile>::m_pInstance = 0;

CConfigFile::CConfigFile()
{
	LoadConfig();
}


CConfigFile::~CConfigFile()
{
}

std::string GetIpFromUrl(const std::string &strUrl)
{
	std::string strRet;
	return strRet;
}

// 转换为绝对路径
void CConfigFile::SwitchAbsPath(string &strPath)
{
	string s(strPath);
	strPath = CConfigFile::GetInstance()->CreateFullPathName(s);
}

// 获取皮肤列表
bool CConfigFile::GetClientSkinNameList(list<wstring> &ltSkin)
{
	if (m_ltSkin.empty())
	{
		char szTemp[MAX_PATH] = { 0 };
		string strFileName = CreateFullPathName(CONFIG_FILENAME);
		// 获取皮肤个数
		int nCount = GetPrivateProfileIntA(CONFIG_SKIN, CONFIG_SKIN_COUNT, 0, strFileName.c_str());
		string strSkinName = "skin";
		for (int i = 0; i < nCount; i++)
		{
			string strKey(strSkinName);
			strKey += IntToStr(i);
			GetPrivateProfileStringA(CONFIG_SKIN, strKey.c_str(), "", szTemp, MAX_PATH - 1, strFileName.c_str());
			string strValue = szTemp;
			strValue = CreateFullPathName(strValue);
			m_ltSkin.push_back(MultCharToWideCharA(strValue.c_str(), strValue.size()));
		}
	}
	ltSkin = m_ltSkin;
	return true;
}

// 获取皮肤列表路径
string CConfigFile::GetSkinPath()
{
	char szTemp[MAX_PATH] = { 0 };
	string strFileName = CreateFullPathName(CONFIG_FILENAME);
	// 获取皮肤路径
	GetPrivateProfileStringA(CONFIG_SKIN, CONFIG_SKIN_PATH, "\\Skin", szTemp, MAX_PATH - 1, strFileName.c_str());
	string strPath = szTemp;
	return CreateFullPathName(strPath);
}
wstring CConfigFile::GetSkinPathW()
{
	string strPath = GetSkinPath();
	return MultCharToWideCharA(strPath.c_str(), strPath.size());
}

// 增加皮肤
void CConfigFile::AddSkin(const wchar_t *szSkin)
{
	wstring strSkin = szSkin;
	m_ltSkin.push_back(strSkin);
	if (strSkin.empty())
	{
		return;
	}
	// 保存皮肤
	SaveSkin();
}

// 删除皮肤
void CConfigFile::DeleteSkin(const wchar_t *szSkin)
{
	wstring s = szSkin;
	m_ltSkin.remove(s);
	// 保存皮肤
	SaveSkin();
}

// 保存皮肤
void CConfigFile::SaveSkin()
{
	string strFileName = CreateFullPathName(CONFIG_FILENAME);
	::WritePrivateProfileStringA(CONFIG_SKIN, CONFIG_SKIN_COUNT, IntToStr(m_ltSkin.size()).c_str(), strFileName.c_str());
	int i = 0;
	for (auto it : m_ltSkin)
	{
		string strKey = "skin";
		strKey += IntToStr(i++);
		string strSkinA = WideCharToMultiCharW(it.c_str(), it.size());
		::WritePrivateProfileStringA(CONFIG_SKIN, strKey.c_str(), strSkinA.c_str(), strFileName.c_str());
	}
}

void CConfigFile::LoadConfig()
{
	char szTemp[MAX_PATH] = { 0 };
	string strFileName = CreateFullPathName(CONFIG_FILENAME);
	// 获取机器名
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_MACHINENAME, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strMachineName = szTemp;
	if (m_strMachineName.empty())
	{
		// 机器名
		std::string strName;
		char szComputerName[51] = { 0 };
		DWORD dwSize = 50;
		::GetComputerNameA(szComputerName, &dwSize);
		m_strMachineName = szComputerName;
	}

	// 获取本地UDP监听端口
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_LOCALUDPLISTENPORT, "7000", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_nLocalUdpPort = StrToInt(szTemp, 7000);
	// 取出客户端版本
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CLIENTVER, "1.0.0.1", szTemp, MAX_PATH - 1, strFileName.c_str());
	 m_strClientVer = szTemp;
	// 取出tcp服务监听端口
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_TCPLISTENKEY, "7070", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerTcpPort = szTemp;
	// 取出udp服务监听端口
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_UDPLISTENKEY, "7070", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerUdpPort = szTemp;
	// 图片文件路径
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_IMAGEPATH, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strImagePath = szTemp;

	if (m_strImagePath.empty())
	{
		m_strImagePath = "\\image";
		m_strImagePath = CreateFullPathName(m_strImagePath);
	}
	// 取MAC,Ip
	GetSysMacAndIp();
	// 取硬盘编号
	GetSysDiskNum();

	// 获取二维码图片
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_GAMEMENU_CODEFILE, "\\Game\\code.png", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strCodePathName = szTemp;
	if (m_strCodePathName.empty())
	{
		m_strCodePathName = "\\right1path";
		m_strCodePathName = CreateFullPathName(m_strCodePathName);
		m_strCodePathName += "\\right1.jpg";
	}
	else
	{
		m_strCodePathName = CreateFullPathName(m_strCodePathName);
	}

	// 获取右边图片1
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_GAMEMENU_RIGHT1FILE, "\\Game\\right1.jpg", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strRight1PathName = szTemp;
	if (m_strRight1PathName.empty())
	{
		m_strRight1PathName = "\\right1path";
		m_strRight1PathName = CreateFullPathName(m_strRight1PathName);
		m_strRight1PathName += "\\right1.jpg";
	}
	else
	{
		m_strRight1PathName = CreateFullPathName(m_strRight1PathName);
	}

	// 获取右边图片2
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_GAMEMENU_RIGHT1FILE, "\\Game\\right2.jpg", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strRight2PathName = szTemp;
	if (m_strRight2PathName.empty())
	{
		m_strRight2PathName = "\\right2path";
		m_strRight2PathName = CreateFullPathName(m_strRight2PathName);
		m_strRight2PathName += "\\right2.jpg";
	}
	else
	{
		m_strRight2PathName = CreateFullPathName(m_strRight2PathName);
	}

	// 获取右边图片3
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_GAMEMENU_RIGHT3FILE, "\\Game\\right3.jpg", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strRight3PathName = szTemp;
	if (m_strRight3PathName.empty())
	{
		m_strRight3PathName = "\\right3path";
		m_strRight3PathName = CreateFullPathName(m_strRight3PathName);
		m_strRight3PathName += "\\right3.jpg";
	}
	else
	{
		m_strRight3PathName = CreateFullPathName(m_strRight3PathName);
	}

	// 登录背景图片
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_GAMEMENU_LOGINBGFILE, "\\Game\\loginbg.jpg", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strLoginBgPathName = szTemp;

	if (m_strLoginBgPathName.empty())
	{
		m_strLoginBgPathName = "\\loginbgpath";
		m_strLoginBgPathName = CreateFullPathName(m_strLoginBgPathName);
		m_strLoginBgPathName += "\\loginbg.jpg";
	}
	else
	{
		m_strLoginBgPathName = CreateFullPathName(m_strLoginBgPathName);
	}

	// 游戏库
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_GAMEMENU_GAMEFILE, "\\Game\\game.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameDbPathName = szTemp;

	if (m_strGameDbPathName.empty())
	{
		m_strGameDbPathName = "\\dbpath";
		m_strGameDbPathName = CreateFullPathName(m_strGameDbPathName);
		m_strGameDbPathName += "\\gamedbpath.txt";
	}
	else
	{
		m_strGameDbPathName = CreateFullPathName(m_strGameDbPathName);
	}
	// 游戏类型库
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_GAMETYPE_INDEXFILE, "\\Game\\type.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strTypeDbPathName = szTemp;

	if (m_strTypeDbPathName.empty())
	{
		m_strTypeDbPathName = "\\dbpath";
		m_strTypeDbPathName = CreateFullPathName(m_strTypeDbPathName);
		m_strTypeDbPathName += "\\type.txt";
	}
	else
	{
		m_strTypeDbPathName = CreateFullPathName(m_strTypeDbPathName);
	}

	// 排位库
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_GAMEMENU_INDEXFILE, "\\Game\\index.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strIndexDbPathName = szTemp;
	if (m_strIndexDbPathName.empty())
	{
		m_strIndexDbPathName = "\\dbpath";
		m_strIndexDbPathName = CreateFullPathName(m_strIndexDbPathName);
		m_strIndexDbPathName += "\\indexdbpath.txt";
	}
	else
	{
		m_strIndexDbPathName = CreateFullPathName(m_strIndexDbPathName);
	}


	HANDLE h = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"BarPlatform Config Struct Map");
	if (h)
	{
		CLIENT_CONFIG_INFO *pBuf = (CLIENT_CONFIG_INFO *)MapViewOfFile(h, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			512);

		string strTemp;
		if (pBuf)
		{
			//CUserOnlineInfo::GetInstance()->SetUserInfo(pBuf->szUserID, pBuf->szUserName, pBuf->szCertID, pBuf->szCertType);
			//m_bInitUser = true;
			m_strClientId = pBuf->clientId;
			m_strIp = pBuf->localIp;
			m_strAgencyId = pBuf->agencyId;
			m_strUploadUrl = pBuf->szUploadUrl;
			// 菜单名称
			m_strMenuName = pBuf->szMenuName;
			if (m_strMenuName.empty())
			{
				m_strMenuName = "Myoubox游戏菜单";
			}
		}
		CloseHandle(h);
	}

}

// 生成全路径名
std::string CConfigFile::CreateFullPathName(std::string strFileName)
{
	std::string strPath = strFileName;
	if (strPath.find(':') == string::npos)
	{
		// 相对路径
		int nOff(0);
		if (strFileName[0] != '\\' && strFileName[0] != '/')
		{
			nOff = 1;
		}
		char szName[MAX_PATH] = { 0 };
		::GetModuleFileNameA(0, szName, MAX_PATH - 1);
		strPath = szName;
		int nIndex = strPath.rfind('\\');
		if (nIndex < 0) nIndex = strPath.rfind('/');
		strPath = strPath.substr(0, nIndex + nOff);
		strPath += strFileName;
	}
	if (strPath.size() > 0)
	{
		if (strPath[strPath.size() - 1] == '\\' || strPath[strPath.size() - 1] == '/')
		{
			strPath = strPath.substr(0, strPath.size() - 1);
		}
	}
	return strPath;
}
std::wstring CConfigFile::CreateFullPathNameW(std::wstring strFileName)
{
	std::wstring strPath = strFileName;
	if (strPath.find(L':') != 0)
	{
		// 相对路径
		int nOff(0);
		if (strFileName[0] != L'\\' && strFileName[0] != L'/')
		{
			nOff = 1;
		}
		wchar_t szName[MAX_PATH] = { 0 };
		::GetModuleFileName(0, szName, MAX_PATH - 1);
		strPath = szName;
		int nIndex = strPath.rfind(L'\\');
		if (nIndex < 0) nIndex = strPath.rfind(L'/');
		strPath = strPath.substr(0, nIndex + nOff);
		strPath += strFileName;
	}
	if (strPath.size() > 0)
	{
		if (strPath[strPath.size() - 1] == L'\\' || strPath[strPath.size() - 1] == L'/')
		{
			strPath = strPath.substr(0, strPath.size() - 1);
		}
	}
	return strPath;
}

// 取MAC
void CConfigFile::GetSysMacAndIp()
{
}

// 取硬盘编号
void CConfigFile::GetSysDiskNum()
{
}


