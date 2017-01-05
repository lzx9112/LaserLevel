// Laser levelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Laser level.h"
#include "Laser levelDlg.h"
#include "math.h"
#include <cstring>
#include <string>
#include <sstream>
#include "cv.h"
#include "highgui.h"

using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PixelSize 0.0053
#define Focus 300

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserlevelDlg dialog

CLaserlevelDlg::CLaserlevelDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLaserlevelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaserlevelDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLaserlevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaserlevelDlg)
	DDX_Control(pDX, IDC_LIST1, m_ctllist);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLaserlevelDlg, CDialog)
//{{AFX_MSG_MAP(CLaserlevelDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_PIC1, OnPic1)
ON_BN_CLICKED(IDC_PIC2, OnPic2)
ON_BN_CLICKED(IDC_PIC3, OnPic3)
ON_BN_CLICKED(IDC_BUTTON2, OnTEST)
ON_BN_CLICKED(IDC_signal, Onsignal)
ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserlevelDlg message handlers

BOOL CLaserlevelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	ShowWindow(SW_SHOWMAXIMIZED);
	DWORD dwStyle = m_ctllist.GetExtendedStyle();          //添加列表框的网格线！！！
	
    dwStyle |= LVS_EX_FULLROWSELECT;            
    dwStyle |= LVS_EX_GRIDLINES;
    m_ctllist.SetExtendedStyle(dwStyle);
    m_ctllist.InsertColumn(0,"相机编号",LVCFMT_CENTER,70); //添加列标题！！！！  这里的80,40,90用以设置列的宽度。！！！LVCFMT_LEFT用来设置对齐方式！！！
    m_ctllist.InsertColumn(1,"工作状态",LVCFMT_CENTER,80);
    m_ctllist.InsertColumn(2,"信号类别",LVCFMT_CENTER,80);
    m_ctllist.InsertColumn(3,"XPOS",LVCFMT_CENTER,80);
    m_ctllist.InsertColumn(4,"YPOS",LVCFMT_CENTER,80);
	m_ctllist.InsertColumn(5,"垂直度",LVCFMT_CENTER,80);
	
	
	for(int i =0;i<11;i++)
	{
		pic[i]=true;
	}
	iWidth = GetSystemMetrics(SM_CXSCREEN); 
	iHeight = GetSystemMetrics(SM_CYSCREEN); 
	space = 7;
	picWidth=iWidth*0.6/4-space;
	picHigth=0.98*(iHeight-4*space)/3;
	RussianWorkers();
	i_test=0;
	SetTimer(1,50,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLaserlevelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLaserlevelDlg::OnPaint() 
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
HCURSOR CLaserlevelDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLaserlevelDlg::OnPic1() 
{
	// TODO: Add your control notification handler code here
	
	if(pic[0]&&PicExpand())
	{
		GetDlgItem(IDC_PIC1)->SetWindowPos(&wndTop,150,150,650,650,SWP_DRAWFRAME);
		pic[0] = false;
	}else
	{
		RussianWorkers();
		pic[0] =true;
	}	
}

void CLaserlevelDlg::OnPic2() 
{
	// TODO: Add your control notification handler code here
	if(pic[1]&&PicExpand())
	{
		GetDlgItem(IDC_PIC2)->SetWindowPos(&wndTop,150,150,650,650,SWP_DRAWFRAME);
		pic[1] = false;
	}else
	{
		RussianWorkers();
		pic[1] =true;
	}
}

void CLaserlevelDlg::OnPic3() 
{
	// TODO: Add your control notification handler code here
	
}



bool CLaserlevelDlg::PicExpand()
{
	bool a = true;
	for(int i =0;i<11;i++)
	{
		a=a&&pic[i];
	}
	return a;
}

void CLaserlevelDlg::RussianWorkers()
{
	GetDlgItem(IDC_PIC1)->SetWindowPos(&wndTop,space,space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC2)->SetWindowPos(&wndTop,picWidth+2*space,space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC3)->SetWindowPos(&wndTop,2*picWidth+3*space,space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC4)->SetWindowPos(&wndTop,3*picWidth+4*space,space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC5)->SetWindowPos(&wndTop,space,picHigth+2*space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC6)->SetWindowPos(&wndTop,picWidth+2*space,picHigth+2*space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC7)->SetWindowPos(&wndTop,2*picWidth+3*space,picHigth+2*space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC8)->SetWindowPos(&wndTop,3*picWidth+4*space,picHigth+2*space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC9)->SetWindowPos(&wndTop,space,2*picHigth+3*space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC10)->SetWindowPos(&wndTop,picWidth+2*space,2*picHigth+3*space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_PIC11)->SetWindowPos(&wndTop,2*picWidth+3*space,2*picHigth+3*space,picWidth,picHigth,SWP_DRAWFRAME);
	GetDlgItem(IDC_LIST1)->SetWindowPos(&wndTop,iWidth*0.6+2*space,space,iWidth*0.28,iHeight*0.4,SWP_DRAWFRAME);	
}



Data CLaserlevelDlg::CrossWire(double **a,int width,int hight)
{
	int i = 0,j = 0;
	Data data;
	//XPos YPos
	double *XArray = new double[width],*XArrayRot1 = new double[width],*XArrayRot2 = new double[width],
		*YArray = new double[hight],*YArrayRot1 = new double[hight],*YArrayRot2 = new double[hight],
		x1=0,y1=0,x2=0,y2=0,x3=0,y3=0,x4=0,y4=0;
	for(i=0;i<hight;i++)
	{
		YArray[i]=0;YArrayRot1[i]=0;YArrayRot2[i]=0;
	}	
	for(i=0;i<width;i++)
	{
		XArray[i]=0;XArrayRot1[i]=0;XArrayRot2[i]=0;
	}
	ThresholdFilterPic(a,width,hight,0.75);
	for(i=0;i<hight;i++)
	{
		for(j=0;j<width;j++)
		{
			XArray[j]+=a[i][j];
			YArray[i]+=a[i][j];
		}
	}
	ThresholdFilterArray(XArray,width,0.75);
	ThresholdFilterArray(YArray,hight,0.75);
	data.XPos = CenterOfGravity(XArray,width);
	data.YPos = CenterOfGravity(YArray,hight);
	if(data.XPos>10&&data.XPos<width-10&&data.YPos>10&&data.YPos<hight-10)
	{
		for(i=0;i<10;i++)
		{
			for(j=0;j<width;j++)
			{
				XArrayRot1[j]+=a[i][j];
				XArrayRot2[j]+=a[hight-i-1][j];
			}
		}
		for(i=0;i<hight;i++)
		{
			for(j=0;j<10;j++)
			{
				YArrayRot1[i]+=a[i][j];
				YArrayRot2[i]+=a[i][width-1-j];
			}
		}
		ThresholdFilterArray(XArrayRot1,width,0.75);
		ThresholdFilterArray(XArrayRot2,width,0.75);
		x1=CenterOfGravity(XArrayRot1,width);
		x3=CenterOfGravity(XArrayRot2,width);
		ThresholdFilterArray(YArrayRot1,hight,0.75);
		ThresholdFilterArray(YArrayRot2,hight,0.75);
		y4=CenterOfGravity(YArrayRot1,hight);
		y2=CenterOfGravity(YArrayRot2,hight);
		data.Rot=atan(1024/(x1-x3))-atan((y2-y4)/1280);
	}
	//delete
	delete []XArray;
	delete []XArrayRot1;
	delete []XArrayRot2;
	delete []YArray;
	delete []YArrayRot1;
	delete []YArrayRot2;
	return data;
}

double CLaserlevelDlg::CenterOfGravity(double *a, int size)
{
	double GraySum = 0,GraySumWeight = 0;
	for(int i=0;i<size;i++)
	{
		GraySumWeight+=i*a[i];
		GraySum+=a[i];
	}
	return GraySumWeight/GraySum;
}

void CLaserlevelDlg::ThresholdFilterPic(double **a,int w,int h,double l)
{
	int width = w,hight = h,i = 0,j = 0;
	double GrayMax = 0;
	for(i=0;i<hight;i++)
	{
		for(j=0;j<width;j++)
		{
			if(a[i][j]>GrayMax)
				GrayMax = a[i][j];
		}
	}
	for(i=0;i<hight;i++)
	{
		for(j=0;j<width;j++)
		{
			if(a[i][j]<l*GrayMax)
				a[i][j]=0;
		}
	}	
}

void CLaserlevelDlg::ThresholdFilterArray(double *a, int size,double l)
{
	int i=0;
	double GrayMax = 0;
	for(i=0;i<size;i++)
	{
		if(a[i]>GrayMax)GrayMax = a[i];
	}
	for(i=0;i<size;i++)
	{
		if(a[i]<l*GrayMax)a[i]=0;
	}
}

Data CLaserlevelDlg::VerticalLine(double **a, int width, int hight)
{
	int i = 0,j = 0;
	Data data;
	double *XArray = new double[width];
	ThresholdFilterPic(a,width,hight,0.75);
	for(i=0;i<width;i++)
	{
		XArray[i]=0;
	}
	for(i=0;i<hight;i++)
	{
		for(j=0;j<width;j++)
		{
			XArray[j]+=a[i][j];
		}
	}
	ThresholdFilterArray(XArray,width,0.75);
	data.XPos=CenterOfGravity(XArray,width);
	delete []XArray;
	return data;
}

Data CLaserlevelDlg::HorizontalLine(double **a, int width, int hight)
{
	int i = 0,j = 0;
	Data data;
	double *YArray = new double[hight];
	ThresholdFilterPic(a,width,hight,0.75);
	for(i=0;i<hight;i++)
	{
		YArray[i]=0;
	}
	for(i=0;i<hight;i++)
	{
		for(j=0;j<width;j++)
		{
			YArray[i]+=a[i][j];
		}
	}
	ThresholdFilterArray(YArray,hight,0.75);
	data.YPos=CenterOfGravity(YArray,hight);
	delete []YArray;
	return data;
}


void CLaserlevelDlg::OnTEST() 
{
	// TODO: Add your control notification handler code here
	int i=0,j=0;
	IplImage* src = cvLoadImage("C:\\Users\\admin\\Desktop\\pic\\221535.BMP",0);
	int width = src->width;
	int height = src->height;
	double **a = new double*[height];
	for( i=0;i<height;i++)
	{
		a[i] = new double[width];
	}
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			a[i][j]=cvGetReal2D(src,i,j);
		}
	}
	Data data = HorizontalLine(a,width,height);
	ldata ldata_show;
	ldata_show.YPos = data.YPos;
	CString str0,str1,str2,str3,str4,str5;
	int index[11]={0};
	for(i=0;i<11;i++)
	{
		ldata_show.cam_num=11-i;
		str0.Format("%d",ldata_show.cam_num);
		str4.Format("%.2f",ldata_show.YPos);
		index[i] = m_ctllist.InsertItem(0,(LPCTSTR)str0);//插入行
		m_ctllist.SetItemText(index[i], 1, "正常");
		m_ctllist.SetItemText(index[i], 2, "水平激光线");
		m_ctllist.SetItemText(index[i], 3, "NULL");
		m_ctllist.SetItemText(index[i], 4, (LPCTSTR)str4);
		m_ctllist.SetItemText(index[i], 5, "NULL");
	}
	ldata a1[11];
	a1[0].cam_num=1;
	//delete
	cvReleaseImage(&src);
	for(i=0;i<height;i++)
	{
		delete []a[i];
	}
	delete []a;
	/*
	str.Format("%d",++i_test);
	m_ctllist.InsertItem(0,(LPCTSTR)str);
	
	  int **a = new int*[1024];
	  for(int i=0;i<1024;i++)
	  {
	  a[i] = new int[1280];
	  }
	  //delete
	  for(i=0;i<1024;i++)
	  {
	  delete []a[i];
	  }
	  delete []a;
	*/
	
	
}

void CLaserlevelDlg::Onsignal() 
{
	// TODO: Add your control notification handler code here
	IplImage* src = cvLoadImage("C:\\Users\\admin\\Desktop\\pic\\221535.BMP",0);
	CvvImage img;
	img.CopyOf(src,src->nChannels);
	for(int i=0;i<11;i++)
	{	
		if(pic[i])
		{
			CDC *pDC=GetDlgItem(IDC_PIC1+i)->GetDC();
			HDC hdc = pDC->GetSafeHdc();
			CRect rect;
			GetDlgItem(IDC_PIC1+i)->GetClientRect(&rect);	
			img.DrawToHDC(hdc,rect);
			ReleaseDC(pDC);	
		}
	}
	for( i=0;i<11;i++)
	{	
		if(!pic[i])
		{
			CDC *pDC=GetDlgItem(IDC_PIC1+i)->GetDC();
			HDC hdc = pDC->GetSafeHdc();
			CRect rect;
			GetDlgItem(IDC_PIC1+i)->GetClientRect(&rect);	
			img.DrawToHDC(hdc,rect);
			ReleaseDC(pDC);	
		}
	}
	img.Destroy();
	cvReleaseImage(&src);
}

void CLaserlevelDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		Onsignal();
	}
	CDialog::OnTimer(nIDEvent);
}

bool CLaserlevelDlg::ShowPic(CDC *pDC, CString lpstrFile, int sx, int sy, int nScrWidth, int nScrHeight)
{
	IPicture  *pPic;  
    IStream   *pStm;  
    BOOL      bResult;  
    HANDLE    hFile=NULL;  
    DWORD     dwFileSize,dwByteRead;  
    //打开硬盘中的图形文件  
    hFile=CreateFile(lpstrFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);  
    if (hFile!=INVALID_HANDLE_VALUE)  
    {  
        dwFileSize=GetFileSize(hFile,NULL);//获取文件字节数  
        if(dwFileSize==0xFFFFFFFF)  
        {
            CloseHandle(hFile);
            return FALSE;  
        }
    }  
    else  
    {  
        return FALSE;  
    }  
	
    //分配全局存储空间  
	HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE,dwFileSize);  
    if(hGlobal==NULL)      return FALSE;  
	
	LPVOID  pvData =GlobalLock(hGlobal);  
    if(pvData==NULL)//锁定分配内存块  
	{
        GlobalFree(hGlobal);
        return FALSE;  
    }
    ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//把文件读入内存缓冲区  
	CloseHandle(hFile);//关闭打开的文件  
	
	GlobalUnlock(hGlobal);  
    CreateStreamOnHGlobal(hGlobal,TRUE,&pStm);  
	
	//装入图形文件  
    bResult=OleLoadPicture(pStm,dwFileSize,TRUE,IID_IPicture,(LPVOID*)&pPic);  
	GlobalFree(hGlobal);
	
    if(FAILED(bResult))  return FALSE;  
	
	long   hmWidth;//图片的真实宽度  
	long   hmHeight;//图片的真实高度  
	pPic->get_Width(&hmWidth);  
    pPic->get_Height(&hmHeight);  
	
    //将图形输出到屏幕上（有点像BitBlt）  
    bResult=pPic->Render(pDC->m_hDC,sx,sy,nScrWidth,nScrHeight,0,hmHeight,hmWidth,-hmHeight,NULL);  
    pPic->Release();  
	
    return   TRUE;  
	
}


void CLaserlevelDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	CDialog::OnClose();
}
