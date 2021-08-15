//--------------------------------------------------------------------------------------------------------------------------//
//													Windows Application - UCopy												//
//													   		UCopy.cpp														//
//																	����2013 - 11 - 07										//
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
										"        лл����֧�֣�����һ�����԰����������������;Ϊ�Խ������"	
										"�����̽����Զ���ȡ��������ָ�����ļ����У������Ԥ��֧��һЩ��ѡ��չ"	
										"���ܣ����Խ������µ��ļ����������ָ�������У������ⲿ���̸�Ŀ¼�Ƿ�"	
										"�������е�ָ���ļ��Ժ��Կ����˴��̣���ֹ���̲�ɱ����Ŀǰ���������"	
										"�ڲ��Խ׶Σ���ʱ��֧�ֽ������µ��ļ����������ָ�������е���չ���ܣ�"
										"����������ʹ�ò��㣬�����½⡣\n"																		\
										"������\n"															
										"        �κ��˲������á�ת���������Ƿ����飬�����а󶨡����á�����"	
										"���������ַ�������Ȩ������Ϊ��\n\n\n"							
										"�����ˣ�KondeU\n2013-12-08\n���а� V1.0.0.0"
										"\n\n\n\n\n"
									 );

LRESULT CALLBACK WndProc	(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    HWND		hwnd;
    MSG			msg;
    WNDCLASS	wndclass;
	
	//���ڽṹ
	wndclass.hInstance		= hInstance;											//����ʵ�������������
	wndclass.lpszClassName	= TEXT("UCopy-MainWindowsClass");						//����������
	wndclass.lpfnWndProc	= WndProc;												//���ڹ���
	//������ʽ
	wndclass.style			= 0;													//���ڷ��
	wndclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROICON));	//����ͼ��
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);							//�������
	wndclass.lpszMenuName	= NULL;													//���ڲ˵���
	wndclass.hbrBackground	= (hbrBk = CreateSolidBrush(RGB(200,200,200)));			//���ڱ�����ˢ
	//��չ
	wndclass.cbClsExtra		= 0;													//��������չ
	wndclass.cbWndExtra		= 0;													//������չ

	//ע�ᴰ���࣬����Ƿ�ɹ���ע��ʧ�ܳ����˳���
	if(!RegisterClass(&wndclass))
	{
		//����ע��ʧ��
		MessageBox(NULL, TEXT("���򴰿�ע��ʧ�ܣ��������˳���"), TEXT("Ӧ�ó�����ʾ"), MB_ICONERROR);
		return 0;
	}

	//�������򴰿�
	hwnd =
		CreateWindow(
						TEXT("UCopy-MainWindowsClass"),		//����������
						TEXT("Ӧ�ó���"),					//���ڱ���
						WS_OVERLAPPED,						//���ڷ�񣨴��ڸ�ʽ��
						CW_USEDEFAULT, CW_USEDEFAULT,		//���ڳ�ʼ����
						300, 300,							//���ڳ�ʼ�ߴ�
						NULL,								//�����ھ��
						NULL,								//���ڲ˵���� 
						hInstance,							//����ʵ�����
						NULL								//���ڴ�������
					);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	//����ϵͳ�������ã���ʼ��NOTIFYICONDATA�ṹ
	nid.cbSize				= sizeof(NOTIFYICONDATA);
	nid.hWnd				= hwnd;
	nid.uID					= IDI_PROICON;
	nid.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage	= WM_PROSYSTRAY;
	nid.hIcon				= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROICON));
	wsprintf(nid.szTip, TEXT("�����ļ�ʵʱ���"));

	//��Ϣѭ��
    while (GetMessage(&msg, NULL, 0, 0))
    {
		//���������Ϣת��Ϊ�ַ���Ϣ
        TranslateMessage(&msg);
        //����Ϣ���͸����ڹ���
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

//���ڹ���
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
		case WM_CREATE:

			//��ó��򴰿�ʵ�����
			hInst = ((LPCREATESTRUCT)lParam)->hInstance;

			//��ť������ʱ�ı���ɫ�Ļ�ˢ
			hbrBtnSelect = CreateSolidBrush(RGB(215, 215, 215));

			//��ô洢������
			GetSystemWindowsDirectory(szBuffer1, 1024);
			for(iBuffer = 0; szBuffer1[iBuffer] != '\\'; iBuffer++);
			szBuffer1[iBuffer] = '\0';
			//wsprintf(szDataFolder, TEXT("%s%s"), szBuffer1,	   TEXT("\\Program Files\\UCY\\")); //szDataFolder: �����ļ���·��
			//-------------------------------------------------
			wsprintf(szDataFolder, TEXT("%s%s"), szBuffer1,	   TEXT("\\UCY\\")); //szDataFolder: �����ļ���·��				
			wsprintf(szDataPath,   TEXT("%s%s"), szDataFolder, TEXT("data.dat"));				//szDataPath:   �����ļ�·��

			if(INVALID_HANDLE_VALUE == (handleBuffer = CreateFile(szDataPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
			{
				dwBuffer = GetLastError();
				CreateDirectory(szDataFolder, NULL);//-----------------------------------------------------------------------
				handleBuffer = CreateFile(szDataPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if(handleBuffer == INVALID_HANDLE_VALUE)//����
				{
					iBuffer = GetLastError();
					wsprintf(szBuffer2, TEXT("Error:\nGetLastError:CreateFile:Create:data.dat:%d,%d"), dwBuffer, iBuffer);
					MessageBox(hwnd, TEXT(szBuffer2), TEXT("����"), MB_ICONERROR);
					PostQuitMessage(iBuffer);
				}
				else
				{
					MessageBox(hwnd, TEXT("��������һ��ʹ�ø������������ݶ�ʧ��Ӧ�ó��򽫳�ʼ���������ݡ�"), TEXT("��ʾ"), MB_OK);
					
					//��������

					wsprintf(szPassword,		TEXT("%s"),		TEXT("123456")						);
					wsprintf(szFileInLocal,		TEXT("%s%s"),	TEXT(szBuffer1), TEXT("\\UCYFILE")	);
					wsprintf(szFileToNet,		TEXT("%s"),		TEXT(".com")						);
					wsprintf(szCheckFileNoCopy, TEXT("%s"),		TEXT("CheckFileNoCopy.txt")			);
					bFileToNet				= 0;
					bCheckFileNoCopy		= 0;
					bPreventProcessClose	= 0;
					
					//��������ܺ󱣴�
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

					MessageBox(hwnd, TEXT("���ݳ�ʼ������ɣ���ʼ������Ϊ123456��"), TEXT("��ʾ"), MB_OK);
				}

			}
			else
			{
				//�����ļ��򿪳ɹ�����������			
				ReadFile(handleBuffer, szPassword,				17		* sizeof(TCHAR), &dwBuffer, NULL);
				ReadFile(handleBuffer, szFileInLocal,			1024	* sizeof(TCHAR), &dwBuffer, NULL);		
				ReadFile(handleBuffer, &bFileToNet,				1		* sizeof(BOOL),	 &dwBuffer, NULL);		
				ReadFile(handleBuffer, szFileToNet,				1024	* sizeof(TCHAR), &dwBuffer, NULL);	
				ReadFile(handleBuffer, &bCheckFileNoCopy,		1		* sizeof(BOOL),	 &dwBuffer, NULL);
				ReadFile(handleBuffer, szCheckFileNoCopy,		260		* sizeof(TCHAR), &dwBuffer, NULL);		
				ReadFile(handleBuffer, &bPreventProcessClose,	1		* sizeof(BOOL),	 &dwBuffer, NULL);
				CloseHandle(handleBuffer);

				//��������ܺ�������ڴ�
				for(iBuffer = 0; iBuffer < 17; iBuffer++)
					szPassword[iBuffer] += iBuffer;
			}
			
			//�����û����������Ӵ��ڱ༭��ؼ���StepLog��
			hwndLogEditPwd = 
				CreateWindow(
								TEXT("edit"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | WS_BORDER | \
								ES_PASSWORD | ES_LEFT,				//���ڷ�񣨴��ڸ�ʽ��
								85, 
								125,								//���ڳ�ʼ����
								105, 
								16,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_LOG_EDIT_PWD,			//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�������ڡ����ء���ť�Ӵ��ڿؼ���StepLog��
			hwndLogBtnHide = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								215, 
								220,								//���ڳ�ʼ����
								40, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_LOG_BTN_HIDE,			//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�������ڡ����롱��ť�Ӵ��ڿؼ���StepLog��
			hwndLogBtnLogin = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								200, 
								123,								//���ڳ�ʼ����
								40, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_LOG_BTN_LOGIN,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�������ڡ���������ť���Ӵ��ڿؼ�
			hwndSetBtnHelp = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								180, 
								10,									//���ڳ�ʼ����
								40, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_BTN_HELP,			//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�������ڡ����桱��ť���Ӵ��ڿؼ�
			hwndSetBtnSave = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								235, 
								10,									//���ڳ�ʼ����
								40, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_BTN_SAVE,			//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�������ڡ��������롱��ť���Ӵ��ڿؼ�
			hwndSetBtnChgPwd = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								20, 
								235,								//���ڳ�ʼ����
								80, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_BTN_CHGPWD,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//��������Set�С����ء���ť���Ӵ��ڿؼ�
			hwndSetBtnHide = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								180, 
								235,								//���ڳ�ʼ����
								40, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_BTN_HIDE,			//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�������ڡ��˳�����ť���Ӵ��ڿؼ�
			hwndSetBtnExit = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								235, 
								235,								//���ڳ�ʼ����
								40, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_BTN_EXIT,			//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�����ļ����汾��·���Ӵ��ڱ༭��ؼ�
			hwndSetEditPathFIL = 
				CreateWindow(
								TEXT("edit"),						//����������
								szFileInLocal,						//���ڱ���
								WS_CHILD | WS_BORDER | \
								ES_AUTOHSCROLL | ES_LEFT,			//���ڷ�񣨴��ڸ�ʽ��
								75, 
								68,									//���ڳ�ʼ����
								184, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_EDIT_PATH_FIL,	//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�����ļ��������临ѡ��ť�Ӵ��ڿؼ�
			hwndSetBtnChooseFTN = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								10, 
								102,								//���ڳ�ʼ����
								12, 
								12,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_BTN_CHOOSE_FTN,	//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�����ļ����������Ӵ��ڱ༭��ؼ�
			hwndSetEditPathFTN = 
				CreateWindow(
								TEXT("edit"),						//����������
								szFileToNet,						//���ڱ���
								WS_CHILD | WS_BORDER | \
								ES_AUTOHSCROLL | ES_LEFT,			//���ڷ�񣨴��ڸ�ʽ��
								75, 
								118,								//���ڳ�ʼ����
								184, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_EDIT_PATH_FTN,	//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//��������ļ��������Ƹ�ѡ��ť�Ӵ��ڿؼ�
			hwndSetBtnChooseCheckNoCopyFile = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								10, 
								152,								//���ڳ�ʼ����
								12, 
								12,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_BTN_CHOOSE_CNCF,	//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//��������ļ����������ļ����Ӵ��ڱ༭��ؼ�
			hwndSetBtnEditCheckNoCopyFileName = 
				CreateWindow(
								TEXT("edit"),						//����������
								szCheckFileNoCopy,					//���ڱ���
								WS_CHILD | WS_BORDER | \
								ES_AUTOHSCROLL | ES_LEFT,			//���ڷ�񣨴��ڸ�ʽ��
								75, 
								168,								//���ڳ�ʼ����
								184, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_EDIT_CNCFN,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//������ֹ���̹رյĸ�ѡ��ť�Ӵ��ڿؼ�
			hwndSetBtnChoosePreventProcessClose = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								10, 
								202,								//���ڳ�ʼ����
								12, 
								12,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_SET_BTN_CHOOSE_PPC,	//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//��������������淵�ذ�ť�Ӵ��ڿؼ�
			hwndChgPwdBtnReturn = 
					CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								45, 
								210,								//���ڳ�ʼ����
								40, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_CHGPWD_BTN_RETURN,	//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//���������������ȷ�ϸ��İ�ť�Ӵ��ڿؼ�
			hwndChgPwdBtnSave = 
					CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								170, 
								210,								//���ڳ�ʼ����
								80, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_CHGPWD_BTN_SAVE,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//������������������������ı༭��ؼ�
			hwndChgPwdEditOld = 
				CreateWindow(
								TEXT("edit"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | WS_BORDER | \
								ES_PASSWORD | ES_LEFT,				//���ڷ�񣨴��ڸ�ʽ��
								140, 
								50,									//���ڳ�ʼ����
								105, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_CHGPWD_EDIT_OLD,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�������������������������1�ı༭��ؼ�
			hwndChgPwdEditNew1 = 
				CreateWindow(
								TEXT("edit"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | WS_BORDER | \
								ES_PASSWORD | ES_LEFT,				//���ڷ�񣨴��ڸ�ʽ��
								140, 
								100,								//���ڳ�ʼ����
								105, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_CHGPWD_EDIT_NEW1,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�������������������������2�ı༭��ؼ�
			hwndChgPwdEditNew2 = 
				CreateWindow(
								TEXT("edit"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | WS_BORDER | \
								ES_PASSWORD | ES_LEFT,				//���ڷ�񣨴��ڸ�ʽ��
								140, 
								150,								//���ڳ�ʼ����
								105, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_CHGPWD_EDIT_NEW2,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//�����������淵�ذ�ť�Ի��Ӵ��ڰ�ť�ؼ�
			hwndHelpBtnReturn = 
				CreateWindow(
								TEXT("button"),						//����������
								NULL,								//���ڱ���
								WS_CHILD | BS_OWNERDRAW,			//���ڷ�񣨴��ڸ�ʽ��
								20, 
								235,								//���ڳ�ʼ����
								40, 
								20,									//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_HELP_BTN_RETURN,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			//���������ĵ���ʾ��̬�ؼ�
			hwndHelpStaticText = 
				CreateWindow(
								TEXT("static"),						//����������
								szHelpText,							//���ڱ���
								WS_CHILD | SS_LEFT ,				//���ڷ�񣨴��ڸ�ʽ��
								35, 
								30,									//���ڳ�ʼ����
								220, 
								200,								//���ڳ�ʼ�ߴ�
								hwnd,								//�����ھ��
								(HMENU)CTL_ID_HELP_STATIC_TEXT,		//�Ӵ���ID 
								hInst,								//����ʵ�����
								NULL								//���ڴ�������
							);

			iStep = STEPLOG;

			return 0;

		case WM_DRAWITEM:
			//��ð�ť�����Ϣ
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
				//��ʾ�Ի洰���ı�
				case CTL_ID_LOG_BTN_HIDE: 
				case CTL_ID_SET_BTN_HIDE:
					DrawText(hdcBuffer, TEXT("����"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_LOG_BTN_LOGIN:
					DrawText(hdcBuffer, TEXT("����"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_SET_BTN_HELP:
					DrawText(hdcBuffer, TEXT("����"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_SET_BTN_SAVE:
					DrawText(hdcBuffer, TEXT("����"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_SET_BTN_CHGPWD:
					DrawText(hdcBuffer, TEXT("��������"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_SET_BTN_EXIT:
					DrawText(hdcBuffer, TEXT("�˳�"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_CHGPWD_BTN_RETURN:
				case CTL_ID_HELP_BTN_RETURN:
					DrawText(hdcBuffer, TEXT("����"), -1, &rectBuffer2, DT_CENTER);
					break;
				case CTL_ID_CHGPWD_BTN_SAVE:
					DrawText(hdcBuffer, TEXT("ȷ�ϸ���"), -1, &rectBuffer2, DT_CENTER);
					break;

				//��ʾ�Ի渴ѡ���״̬
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
			//���ı���ʾģʽ����Ϊ͸��
			SetBkMode(hdc, TRANSPARENT);
			switch(iStep)
			{
				case STEPLOG:	
					//��ʾ��������Ĵ���
					ShowWindow(hwndLogEditPwd, SW_NORMAL);
					//��ʾ�����ء���ť����
					ShowWindow(hwndLogBtnHide, SW_NORMAL);
					//��ʾ�����롱��ť����
					ShowWindow(hwndLogBtnLogin, SW_NORMAL);
					//��ʾ��ʾ���������ı�
					TextOut(hdc, 35, 125, TEXT("���룺"), lstrlen(TEXT("���룺")));
					//��ʾӦ�ó��������Ϣ
					TextOut(hdc, 25, 225, TEXT("�汾�����а� V 1.0.0.0"), lstrlen(TEXT("�汾�����԰� V 1.0.0.0")));
					break;

				case STEPSET:
					TextOut(hdc, 10, 10,  TEXT("���ã�"),				lstrlen(TEXT("���ã�"))					);
					TextOut(hdc, 10, 50,  TEXT("�ļ����ر���"),			lstrlen(TEXT("�ļ����ر���"))			);
					TextOut(hdc, 30, 70,  TEXT("·����"),				lstrlen(TEXT("·����"))					);
					TextOut(hdc, 25, 100, TEXT("�����ļ���ָ������"),	lstrlen(TEXT("�����ļ���ָ������"))		);
					TextOut(hdc, 30, 120, TEXT("·����"),				lstrlen(TEXT("·����"))					);
					TextOut(hdc, 25, 150, TEXT("����ָ���ļ�������"),	lstrlen(TEXT("����ָ���ļ�������"))		);
					TextOut(hdc, 30, 170, TEXT("�ļ���"),				lstrlen(TEXT("�ļ���"))					);
					TextOut(hdc, 25, 200, TEXT("��ֹ���̹ر�"),			lstrlen(TEXT("��ֹ���̹ر�"))			);
					break;

				case STEPCHGPWD:
					TextOut(hdc, 30, 50,  TEXT("����������룺"), lstrlen(TEXT("����������룺")));
					TextOut(hdc, 30, 100, TEXT("�����������룺"), lstrlen(TEXT("�����������룺")));
					TextOut(hdc, 30, 150, TEXT("��ȷ�������룺"), lstrlen(TEXT("��ȷ�������룺")));
					break;

				case STEPHELP:
					TextOut(hdc, 10, 10, TEXT("�����ĵ�"), lstrlen(TEXT("�����ĵ�")));
					break;
			}
			EndPaint(hwnd, &ps);
			return 0;

		case WM_COMMAND:
			if(HIWORD(wParam) == BN_CLICKED)//���а�����ֻ��������Ϣ
			{
				switch(LOWORD(wParam))//�Ӵ���ID��Ϣ
				{
					//���ذ�ť
					case CTL_ID_LOG_BTN_HIDE:
						ShowWindow(hwnd, SW_HIDE);
						Shell_NotifyIcon(NIM_ADD, &nid);
						break;
					case CTL_ID_SET_BTN_HIDE:
						ShowWindow(hwnd, SW_HIDE);
						Shell_NotifyIcon(NIM_ADD, &nid);
						SetWindowText(hwnd,				TEXT("Ӧ�ó���")	);
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

					//���밴ť
					case CTL_ID_LOG_BTN_LOGIN:
						GetWindowText(hwndLogEditPwd, szBuffer1, 17);
						if(!lstrcmp(szBuffer1, szPassword))
						{
							//������ȷ�ػ洰�ں͸ı�ؼ���ʾ
							SetWindowText(hwnd, TEXT("�Զ�U�̸����� ���а�V1.0.0.0"));
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
							MessageBox(hwnd, TEXT("����������������룡"), TEXT("��ʾ"), MB_OK | MB_ICONWARNING);
						}
						break;
					
					//�����Ի渴ѡ��
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
			
					//������ť
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


					//���水ť
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
							MessageBox(hwnd, TEXT("���ñ���ɹ���"), TEXT("��ʾ"), MB_OK);
						}
						else
						{
							iBuffer = GetLastError();
							wsprintf(szBuffer1, TEXT("���ñ���ʧ�ܣ�\nError:\nGetLastError:CreateFile:Save:data.dat:%d"), iBuffer);
							MessageBox(hwnd, szBuffer1, TEXT("����"), MB_ICONERROR);
						}
						break;


					//�������밴ť
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

					//ȷ�ϸ������밴ť
					case CTL_ID_CHGPWD_BTN_SAVE:
						GetWindowText(hwndChgPwdEditOld, szBuffer1, 17);
						if(lstrcmp(szBuffer1, szPassword))
						{
							MessageBox(hwnd, TEXT("����������������������룡"), TEXT("��ʾ"), MB_ICONWARNING);
							break;
						}
						GetWindowText(hwndChgPwdEditNew1, szBuffer1, 17);
						GetWindowText(hwndChgPwdEditNew2, szBuffer2, 17);
						if(lstrcmp(szBuffer1, szBuffer2))
						{
							MessageBox(hwnd, TEXT("�������������벻��ͬ�����������룡"), TEXT("��ʾ"), MB_ICONWARNING);
							break;
						}
						lstrcpy(szPassword, szBuffer1);
						for(iBuffer = 0; iBuffer < 17; iBuffer++)
							szBuffer1[iBuffer] -= iBuffer;
						if(INVALID_HANDLE_VALUE != (handleBuffer = CreateFile(szDataPath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
						{
							WriteFile(handleBuffer, TEXT(szBuffer1), 17 * sizeof(TCHAR), &dwBuffer, NULL);
							CloseHandle(handleBuffer);
							MessageBox(hwnd, TEXT("������ĳɹ���"), TEXT("��ʾ"), MB_OK);
						}
						else
						{
							iBuffer = GetLastError();
							wsprintf(szBuffer1, TEXT("�������ʧ�ܣ�\nError:\nGetLastError:CreateFile:Save:data.dat:%d"), iBuffer);
							MessageBox(hwnd, szBuffer1, TEXT("����"), MB_ICONERROR);
						}
						break;

					//���ذ�ť
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

					//�˳���ť
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
			SetWindowText(hwndHelpStaticText, szHelpText + iHelpTextPos * 28/*һ��28��char�ַ�*/);
			return 0;
		

		case WM_SYSCOMMAND:
			if(wParam == SC_CLOSE && bPreventProcessClose)
				return 0;
			else
				break;


		//����ϵͳ������Ϣ
		case WM_PROSYSTRAY:
			if(wParam == IDI_PROICON && lParam == WM_RBUTTONDBLCLK)
			{    
				Shell_NotifyIcon(NIM_DELETE, &nid);
				ShowWindow(hwnd, SW_SHOWNORMAL);
				return TRUE;      
			}  
			return FALSE;


		//ʵ���ļ�����
		case WM_DEVICECHANGE:
			//���豸��װ�ɹ��ͷ��߼����̵���Ϣ������
			if(wParam != DBT_DEVICEARRIVAL || ((PDEV_BROADCAST_HDR)lParam)->dbch_devicetype != DBT_DEVTYP_VOLUME)
				return BROADCAST_QUERY_DENY;
			else
			{
				dwBuffer = ((PDEV_BROADCAST_VOLUME)lParam)->dbcv_unitmask;
				for(iBuffer = 0; !(dwBuffer & 1); iBuffer++)
				{
					dwBuffer = dwBuffer >> 1;
				}
				if(iBuffer >= 26)//�����̷�����
					return BROADCAST_QUERY_DENY;
				else
					wsprintf(szBuffer2, TEXT("%c%s"), 'A' + iBuffer, TEXT(":\\"));
			}
			//����Ƿ���ڲ����и���������ļ�
			if( bCheckFileNoCopy && (FileCheck(szBuffer2, szCheckFileNoCopy) == -1) )
				return TRUE;
			//��õ��Ե���ʱ���Խ�������ļ���
			GetLocalTime(&st);
			wsprintf(szBuffer1, TEXT("%s%s"),		szFileInLocal,TEXT("\\"));
			wsprintf(szBuffer1, TEXT("%s%d��"),		szBuffer1, st.wYear		);
			wsprintf(szBuffer1, TEXT("%s%d��"),		szBuffer1, st.wMonth	);
			wsprintf(szBuffer1, TEXT("%s%d��"),		szBuffer1, st.wDay		);
			wsprintf(szBuffer1, TEXT("%s%dʱ"),		szBuffer1, st.wHour		);
			wsprintf(szBuffer1, TEXT("%s%d��"),		szBuffer1, st.wMinute	);
			wsprintf(szBuffer1, TEXT("%s%d��\\"),	szBuffer1, st.wSecond	);
			MakeSureDirectoryPathExists(szBuffer1);	
			//�����ļ�
			FileCopy(szBuffer1, szBuffer2);
			return TRUE;


		case WM_DESTROY:
			if(!bPreventProcessClose)
				PostQuitMessage(0);
			return 0;
    }
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
