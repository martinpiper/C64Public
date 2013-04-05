// Configuration.cpp : implementation file
//

#include "stdafx.h"
#include "MusicStudio.h"
#include "Configuration.h"
#include "../RESID-FP/trunk/sidplay-2.0.9/src/audio/mmsystem/ExternalSystem.h"


// CConfiguration dialog

IMPLEMENT_DYNAMIC(CConfiguration, CDialog)

CConfiguration::CConfiguration(CWnd* pParent /*=NULL*/)
	: CDialog(CConfiguration::IDD, pParent)
	, mFrequency(0)
	, mViewRefreshFPS(0)
	, mBufferSize(0)
	, mNumBuffers(0)
{

}

CConfiguration::~CConfiguration()
{
}

void CConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mFrequency);
	DDV_MinMaxInt(pDX, mFrequency, 8000, 48000);
	DDX_Text(pDX, IDC_EDIT61, mViewRefreshFPS);
	DDV_MinMaxInt(pDX, mViewRefreshFPS, 5, 50);
	DDX_Text(pDX, IDC_EDIT62, mBufferSize);
	DDV_MinMaxInt(pDX, mBufferSize, 1, 10);
	DDX_Text(pDX, IDC_EDIT63, mNumBuffers);
	DDV_MinMaxInt(pDX, mNumBuffers, 3, 16);
}


BEGIN_MESSAGE_MAP(CConfiguration, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfiguration::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CConfiguration::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CConfiguration::OnBnClickedButton3)
END_MESSAGE_MAP()


// CConfiguration message handlers


void CConfiguration::OnBnClickedButton1()
{
	mFrequency = 22050;
	mViewRefreshFPS = 10;
	mBufferSize = 10;
	mNumBuffers = 5;
	
	UpdateData(FALSE);
}

void CConfiguration::OnBnClickedButton2()
{
	mFrequency = 44100;
	mViewRefreshFPS = 10;
	mBufferSize = 10;
	mNumBuffers = 3;

	UpdateData(FALSE);
}

void CConfiguration::OnBnClickedButton3()
{
	mFrequency = 44100;
	mViewRefreshFPS = 50;
	mBufferSize = 1;
	mNumBuffers = 8;

	UpdateData(FALSE);
}
