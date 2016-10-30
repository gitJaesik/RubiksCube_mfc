
// RubiksCubeView.h : CRubiksCubeView 클래스의 인터페이스
//

#pragma once


class CRubiksCubeView : public CView
{
protected: // serialization에서만 만들어집니다.
	CRubiksCubeView();
	DECLARE_DYNCREATE(CRubiksCubeView)

// 특성입니다.
public:
	CRubiksCubeDoc* GetDocument() const;

// 작업입니다.
public:
	// 기준 점 및 마우스 위치
	//int m_x;		// 아직은 사용 안함
	//int m_y;
	CPoint ptChild;
	CPoint start_point;
	CPoint end_point;
	bool m_bDragFlag;

	// a, b, r (알파, 베타, 감마) 페이스의 [3][3] 배열을 설정
	// 생성자에서 위치 초기화

	// Doc과 값 매칭 시
	// a는 Face_Header가 지목하는 값을 가지고
	// b는 Face_Header->GetP_Up();
	// r는 Face_Header->GetP_Next();

	CRect m_a_face[3][3];			// a
	CPoint m_b_face[3][3][4];		// b이며 Polygon이기 때문에 뒤에 4개의 점이 필요하다. (사다리꼴은 꼭지점이 4개이기 때문)
	CPoint m_r_face[3][3][4];
	
	// 생성자에서 위치 바로 초기화하기

	// Polygon 정의 (배열에서)
	// 0 : 왼쪽 위, 1 : 오른쪽 , 2 : 오른쪽 아래, 3 : 왼쪽 아래 (원점)

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CRubiksCubeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // RubiksCubeView.cpp의 디버그 버전
inline CRubiksCubeDoc* CRubiksCubeView::GetDocument() const
   { return reinterpret_cast<CRubiksCubeDoc*>(m_pDocument); }
#endif

