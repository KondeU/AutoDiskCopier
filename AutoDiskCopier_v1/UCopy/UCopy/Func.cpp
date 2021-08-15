//--------------------------------------------------------------------------------------------------------------------------//
//													Windows Application - UCopy												//
//													   		Func.cpp														//
//																	——2013 - 11 - 07										//
//--------------------------------------------------------------------------------------------------------------------------//
#include<Windows.h>
void FileCopy(PTCHAR szStorePath, PTCHAR szResourcePath)
{
	TCHAR	szFileStorePath		[260];	//目标存储位置路径	C:\UCYFILE\DIRECTORY\...
	TCHAR	szFileResourcePath	[260];	//源文件位置路径		U:\DIRECTORY\...
	TCHAR	szFileRsrcSrchPath	[260];	//源文件位置路径搜索	U:\*

	lstrcpy(szFileStorePath,	szStorePath		);	//获取目标存储位置路径
	lstrcpy(szFileResourcePath,	szResourcePath	);	//获取源文件位置路径
	wsprintf(szFileRsrcSrchPath, TEXT("%s%s"), szFileResourcePath, TEXT("*"));	//设置目标存储位置路径搜索

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

int FileCheck(PTCHAR szDiskPath, PTCHAR szFileName)//检测某个路径下是否存在某个文件，存在反回-1，不存在反回0，出错反回1//GetLastError值
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