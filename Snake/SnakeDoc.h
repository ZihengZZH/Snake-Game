
// SnakeDoc.h : interface of the CSnakeDoc class
//


#pragma once
#include "CSnake.h"


class CSnakeDoc : public CDocument
{
protected: // create from serialization only
	CSnakeDoc();
	DECLARE_DYNCREATE(CSnakeDoc)

// Attributes
public:
	struct m_theme
	{
		CBrush bg;
		CBrush score;
		CBrush food;
		CBrush snake;
		CBrush snake_head;
		CString m_bg;
	}; // theme structure
	m_theme light;
	m_theme dark;
	m_theme defalt;
	m_theme *current;
	// All the theme attributes
	UINT speed_low;
	UINT speed_default;
	UINT speed_high;
	UINT *speed_current;
	// All the speed attributes
	UINT new_highest; // new highest score
	UINT new_level; // new highest level


// Operations
public:
	UINT RetrieveHighest(); // Function to retrieve the highest score from database
	void UpdateDatabase(); // Function to update the database


// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSnakeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnThemeLight();
	afx_msg void OnThemeDark();
	afx_msg void OnThemeDefault();
	afx_msg void OnUpdateThemeDefault(CCmdUI *pCmdUI);
	afx_msg void OnUpdateThemeLight(CCmdUI *pCmdUI);
	afx_msg void OnUpdateThemeDark(CCmdUI *pCmdUI);
	afx_msg void OnSpeedLow();
	afx_msg void OnSpeedDefault();
	afx_msg void OnSpeedHigh();
	afx_msg void OnUpdateSpeedLow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSpeedDefault(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSpeedHigh(CCmdUI *pCmdUI);
};
