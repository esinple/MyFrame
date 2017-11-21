
// ExcelParserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ExcelParser.h"
#include "ExcelParserDlg.h"
#include "afxdialogex.h"
#include <thread>
#include "FilePathMgr.h"
#include "ExcelParseMgr.h"
#include <afxwin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExcelParserDlg 对话框



CExcelParserDlg::CExcelParserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXCELPARSER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExcelParserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExcelParserDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_IN, &CExcelParserDlg::OnBnClickedOpenIn)
	ON_BN_CLICKED(IDC_OPEN_OUT, &CExcelParserDlg::OnBnClickedOpenOut)
	ON_BN_CLICKED(IDC_START, &CExcelParserDlg::OnBnClickedStart)
END_MESSAGE_MAP()


// CExcelParserDlg 消息处理程序

BOOL CExcelParserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemTextA(IDC_PROGRESS_INFO, "0%");
	SetDlgItemTextA(IDC_PROGRESS_INFO2, "");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExcelParserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExcelParserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CExcelParserDlg::OnBnClickedOpenIn()
{
	//TCHAR szPathName[MAX_PATH] = { 0 };
	char szPathName[MAX_PATH] = { 0 };
	BROWSEINFO bInfo = { 0 };
	bInfo.hwndOwner = GetForegroundWindow()->GetSafeHwnd();
	bInfo.lpszTitle = _T("选择目录");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI /*包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的..;*/
		| BIF_UAHINT /*带TIPS提示*/ /*| BIF_NONEWFOLDERBUTTON 不带新建文件夹按钮*/;
	// 关于更多的 ulFlags 参考 http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx;

	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);
	if (nullptr == lpDlist) // 单击了确定按钮;
	{
		return;
		//strFilePath.clear();
		//return false;
	}
	SHGetPathFromIDListA(lpDlist,szPathName);
	FilePathMgr::GetInstance()->SetOpenInPath(szPathName);
	//strFilePath = szPathName;

	SetDlgItemTextA(IDC_PATH_IN, szPathName);
}


void CExcelParserDlg::OnBnClickedOpenOut()
{
	// TODO: 在此添加控件通知处理程序代码
	//TCHAR szPathName[MAX_PATH] = { 0 };
	char szPathName[MAX_PATH] = { 0 };
	BROWSEINFO bInfo = { 0 };
	bInfo.hwndOwner = GetForegroundWindow()->GetSafeHwnd();
	bInfo.lpszTitle = _T("选择目录");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI /*包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的..;*/
		| BIF_UAHINT /*带TIPS提示*/ /*| BIF_NONEWFOLDERBUTTON 不带新建文件夹按钮*/;
	// 关于更多的 ulFlags 参考 http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx;

	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);
	if (nullptr == lpDlist) // 单击了确定按钮;
	{
		return;
		//strFilePath.clear();
		//return false;
	}
	SHGetPathFromIDListA(lpDlist, szPathName);
	FilePathMgr::GetInstance()->SetOpenOutPath(szPathName);

	SetDlgItemTextA(IDC_PATH_OUT, szPathName);
}


void CExcelParserDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//test
	auto pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	if (pProgressCtrl == nullptr)
	{
		return;
	}

	pProgressCtrl->SetRange(0, 10000);
	pProgressCtrl->SetPos(0);

	GetDlgItem(IDC_OPEN_IN)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPEN_OUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	int nAddStep = 3;
	ExcelParseMgr::GetInstance()->ParseExcel();
	/*for (int i = 0; i < 35; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		int nPos = pProgressCtrl->GetPos();
		if (nPos + nAddStep > 100)
		{
			nAddStep -= nPos + nAddStep - 100;
		}
		pProgressCtrl->SetStep(nAddStep);
		pProgressCtrl->StepIt();
	}*/

	GetDlgItem(IDC_OPEN_IN)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPEN_OUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_START)->EnableWindow(TRUE);

}
