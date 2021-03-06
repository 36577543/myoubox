/******************************************************************************
* 版权所有 (C)2012, Sandongcun开发组
*
* 文件名称：configfile.h
* 内容摘要：配置模块
* 其它说明：
* 当前版本：v1.0
* 作    者：刘坤
* 完成日期：2014.7.28
* 修改记录：
* 1.
* 修改日期：
* 版 本 号：
* 修 改 人：
* 修改内容：
******************************************************************************/
#pragma once

#include "gclass.h"

typedef struct tag_MEM_CONFIG_INFO_STRUCT
{
	// clientId
	char clientId[20];
	// 本地Ip
	char localIp[16];
	// 渠道编号
	char agencyId[40];
	// 上传URL
	char szUploadUrl[512];            
	// 菜单名称
	char szMenuName[40];
}CLIENT_CONFIG_INFO, *LP_CLIENT_CONFIG_INFO, *P_CLIENT_CONFIG_INFO;

class CConfigFile :
	public CSafeSingle<CConfigFile>
{
public:
	// 获取二维码图片
	string GetCodePathNameA(){ return m_strCodePathName; }
	// 获取右边图片1
	string GetRight1PathNameA(){ return m_strRight1PathName; }
	// 获取右边图片2
	string GetRight2PathNameA(){ return m_strRight2PathName; }
	// 获取右边图片3
	string GetRight3PathNameA(){ return m_strRight3PathName; }
	// 获取登录背景图片
	string GetLoginBgPathNameA(){ return m_strLoginBgPathName; }
	// 获取游戏库
	string GetGameDbPathNameA(){ return m_strGameDbPathName; }
	// 游戏类型库
	string GetGameTypeDbPathNameA(){ return m_strTypeDbPathName; }
	// 获取排位库
	string GetIndexDbPathNameA(){ return m_strIndexDbPathName; }
	// 获取渠道Id
	string GetAgencyId() { return m_strAgencyId; }
	// 获取ClientId
	string GetClientId() { return m_strClientId; }
	// 获取本地IP
	string GetLocalIp() { return m_strIp; }
	// 获取上传URL
	string GetUploadUrl() { return m_strUploadUrl; }
	// 获取菜单名称
	string GetMenuName(){ return m_strMenuName; }

	// 获取皮肤列表
	bool GetClientSkinNameList(list<wstring> &ltSkin);
	// 获取皮肤列表路径
	string GetSkinPath();
	wstring GetSkinPathW();
	// 增加皮肤
	void AddSkin(const wchar_t *szSkin);
	// 删除皮肤
	void DeleteSkin(const wchar_t *szSkin);
public:
	// 转换为绝对路径
	static void SwitchAbsPath(string &strPath);
public:
	// 数据库主机名（Ip）
	string GetDbIp(){ return m_strHostName; }
	// 数据库用户名
	string GetDbUserName(){ return m_strUserName; }
	//数据库用户密码
	string GetDbUserPass(){ return m_strPassword; }
	// 数据库名
	string GetDbName(){ return m_strDbName; }

	// 网吧服务监听端口
	int GetServerUdpPort(){ return atoi(m_strServerUdpPort.c_str()); }
	string GetServerUdpPortStr(){ return m_strServerUdpPort; }
	//// 网吧服务监听端口
	//int GetServerTcpPort(){ return atoi(m_strServerTcpPort.c_str()); }
	//string GetServerTcpPortStr(){ return m_strServerTcpPort; }
	//// 注册接口URL
	//string GetRegeditAddr(){ return m_strRegeditAddr; }
	// 服务端版本
	string GetServerVer(){ return m_strServerVer; }
	void SetServerVer(string &strServerVer){ m_strServerVer = strServerVer; }
	// 客户端版本
	string GetClientVer(){ return m_strClientVer; }
	void SetClientVer(string &strClientVer){ m_strClientVer = strClientVer; }

	// 获取图片文件路径
	string GetImageFilePath(){ return m_strImagePath; }
	// 加载升级配置文件
	void LoadUpdateConfig(const string &strUpdateFileName);

	// 获取本地UDP监听端口
	word GetLocalUDPListenPort(){ return m_nLocalUdpPort; }
	// 获取机器名
	string GetLocalMachineName(){ return m_strMachineName; }
private:
	void	LoadConfig();

	// 生成全路径名
	std::string CreateFullPathName(std::string strFileName);
	std::wstring CreateFullPathNameW(std::wstring strFileName);
	// 取MAC,Ip
	void GetSysMacAndIp();
	// 取硬盘编号
	void GetSysDiskNum();
private:
	// 网吧服务监听端口
	string m_strServerUdpPort;
	string m_strServerTcpPort;
private:
	// 皮肤路径列表
	list<wstring> m_ltSkin;
	// 上传URL
	string m_strUploadUrl;
	// 菜单名称
	string m_strMenuName;
	// ClientID
	string m_strClientId;
	// Ip地址
	string m_strIp;
	// 渠道Id
	string  m_strAgencyId;
	// 数据库主机名（Ip）
	string m_strHostName;
	// 数据库用户名
	string m_strUserName;
	//数据库用户密码
	string m_strPassword;
	// 数据库名
	string m_strDbName;

	//// MAC地址
	//string	m_strMac; 
	//// Ip地址
	//string m_strIp;
	// 硬盘编号
	string	m_strDiskNum; 
	// 服务端版本
	string  m_strServerVer;
	// 客户端版本
	string  m_strClientVer;
	// 图片文件路径
	string  m_strImagePath; 
	// 本地UDP监听端口
	word m_nLocalUdpPort;
	// 机器名
	string m_strMachineName; 
	// 获取二维码图片
	string m_strCodePathName; 
	// 获取右边图片1
	string m_strRight1PathName; 
	// 获取右边图片2
	string m_strRight2PathName; 
	// 获取右边图片3
	string m_strRight3PathName; 
	// 获取登录背景图片
	string m_strLoginBgPathName; 
	// 游戏库
	string m_strGameDbPathName;
	// 游戏类型库
	string m_strTypeDbPathName;
	// 排位库
	string m_strIndexDbPathName;
	// 保存皮肤
	void SaveSkin();
private:
	CConfigFile();
	~CConfigFile();
	friend class CSingle<CConfigFile>;
};

