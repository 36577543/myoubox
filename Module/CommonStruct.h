// CommonStruct.h
#ifndef _COMMONSTRUCT_H
#define _COMMONSTRUCT_H
#include <string>
#include "datatypedef.h"

using namespace std;

// PUBWIN�ͻ����û���Ϣ
typedef struct _PUBWINBARCLIENT_INFO
{
	//	������Ϣ
	char btClientStatus;				//	����״̬

	//	�û���Ϣ
	char szUserID[21];					//	��ǰ�û����
	int nUserType;						//	��ǰ�û�����
	char szCardSN[21];					//	��ǰ�û�����
	int nCerOriginType;					//	ʵ������
	char szUserName[21];				//	�û�����
	char szCertType[21];				//	֤������
	char szCertID[21];					//	֤�����
}PUBWINBARCLIENT_INFO;

// �û�������Ϣ
class CUserInfo
{
public:
	//
	string id;
	//int nMachineId;
	string strUserId;
	// �û�����
	string strUserName;
	//������
	string strMachineName;
	// �û��ǳ�
	string strNickName;
	// �û�֤������
	string strUserIDType;
	// �û�֤������
	string strUserIDNum;
	//ͷ��ID
	string strHeadId;
	// �û��Ա�
	string strUserSex;
	// �û�E-mail
	string strUserEmail;
	// �û��绰
	string strUserPhone;
	// �������1
	int64 nMoney1 = 0;
	// �������2
	int64 nMoney2 = 0;
	// ������Ʒ1���ʻ���
	int64 nVirtualItem1 = 0;
	// ������Ʒ2�������
	int64 nVirtualItem2 = 0;
	// �Ƿ��ȡ������������
	int nGetConfig = 0;
};

// {"id":"cb5e90555bc548d9861a163ad8eafde2","broadContent":"��ӭ������è����","broadTime":"13:00","repeatType":"5","ba_1":"1"}

struct YYBBItem
{
	// id
	string id;
	// broadContent
	string broadContent;
	// broadContent2
	string broadContent2;
	// broadTime
	string broadTime;
	// repeatType
	string repeatType;
	// ba_1 ״̬��0ɾ����1�򿪣�2�رա�;
	string ba_1;
	// ba_1 ״̬��0����������1-�ö���2����������;
	string ba_2;
	// �ϴβ���ʱ��(�����Զ�ɨ����Ч)
	int64 llBBTime = 0;
};

// ����������
struct ConfigItem
{
	// ��������URL
	string strYYBBUrl;
	// ��ֹ��������ʱ��(��ʼʱ�� | ����ʱ��)
	string strForbidTime;
	// �Ƿ�������������1 �C ������ 0 �C �رգ�
	string strYYBBSwitch;
	// �Ƿ����ͻ��˺��У�1 �C ������ 0 �C �رգ�
	string strCallServerSwitch;
	// ���ɱ���
	string strSoliderMoney;
	// ���ɶ������
	string strIceSoliderMoney;
	//��������
	string strNetBarName;
	// TGPȫ·����
	string strTGPName;
	// �Ƿ��Զ�����΢����("0" - ���Զ�����΢���£� "1" - �Զ�����΢����)
	string strIsStratMatch;
	// ΢�������·����
	string strMatchName;
	// LOLȫ·����
	string strLOLPathName;
	//// �Ƿ��Զ�����ͬ
	//string strTGPName;
	//// TGPȫ·����
	//string strTGPName;
};

// ��Ļ��
struct CSSItem
{
	// {"id":"c767cfe90a564fce95bf2d35743e417d",
	string id;
	// "barrageContent":"�ƿ�����111",
	string barrageContent;
	// "barrageTime":"2:30",
	string barrageTime;
	// "barrageRange":"",
	string barrageRange;
	// "rangeNumber":"",
	string rangeNumber;
	// "repeatType":"1",
	string repeatType;
	// ba_1 ״̬��0ɾ����1�򿪣�2�رա�;
	string ba_1;
	// �ϴβ���ʱ��(�����Զ�ɨ����Ч)
	int64 llBBTime = 0;
};

// �û���Ϸ��Ϣ
typedef struct UserGameInfoItem
{
	UserGameInfoItem(){}
	UserGameInfoItem(const string &strTierA, const string &strLevelA, 
		const string &strWinA, 
		const string &strAreaIdA, 
		const string &strUserQQIdA, 
		const string strMachineNameA,
		const string &strNickNameA,
		string strStateA = "0")
	{
		strTier = strTierA;
		strLevel = strLevelA;
		strWin = strWinA;
		strAreaId = strAreaIdA;
		strUserQQId = strUserQQIdA;
		strNickName = strNickNameA;
		strMachineName = strMachineNameA;
		strState = strStateA;
	}
	string strTier;
	string strLevel;
	string strWin;
	string strAreaId;
	string strUserQQId;
	string strNickName;
	string strMachineName;
	// 3.��Ϸ״̬��0 - ����Ϸ�� 1 - �˳���Ϸ��2 - ��ʼ��Ϸ��...... �� 
	string strState;
}*PUserGameInfoItem;

// �û���Ϣ
typedef struct UserInfoItem
{
	UserInfoItem(){ bGameInfoValid = false; }
	// �û�IP-�˿�
	NetAddr addr;
	// �û�������Ϣ
	CUserInfo baseInfo;
	// ��Ϸ��Ϣ�Ƿ���Ч
	bool bGameInfoValid;
	// ��Ϸ��Ϣ
	UserGameInfoItem gameInfo;
}*PUserInfoItem;

#endif