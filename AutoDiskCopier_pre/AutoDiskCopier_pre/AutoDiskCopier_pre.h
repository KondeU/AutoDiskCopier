#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include<Windows.h>
#include<process.h>
#include<Dbt.h>

#include<Shlwapi.h>
#pragma comment(lib, "shlwapi.lib") 

#include<shlobj.h>
#pragma comment(lib, "shell32.lib")

#include<imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

#include"resource.h"

#include"CmnDlg.h"
#include"NotifyIcon.h"
#include"MD5.h"

using namespace COMMONDIALOG;
using namespace NOTIFYICON;
using namespace MD5;

#define WM_NOTIFYICON (WM_USER + 1)


class CAutoDiskCopier
{
public:

	CMD5             m_md5;
	COSDlg           m_osdlg;
	CBrowseFolderDlg m_bfdlg;
	CNotifyIcon      m_ni;

	char  m_szSettingFilePath[MAX_PATH];

	bool  m_bEnableNotifyIcon;
	bool  m_bMD5Check;
	bool  m_bPopupConfirmDialog;
	bool  m_bAutorun;

	TCHAR m_szCopyFileSavePath[MAX_PATH];

	bool  m_bCheckKeyFile;
	TCHAR m_szKey[MAX_PATH];

	CAutoDiskCopier(HWND hwnd) : m_md5(), m_osdlg(NULL), m_bfdlg(NULL), m_ni(hwnd, IDI_ICON, WM_NOTIFYICON, false)
	{
		SHGetSpecialFolderPathA(hwnd, m_szSettingFilePath, 26/*��C:/Program Files��*/, TRUE);

		sprintf(m_szSettingFilePath, "%s%s", m_szSettingFilePath, "\\AutoDiskCopier\\Setting\\");
		MakeSureDirectoryPathExists(m_szSettingFilePath);
		sprintf(m_szSettingFilePath, "%s%s", m_szSettingFilePath, "ADC_Setting.set");

		 HANDLE hFile = CreateFileA(m_szSettingFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			//�������ļ�ʧ�� -> ��ʼ��

			hFile = CreateFileA(m_szSettingFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == hFile)
			{
				//��ʼ��ʧ�� -> �����˳�

				MessageBox(NULL, TEXT("�����ʼ��ʧ�ܣ�\n���������ļ�ʧ�ܣ�"), TEXT("ADC - ��ʼ������"), MB_ICONERROR);
				PostQuitMessage(GetLastError());
			}
			else
			{
				//��ʼ���ɹ�

				SHGetSpecialFolderPath(hwnd, m_szCopyFileSavePath, 26/*��C:/Program Files��*/, TRUE);
				wsprintf(m_szCopyFileSavePath, TEXT("%s%s"), m_szCopyFileSavePath, TEXT("\\AutoDiskCopier\\CopiedFiles\\"));
				CreateDirectory(m_szCopyFileSavePath, NULL);

				m_bEnableNotifyIcon   = true;
				m_bAutorun            = false;
				m_bPopupConfirmDialog = true;
				m_bMD5Check           = false;

				m_bCheckKeyFile = false;
				wsprintf(m_szKey, TEXT(""));

				DWORD dwWrited = 0;
				WriteFile(hFile, &m_bEnableNotifyIcon,              sizeof(bool),  &dwWrited, NULL);
				WriteFile(hFile, &m_bAutorun,                       sizeof(bool),  &dwWrited, NULL);
				WriteFile(hFile, &m_bPopupConfirmDialog,            sizeof(bool),  &dwWrited, NULL);
				WriteFile(hFile, &m_bMD5Check,                      sizeof(bool),  &dwWrited, NULL);
				WriteFile(hFile, &m_bCheckKeyFile,                  sizeof(bool),  &dwWrited, NULL);
				WriteFile(hFile, m_szCopyFileSavePath,   MAX_PATH * sizeof(TCHAR), &dwWrited, NULL);
				WriteFile(hFile, m_szKey,                MAX_PATH * sizeof(TCHAR), &dwWrited, NULL);	
	
				CloseHandle(hFile);

				MessageBox(NULL, TEXT("�����ʼ����ɡ�"), TEXT("ADC - ��ʼ�����"), MB_OK);
			}
		}
		else
		{
			//�������ļ��ɹ�

			DWORD dwReaded = 0;
			ReadFile(hFile, &m_bEnableNotifyIcon,              sizeof(bool),  &dwReaded, NULL);
			ReadFile(hFile, &m_bAutorun,                       sizeof(bool),  &dwReaded, NULL);
			ReadFile(hFile, &m_bPopupConfirmDialog,            sizeof(bool),  &dwReaded, NULL);
			ReadFile(hFile, &m_bMD5Check,                      sizeof(bool),  &dwReaded, NULL);
			ReadFile(hFile, &m_bCheckKeyFile,                  sizeof(bool),  &dwReaded, NULL);
			ReadFile(hFile, m_szCopyFileSavePath,   MAX_PATH * sizeof(TCHAR), &dwReaded, NULL);
			ReadFile(hFile, m_szKey,                MAX_PATH * sizeof(TCHAR), &dwReaded, NULL);
			CloseHandle(hFile);
		}
	}

	void Copying(PCTSTR szCopyDiskPath)
	{
		if (m_bPopupConfirmDialog)
		{
			if (IDYES != MessageBox(NULL, TEXT("�Ѽ�⵽���̽��룬�Ƿ�ִ�и��ƣ�"), TEXT("ADC - ȷ�ϸ���"), MB_YESNO))
			{
				return;
			}
		}

		/*_beginthread(&(CAutoDiskCopier::CopyingThread), 0, (void *)szCopyDiskPath);*/
		CopyingThread(szCopyDiskPath); //
	}

	void SetDlgInfo(HWND hwnd)
	{
		CheckDlgButton(hwnd, IDC_CHCK_SHOWNOTIFYICON, m_bEnableNotifyIcon   ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(hwnd, IDC_CHCK_MD5,            m_bMD5Check           ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(hwnd, IDC_CHCK_CONFIRMDIALOG, m_bPopupConfirmDialog  ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(hwnd, IDC_CHCK_AUTORUN,        m_bAutorun            ? BST_CHECKED : BST_UNCHECKED);

		SetDlgItemText(hwnd, IDC_EDIT_COPYFILEPATH, m_szCopyFileSavePath);

		CheckDlgButton(hwnd, IDC_CHCK_CHECKKEY, m_bCheckKeyFile ? BST_CHECKED : BST_UNCHECKED);
		SetDlgItemText(hwnd, IDC_EDIT_KEYFILEPATH, m_szKey);
	}

	void GetDlgInfo(HWND hwnd)
	{
		m_bEnableNotifyIcon   = (BST_CHECKED == IsDlgButtonChecked(hwnd, IDC_CHCK_SHOWNOTIFYICON)) ? true : false;
		m_bMD5Check           = (BST_CHECKED == IsDlgButtonChecked(hwnd, IDC_CHCK_MD5))            ? true : false;
		m_bPopupConfirmDialog = (BST_CHECKED == IsDlgButtonChecked(hwnd, IDC_CHCK_CONFIRMDIALOG))  ? true : false;
		m_bAutorun            = (BST_CHECKED == IsDlgButtonChecked(hwnd, IDC_CHCK_AUTORUN))        ? true : false;

		GetDlgItemText(hwnd, IDC_EDIT_COPYFILEPATH, m_szCopyFileSavePath, MAX_PATH);

		m_bCheckKeyFile = (BST_CHECKED == IsDlgButtonChecked(hwnd, IDC_CHCK_CHECKKEY)) ? true : false;
		GetDlgItemText(hwnd, IDC_EDIT_KEYFILEPATH, m_szKey, MAX_PATH);
	}

	void SaveSetting(HWND hwnd)
	{
		HANDLE hFile = CreateFileA(m_szSettingFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(hwnd, TEXT("��������ʧ�ܣ�"), TEXT("ADC - ��������"), MB_ICONERROR);
		}
		else
		{
			DWORD dwWrited = 0;
			WriteFile(hFile, &m_bEnableNotifyIcon,            sizeof(bool),  &dwWrited, NULL);
			WriteFile(hFile, &m_bAutorun,                     sizeof(bool),  &dwWrited, NULL);
			WriteFile(hFile, &m_bPopupConfirmDialog,          sizeof(bool),  &dwWrited, NULL);
			WriteFile(hFile, &m_bMD5Check,                    sizeof(bool),  &dwWrited, NULL);
			WriteFile(hFile, &m_bCheckKeyFile,                sizeof(bool),  &dwWrited, NULL);
			WriteFile(hFile, m_szCopyFileSavePath, MAX_PATH * sizeof(TCHAR), &dwWrited, NULL);
			WriteFile(hFile, m_szKey,              MAX_PATH * sizeof(TCHAR), &dwWrited, NULL);

			CloseHandle(hFile);

			MessageBox(hwnd, TEXT("�������óɹ���"), TEXT("ADC - ��������"), MB_OK);
		}
	}

	void RegisterAutorun()
	{
		TCHAR szFilePath[MAX_PATH];
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		//
		HKEY hReg;
		RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &hReg);
		if (m_bAutorun)
		{
			RegSetValue(hReg, TEXT("AutoDiskCopier_pre"), REG_SZ, szFilePath, (lstrlen(szFilePath) + 1) * sizeof(TCHAR));
		}
		else
		{
			RegDeleteKey(hReg, TEXT("AutoDiskCopier_pre"));
		}
		RegCloseKey(hReg);
	}


private:

	void CopyingThread(PCTSTR szCopyDiskPath)
	{
		if ((m_bCheckKeyFile) &&
			(1 == this->CheckIsKeyFileSame(szCopyDiskPath, m_szKey)))
		{
			//��⣺���ش򿪣����ڲ����и��������Key�ļ�
			return;
		}

		//��õ��Ե��ص�ǰʱ���Խ�������ļ���
		SYSTEMTIME st;
		TCHAR szSavePath[MAX_PATH] = { 0 };
		wsprintf(szSavePath, m_szCopyFileSavePath);
		GetLocalTime(&st);
		wsprintf( szSavePath, TEXT("%s%d��"), szSavePath,   st.wYear   );
		wsprintf( szSavePath, TEXT("%s%d��"), szSavePath,   st.wMonth  );
		wsprintf( szSavePath, TEXT("%s%d��"), szSavePath,   st.wDay    );
		wsprintf( szSavePath, TEXT("%s%dʱ"), szSavePath,   st.wHour   );
		wsprintf( szSavePath, TEXT("%s%d��"), szSavePath,   st.wMinute );
		wsprintf( szSavePath, TEXT("%s%d��\\"), szSavePath, st.wSecond );
		if (!CreateDirectory(szSavePath, NULL))
		{
			SHGetSpecialFolderPath(NULL, szSavePath, 26/*��C:/Program Files��*/, TRUE);
			wsprintf(szSavePath, TEXT("%s%s"), szSavePath, TEXT("\\AutoDiskCopier\\CopiedFiles\\"));
			GetLocalTime(&st);
			wsprintf( szSavePath, TEXT("%s%d��"), szSavePath,   st.wYear   );
			wsprintf( szSavePath, TEXT("%s%d��"), szSavePath,   st.wMonth  );
			wsprintf( szSavePath, TEXT("%s%d��"), szSavePath,   st.wDay    );
			wsprintf( szSavePath, TEXT("%s%dʱ"), szSavePath,   st.wHour   );
			wsprintf( szSavePath, TEXT("%s%d��"), szSavePath,   st.wMinute );
			wsprintf( szSavePath, TEXT("%s%d��\\"), szSavePath, st.wSecond );
			CreateDirectory(szSavePath, NULL);
		}

		//�����ļ�
		Copy(szSavePath, szCopyDiskPath);

		/*_endthread();*/
	}

	int CheckIsKeyFileSame(PCTSTR szCopyDiskPath, PCTSTR szKeyFilePath)
	{
		TCHAR szDiskPath    [260] = { 0 };
		TCHAR szKeyFileName [260] = { 0 };
		wsprintf(szDiskPath, TEXT("%s%s"), szCopyDiskPath, TEXT("*"));
		wsprintf(szKeyFileName, PathFindFileName(szKeyFilePath));

		WIN32_FIND_DATA wfd;
		HANDLE hFindFile = FindFirstFile(szDiskPath, &wfd);

		if (INVALID_HANDLE_VALUE == hFindFile)
		{
			return -1;
		}

		do
		{
			if (!lstrcmp(wfd.cFileName, szKeyFileName))
			{
				TCHAR szCheckFile[MAX_PATH] = { 0 };
				wsprintf(szCheckFile, TEXT("%s%s"), szCopyDiskPath, szKeyFileName);

				HANDLE hFileCheck = CreateFile(szCheckFile,   GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				HANDLE hFileKey   = CreateFile(szKeyFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				if ((INVALID_HANDLE_VALUE == hFileCheck) || (INVALID_HANDLE_VALUE == hFileKey))
				{
					return -1;
				}

				BYTE  byCheck [64] = { 0 };
				BYTE  byKey   [64] = { 0 };
				DWORD dwReaded = 0;

				ReadFile(hFileCheck, byCheck, 64, &dwReaded, NULL);
				ReadFile(hFileKey,   byKey,   64, &dwReaded, NULL);

				for (UINT ui = 0; ui < 64; ui++)
				{
					if (byCheck[ui] != byKey[ui])
					{
						CloseHandle(hFileCheck);
						CloseHandle(hFileKey);

						return 0;
					}
				}

				CloseHandle(hFileCheck);
				CloseHandle(hFileKey);

				return 1;
			}
		} 
		while (FindNextFile(hFindFile, &wfd));
		
		return 0;
	}

	void Copy(PCTSTR szStorePath, PCTSTR szResourcePath)
	{
		TCHAR	szFileStorePath[MAX_PATH];    //Ŀ��洢λ��·��   C:\A.D.C.\CopiedFiles\DIRECTORY\....
		TCHAR	szFileResourcePath[MAX_PATH]; //Դ�ļ�λ��·��     U:\DIRECTORY\....
		TCHAR	szFileRsrcSrchPath[MAX_PATH]; //Դ�ļ�λ��·������ U:\*

		lstrcpy(szFileStorePath, szStorePath);       //��ȡĿ��洢λ��·��
		lstrcpy(szFileResourcePath, szResourcePath); //��ȡԴ�ļ�λ��·��
		wsprintf(szFileRsrcSrchPath, TEXT("%s%s"), szFileResourcePath, TEXT("*")); //����Ŀ��洢λ��·������

		HANDLE hFindFile = NULL;
		WIN32_FIND_DATA wfd;

		TCHAR szBuffer[MAX_PATH];

		if ((hFindFile = FindFirstFile(szFileRsrcSrchPath, &wfd)) == INVALID_HANDLE_VALUE)
		{
			return;
		}

		do
		{
			if ((lstrcmp(wfd.cFileName, TEXT(".")) && lstrcmp(wfd.cFileName, TEXT(".."))))
			{
				wsprintf(szBuffer, TEXT("%s%s"), szFileResourcePath, wfd.cFileName);

				if (wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					wsprintf(szFileRsrcSrchPath, TEXT("%s\\"), szBuffer);
					wsprintf(szFileStorePath, TEXT("%s%s\\"), szFileStorePath, wfd.cFileName);
					CreateDirectory(szFileStorePath, NULL);
					Copy(szFileStorePath, szFileRsrcSrchPath);
				}
				else
				{
					lstrcat(szFileStorePath, wfd.cFileName);

					for (UINT ui = 0; ui < 3; ui++)
					{
						CopyFile(szBuffer, szFileStorePath, FALSE);

						if (m_bMD5Check)
						{
							//MD5�ļ�У�� - �˴���Unicode��Ansi�ĳ�ͻ����Ҫ����BUG
							char szOrgnMD5[33] = { 0 };
							char szDestMD5[33] = { 0 };
							m_md5.CalcMD5(szBuffer, true);
							m_md5.GetMD5(szOrgnMD5);
							m_md5.Reset();
							m_md5.CalcMD5(szFileStorePath, true);
							m_md5.GetMD5(szDestMD5);
							m_md5.Reset();
							if (!strcmp(szOrgnMD5, szDestMD5))
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
			}

			lstrcpy(szFileStorePath, szStorePath);
			lstrcpy(szFileResourcePath, szResourcePath);
			wsprintf(szFileRsrcSrchPath, TEXT("%s%s"), szFileResourcePath, TEXT("*"));
		}
		while (FindNextFile(hFindFile, &wfd));
		
		FindClose(hFindFile);
		
		return;
	}

};