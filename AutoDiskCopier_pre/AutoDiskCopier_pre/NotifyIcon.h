#pragma once

#include<Windows.h>
#include<CommCtrl.h>

namespace NOTIFYICON
{
	
	class CNotifyIcon
	{

	public:

		CNotifyIcon(HWND hOwnerWnd, UINT uiID, UINT uiCallbackMessage, bool bHighVersion = false);

		inline void SetValidParamFlags(UINT uiFlags);

		inline void SetOwnerWindow(HWND hwnd);
		inline void SetID(UINT uiID);
		inline void SetCallbackMessageCode(UINT uiCallbackMessage);
		inline void SetIcon(HINSTANCE hInst, PCTSTR szIconResourceName);
		inline void SetTip(PCTSTR szTip);

		inline void SetBalloonTimeout(UINT uiTimeoutMS);
		inline void SetBalloonTitle(PCTSTR szTitle = nullptr);
		inline void SetBalloonInfo(PCTSTR szInfo = nullptr);
		inline void SetBalloonIcon(DWORD dwFlags, HINSTANCE hInst = NULL, PCTSTR szIconResourceName = nullptr);
		inline void SetBalloonLargeIcon(HINSTANCE hInst = NULL, PCWSTR szIconResourceName = MAKEINTRESOURCEW(IDI_APPLICATION));
		
		inline BOOL AddNotificationIcon();
		inline BOOL DeleteNotificationIcon();
		inline BOOL ShowBalloon();


	private:

		NOTIFYICONDATA m_nid;

	};


	//CNotifyIcon实现：

	CNotifyIcon::CNotifyIcon(HWND hOwnerWnd, UINT uiID, UINT uiCallbackMessage, bool bHighVersion)
	{
		memset(&m_nid, 0, sizeof(NOTIFYICONDATA));

		m_nid.cbSize = sizeof(NOTIFYICONDATA);

		m_nid.hWnd = hOwnerWnd;
		m_nid.uID = uiID;
		m_nid.uCallbackMessage = uiCallbackMessage;

		m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_nid.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
		wsprintf(m_nid.szTip, TEXT("标准工具提示信息。"));

		if (bHighVersion) //Version 5.0 (Version 6.0: NIIF_NOSOUND)
		{
			m_nid.uFlags |= NIF_INFO;

			//气球提示超时时间（10000ms-30000ms）
			m_nid.uTimeout = 15000;

			//气球提示标题
			wsprintf(m_nid.szInfoTitle, TEXT("气球提示标题"));

			//气球提示内容（是否显示气球和立即去除气球通过m_nid.szInfo的值和NIM_MODIFY设置）
			wsprintf(m_nid.szInfo, TEXT("气球提示内容。"));

			//气球提示框图标（若m_nid.szInfoTitle为空字符串，图标不显示）
			m_nid.dwInfoFlags = NIIF_INFO;
		}
	}

	inline void CNotifyIcon::SetValidParamFlags(UINT uiFlags)
	{
		m_nid.uFlags = uiFlags;
	}

	inline void CNotifyIcon::SetOwnerWindow(HWND hwnd)
	{
		m_nid.hWnd = hwnd;
	}

	inline void CNotifyIcon::SetID(UINT uiID)
	{
		m_nid.uID = uiID;
	}

	inline void CNotifyIcon::SetCallbackMessageCode(UINT uiCallbackMessage)
	{
		m_nid.uCallbackMessage = uiCallbackMessage;
	}

	inline void CNotifyIcon::SetIcon(HINSTANCE hInst, PCTSTR szIconResourceName)
	{
		m_nid.hIcon = LoadIcon(hInst, szIconResourceName);
	}

	inline void CNotifyIcon::SetTip(PCTSTR szTip)
	{
		wsprintf(m_nid.szTip, szTip);
	}

	inline void CNotifyIcon::SetBalloonTimeout(UINT uiTimeoutMS)
	{
		m_nid.uTimeout = uiTimeoutMS;
	}

	inline void CNotifyIcon::SetBalloonTitle(PCTSTR szTitle)
	{
		if (szTitle)
		{
			wsprintf(m_nid.szInfoTitle, szTitle);
		}
		else
		{
			wsprintf(m_nid.szInfoTitle, TEXT(""));
		}
	}

	inline void CNotifyIcon::SetBalloonInfo(PCTSTR szInfo)
	{
		if (szInfo)
		{
			wsprintf(m_nid.szInfo, szInfo);
		}
		else
		{
			wsprintf(m_nid.szInfo, TEXT(""));
		}
	}

	inline void CNotifyIcon::SetBalloonIcon(DWORD dwFlags, HINSTANCE hInst, PCTSTR szIconResourceName)
	{
		m_nid.dwInfoFlags = dwFlags;

		if (dwFlags & NIIF_USER)
		{
			m_nid.hBalloonIcon = LoadIcon(hInst, szIconResourceName);
		}
	}

	inline void CNotifyIcon::SetBalloonLargeIcon(HINSTANCE hInst, PCWSTR szIconResourceName)
	{
		m_nid.dwInfoFlags = NIIF_USER | NIIF_LARGE_ICON;

		LoadIconMetric(hInst, szIconResourceName, LIM_LARGE, &m_nid.hBalloonIcon);
	}

	inline BOOL CNotifyIcon::AddNotificationIcon()
	{
		return Shell_NotifyIcon(NIM_ADD, &m_nid);
	}

	inline BOOL CNotifyIcon::DeleteNotificationIcon()
	{
		return Shell_NotifyIcon(NIM_DELETE, &m_nid);
	}

	inline BOOL CNotifyIcon::ShowBalloon()
	{
		//要求启用高版本
		//更改m_nid的：dwInfoFlags（气泡图标选择子）、szInfoTitle（气泡标题）、szInfo（气泡内容）
		return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	}

}