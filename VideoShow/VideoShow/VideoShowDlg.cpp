
// VideoShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoShow.h"
#include "VideoShowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
void EnumerateFiles(CStringArray &strArray)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(_T("*.*"), &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do 
		{
			// ���ΪĿ¼
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// �����Ϊ��ǰĿ¼.����һ��Ŀ¼..
				if (_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
				{
					::SetCurrentDirectory(fd.cFileName);    // �����Ŀ¼
					EnumerateFiles(strArray);               // �ݹ�ö��
					::SetCurrentDirectory(_T(".."));        // ���ظ�Ŀ¼��һ��Ŀ¼
				}
			}
			// ���Ϊ�ļ�
			else
			{
				CString strDir;
				TCHAR   lpDir[MAX_PATH];

				::GetCurrentDirectory(MAX_PATH, lpDir);
				strDir = lpDir;
				if ( strDir.Right(1) != _T("\\") )
				{
					strDir += _T("\\");
				}
				strDir += fd.cFileName;

				strArray.Add(strDir);
			}
		} while (::FindNextFile(hFind, &fd));

		::FindClose(hFind);
	}
}

BOOL IsWantedFile(const CString &str)
{
	CString strLower;
	TCHAR   szExt[_MAX_EXT] = _T("");

	// ����͵��ֱ������֮ǰ�Ĺ����ַ����������ļ������ܺ��С�*�������Կ����ں�׺��������ϡ�;*�����ж��Ƿ���ȫƥ��
	const   CString STR_FileFilter =
		_T("*.rm;*.rmvb;*.flv;*.f4v;*.avi;*.3gp;*.mp4;*.wmv;*.mpeg;*.mpga;*.asf;*.dat;*.mov;*")
		_T("*.mp3;*.wma;*.wav;*.mid;*.rmi;*.aac;*.ac3;*.aiff;*.m4a;*.mka;*.mp2;*.ogg;*");

	_tsplitpath_s(str, NULL, 0, NULL, 0, NULL, 0, szExt, _MAX_EXT);   // ��ȡ��׺��
	strLower = szExt;
	strLower.MakeLower();

	if(! strLower.IsEmpty())    // û�к�׺���Ĳ���������
	{
		strLower += _T(";*");   // .mo���������������ڻ�ƥ�䵽.mov�������ں�����ϡ�;*�����ж��Ƿ���ȫƥ��
		return -1 != STR_FileFilter.Find(strLower);
	}

	return FALSE;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoShowDlg �Ի���




CVideoShowDlg::CVideoShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoShowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bFullScreen = FALSE;
	m_iBGChange = 0;
}

void CVideoShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_PLAY, m_Slider);
	DDX_Control(pDX, IDC_LIST_PLAY, m_List);
	DDX_Control(pDX, IDC_SLIDER_VOICE, m_SliderVoice);
	DDX_Control(pDX, IDC_STATIC_URL, m_staURL);
}

BEGIN_MESSAGE_MAP(CVideoShowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_VIDEO_OPENFILE, &CVideoShowDlg::OnBnClickedBtnVideoOpenfile)
	ON_BN_CLICKED(IDC_BTN_VIDEO_PLAY, &CVideoShowDlg::OnBnClickedBtnVideoPlay)
	ON_BN_CLICKED(IDC_BTN_VIDEO_SPEEN_LEFT, &CVideoShowDlg::OnBnClickedBtnVideoSpeenLeft)
	ON_BN_CLICKED(IDC_BTN_VIDEO_SPEEN_RIGHT, &CVideoShowDlg::OnBnClickedBtnVideoSpeenRight)
	ON_BN_CLICKED(IDC_BTN_VIDEO_ALLSCEEN, &CVideoShowDlg::OnBnClickedBtnVideoAllsceen)
	ON_BN_CLICKED(IDC_BTN_VIDEO_LIST, &CVideoShowDlg::OnBnClickedBtnVideoList)
	ON_WM_DROPFILES()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_GETMINMAXINFO()
	ON_LBN_DBLCLK(IDC_LIST_PLAY, &CVideoShowDlg::OnLbnDblclkListPlay)
//	ON_STN_DBLCLK(IDC_STA_PLAY, &CVideoShowDlg::OnStnDblclickStaPlay)
	ON_STN_CLICKED(IDC_STA_PLAY, &CVideoShowDlg::OnStnClickedStaPlay)
	ON_BN_CLICKED(IDC_BTN_OPENURL_OPEN, &CVideoShowDlg::OnBnClickedBtnOpenurlOpen)
	ON_BN_CLICKED(IDC_BTN_OPENURL, &CVideoShowDlg::OnBnClickedBtnOpenurl)
	ON_COMMAND(ID_MENU_R_LIST_OPEN, &CVideoShowDlg::OnMenuRListOpen)
	ON_COMMAND(ID_MENU_R_LIST_DELETE, &CVideoShowDlg::OnMenuRListDelete)
	ON_COMMAND(ID_MENU_R_LIST_COPY, &CVideoShowDlg::OnMenuRListCopy)
	ON_COMMAND(ID_MENU_R_LIST_CLEAR, &CVideoShowDlg::OnMenuRListClear)
	ON_BN_CLICKED(IDC_BTN_SNAPSHOT, &CVideoShowDlg::OnBnClickedBtnSnapshot)
	ON_BN_CLICKED(IDC_BTN_MUTE, &CVideoShowDlg::OnBnClickedBtnMute)
END_MESSAGE_MAP()


// CVideoShowDlg ��Ϣ�������

BOOL CVideoShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	this->SetBackgroundColor(RGB(255, 255, 255));

	//ë��������
	//SetLayeredWindowAttributes (TRANSPARENT_COLOR , 0, LWA_COLORKEY );
	//BOOL bDwm ;
	//DwmIsCompositionEnabled (&bDwm );
	//if (bDwm )
	//{  
	//	MARGINS mrg = {-1};  
	//	DwmExtendFrameIntoClientArea (m_hWnd , &mrg );
	//	SetBackgroundColor (TRANSPARENT_COLOR );
	//}

	this->GetDlgItem(IDC_STA_PLAY)->GetClientRect(m_Audiorect);

	ModifyStyleEx(NULL, WS_EX_ACCEPTFILES);                             // �����ļ���ק
	this->m_cPlayer.SetHWND(GetDlgItem(IDC_STA_PLAY)->GetSafeHwnd());   // ���ò������Ĵ��ھ��

	// ���ز����б����������ڴ�С
	CRect rcWnd, rcPlaylist;
	GetWindowRect(&rcWnd);  
	m_List.GetClientRect(&rcPlaylist);  
	SetWindowPos(NULL, 0, 0, rcWnd.Width() - rcPlaylist.Width() - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXBORDER), rcWnd.Height(), SWP_NOMOVE);
	GetDlgItem(IDC_LIST_PLAY)->ShowWindow(SW_HIDE); 

	this->ShowURL(SW_HIDE);

	this->m_Slider.SetRange(0, 1000);
	this->m_Slider.SetTicFreq(1);
	m_SliderVoice.SetRange(0, 100);
	m_SliderVoice.SetPos(100);

	this->m_List.SetHorizontalExtent(300);
	this->m_tools.SetFont(&m_font, 17, _T("΢���ź�"));
	this->m_List.SetFont(&m_font);

	this->LoadPlaylist();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVideoShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVideoShowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVideoShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoShowDlg::OnBnClickedBtnVideoOpenfile()
{
	// TODO: Add your control notification handler code here
	CString csFilePath;
	CString csFileFilter=_T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.flv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT|OFN_NOCHANGEDIR, csFileFilter);
	POSITION pos = dlg.GetStartPosition();
	if ( dlg.DoModal() == IDOK )
	{
		if ( pos != NULL )
		{
			csFilePath = dlg.GetNextPathName(pos);  
			this->AddMessage(csFilePath);
			if ( m_cPlayer.Play(m_tools.WTUTF8(csFilePath)) )
			{
				GetDlgItem(IDC_BTN_VIDEO_PLAY)->SetWindowText(_T("��ͣ"));
				this->SetWindowText(csFilePath);
				m_cPlayer.FullSceen();
				m_playerTimer = SetTimer(100,1*100,NULL);
			}
		}
		while ( pos != NULL )
		{
			csFilePath = dlg.GetNextPathName(pos);  
			this->AddMessage(csFilePath);
		}
	}
}


void CVideoShowDlg::OnBnClickedBtnVideoPlay()
{
	// TODO: Add your control notification handler code here
	if (! m_cPlayer.IsOpen())
	{
		return;
	}

	if (m_cPlayer.IsPlaying())
	{
		m_cPlayer.Pause();
		//KillTimer(100);
		//m_Slider.SetPos(0);
		GetDlgItem(IDC_BTN_VIDEO_PLAY)->SetWindowText(_T("����"));
	} 
	else
	{
		m_cPlayer.Play();
		GetDlgItem(IDC_BTN_VIDEO_PLAY)->SetWindowText(_T("��ͣ"));
	}
}


void CVideoShowDlg::OnBnClickedBtnVideoSpeenLeft()
{
	// TODO: Add your control notification handler code here
	m_cPlayer.SeekBackward();
}


void CVideoShowDlg::OnBnClickedBtnVideoSpeenRight()
{
	// TODO: Add your control notification handler code here
	m_cPlayer.SeekForward();
}


void CVideoShowDlg::OnBnClickedBtnVideoAllsceen()
{
	// TODO: Add your control notification handler code here
	if ( TRUE == m_cPlayer.IsPlaying() && FALSE == m_bFullScreen )  
	{  
		m_bFullScreen = TRUE;  

		//��ȡ�Ի���ԭʼλ��  
		GetWindowPlacement(&m_OldWndPlacement);  
		this->GetDlgItem(IDC_STA_PLAY)->GetClientRect(m_Audiorect);
		CRect WindowRect;  
		GetWindowRect(&WindowRect);  
		CRect ClientRect;  
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect);  
		ClientToScreen(&ClientRect);  

		//��ȡ��Ļ�ķֱ���  
		int nFullWidth  = GetSystemMetrics(SM_CXSCREEN);  
		int nFullHeight = GetSystemMetrics(SM_CYSCREEN);  

		//�Ի���ȫ����ʾ  
		m_FullScreenRect.left = WindowRect.left - ClientRect.left;  
		int m_top = WindowRect.top - ClientRect.top;  
		m_FullScreenRect.top    = m_top;  
		m_FullScreenRect.right  = WindowRect.right - ClientRect.right + nFullWidth;  
		m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + nFullHeight;  

		m_NewWndPlacement.length           = sizeof(WINDOWPLACEMENT);  
		m_NewWndPlacement.flags            = 0;  
		m_NewWndPlacement.showCmd          = SW_SHOWNORMAL;  
		m_NewWndPlacement.rcNormalPosition = m_FullScreenRect;  
		SetWindowPlacement(&m_NewWndPlacement);  

		this->GetDlgItem(IDC_STA_PLAY)->MoveWindow(CRect(0, 0, nFullWidth, nFullHeight));  



		//���ؿؼ�  
		this->DealCtrlShow(SW_HIDE);
	}
}


void CVideoShowDlg::OnBnClickedBtnVideoList()
{
	// TODO: Add your control notification handler code here
	this->ShowPlaylist(! m_List.IsWindowVisible());
}

void CVideoShowDlg::ShowPlaylist( BOOL bShow )
{
	CRect rcWnd, rcPlaylist;

	GetWindowRect(&rcWnd);  
	m_List.GetClientRect(&rcPlaylist);  

	if (m_List.IsWindowVisible() != bShow) // ֻ����ʾ״̬�����仯ʱ���ŵ������ڴ�С
	{
		if(bShow)
		{
			SetWindowPos(NULL, 0, 0, rcWnd.Width() + rcPlaylist.Width(), rcWnd.Height(), SWP_NOMOVE);
		}
		else
		{
			SetWindowPos(NULL, 0, 0, rcWnd.Width() - rcPlaylist.Width(), rcWnd.Height(), SWP_NOMOVE);
		}
	}

	m_List.ShowWindow( bShow ? SW_SHOW : SW_HIDE);
}

void CVideoShowDlg::DealCtrlShow(int iMode)
{
	this->GetDlgItem(IDC_BTN_VIDEO_OPENFILE)->ShowWindow(iMode);
	this->GetDlgItem(IDC_BTN_VIDEO_SPEEN_LEFT)->ShowWindow(iMode);
	this->GetDlgItem(IDC_BTN_VIDEO_SPEEN_RIGHT)->ShowWindow(iMode);
	this->GetDlgItem(IDC_BTN_VIDEO_PLAY)->ShowWindow(iMode);
	this->GetDlgItem(IDC_SLIDER_VOICE)->ShowWindow(iMode);
	this->GetDlgItem(IDC_BTN_VIDEO_ALLSCEEN)->ShowWindow(iMode);
	this->GetDlgItem(IDC_BTN_VIDEO_LIST)->ShowWindow(iMode);
	this->GetDlgItem(IDC_SLIDER_PLAY)->ShowWindow(iMode);
	this->GetDlgItem(IDC_STA_VIDEO_TITLE_VOICE)->ShowWindow(iMode);
}


void CVideoShowDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	UINT nFileCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	TCHAR        szFileName[_MAX_PATH] = _T("");
	CFileStatus  fStatus;
	CStringArray strArray;

	ShowPlaylist(TRUE);

	// ��ȡ��ק�����ļ����ļ���
	for (UINT i = 0; i < nFileCount; i++)
	{
		// ��ȡ�ļ�·��
		::DragQueryFile(hDropInfo, i, szFileName, sizeof(szFileName));
		CFile::GetStatus(szFileName, fStatus);

		// �Ƿ�Ϊ�ļ���
		if ( fStatus.m_attribute & FILE_ATTRIBUTE_DIRECTORY )
		{          
			::SetCurrentDirectory(szFileName);  // ����ǰĿ¼�����ڴ�           
			EnumerateFiles(strArray);	        // ö��Ŀ¼�������ļ�
		}
		else
		{
			strArray.Add(szFileName);
		}
	}

	// ������Ҫ���ļ�
	m_List.SetRedraw(FALSE);
	m_List.SetHorizontalExtent(1500); // ����ֻ�Ǽ򵥵�����Ϊ500��ȣ���Ҫ�պ�ƥ��·����ȣ���μ�msdn�������

	for(int i = 0; i < strArray.GetCount(); i++)
	{
		if (IsWantedFile(strArray[i]))
		{
			this->AddMessage(strArray[i]);
		}        
	}

	m_List.SetRedraw(TRUE);
	::DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CVideoShowDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar->GetSafeHwnd() == m_Slider.GetSafeHwnd())
	{
		if (m_cPlayer.IsPlaying())
		{
			libvlc_time_t m_CurTime;
			m_TotalTime = this->m_cPlayer.GetLength();
			double pos = m_Slider.GetPos() / ((double)m_Slider.GetLineSize() * 1000);
			m_CurTime = (libvlc_time_t)(m_TotalTime * pos);
			this->m_cPlayer.SetTime(m_CurTime);
			m_Slider.SetPos(nPos);
		}
	}
	else if ( pScrollBar->GetSafeHwnd() == m_SliderVoice.GetSafeHwnd() )
	{
		if ( m_cPlayer.IsPlaying() )
		{
			int iVolume = m_SliderVoice.GetPos();
			this->m_cPlayer.Volume(iVolume);
		}
	} 
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void CVideoShowDlg::NormalShow(void)  
{  
	if ( m_cPlayer.IsPlaying()&& (TRUE == m_bFullScreen))  
	{  
		//�ָ�Ĭ�ϴ���  
		this->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_STA_PLAY)->ShowWindow(SW_HIDE);  

		SetWindowPlacement(&m_OldWndPlacement); 
		this->GetDlgItem(IDC_STA_PLAY)->MoveWindow(m_Audiorect);  

		this->GetDlgItem(IDC_STA_PLAY)->ShowWindow(SW_SHOW); 
		this->ShowWindow(SW_SHOW);

		m_bFullScreen = FALSE;  

		//��ʾ�ؼ�  
		this->DealCtrlShow(SW_SHOW);
	}  
} 

void CVideoShowDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 100)
	{
		libvlc_time_t m_CurTime = this->m_cPlayer.GetTime();
		//float fPos = libvlc_media_player_get_position(m_vlcMplay, &m_vlcEx);

		if ( m_CurTime != 0 )
		{
			m_TotalTime = this->m_cPlayer.GetLength();
			int m_npos = (int)(m_CurTime*1000/m_TotalTime);
			m_Slider.SetPos(m_npos);
		}
	}
	if(libvlc_Ended == this->m_cPlayer.GetState())
	{
		this->m_cPlayer.Stop();
		KillTimer(100);
		m_Slider.SetPos(0);
		GetDlgItem(IDC_BTN_VIDEO_PLAY)->SetWindowText(_T("����"));
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CVideoShowDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	if (TRUE == m_bFullScreen)  
	{  
		lpMMI->ptMaxSize.x     = m_FullScreenRect.Width();  
		lpMMI->ptMaxSize.y     = m_FullScreenRect.Height();  
		lpMMI->ptMaxPosition.x = m_FullScreenRect.Width();  
		lpMMI->ptMaxPosition.y = m_FullScreenRect.Height();  

		//����Track�ߴ�ҲҪ�ı�  
		lpMMI->ptMaxTrackSize.x = m_FullScreenRect.Width();  
		lpMMI->ptMaxTrackSize.y = m_FullScreenRect.Height();  
	}  

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CVideoShowDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	if ( this->m_cPlayer.IsPlaying() )
	{
		KillTimer(100);
		this->m_cPlayer.Stop();
	}

	this->SavePlaylist();

	CDialogEx::OnCancel();
}


void CVideoShowDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


BOOL CVideoShowDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN ) 
	{  
		switch(pMsg->wParam) 
		{  
		case VK_ESCAPE: 
			NormalShow();
			return TRUE;
			break;

		case VK_SPACE: 
			OnBnClickedBtnVideoPlay();
			return TRUE;
			break;

		case VK_RETURN:
			if ( m_bFullScreen  )
			{
				this->NormalShow();
			}
			else
			{
				this->OnBnClickedBtnVideoAllsceen();
			}
			return -1;
			break;

		default:
			break;
		} 
	}
	else if (WM_LBUTTONDBLCLK == pMsg->message)   
	{  
		CPoint ptCursor;  
		GetCursorPos(&ptCursor);  

		CRect rc;  
		GetDlgItem(IDC_STA_PLAY)->GetWindowRect(&rc);    
		if (rc.PtInRect(ptCursor))  
		{  
			if ( FALSE == m_bFullScreen )  
			{  
				OnBnClickedBtnVideoAllsceen(); 
			}  
			else  
			{  
				NormalShow();  
			}  
		}  
	} 
	else if ( WM_RBUTTONDOWN == pMsg->message )
	{
		if ( pMsg->hwnd == this->m_List.m_hWnd )
		{
			this->DealRMenu(&this->m_List);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CVideoShowDlg::DealRMenu(CListBox *pList)
{
	if ( pList == NULL )
	{
		return;
	}

	CMenu menu;
	menu.LoadMenu(IDR_MENU_R); 
	CMenu *pSubMenu = menu.GetSubMenu(0);

	int iState = pList->GetCurSel();
	if ( iState != -1  )
	{
		CPoint Point; 
		GetCursorPos( &Point); 

		pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, Point.x, Point.y, this);
	}
}


void CVideoShowDlg::ShowURL(BOOL bShow)
{
	CRect rcWnd, rcPlaylist;

	GetWindowRect(&rcWnd);  
	m_staURL.GetClientRect(&rcPlaylist);  

	if(bShow)
	{
		SetWindowPos(NULL, 0, 0, rcWnd.Width(), rcWnd.Height() + rcPlaylist.Height(), SWP_NOMOVE);
	}
	else
	{
		SetWindowPos(NULL, 0, 0, rcWnd.Width(), rcWnd.Height() -  rcPlaylist.Height(), SWP_NOMOVE);
	}

	m_staURL.ShowWindow( bShow ? SW_SHOW : SW_HIDE);
}

void CVideoShowDlg::OnLbnDblclkListPlay()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_List.GetCurSel();
	if (LB_ERR == nIndex)
	{
		return;
	}

	CRect   rcVideo; 

	GetDlgItem(IDC_STA_PLAY)->GetClientRect(&rcVideo);  
	InvalidateRect(rcVideo);

	stLISTITEM *pItem = (stLISTITEM*)this->m_List.GetItemDataPtr(nIndex);
	CString csFilePath = pItem->csData;
	csFilePath.Replace(_T("\r\n"), _T(""));
	csFilePath.Replace(_T("\n"), _T(""));

	if ( m_cPlayer.Play(this->m_tools.WTUTF8(csFilePath)))
	{
		this->m_Slider.SetPos(0);
		GetDlgItem(IDC_BTN_VIDEO_PLAY)->SetWindowText(_T("��ͣ"));
	}
}


//void CVideoShowDlg::OnStnDblclickStaPlay()
//{
//	// TODO: Add your control notification handler code here
//	if ( m_bFullScreen  )
//	{
//		this->NormalShow();
//	}
//	else
//	{
//		this->OnBnClickedBtnVideoAllsceen();
//	}
//}


void CVideoShowDlg::OnStnClickedStaPlay()
{
	// TODO: Add your control notification handler code here
	if (! m_cPlayer.IsOpen())
	{
		return;
	}

	if (m_cPlayer.IsPlaying())
	{
		m_cPlayer.Pause();
		//KillTimer(100);
		//m_Slider.SetPos(0);
		GetDlgItem(IDC_BTN_VIDEO_PLAY)->SetWindowText(_T("����"));
	} 
	else
	{
		m_cPlayer.Play();
		GetDlgItem(IDC_BTN_VIDEO_PLAY)->SetWindowText(_T("��ͣ"));
	}
}


void CVideoShowDlg::OnBnClickedBtnOpenurlOpen()
{
	// TODO: Add your control notification handler code here
	CString csTmp;
	GetDlgItemText(IDC_EDIT_URL, csTmp);
	if ( !csTmp.IsEmpty() )
	{
		if ( m_cPlayer.PlaySteam(m_tools.WTUTF8(csTmp)) )
		{
			GetDlgItem(IDC_BTN_VIDEO_PLAY)->SetWindowText(_T("��ͣ"));
			this->SetWindowText(csTmp);
			m_Slider.SetPos(0);
		}	
	}
}

void CVideoShowDlg::OnBnClickedBtnOpenurl()
{
	// TODO: Add your control notification handler code here
	this->ShowURL(!m_staURL.IsWindowVisible());
}

void CVideoShowDlg::LoadPlaylist()
{
	CFileFind finder;
	if ( !finder.FindFile(FILE_PLAYLIST) )
	{
		CFile File(FILE_PLAYLIST, CFile::modeCreate);
		File.Close();
	} 

	CStdioFileEx File;
	File.Open(_T("PlayList.txt"), CFile::modeReadWrite);

	CString csFilePath;
	try
	{
		while ( File.ReadString(csFilePath) )
		{
			this->AddMessage(csFilePath);
		}
	}
	catch (CFileException* e)
	{
		File.Close();
		return;
	}
	File.Close();
}

void CVideoShowDlg::SavePlaylist()
{
	DeleteFile(FILE_PLAYLIST);
	CFile fileTmp(FILE_PLAYLIST, CFile::modeCreate);
	fileTmp.Close();

	CStdioFileEx File;
	File.Open(FILE_PLAYLIST, CFile::modeReadWrite);

	try
	{
		for ( int i = 0; i < this->m_List.GetCount(); i++ )
		{
			CString csTmp;
			stLISTITEM *pItem = (stLISTITEM*)this->m_List.GetItemDataPtr(i);
			csTmp = pItem->csData;
			if ( !csTmp.IsEmpty() )
			{
				File.WriteString(csTmp);
				File.WriteString(_T("\n"));
			}
		}
	}
	catch (CFileException* e)
	{
		File.Close();
		return;
	}

	File.Close();
}

void  CVideoShowDlg::AddMessage(CString csMessage)
{
	if ( csMessage.GetLength() <= 2 )
	{
		return;
	}

	COLORREF BGColor = RGB(255, 255, 255);
	if ( this->m_iBGChange == 0 )
	{
		BGColor = RGB(255, 255, 255);
		this->m_iBGChange = 1;
	}
	else
	{
		BGColor = RGB(0xF0, 0xF8, 0xFF);
		this->m_iBGChange = 0;
	}

	this->m_List.SetCurSel (this->m_List.AddListItem(csMessage ,RGB(0x10, 0x4E, 0x8B), BGColor));
}


void CVideoShowDlg::OnMenuRListOpen()
{
	// TODO: Add your command handler code here
	this->OnLbnDblclkListPlay();
}


void CVideoShowDlg::OnMenuRListDelete()
{
	// TODO: Add your command handler code here
	int nIndex = m_List.GetCurSel();
	if ( nIndex != -1 )
	{
		m_List.DeleteString(nIndex);
	}
}


void CVideoShowDlg::OnMenuRListCopy()
{
	// TODO: Add your command handler code here
	int nIndex = m_List.GetCurSel();
	stLISTITEM *pItem = (stLISTITEM*)this->m_List.GetItemDataPtr(nIndex);
	CString csMessage = pItem->csData;

	this->m_List.SetMessage(csMessage);
	this->m_List.CopyMessage();
}


void CVideoShowDlg::OnMenuRListClear()
{
	// TODO: Add your command handler code here
	this->m_List.ResetContent();
}


void CVideoShowDlg::OnBnClickedBtnSnapshot()
{
	// TODO: Add your control notification handler code here
	if ( this->m_cPlayer.SnapShot() )
	{
		MessageBox(_T("�ѳɹ���ͼ��"));
	}
}


void CVideoShowDlg::OnBnClickedBtnMute()
{
	// TODO: Add your control notification handler code here
	this->m_cPlayer.Mute();
	if ( this->m_cPlayer.IsMute() )
	{
		this->m_SliderVoice.EnableWindow(TRUE);
	}
	else
	{
		this->m_SliderVoice.EnableWindow(FALSE);
	}	
}
