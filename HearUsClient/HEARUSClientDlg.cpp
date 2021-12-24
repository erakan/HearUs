/**
 * HearUs ~ Announcement device application and client for DM&P's Mity-Mite Module
 * Copyright (c) 2005 Erdinc AKAN (erakan@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **/
// HEARUSClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HEARUSClient.h"
#include "HEARUSClientDlg.h"
#include <winsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHEARUSClientDlg dialog

CHEARUSClientDlg::CHEARUSClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHEARUSClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHEARUSClientDlg)
	m_strDuyuru = _T("");
	m_strFakulte = _T("");
	m_ipModul = _T("");
	m_strOgretmen = _T("");
	m_oleTarih = COleDateTime::GetCurrentTime();
	m_oleSaat = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bWinSockOk = FALSE;
}

void CHEARUSClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHEARUSClientDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Total);
	DDX_Control(pDX, IDC_SURE, m_ctrlSure);
	DDX_Text(pDX, IDC_DUYURU, m_strDuyuru);
	DDX_Text(pDX, IDC_FAKULTE, m_strFakulte);
	DDX_CBString(pDX, IDC_MODUL, m_ipModul);
	DDX_Text(pDX, IDC_OGRETMEN, m_strOgretmen);
	DDX_DateTimeCtrl(pDX, IDC_TARIH, m_oleTarih);
	DDX_DateTimeCtrl(pDX, IDC_SAAT, m_oleSaat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHEARUSClientDlg, CDialog)
	//{{AFX_MSG_MAP(CHEARUSClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GONDER, OnGonder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHEARUSClientDlg message handlers

BOOL CHEARUSClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	WSADATA WSAData;
	if(WSAStartup(0x101,(LPWSADATA)&WSAData)!=0)
	{
		AfxMessageBox("Winsock Yüklenemedi!!!");
	}
	else
		m_bWinSockOk = TRUE;
	m_Total.SetRange(0,100);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHEARUSClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHEARUSClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHEARUSClientDlg::OnGonder() 
{
	CString strPackage;
	UpdateData();
	if(m_ipModul.GetLength() == 0)
	{
		AfxMessageBox("Lütfen modülün adresini giriniz!!");
		return;
	}
	strPackage = "MSG\1""0""\1";
	strPackage += "P\1" + m_strOgretmen + "\1";
	strPackage += "F\1" + m_strFakulte + "\1";
	strPackage += "D\1" + m_oleTarih.Format("%d-%m-%Y ") + m_oleSaat.Format("%H:%M:%S")+ "\1";
	/*strPackage += "T\1";
	switch(m_ctrlSure.GetCurSel())
	{
		case 1:	strPackage += "1800"; break;// 30 Dakika
		case 2:	strPackage += "2700"; break;// 45 Dakika
		case 3:	strPackage += "3600"; break;// 1 saat
		case 4:	strPackage += "86400"; break;// Tüm gün
		default:strPackage += "900"; // 15 Dakika
	}
	strPackage += "\1";*/
	strPackage += "M\1" + m_strDuyuru;
	m_Total.SetPos(10);
	SOCKET s = socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in sai;
	sai.sin_family = AF_INET;
	sai.sin_addr.s_addr = inet_addr(m_ipModul);
	sai.sin_port = htons(1010);
	m_Total.SetPos(20);
	if(connect(s,(LPSOCKADDR)&sai,sizeof(sai))==SOCKET_ERROR)
	{
		CString s;
		s.Format("(%d) bağlantı kurulamadı!",WSAGetLastError());
		AfxMessageBox(s);
		m_Total.SetPos(0);
		return;
	}
	m_Total.SetPos(30);
	strPackage.Replace("ğ","g");
	strPackage.Replace("ü","u");
	strPackage.Replace("ı","i");
	strPackage.Replace("ş","s");
	strPackage.Replace("ö","o");
	m_Total.SetPos(40);
	strPackage.Replace("ç","c");
	strPackage.Replace("Ğ","G");
	strPackage.Replace("Ü","U");
	m_Total.SetPos(50);
	strPackage.Replace("İ","I");
	m_Total.SetPos(60);
	strPackage.Replace("Ş","S");
	strPackage.Replace("Ö","O");
	strPackage.Replace("Ç","C");
	send(s,strPackage, strPackage.GetLength(),0);
	m_Total.SetPos(80);
	closesocket(s);
	m_Total.SetPos(100);
	AfxMessageBox("Mesajınız Gönderildi!");
	m_Total.SetPos(0);
}

void CHEARUSClientDlg::OnCancel() 
{
	if(m_bWinSockOk)
		WSACleanup();
	CDialog::OnCancel();
}
