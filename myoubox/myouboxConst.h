#pragma once
#include "..\..\..\common\LKComponent\GDI\LKConst.h" 


// 帮助图标
#define LKIMAGEBUTTONINDEX_ICONHELP			LKPROJECT_INDEX_BEGIN + 0
// 系统设置
#define LKIMAGEBUTTONINDEX_MENUITEM_SYSCONFIG	LKPROJECT_INDEX_BEGIN + 1
// 系统升级
#define LKIMAGEBUTTONINDEX_MENUITEM_UPDATE	    LKPROJECT_INDEX_BEGIN + 2
// 关于
#define LKIMAGEBUTTONINDEX_MENUITEM_ABOUT	    LKPROJECT_INDEX_BEGIN + 3
// 退出 
#define LKIMAGEBUTTONINDEX_MENUITEM_EXIT	    LKPROJECT_INDEX_BEGIN + 4
// 打开面板
#define LKIMAGEBUTTONINDEX_OPENMAINPANEL        LKPROJECT_INDEX_BEGIN + 5
// 搜索查找图标
#define LKIMAGEBUTTONINDEX_BUTTON_SEARCH        LKPROJECT_INDEX_BEGIN + 6
// 头部背景
#define LKIMAMYOUBOX_DIALOG_HEAD				LKPROJECT_INDEX_BEGIN + 7
// 头部标签 
#define LKIMAMYOUBOX_DIALOG_HEADLABEL			LKPROJECT_INDEX_BEGIN + 8
// 头部分隔条 
#define LKIMAGELISTINDEX_COMMON_SPLIT			LKPROJECT_INDEX_BEGIN + 9
// 编辑框按钮图片
#define LKIMAGEBUTTONINDEX_EDITBUTTON			LKPROJECT_INDEX_BEGIN + 10
// 索引0
#define LKPROJECTHEADIMAGE_INDEX_0				LKPROJECTHEADIMAGE_INDEX_BEGIN + 0

#define AppLoadAllResourceImageToImageArray(pImgMgr)\
{\
	LoadAllResourceImageToImageArray(pImgMgr)\
	/*项目资源图片*/\
	int i = LKPROJECT_INDEX_BEGIN; \
	/*帮助图标*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_HELP);\
	/*系统设置*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_MENUITEM_SYSCONFIG);\
	/*系统升级*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_MENUITEM_UPDATE);\
	/*关于*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_MENUITEM_LOGOS);\
	/*退出*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_MENUITEM_EXIT);\
	/*打开面板*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_MAINPANEL);\
	/*搜索查找图标*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SEARCH); \
	/*头部背景*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_MYOUBOX_HEAD, CRect(0, 0, 0, 0), 1, false); \
	/*头部标签*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_MYOUBOX_HEADLABEL); \
	/*头部分隔条*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_MYOUBOX_HEADSPLIT, CRect(0, 0, 0, 0), 1, false); \
	/*编辑框按钮图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_MYOUBOX_EDITBUTTON, CRect(0, 0, 0, 0), 3); \
}

// 默认的头像 
// 应用程序消息分类
// 自定义消息
#define MSG_APPLICATION			WM_USER	+ 2000

// 主模块内部消息定义常量
// 锁定窗口消息
#define LOCKDLG_MSG             WM_USER + 100
// 用户状态改变消息
#define USERSTATECHANGE_MSG     WM_USER + 101

// 锁定窗口消息WPARAM参数
// 解锁
#define LOCKDLGMSG_WPARAM_UNLOCK    1

// 用户状态改变消息WPARAM参数
// 上线
#define USERSTATECHANGEMSG_WPARAM_ONLINE    1
// 离线
#define USERSTATECHANGEMSG_WPARAM_OFFLINE   2

// 加载表情消息
#define WPARAM_CFACEMAINDLG_LOADFACE		100
// 加载表情消息完成
#define WPARAM_CFACEMAINDLG_LOADFACEFIN		101
// WEBSERVICE接收到数据通知主线程处理消息
#define WPARAM_CWEBSERVICECLIENT_RECVDATA	200

// facedlg选中表情通知处理消息
#define WPARAM_FACEDLG_SELECTED				300

// 显示指定用户聊天消息
#define WPARAM_CHATRECORDDLG_SHOWUSER		310

// 文件发送消息
// 文件部分发送开始
#define WPARAM_TRANSFILE_SENDBEN			400
// 文件部分发送完成
#define WPARAM_TRANSFILE_SENDPART			401
// 文件部分发送完成
#define WPARAM_TRANSFILE_SENDEND			402
// 文件发送失败
#define WPARAM_TRANSFILE_SENDERR			403

// 文件部分接收开始
#define WPARAM_TRANSFILE_RECVBEN			405
// 文件部分接收完成
#define WPARAM_TRANSFILE_RECVPART			406
// 文件部分接收完成
#define WPARAM_TRANSFILE_RECVEND			407
// 文件接收失败
#define WPARAM_TRANSFILE_RECVERR			408

// 进度条消息
// 发送完成
#define WPARAM_PROGRESSBAR_COMPLETESEND		410
// 接收完成
#define WPARAM_PROGRESSBAR_COMPLETERECV		411
// 中止发送
#define WPARAM_PROGRESSBAR_BREAKSEND		412
// 中止接收
#define WPARAM_PROGRESSBAR_BREAKRECV		413
// 开始发送
#define WPARAM_PROGRESSBAR_BEGINSEND		414
// 开始接收
#define WPARAM_PROGRESSBAR_BEGINRECV		415
// 拒绝接收
#define WPARAM_PROGRESSBAR_REFUSERECV		416
// 另存为
#define WPARAM_PROGRESSBAR_SAVEAS			417
// 发送离线文件
#define WPARAM_PROGRESSBAR_SENDOFFLINE		418
// 发送在线文件通知消息
#define WPARAM_PROGRESSBAR_SENDONLINE		419

// 网页获取模式消息
// 接收成功
#define WPARAM_WEBURLCLIENT_RECVSUCCESS		500


// 在线升级文件接收开始
#define WPARAM_UPDATE_RECVBEGIN			600
// 在线升级文件接收部分成功
#define WPARAM_UPDATE_RECVPART			601
// 在线升级文件接收完成
#define WPARAM_UPDATE_RECVEND			602
// 在线升级文件接收错误
#define WPARAM_UPDATE_RECVERR			603


// 有更新的版本通知
#define WPARAM_HAVE_NEWVERSION			610

// 最新版本(0 – 最新版本；1 – 不是最新版本，但可使用， 2 – 必须更新)
#define LPARAM_HAVE_LASTVERSION			0
// 不是最新版本，但可使用
#define LPARAM_HAVE_NEWVERSION			1
// 必须更新
#define LPARAM_HAVE_MUSTUPDATE			2


// Hessian客户端收到用户资料数据
#define WPARAM_HESSIAN_RECVECONTACT		700
