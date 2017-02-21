#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;

// ListBox��Item��
struct stLISTITEM
{
	HICON hIcon;             // ͼ��
	CString csData;         // ����
	CString csTime;
	COLORREF cTextColor;
	COLORREF cBGColor;
};

class COwnerListBox :
	public CListBox
{
	DECLARE_DYNAMIC(COwnerListBox)

public:
	COwnerListBox(void);
	~COwnerListBox(void);

	void ShowTime(BOOL bState)
	{
		this->m_bShowTime = bState;
	}

protected:
	CRect m_rectListBox;          // �б������Rect
	HWND m_hParWnd;           // �����ھ��
	UINT	  m_iListheight; 

	BOOL m_bShowTime;

	DECLARE_MESSAGE_MAP()

public:
	// �����Ի�ListBox��Item����͸�
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	// �ػ�ListBox��Item
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ���Ի�ListBox�����һ���µ�Item��
	int AddListItem(CString csData, COLORREF TextColor = RGB(0, 0, 0), COLORREF BGColor = RGB(255, 255, 255), HICON hIcon = NULL);

	// ���������ջ
	afx_msg void OnDestroy();
	virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);

	// �������ھ��������
	void SetParWndHandle(HWND hWnd);

	CString GetLocTime(void);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void COwnerListBox::CopyMessage();

	void SetMessage(CString csMessage)
	{
		m_csMessage = csMessage;
	}

private:
	CString m_csMessage;
};

