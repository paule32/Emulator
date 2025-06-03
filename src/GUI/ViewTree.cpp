
#include "pch.h"
#include "framework.h"
#include "ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CViewTree m_wndFileView; 
CViewTree::CViewTree() noexcept
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK , OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblClick)
END_MESSAGE_MAP()


void CViewTree::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	UINT uFlags = 0;
	HTREEITEM hItem = HitTest(pt, &uFlags);
	if (hItem && (uFlags & TVHT_ONITEM))
	{
		CString text = GetItemText(hItem);
		//AfxMessageBox(_T("Geklicktes Item:  ") + text);
	}

	*pResult = 0;
}

void CViewTree::OnNMDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	UINT uFlags = 0;
	HTREEITEM hItem = HitTest(pt, &uFlags);

	if (hItem && (uFlags & TVHT_ONITEM))
	{
		CString text = GetItemText(hItem);
		AfxMessageBox(_T("Doppelklick:  ") + text);
	}

	*pResult = 0;
}

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != nullptr);

#pragma warning(suppress : 26454)
	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}
