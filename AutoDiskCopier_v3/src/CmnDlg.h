#pragma once

#include<Windows.h>

namespace COMMONDIALOG
{
    class COSDlg
    {

    public:

        COSDlg(HWND hOwnerWnd);
        ~COSDlg();

        inline void SetOwnerWindow(HWND hwnd);

        void SetTitle(LPCTSTR szTitle = nullptr);
        void SetFilter(UINT uiExtCount, ...);
        void SetDefExt(const TCHAR szDefExt[4] = nullptr);

        inline const TCHAR * GetFilePath() const;
        inline DWORD GetFilterIndex() const;
        inline WORD GetFileNameOffset() const;
        inline WORD GetFileExtOffset() const;

        inline BOOL CmnDlgOpenFile();
        inline BOOL CmnDlgSaveFile();

    private:

        OPENFILENAME m_ofn;

        TCHAR * m_szFilter;
        TCHAR * m_szTitle;
        TCHAR * m_szDefExt;
        TCHAR m_szFilePath [MAX_PATH];

    };

    class CBrowseFolderDlg
    {

    public:

        CBrowseFolderDlg::CBrowseFolderDlg(HWND hOwnerWnd);

        inline void SetOwnerWindow(HWND hwnd);

        inline bool CmmDlgBrowse();

        inline const TCHAR * GetDir() const;

    private:

        BROWSEINFO m_bi;

        TCHAR m_szSelectDir[MAX_PATH];
        TCHAR m_szTitle[64];

    };



    //COSDlg实现：

    COSDlg::COSDlg(HWND hOwnerWnd)
{
    m_szFilter = nullptr;
    m_szTitle  = nullptr;
    m_szDefExt = nullptr;
    memset(m_szFilePath, 0, sizeof(m_szFilePath));

    //--------------------- 初始化ofn ---------------------//

    memset(&m_ofn, 0, sizeof(OPENFILENAME)); 
    m_ofn.lStructSize = sizeof(OPENFILENAME);

    //---------- 使用的参数 ----------//
    
    m_ofn.hwndOwner      = hOwnerWnd;    //[in]------打开、保存对话框父窗口
    m_ofn.Flags          = 0;            //[in]------功能选择子（在函数中具体设置）
    m_ofn.lpstrTitle     = nullptr;      //[in]------打开、保存对话框窗口标题栏字符串（独立函数设置）
    m_ofn.lpstrFilter    = nullptr;      //[in]------筛选器字符串（独立函数设置）
    m_ofn.nFilterIndex   = 0;            //[in/out]--控件中当前选择的筛选器的索引
    m_ofn.lpstrFile      = m_szFilePath; //[in/out]--控件当前获取的文件及其路径
    m_ofn.nMaxFile       = MAX_PATH;     //[in]------ofn.lpstrFile的最大字符个数
    m_ofn.lpstrDefExt    = nullptr;      //[in]------默认扩展名（独立函数设置）
    m_ofn.nFileOffset    = 0;            //[out]-----从路径开始到文件名的偏移字符个数
    m_ofn.nFileExtension = 0;            //[out]-----从路径开始到文件名的扩展名的偏移字符个数

    //--------- 未启用的参数 ---------//

    m_ofn.hInstance         = NULL;    //[in]---对话框模板对象实例句柄
    m_ofn.lpTemplateName    = nullptr; //[in]---对话框模板资源的名字
    m_ofn.lpstrCustomFilter = nullptr; //[out]--定制筛选器缓冲区指针
    m_ofn.nMaxCustFilter    = 0;       //[in]---ofn.lpstrCustomFilter的最大字符个数
    m_ofn.lpstrTitle        = nullptr; //[out]--控件当前获取的文件的文件名
    m_ofn.nMaxFileTitle     = 0;       //[in]---ofn.lpstrTitle的最大字符个数
    m_ofn.lpstrInitialDir   = nullptr; //[in]---打开、保存对话框的初始目录
    m_ofn.lpfnHook          = nullptr; //[in]---钩子函数
    m_ofn.lCustData         = 0;       //[in]---传递到钩子函数的数据
}

    COSDlg::~COSDlg()
    {
        if (m_szFilter)
        {
            delete [] m_szFilter;
            m_szFilter = nullptr;
        }

        if (m_szTitle)
        {
            delete [] m_szTitle;
            m_szTitle = nullptr;
        }

        if (m_szDefExt)
        {
            delete [] m_szDefExt;
            m_szDefExt = nullptr;
        }
    }

    inline void COSDlg::SetOwnerWindow(HWND hwnd)
    {
        m_ofn.hwndOwner = hwnd;
    }

    void COSDlg::SetTitle(LPCTSTR szTitle)
    {
        if (m_szTitle)
        {
            delete [] m_szTitle;
        }

        if (szTitle)
        {
            m_szTitle = new TCHAR [lstrlen(szTitle) + 1];
            lstrcpy(m_szTitle, szTitle);
        }
        else
        {
            m_szTitle = nullptr;
        }

        m_ofn.lpstrTitle = m_szTitle;
    }

    void COSDlg::SetFilter(UINT uiExtCount, ...)
    {
        if (m_szFilter)
        {
            delete [] m_szFilter;
        }

        if (!uiExtCount)
        {
            m_szFilter = nullptr;
            m_ofn.lpstrFilter = m_szFilter;
            return;
        }

        UINT uiTotalStringLen = 0;
        TCHAR ** pszFilterPart = new TCHAR * [2 * uiExtCount];

        va_list arg_ptr;
        va_start(arg_ptr, uiExtCount);

        for (UINT ui = 0; ui < 2 * uiExtCount; ui++)
        {
            pszFilterPart[ui] = va_arg(arg_ptr, TCHAR *);
            uiTotalStringLen += lstrlen(pszFilterPart[ui]) + 1;
        }
        uiTotalStringLen++;

        va_end(arg_ptr);

        m_szFilter = new TCHAR [uiTotalStringLen];
        memset(m_szFilter, '\0', uiTotalStringLen * sizeof(TCHAR));

        TCHAR * pPos = &m_szFilter[0];
        for (UINT ui = 0; ui < 2 * uiExtCount; ui++)
        {
            lstrcat(pPos, pszFilterPart[ui]);
            pPos += lstrlen(pszFilterPart[ui]) + 1;
        }

        m_ofn.lpstrFilter = m_szFilter;
    }

    void COSDlg::SetDefExt(const TCHAR szDefExt[4])
    {
        if (szDefExt)
        {
            if (!m_szDefExt)
            {
                m_szDefExt = new TCHAR [4];
            }
            lstrcpy(m_szDefExt, szDefExt);
        }
        else
        {
            if (m_szDefExt)
            {
                delete [] m_szDefExt;
                m_szDefExt = nullptr;
            }
        }

        m_ofn.lpstrDefExt = m_szDefExt;
    }

    inline const TCHAR * COSDlg::GetFilePath() const
    {
        return m_szFilePath;
    }

    inline DWORD COSDlg::GetFilterIndex() const
    {
        return m_ofn.nFilterIndex;
    }

    inline WORD COSDlg::GetFileNameOffset() const
    {
        return m_ofn.nFileOffset;
    }

    inline WORD COSDlg::GetFileExtOffset() const
    {
        return m_ofn.nFileExtension;
    }

    inline BOOL COSDlg::CmnDlgOpenFile()
    {
        m_ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
        return GetOpenFileName(&m_ofn);
    }

    inline BOOL COSDlg::CmnDlgSaveFile()
    {
        m_ofn.Flags = OFN_EXPLORER | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;
        return GetSaveFileName(&m_ofn);
    }



    //CBrowseFolderDlg实现：

    CBrowseFolderDlg::CBrowseFolderDlg(HWND hOwnerWnd)
    {
        m_bi.hwndOwner      = hOwnerWnd;
        m_bi.pidlRoot       = NULL;
        m_bi.pszDisplayName = m_szSelectDir;
        m_bi.lpszTitle      = m_szTitle;
        m_bi.ulFlags        = /*BIF_RETURNONLYFSDIRS*/0;

        m_bi.lpfn   = NULL;
        m_bi.lParam = 0;
        m_bi.iImage = 0;

        wsprintf(m_szTitle, TEXT("选择文件夹："));
    }

    inline void CBrowseFolderDlg::SetOwnerWindow(HWND hwnd)
    {
        m_bi.hwndOwner = hwnd;
    }

    inline bool CBrowseFolderDlg::CmmDlgBrowse()
    {
        LPITEMIDLIST pItemIDList = SHBrowseForFolder(&m_bi);
        
        if (!pItemIDList)
        {
            return false;
        }

        if (!SHGetPathFromIDList(pItemIDList, m_szSelectDir))
        {
            //获取文件夹信息失败
            //MessageBox(NULL, TEXT("获取文件夹信息失败！"), TEXT("错误"), MB_ICONERROR);
        }

        //使用IMalloc接口防止内存泄漏
        IMalloc * pMalloc;
        if (NOERROR != SHGetMalloc(&pMalloc))
        {
            //未返回有效的IMalloc接口指针,无法取得外壳程序的IMalloc接口
            //MessageBox(NULL, TEXT("释放接口指针失败！"), TEXT("错误"), MB_ICONERROR);
        }

        pMalloc->Free(pItemIDList);

        if (pMalloc)
        {
            pMalloc->Release();
        }

        return true;
    }

    inline const TCHAR * CBrowseFolderDlg::GetDir() const
    {
        return m_szSelectDir;
    }
}
