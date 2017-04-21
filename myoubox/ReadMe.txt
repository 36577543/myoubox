项目属性：调试-工作目录，值改为$(OutDir)


CDlgGameMenu（菜单主界面）：此类为程序主对话框类，从配置文件中读取相关的游戏库（game.txt），排位库(index.txt)，游戏类型库(type.txt)的路径并加载解析，在界面上展示。

CDlgLockScreen(锁屏类)：此类为程序运行后的第一个界面。锁屏和解锁作用。

CDlgAdminLogin(解锁类)：管理员登录解锁窗口

CConfigFile(本地配置类)：此类为本地配置类，目前只用到锁屏背景配置，二维码配置，游戏库，排位库，游戏类型库的配置。

CCenterInterface(网络传输模块接口类)：此类为接口类，配合Communication.dll模块可以发送接收网络数据，暂未用到。

CLocalDataInfo(数据存储类)：此类为数据存储类，游戏库，排位库，游戏类型库相关数据的内存存储，为界面展示提供数据支持。

CTrayMainMenu(菜单类)：此类为右下角弹出菜单类。

CChangeSkinDlg(改变皮肤类)：此类接受用户选择背景，尚未用上。


其他注释:

CLK***(自绘控件类)：此类为自绘控件类，大部分基于MFC的CWnd的派生类。

CLog***(日志类)：此类为日志类，不同的类名会生成不同文件名的日志。

CDyn***（动态窗口类）此类为动态窗口类，目的是不用资源创建窗口。用于菜单中获取焦点的焦点框。

微信充值：http://www.hf-vr.cn/open/recharge/view
注册登录：http://hf-vr.cn/open/game/login?deviceId=50E5493D5D78
www.bundjr.com/vr