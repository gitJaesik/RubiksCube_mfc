
// RubiksCubeView.h : CRubiksCubeView Ŭ������ �������̽�
//

#pragma once


class CRubiksCubeView : public CView
{
protected: // serialization������ ��������ϴ�.
	CRubiksCubeView();
	DECLARE_DYNCREATE(CRubiksCubeView)

// Ư���Դϴ�.
public:
	CRubiksCubeDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	// ���� �� �� ���콺 ��ġ
	//int m_x;		// ������ ��� ����
	//int m_y;
	CPoint ptChild;
	CPoint start_point;
	CPoint end_point;
	bool m_bDragFlag;

	// a, b, r (����, ��Ÿ, ����) ���̽��� [3][3] �迭�� ����
	// �����ڿ��� ��ġ �ʱ�ȭ

	// Doc�� �� ��Ī ��
	// a�� Face_Header�� �����ϴ� ���� ������
	// b�� Face_Header->GetP_Up();
	// r�� Face_Header->GetP_Next();

	CRect m_a_face[3][3];			// a
	CPoint m_b_face[3][3][4];		// b�̸� Polygon�̱� ������ �ڿ� 4���� ���� �ʿ��ϴ�. (��ٸ����� �������� 4���̱� ����)
	CPoint m_r_face[3][3][4];
	
	// �����ڿ��� ��ġ �ٷ� �ʱ�ȭ�ϱ�

	// Polygon ���� (�迭����)
	// 0 : ���� ��, 1 : ������ , 2 : ������ �Ʒ�, 3 : ���� �Ʒ� (����)

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CRubiksCubeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnColorOnly();
	afx_msg void OnColorWithNumber();
	afx_msg void OnUpdateColorOnly(CCmdUI *pCmdUI);
	afx_msg void OnUpdateColorWithNumber(CCmdUI *pCmdUI);
private:
	bool m_bIsColorOnly;
public:
	CBrush color_mapping(int face_element_val);
	int WhereIClick(CPoint point);
	bool slice_rotation(CPoint start_point, CPoint end_point);
	afx_msg void OnFaceChangedLog();
	afx_msg void OnSliceRotateLog();
private:
	bool m_bKeyboardLogOn;
	bool m_bMouseLogOn;
public:
	afx_msg void OnUpdateFaceChangedLog(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSliceRotateLog(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // RubiksCubeView.cpp�� ����� ����
inline CRubiksCubeDoc* CRubiksCubeView::GetDocument() const
   { return reinterpret_cast<CRubiksCubeDoc*>(m_pDocument); }
#endif

