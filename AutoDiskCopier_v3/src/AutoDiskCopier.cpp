#include"AutoDiskCopier.h"

#pragma comment(linker,                                      \
                "/manifestdependency:\""                     \
                "type='win32' "                              \
                "name='Microsoft.Windows.Common-Controls' "  \
                "version='6.0.0.0' "                         \
                "processorArchitecture='*' "                 \
                "publicKeyToken='6595b64144ccf1df' "         \
                "language='*'\"")

static const TCHAR szAppNameEng[] = TEXT("AutoDiskCopier");
static const TCHAR szAppNameChn[] = TEXT("自动磁盘拷贝器");

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL    CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, INT iCmdShow)
{
    //--- Create a no show window to get other msg. ---//

    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    memset(&hwnd,     0, sizeof(HWND));
    memset(&msg,      0, sizeof(MSG));
    memset(&wndclass, 0, sizeof(WNDCLASS));

    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = szAppNameEng;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("错误：主窗口类注册失败，该应用程序最低版本限制为WindowsNT！"), szAppNameChn, MB_ICONERROR | MB_OK);
        return 0;
    }

    hwnd = CreateWindow(
        szAppNameEng, szAppNameChn, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,
        0, 0, 0, 0, GetDesktopWindow(), NULL, hInstance, NULL);

    //--- Program init. ---//

    //Accel.

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

    //--- Message loop. ---//

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (TranslateAccelerator(hwnd, hAccel, &msg))
        {
            continue;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //--- Program end. ---//

    return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static CAutoDiskCopier adc(hwnd);

    static HWND hDlgMain = NULL;

    switch (message)
    {

    case WM_CREATE:

        hDlgMain = CreateDialogParam(
            ((CREATESTRUCT*)lParam)->hInstance, MAKEINTRESOURCE(IDD_DIALOG),
            hwnd, DlgProc, (LPARAM)(&adc));

        //热键启动（Control + Alt + Shift + U）
        RegisterHotKey(hwnd, 0, MOD_CONTROL | MOD_ALT | MOD_SHIFT, 'U');

        //托盘图标
        adc.m_ni.SetTip(TEXT("AutoDiskCopier v3.0"));
        adc.m_ni.SetIcon(((CREATESTRUCT*)lParam)->hInstance, MAKEINTRESOURCE(IDI_ICON));

        if (adc.m_bEnableNotifyIcon)
        {
            adc.m_ni.AddNotificationIcon();
        }

        return 0;


    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDR_ACCELERATOR_HIDE:
            SendMessage(hDlgMain, WM_COMMAND, wParam, lParam);
        }
        break;

    case WM_HOTKEY:

        ShowWindow(hDlgMain, SW_NORMAL);
        return 0;


    case WM_NOTIFYICON:

        if (wParam == IDI_ICON && lParam == WM_RBUTTONDBLCLK)
        {
            adc.m_ni.DeleteNotificationIcon();
            ShowWindow(hDlgMain, SW_SHOWNORMAL);
        }
        break;


    case WM_DEVICECHANGE:

        if ((DBT_DEVICEARRIVAL != wParam) ||
            (DBT_DEVTYP_VOLUME != ((PDEV_BROADCAST_HDR)lParam)->dbch_devicetype))
        {
            //忽略设备安装不成功或非逻辑磁盘的消息
            return BROADCAST_QUERY_DENY;
        }
        else
        {
            DWORD dwDiskMask = ((PDEV_BROADCAST_VOLUME)lParam)->dbcv_unitmask;
            UINT  uiPos = 0;
            TCHAR szCopyDiskPath[4];

            for (uiPos = 0; !(dwDiskMask & 1); uiPos++)
            {
                dwDiskMask >>= 1;
            }

            if (uiPos >= 26)
            {
                //磁盘盘符超标
                return BROADCAST_QUERY_DENY;
            }
            else
            {
                wsprintf(szCopyDiskPath, TEXT("%c%s"), 'A' + uiPos, TEXT(":\\"));
            }

            adc.Copying(szCopyDiskPath);

            return TRUE;
        }


    case WM_DESTROY:

        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}


BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static CAutoDiskCopier * padc;

    switch (message)
    {
    case WM_INITDIALOG:
        padc = (CAutoDiskCopier *)lParam;
        padc->SetDlgInfo(hDlg);
        padc->m_bfdlg.SetOwnerWindow(hDlg);
        padc->m_osdlg.SetOwnerWindow(hDlg);
        padc->m_osdlg.SetFilter(2, TEXT("签名文件（*.key）"), TEXT("*.key"), TEXT("所有文件（*.*）"), TEXT("*.*"));
        padc->m_osdlg.SetDefExt(TEXT("key"));
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
        case IDC_BTN_HIDE:
        case IDR_ACCELERATOR_HIDE:
            ShowWindow(hDlg, SW_HIDE);
            if (padc->m_bEnableNotifyIcon)
            {
                padc->m_ni.AddNotificationIcon();
            }
            return TRUE;

        case IDC_BTN_QUIT:
            SendMessage(GetParent(hDlg), WM_DESTROY, 0, 0);
            return TRUE;

        case IDC_BTN_SAVESETTING:
            padc->GetDlgInfo(hDlg);
            padc->SaveSetting(hDlg);
            padc->RegisterAutorun();//
            return TRUE;

        case IDC_BTN_BROWSECOPYFILEPATH:
            if (padc->m_bfdlg.CmmDlgBrowse())
            {
                TCHAR szPath[MAX_PATH];
                wsprintf(szPath, TEXT("%s%s"), padc->m_bfdlg.GetDir(), TEXT("\\"));
                SetDlgItemText(hDlg, IDC_EDIT_COPYFILEPATH, szPath);
            }
            return TRUE;

        case IDC_BTN_CREATEKEYFILE:
            if(padc->m_osdlg.CmnDlgSaveFile())
            {
                HANDLE hFile = CreateFile(padc->m_osdlg.GetFilePath(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (INVALID_HANDLE_VALUE == hFile)
                {
                    MessageBox(hDlg, TEXT("生成签名文件失败！"), TEXT("ADC - 签名"), MB_ICONERROR);
                }
                else
                {
                    int  iRand[2];
                    char szMD5[33];
                    for (UINT ui = 0; ui < 2; ui++)
                    {
                        srand(GetTickCount());
                        iRand[ui] = rand();

                        padc->m_md5.CalcMD5((void *)(&iRand[ui]), sizeof(int));
                        padc->m_md5.GetMD5(szMD5);
                        padc->m_md5.Reset();
                    
                        DWORD dwWrited;
                        WriteFile(hFile, szMD5, 32 * sizeof(char), &dwWrited, NULL);
                    }

                    CloseHandle(hFile);
                    MessageBox(hDlg, TEXT("已生成签名文件。"), TEXT("ADC - 签名"), MB_OK);
                }
            }
            return TRUE;

        case IDC_BTN_BROWSEKEYFILEPATH:
            if (padc->m_osdlg.CmnDlgOpenFile())
            {
                SetDlgItemText(hDlg, IDC_EDIT_KEYFILEPATH, padc->m_osdlg.GetFilePath());
            }
            return TRUE;

        }
    }
    return FALSE;
}
