
#pragma once


class CViewTree : public CTreeCtrl
{
// Konstruktion
public:
	CViewTree() noexcept;

// Überschreibungen
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementierung
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblClick(NMHDR* pNMHDR, LRESULT* pResult);
};
