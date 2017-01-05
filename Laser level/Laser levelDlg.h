// Laser levelDlg.h : header file
//

#if !defined(AFX_LASERLEVELDLG_H__6AE1926B_2E22_4E3D_8114_ED2B65049A49__INCLUDED_)
#define AFX_LASERLEVELDLG_H__6AE1926B_2E22_4E3D_8114_ED2B65049A49__INCLUDED_

#include "Data.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLaserlevelDlg dialog

class CLaserlevelDlg : public CDialog
{
// Construction
public:
	bool ShowPic(CDC *pDC,CString lpstrFile,int sx,int sy,int nScrWidth, int nScrHeight);
	
	Data HorizontalLine(double **a,int width,int hight);
	Data VerticalLine(double **a,int width,int hight);
	void ThresholdFilterArray(double *a,int size,double l);
	void ThresholdFilterPic(double **a,int width,int hight,double l);
	double CenterOfGravity(double *a,int size);
	Data CrossWire(double **a,int width,int hight);
	void RussianWorkers();
	bool PicExpand ();
	CLaserlevelDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLaserlevelDlg)
	enum { IDD = IDD_LASERLEVEL_DIALOG };
	CListCtrl	m_ctllist;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserlevelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLaserlevelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPic1();
	afx_msg void OnPic2();
	afx_msg void OnPic3();
	afx_msg void OnTEST();
	afx_msg void Onsignal();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	bool pic[11];
private:
	int i_test;
	int iWidth;
	int iHeight;
	int picHigth;
	int picWidth;
	int space;
};

typedef struct list_data
{
	int cam_num;
	bool open_success;
	int signal;
	double XPos;
	double YPos;
	double Rot;
	list_data *next;
}ldata;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASERLEVELDLG_H__6AE1926B_2E22_4E3D_8114_ED2B65049A49__INCLUDED_)
