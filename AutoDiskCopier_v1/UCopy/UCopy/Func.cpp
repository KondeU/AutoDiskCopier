//--------------------------------------------------------------------------------------------------------------------------//
//													Windows Application - UCopy												//
//													   		Func.cpp														//
//																	����2013 - 11 - 07										//
//--------------------------------------------------------------------------------------------------------------------------//
#include<Windows.h>
void FileCopy(PTCHAR szStorePath, PTCHAR szResourcePath)
{
	TCHAR	szFileStorePath		[260];	//Ŀ��洢λ��·��	C:\UCYFILE\DIRECTORY\...
	TCHAR	szFileResourcePath	[260];	//Դ�ļ�λ��·��		U:\DIRECTORY\...
	TCHAR	szFileRsrcSrchPath	[260];	//Դ�ļ�λ��·������	U:\*

	lstrcpy(szFileStorePath,	szStorePath		);	//��ȡĿ��洢λ��·��
	lstrcpy(szFileResourcePath,	szResourcePath	);	//��ȡԴ�ļ�λ��·��
	wsprintf(szFileRsrcSrchPath, TEXT("%s%s"), szFileResourcePath, TEXT("*"));	//����Ŀ��洢λ��·������

	HANDLE				hFindFile;
	WIN32_FIND_DATA		wfd;

	TCHAR	szBuffer	[260];

	if( (hFindFile = FindFirstFile(szFileRsrcSrchPath, &wfd)) == INVALID_HANDLE_VALUE )
		return;

	do
	{
		if( ( lstrcmp(wfd.cFileName,TEXT(".")) && lstrcmp(wfd.cFileName, TEXT("..")) ) )
		{
			wsprintf(szBuffer, TEXT("%s%s"), szFileResourcePath, wfd.cFileName);

			if(wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				lstrcat(szBuffer, TEXT("\\"));
				wsprintf(szFileRsrcSrchPath, TEXT("%s"), szBuffer);
				wsprintf(szFileStorePath, TEXT("%s%s\\"), szFileStorePath, wfd.cFileName);
				CreateDirectory(szFileStorePath, NULL);
				FileCopy(szFileStorePath, szFileRsrcSrchPath);
			}
			else
			{
				lstrcat(szFileStorePath, wfd.cFileName);
				CopyFile(szBuffer, szFileStorePath, TRUE);
			}
		}

		lstrcpy(szFileStorePath, szStorePath);
		lstrcpy(szFileResourcePath, szResourcePath);
		wsprintf(szFileRsrcSrchPath, TEXT("%s%s"), szFileResourcePath, TEXT("*"));
	}
	while(FindNextFile(hFindFile, &wfd));
	FindClose(hFindFile);
	return;
}

int FileCheck(PTCHAR szDiskPath, PTCHAR szFileName)//���ĳ��·�����Ƿ����ĳ���ļ������ڷ���-1�������ڷ���0��������1//GetLastErrorֵ
{
	TCHAR szDisk [260];
	TCHAR szFile [260];
	lstrcpy(szDisk, szDiskPath);
	lstrcpy(szFile, szFileName);
	lstrcat(szDisk, TEXT("*"));

	HANDLE				hFindFile;
	WIN32_FIND_DATA		wfd;

	if( (hFindFile = FindFirstFile(szDisk, &wfd)) == INVALID_HANDLE_VALUE )
		return 1;

	do
	{
		if( ! lstrcmp(wfd.cFileName, szFile) )
			return -1;
	}
	while(FindNextFile(hFindFile, &wfd));
	return 0;
}