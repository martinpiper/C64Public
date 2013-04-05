#include "stdafx.h"
#include "ViewUtility.h"

unsigned char GetNumFromEdit(CEdit &edit)
{
	CString num;
	edit.GetWindowText(num);
	int theNum = 0;
	_stscanf(num,_T("%x"),&theNum);
	return (unsigned char)theNum;
}

void GetNumFromEdit(CEdit &edit,unsigned char &val)
{
	val = GetNumFromEdit(edit);
}

void GetNumFromEdit(CEdit &edit,int &val)
{
	val = GetNumFromEdit(edit);
}

void SetNumToEdit(CEdit &edit,const unsigned char num)
{
	if (!edit.m_hWnd)
	{
		return;
	}
	CString newNum;
	newNum.Format(_T("%02X"),num);
	edit.SetWindowText(newNum);
}

void SetNumToEdit(CEdit &edit,const unsigned char num,LPCTSTR separator,const unsigned char num2)
{
	if (!edit.m_hWnd)
	{
		return;
	}
	CString newNum;
	newNum.Format(_T("%02X %s %02X"),num,separator,num2);
	edit.SetWindowText(newNum);
}
