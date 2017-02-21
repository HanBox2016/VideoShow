#include "StdAfx.h"
#include "COwnerListBox.h"

IMPLEMENT_DYNAMIC(COwnerListBox, CListBox)

	COwnerListBox::COwnerListBox()
{
	this->m_iListheight = 20;
	this->m_bShowTime = FALSE;
}

COwnerListBox::~COwnerListBox()
{

}

BEGIN_MESSAGE_MAP(COwnerListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// COwnerDrawListBox ��Ϣ�������
void COwnerListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  ������Ĵ�����ȷ��ָ����Ĵ�С
	lpMeasureItemStruct->itemHeight = this->m_iListheight;
}

void COwnerListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  ������Ĵ����Ի���ָ����
	UpdateData(FALSE);

	if ( lpDrawItemStruct->itemID == -1 ) 
	{
		DrawFocusRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem);
		return;
	}

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	COLORREF crOldTextColor = pDC->GetTextColor();
	COLORREF crOldBkColor = pDC->GetBkColor();

	m_rectListBox = lpDrawItemStruct->rcItem;

	// �б�Item�е�ͼ��λ��
	CRect rectIcon(m_rectListBox.left + 10, m_rectListBox.top, m_rectListBox.left + 50, m_rectListBox.top);

	// �б�Item�е��ı�λ��
	CRect rectData(m_rectListBox.left + 60, m_rectListBox.top,m_rectListBox.left + 1000, m_rectListBox.top + 20);
	CRect rectNoIconData(m_rectListBox.left + 0, m_rectListBox.top,m_rectListBox.left + 1000, m_rectListBox.top + 20);

	//�б�Item�е�ʱ��
	CRect rectTime(m_rectListBox.right  - 50, m_rectListBox.top,m_rectListBox.right, m_rectListBox.top + 20);

	stLISTITEM *pItem = (stLISTITEM*)GetItemDataPtr(lpDrawItemStruct->itemID);
	if ( pItem == NULL )
	{
		return;
	}

	if ( ::IsBadStringPtrW(pItem->csData, MAX_PATH) )
	{
		return;
	}

	if ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		// ѡ��״̬
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->FillSolidRect(&m_rectListBox,RGB(8,129,222));
	}
	else
	{
		// δѡ��״̬
		pDC->SetTextColor(pItem->cTextColor);
		pDC->FillSolidRect(&m_rectListBox, pItem->cBGColor);
	}

	if ( pItem->hIcon == NULL )
	{
		pDC->DrawText(pItem->csData, pItem->csData.GetLength(), &rectNoIconData, DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
	}
	else
	{
		pDC->DrawIcon(rectIcon.left, rectIcon.top, pItem->hIcon);
		pDC->DrawText(pItem->csData, pItem->csData.GetLength(), &rectData, DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
	}

	if ( this->m_bShowTime )
	{
		pDC->SetTextColor(RGB(0xAA,0xAA,0xAA));
		pDC->DrawText(pItem->csTime,pItem->csTime.GetLength(), &rectTime, DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
	}

	pDC->SetTextColor(crOldTextColor);
	pDC->SetBkColor(crOldBkColor);
}

int COwnerListBox::AddListItem(CString csData, COLORREF TextColor, COLORREF BGColor, HICON hIcon)
{
	stLISTITEM *stItem = new stLISTITEM;
	stItem->hIcon = hIcon;
	stItem->csData = csData;
	stItem->csTime = this->GetLocTime();
	stItem->cTextColor = TextColor;
	stItem->cBGColor = BGColor;

	int iIndex = this->AddString(_T(""));
	SetItemDataPtr(iIndex, stItem);

	return iIndex;
}

void COwnerListBox::OnDestroy()
{
	int ItemCount;
	ItemCount = GetCount();

	if (ItemCount != 0 )
	{
		for (int i = 0; i < ItemCount; i++)
		{
			stLISTITEM *stItem = (stLISTITEM*)GetItemDataPtr(i);
			delete stItem;
			stItem = NULL;
		}
	}

	CListBox::OnDestroy();
}

int COwnerListBox::CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/)
{

	// TODO:  ������Ĵ�����ȷ��ָ���������˳��
	// ����-1 ��ʾ��1 ������2 ֮ǰ
	// ����0 ��ʾ��1 ����2 ˳����ͬ
	// ����1 ��ʾ��1 ������2 ֮��

	return 0;
}

void COwnerListBox::SetParWndHandle(HWND hWnd)
{
	m_hParWnd = hWnd;
}

CString COwnerListBox::GetLocTime(void)
{
	CString csTime;
	SYSTEMTIME stLoctime; 
	GetLocalTime(&stLoctime); 
	csTime.Format(_T("%02d:%02d:%02d"),stLoctime.wHour, stLoctime.wMinute, stLoctime.wSecond);

	return csTime;
}

void COwnerListBox::OnMove(int x, int y)
{
	CListBox::OnMove(x, y);

	// TODO: Add your message handler code here
}


void COwnerListBox::OnSize(UINT nType, int cx, int cy)
{
	CListBox::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	this->RedrawWindow();
}

void COwnerListBox::CopyMessage()
{
	CString csMessage = m_csMessage;

	BOOL bRet = FALSE;
	if ( !OpenClipboard() )
	{
		MessageBox(_T("�������ʧ�ܣ�"));
		return;
	}

	if ( !EmptyClipboard() )
	{
		MessageBox(_T("���������ʧ�ܣ�"));
		return;
	}

	int iLen = csMessage.GetLength() * 2;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, iLen + 2);
	wchar_t *p = (wchar_t*)GlobalLock(hGlobal);
	wmemset(p, 0, csMessage.GetLength() + 1);
	wmemcpy(p, csMessage.GetBuffer(), csMessage.GetLength() + 1);
	bRet = GlobalUnlock(hGlobal);
	HANDLE hResult = SetClipboardData(CF_UNICODETEXT, hGlobal);
	if ( !SUCCEEDED(hResult) )
	{
		return;
	}

	CloseClipboard();
}
