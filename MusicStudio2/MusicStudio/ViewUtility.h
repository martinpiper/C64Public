#pragma once
#include "afxwin.h"

extern unsigned char GetNumFromEdit(CEdit &edit);
extern void GetNumFromEdit(CEdit &edit,unsigned char &val);
extern void GetNumFromEdit(CEdit &edit,int &val);

extern void SetNumToEdit(CEdit &edit,const unsigned char num);

extern void SetNumToEdit(CEdit &edit,const unsigned char num,LPCTSTR separator,const unsigned char num2);

#define IgnoreAnyUpdates() ScopedUpdate _scopedUpdate(&mIgnoreAnyUpdates);
class ScopedUpdate
{
public:
	ScopedUpdate(int *scopeCount) : mScoped(scopeCount)
	{
		mScoped[0]++;
	}

	virtual ~ScopedUpdate()
	{
		mScoped[0]--;
		
		ASSERT(mScoped >= 0);
	}
private:
	int *mScoped;
};

#define IGNOREANYUPDATES() \
	if (mIgnoreAnyUpdates) return;
