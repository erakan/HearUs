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
// HEARUSClientDlg.h : header file
//

#if !defined(AFX_HEARUSCLIENTDLG_H__54A3D7E0_303A_4630_91EB_10F3CDC058DC__INCLUDED_)
#define AFX_HEARUSCLIENTDLG_H__54A3D7E0_303A_4630_91EB_10F3CDC058DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CHEARUSClientDlg dialog

class CHEARUSClientDlg : public CDialog
{
// Construction
public:
	CHEARUSClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHEARUSClientDlg)
	enum { IDD = IDD_HEARUSCLIENT_DIALOG };
	CProgressCtrl	m_Total;
	CComboBox	m_ctrlSure;
	CString	m_strDuyuru;
	CString	m_strFakulte;
	CString	m_ipModul;
	CString	m_strOgretmen;
	COleDateTime	m_oleTarih;
	COleDateTime	m_oleSaat;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHEARUSClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bWinSockOk;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHEARUSClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGonder();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEARUSCLIENTDLG_H__54A3D7E0_303A_4630_91EB_10F3CDC058DC__INCLUDED_)
