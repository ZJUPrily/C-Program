
// CppTextEditorDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CppTextEditor.h"
#include "CppTextEditorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCppTextEditorDlg 对话框



CCppTextEditorDlg::CCppTextEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CPPTEXTEDITOR_DIALOG, pParent)
	, FilePathSave(_T(""))
	, FilePathOpen(_T(""))
	, StringInput(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCppTextEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_MFCEDITBROWSE1, FilePathIn);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, FilePathSave);
	DDX_Text(pDX, IDC_MFCEDITBROWSE2, FilePathOpen);
	DDX_Text(pDX, IDC_EDIT1, StringInput);
}

BEGIN_MESSAGE_MAP(CCppTextEditorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCppTextEditorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCppTextEditorDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCppTextEditorDlg 消息处理程序

BOOL CCppTextEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCppTextEditorDlg::OnPaint()
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
HCURSOR CCppTextEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCppTextEditorDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	if (FilePathSave == "")
	{
		AfxMessageBox("保存路径缺失");
		return;
	}
	FILE* fp;
	fp = fopen(FilePathSave, "w");
	fprintf(fp, StringInput);
	fclose(fp);
	MessageBox("已保存", "提示");
}


void CCppTextEditorDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	if (FilePathOpen == "")
	{
		AfxMessageBox("打开路径缺失");
		return;
	}
	FILE* fp;
	fp = fopen(FilePathOpen, "r");
	char text[10000];
	fgets(text,10000,fp);
	GetDlgItem(IDC_EDIT1)->SetWindowText(text);
	fclose(fp);
}
