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


	//CNotifyIconʵ�֣�

	CNotifyIcon::CNotifyIcon(HWND hOwnerWnd, UINT uiID, UINT uiCallbackMessage, bool bHighVersion)
	{
		memset(&m_nid, 0, sizeof(NOTIFYICONDATA));

		m_nid.cbSize = sizeof(NOTIFYICONDATA);

		m_nid.hWnd = hOwnerWnd;
		m_nid.uID = uiID;
		m_nid.uCallbackMessage = uiCallbackMessage;

		m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_nid.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
		wsprintf(m_nid.szTip, TEXT("��׼������ʾ��Ϣ��"));

		if (bHighVersion) //Version 5.0 (Version 6.0: NIIF_NOSOUND)
		{
			m_nid.uFlags |= NIF_INFO;

			//������ʾ��ʱʱ�䣨10000ms-30000ms��
			m_nid.uTimeout = 15000;

			//������ʾ����
			wsprintf(m_nid.szInfoTitle, TEXT("������ʾ����"));

			//������ʾ���ݣ��Ƿ���ʾ���������ȥ������ͨ��m_nid.szInfo��ֵ��NIM_MODIFY���ã�
			wsprintf(m_nid.szInfo, TEXT("������ʾ���ݡ�"));

			//������ʾ��ͼ�꣨��m_nid.szInfoTitleΪ���ַ�����ͼ�겻��ʾ��
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
		//Ҫ�����ø߰汾
		//����m_nid�ģ�dwInfoFlags������ͼ��ѡ���ӣ���szInfoTitle�����ݱ��⣩��szInfo���������ݣ�
		return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	}

}