//--------------------------------------------------------------------------------------------------------------------------//
//													Windows Application - UCopy												//
//													   			Def.cpp														//
//																	——2013 - 11 - 07										//
//--------------------------------------------------------------------------------------------------------------------------//
#ifndef _DEF_H_
#define _DEF_H_

//宏定义

#define STEPLOG						1
#define STEPSET						2
#define STEPCHGPWD					3
#define STEPHELP					4

#define CTL_ID_LOG_EDIT_PWD			1
#define CTL_ID_LOG_BTN_HIDE			2
#define CTL_ID_LOG_BTN_LOGIN		3
#define CTL_ID_SET_BTN_HELP			4
#define CTL_ID_SET_BTN_SAVE			5
#define CTL_ID_SET_BTN_CHGPWD		6
#define CTL_ID_SET_BTN_HIDE			7
#define CTL_ID_SET_BTN_EXIT			8
#define CTL_ID_SET_EDIT_PATH_FIL	9
#define CTL_ID_SET_BTN_CHOOSE_FTN	10
#define CTL_ID_SET_EDIT_PATH_FTN	11
#define CTL_ID_SET_BTN_CHOOSE_CNCF	12
#define CTL_ID_SET_EDIT_CNCFN		13
#define CTL_ID_SET_BTN_CHOOSE_PPC	14
#define CTL_ID_CHGPWD_BTN_RETURN	15
#define CTL_ID_CHGPWD_BTN_SAVE		16
#define CTL_ID_CHGPWD_EDIT_OLD		17
#define CTL_ID_CHGPWD_EDIT_NEW1		18
#define CTL_ID_CHGPWD_EDIT_NEW2		19
#define CTL_ID_HELP_BTN_RETURN		20
#define CTL_ID_HELP_STATIC_TEXT		21

#define TIMER_ID_HELP				1

#define WM_PROSYSTRAY				(WM_USER + 1)


//函数声明
void	FileCopy(PTCHAR szStorePath, PTCHAR szResourcePath);
int		FileCheck(PTCHAR szDiskPath, PTCHAR szFileName);

#endif