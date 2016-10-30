
// RubiksCubeView.cpp : CRubiksCubeView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "RubiksCube.h"
#endif

#include "RubiksCubeDoc.h"
#include "RubiksCubeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRubiksCubeView

IMPLEMENT_DYNCREATE(CRubiksCubeView, CView)

BEGIN_MESSAGE_MAP(CRubiksCubeView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_COLOR_ONLY, &CRubiksCubeView::OnColorOnly)
	ON_COMMAND(ID_COLOR_WITH_NUMBER, &CRubiksCubeView::OnColorWithNumber)
	ON_UPDATE_COMMAND_UI(ID_COLOR_ONLY, &CRubiksCubeView::OnUpdateColorOnly)
	ON_UPDATE_COMMAND_UI(ID_COLOR_WITH_NUMBER, &CRubiksCubeView::OnUpdateColorWithNumber)
	ON_COMMAND(ID_FACE_CHANGED_LOG, &CRubiksCubeView::OnFaceChangedLog)
	ON_COMMAND(ID_SLICE_ROTATE_LOG, &CRubiksCubeView::OnSliceRotateLog)
	ON_UPDATE_COMMAND_UI(ID_FACE_CHANGED_LOG, &CRubiksCubeView::OnUpdateFaceChangedLog)
	ON_UPDATE_COMMAND_UI(ID_SLICE_ROTATE_LOG, &CRubiksCubeView::OnUpdateSliceRotateLog)
END_MESSAGE_MAP()

// CRubiksCubeView 생성/소멸

CRubiksCubeView::CRubiksCubeView()
	: m_bIsColorOnly(true)
	, m_bKeyboardLogOn(false)
	, m_bMouseLogOn(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.

	ptChild.x = 500;
	ptChild.y = 300;

	//m_a_face[3][3];			// a
	//m_b_face[3][3][4];		// b이며 Polygon이기 때문에 뒤에 4개의 점이 필요하다. (사다리꼴은 꼭지점이 4개이기 때문)
	//m_r_face[3][3][4];		// r이며 Polygon이기 때문에 뒤에 4개의 점이 필요하다. (사다리꼴은 꼭지점이 4개이기 때문)

	// a 부분 초기화 
	// j부분은 x 축 방향 (column), i부분은 y축 방향(row)을 그리기 위하여 사용
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_a_face[i][j] = CRect(ptChild.x + j*64, ptChild.y + i*64, ptChild.x + 64 + j*64, ptChild.y + 64 + i*64);
		}
	}

	// b (베타)
	// Polygon 정의 (배열에서)
	// 0 : 왼쪽 위, 1 : 오른쪽 위, 2 : 오른쪽 아래, 3 : 왼쪽 아래 (원점)
	// 기준점은 ptChiuld.x + 90과 ptChild.y -90. 위 점에 m_b_face[0][0]이 존재
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_b_face[i][j][0].x = ptChild.x + 60 + 30 + j*64 - i*30;
			m_b_face[i][j][0].y = ptChild.y - 60 - 30 + i*30;

			m_b_face[i][j][1].x = ptChild.x + 60 + 64 + 30 + j*64 - i*30;
			m_b_face[i][j][1].y = ptChild.y - 60 - 30 + i*30;

			m_b_face[i][j][2].x = ptChild.x + 60 + 64 + j*64 - i*30;
			m_b_face[i][j][2].y = ptChild.y - 60 + i*30;

			m_b_face[i][j][3].x = ptChild.x + 60 + j*64 - i*30;
			m_b_face[i][j][3].y = ptChild.y - 60 + i*30;
		}
	}

	// r (감마)
	// Polygon 정의 (배열에서)
	// 0 : 왼쪽 위, 1 : 오른쪽 위, 2 : 오른쪽 아래, 3 : 왼쪽 아래 (원점)
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			// 기준 (ptChild.x + 64*3, ptChild.y)
			m_r_face[i][j][0].x = ptChild.x + 64*3 + j*30;
			m_r_face[i][j][0].y = ptChild.y + i*64 - j*30;

			m_r_face[i][j][1].x = ptChild.x + 64*3 + 30 + j*30;
			m_r_face[i][j][1].y = ptChild.y + i*64 - 30 - j*30;

			m_r_face[i][j][2].x = ptChild.x + 64*3 + 30 + j*30;
			m_r_face[i][j][2].y = ptChild.y + i*64 + 64 - 30- j*30;

			m_r_face[i][j][3].x = ptChild.x + 64*3 + j*30;
			m_r_face[i][j][3].y = ptChild.y + i*64 + 64 - j*30;
		}
	}
}

CRubiksCubeView::~CRubiksCubeView()
{
}

BOOL CRubiksCubeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CRubiksCubeView 그리기

void CRubiksCubeView::OnDraw(CDC* pDC)
{
	CRubiksCubeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	// 그림 그리기 전반적인 개요
	// 1. Document에 존재하는 m_RubiksCube의 값을 가져온다.
	// 2. m_RubiksCube의 a에 해당하는 Face_Header의 면과 b에 해당하는 Face_Header->GetP_Up()과 r에 해당하는 Face_Header->GetP_Next()의 unique value를 가져온다.
	// 3. 그 값을 가지고 그림을 그린 후 값을 반환한다.
	// * 추가적으로 모든 동적 메모리를 할당 해제할 수 있도록 한다.

	// Header와 a를 매칭하기 위하여 pDoc에서 자료를 가져온다.
	int** a_element_values = pDoc->m_RubiksCube.GetFace_Header()->GetM_element_values();

	// a 부분 출력
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			CBrush rectangle_colora = color_mapping(a_element_values[i][j]);
			CBrush* pOldBrush = pDC->SelectObject(&rectangle_colora);

			pDC->Rectangle(m_a_face[i][j]);		// 사각형을 출력하는 부분
			
			if(m_bIsColorOnly == false)
			{
				// 숫자의 background color를 transparent로 바꿈
				pDC->SetBkMode(TRANSPARENT);
				// 숫자를 출혁하는 부분, 숫자 출력 위치는 CRect의 (Left + 25, Top + 25)으로 정함
				CString numberString;
				numberString.Format(_T("%d"), a_element_values[i][j]);
				pDC->TextOutW(m_a_face[i][j].left + 25, m_a_face[i][j].top + 25, numberString);
			}
			// Brush 재 설정
			pDC->SelectObject(pOldBrush);

		}
	}

	// 동적 메모리 해제
	for(int i = 0; i < 3; i++)
	{
		delete[] a_element_values[i];
	}
	delete a_element_values;
	a_element_values = 0;




	// Header->Up과 b를 매칭하기 위하여 pDoc에서 자료를 가져온다.
	int** b_element_values = pDoc->m_RubiksCube.GetFace_Header()->GetP_Up()->GetM_element_values();
	// b 부분 출력
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			CBrush polygon_colorb = color_mapping(b_element_values[i][j]);
			CBrush* pOldBrush = pDC->SelectObject(&polygon_colorb);

			pDC->Polygon(m_b_face[i][j], 4);

			if(m_bIsColorOnly == false)
			{
				// 숫자의 background color를 transparent로 바꿈
				pDC->SetBkMode(TRANSPARENT);
				// 숫자를 출혁하는 부분, 숫자 출력 위치는 왼쪽 위 점의 (x + 8, y + 8)로 정함
				CString numberString;
				numberString.Format(_T("%d"), b_element_values[i][j]);
				pDC->TextOutW(m_b_face[i][j][0].x + 8, m_b_face[i][j][0].y + 8, numberString);
			}

			// Brush 재 설정
			pDC->SelectObject(pOldBrush);
		}
	}

	// 동적 메모리 해제
	for(int i = 0; i < 3; i++)
	{
		delete[] b_element_values[i];
	}
	delete b_element_values;
	b_element_values = 0;


	// Header->Next와 r를 매칭하기 위하여 pDoc에서 자료를 가져온다.
	int** r_element_values = pDoc->m_RubiksCube.GetFace_Header()->GetP_Next()->GetM_element_values();
	// r 부분 출력
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			CBrush polygon_colorr = color_mapping(r_element_values[i][j]);
			CBrush* pOldBrush = pDC->SelectObject(&polygon_colorr);

			pDC->Polygon(m_r_face[i][j], 4);

			if(m_bIsColorOnly == false)
			{
				// 숫자의 background color를 transparent로 바꿈
				pDC->SetBkMode(TRANSPARENT);
				// 숫자를 출혁하는 부분, 숫자 출력 위치는 왼쪽 위 점의 (x + 8, y + 8)로 정함
				CString numberString;
				numberString.Format(_T("%d"), r_element_values[i][j]);
				pDC->TextOutW(m_r_face[i][j][0].x + 8, m_r_face[i][j][0].y + 8, numberString);
			}

			// Brush 재 설정
			pDC->SelectObject(pOldBrush);
		}
	}


	// 동적 메모리 해제
	for(int i = 0; i < 3; i++)
	{
		delete[] r_element_values[i];
	}
	delete r_element_values;
	r_element_values = 0;
	

	// Keydown만들기
	pDC->SetBkMode(TRANSPARENT);
	if(m_bKeyboardLogOn)
		pDC->DrawTextW(keydownLog, CRect(800, 210, 900, 500), DT_NOCLIP);

	// Mouse Drag log 만들기
	if(m_bMouseLogOn)
		pDC->DrawTextW(MouseLog, CRect(900, 210, 1300, 500), DT_NOCLIP);
}


// CRubiksCubeView 인쇄

BOOL CRubiksCubeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CRubiksCubeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CRubiksCubeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CRubiksCubeView 진단

#ifdef _DEBUG
void CRubiksCubeView::AssertValid() const
{
	CView::AssertValid();
}

void CRubiksCubeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRubiksCubeDoc* CRubiksCubeView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRubiksCubeDoc)));
	return (CRubiksCubeDoc*)m_pDocument;
}
#endif //_DEBUG


// CRubiksCubeView 메시지 처리기


void CRubiksCubeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	// 로그 파일은 FraME부분에서 만들어야 하나?

	// Document를 가져온다.
	CRubiksCubeDoc* pDoc = GetDocument();

	switch(nChar)
	{
	case VK_UP:
		keydownLogCnt++;
		keydownLog.AppendFormat(_T("%d. Key_Up"), keydownLogCnt);
		keydownLog += "\r\n";
		pDoc->m_RubiksCube.RotateCubeUp();
		break;
	case VK_DOWN:
		keydownLogCnt++;
		keydownLog.AppendFormat(_T("%d. Key_Down"), keydownLogCnt);
		keydownLog += "\r\n";
		pDoc->m_RubiksCube.RotateCubeUp();
		pDoc->m_RubiksCube.RotateCubeUp();
		pDoc->m_RubiksCube.RotateCubeUp();
		break;
	case VK_LEFT:
		keydownLogCnt++;
		keydownLog.AppendFormat(_T("%d. Key_Left"), keydownLogCnt);
		keydownLog += "\r\n";
		pDoc->m_RubiksCube.RotateCubeRight();
		pDoc->m_RubiksCube.RotateCubeRight();
		pDoc->m_RubiksCube.RotateCubeRight();
		break;
	case VK_RIGHT:
		keydownLogCnt++;
		keydownLog.AppendFormat(_T("%d. Key_Right"), keydownLogCnt);
		keydownLog += "\r\n";
		pDoc->m_RubiksCube.RotateCubeRight();
		break;
	default:
		break;
	}

	// 그림을 다시 그려 준다.
	RedrawWindow();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CRubiksCubeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 어디에서 LButtonDown 혹은  LButtonUp이 호출되었는지 위치를 찾아주는 메소드 호출 : 메소드는 RubiksCubeView에서 구현
	//int whereReturnVal = WhereIClick(point);

	start_point = point;
	m_bDragFlag = true;
	SetCapture();
	RedrawWindow();

	
	//CString sMessageBox;
	//sMessageBox.Format(_T("%d"),whereReturnVal);

	//MessageBox(sMessageBox);

	//MessageBox(_T("mouse down"));
	CView::OnLButtonDown(nFlags, point);
}


void CRubiksCubeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//int whereReturnVal = WhereIClick(point);

	end_point = point;
	if(m_bDragFlag == true)
	{
		m_bDragFlag = false;
		slice_rotation(start_point, end_point);
		RedrawWindow();
		ReleaseCapture();
	}
	//
	//CString sMessageBox;
	//sMessageBox.Format(_T("%d"),whereReturnVal);

	//MessageBox(_T("mouse up"));
	CView::OnLButtonUp(nFlags, point);
}


void CRubiksCubeView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);
}


void CRubiksCubeView::OnColorOnly()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// TODO : 여기에서 숫자를 안보이도록 설정 후 그림을 다시 그리도록 호출
	//MessageBox(_T("print color only"));
	m_bIsColorOnly = true;

	RedrawWindow();		// 변경 사항이 바로 보이도록 RedrawWindow() 실시
}


void CRubiksCubeView::OnColorWithNumber()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//MessageBox(_T("print color & number"));
	m_bIsColorOnly = false;
	RedrawWindow();		// 변경 사항이 바로 보이도록 RedrawWindow() 실시
}


void CRubiksCubeView::OnUpdateColorOnly(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	
	// 메뉴에서 색상 출력 체크 상태인지 아닌지를 확인 하기 위한 조건문
	if(m_bIsColorOnly)	// 색상 출력이 선택 되어있음
		pCmdUI->SetCheck(1);	// 색상 출력 메뉴를 체크
	else // 색상 출력이 선택 되어있지 않음
		pCmdUI->SetCheck(0);	// 색상 출력 메뉴를 체크 해제
}



void CRubiksCubeView::OnUpdateColorWithNumber(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_bIsColorOnly)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(1);
}


CBrush CRubiksCubeView::color_mapping(int unique_value)
{
	if(unique_value >= 1 && unique_value <= 9)
	{
		// Red
		return CBrush(RGB(255, 0, 0));
	}
	else if(unique_value >= 10 && unique_value <= 18)
	{
		// Green
		return CBrush(RGB(0, 255, 0));
	}
	else if(unique_value >= 19 && unique_value <= 27)
	{
		// Orange
		return CBrush(RGB(255, 127, 0));
	}
	else if(unique_value >= 28 && unique_value <= 36)
	{
		// Blue
		return CBrush(RGB(0, 0, 255));
	}
	else if(unique_value >= 37 && unique_value <= 45)
	{
		// Yellow
		return CBrush(RGB(255, 255, 0));
	}
	else if(unique_value >= 46 && unique_value <= 54)
	{
		// White
		return CBrush(RGB(255, 255, 255));
	}

	return CBrush();
}

// PtInPoly 함수 구현
int pnpoly(CPoint* polygonPoint, CPoint point, int iFaceValue)	// 첫 번째 인자는 도형의 4점의 포인트이며, 두 번째 인자는 클릭한 포인트, 세 번째 인자는 현재 수행하고 있는 면(face)값
{
	/* 클릭이 되었는지 위치를 확인하는 방법 */
	// 1. 작은 사각형 (왼쪽위 cpoint와 오른쪽 아래 cpoint)안에 존재하면 (CRect.PtInRect으로 확인)  무조건 리턴 1
	// Polygon 정의 (배열에서)
	// 0 : 왼쪽 위, 1 : 오른쪽 위, 2 : 오른쪽 아래, 3 : 왼쪽 아래 (원점)
	// 작은 사각형, 왼쪽 위, 오른쪽 아래
	CRect smallRect (polygonPoint[0], polygonPoint[2]);
	if(smallRect.PtInRect (point))
	{
		return 1 ;
	}

	// 2. 큰 사각형 안에 존재하면
	//CRect largeRect(polygonPoint[3], polygonPoint[1]);
	CRect largeRect( polygonPoint[3].x, polygonPoint[1].y, polygonPoint[1].x, polygonPoint[3].y);
	if(largeRect.PtInRect(point))
	{
		// 여기서 존재하면 삼각형의 비례식으로 확인
		if ( iFaceValue == 1)    // B 베타
		{
			// 큰 사각형의 왼쪽 부분인지 오른쪽 부분인지 확인
			//CRect leftRect( polygonPoint[3], polygonPoint[0]);
			CRect leftRect( polygonPoint[3].x, polygonPoint[0].y, polygonPoint[0].x, polygonPoint[3].y);
			//CRect rightRect( polygonPoint[1], polygonPoint[2]);
			CRect rightRect( polygonPoint[2].x, polygonPoint[1].y, polygonPoint[1].x, polygonPoint[2].y);

			if (leftRect.PtInRect(point))
			{
				// 왼쪽에 대해 계산 : point 값이 오른쪽에 존재할 시 사각형 내에 존재
				// 작은 사각형 내에 존재할 시 삼각형의 비례식을 사용하여 해당 영역 내에 존재하는지 확인
				// 밑변 : 밑변작음 = 높이 : 높이 작음 <= 이와 같은 공식을 사용하여 높이를 구하고 
				// 높이보다 크면 밖에 존재, 작으면 내에 존재함을 확인
				// 높이 작음 = 높이 x 밑변 작음 / 밑변

				double boundaryLineHeight;
				boundaryLineHeight = (double)(point.x  - polygonPoint[3].x )*(polygonPoint[3].y - polygonPoint[0].y)/(double)(polygonPoint[0].x - polygonPoint[3].x);
				if(polygonPoint[3].y- point.y < boundaryLineHeight)
					return 1;
			}
			else if (rightRect.PtInRect( point ))
			{
				// 오른쪽에 대해 계산
				double boundaryLineHeight;
				boundaryLineHeight = (double)(point.x  - polygonPoint[2].x )*(polygonPoint[2].y - polygonPoint[1].y)/(double)(polygonPoint[1].x - polygonPoint[2].x);
				if(polygonPoint[2].y- point.y > boundaryLineHeight)
					return 1;
			}

		}
		else if (iFaceValue == 2 ) // R 감마
		{
			// 2.1 왼쪽(or 위쪽) 작은 사각형에 존재하는지 확인 or 오른쪽(or 아래쪽) 작은 사각형에 존재하는지 확인
			CRect upperRect(polygonPoint[0].x,  polygonPoint[1].y, polygonPoint[1].x, polygonPoint[0].y);
			CRect downRect(polygonPoint[3].x, polygonPoint[2].y, polygonPoint[2].x, polygonPoint[3].y);

			if(upperRect.PtInRect(point))
			{
				double boundaryLineHeight;
				boundaryLineHeight = (double)(point.x  - polygonPoint[0].x )*(polygonPoint[0].y - polygonPoint[1].y)/(double)(polygonPoint[1].x - polygonPoint[0].x);
				if(polygonPoint[0].y- point.y < boundaryLineHeight)
					return 1;
			}
			else if(downRect.PtInRect(point))
			{
				double boundaryLineHeight;
				boundaryLineHeight = (double)(point.x  - polygonPoint[3].x )*(polygonPoint[3].y - polygonPoint[2].y)/(double)(polygonPoint[2].x - polygonPoint[3].x);
				if(polygonPoint[3].y- point.y > boundaryLineHeight)
					return 1;
			}
			// 2.2 정밀 검사 수행 후 존재하면 return 1, 존재하지 않으면 return 0
			// * 정밀 검사는 삼각형의 비례식으로 수행
			// 큰 사각형 안에 존재하지 않으면 return 0
		}
	}

	return 0;


}

// 어디에서 LButtonDown 혹은  LButtonUp이 호출되었는지 위치를 찾아주는 메소드
//0~8는 알파 // 9~17은 베타 // 18~26은 감마이며, 각각 012는 1행 1열2열3열 방식으로 값을 리턴
// -1은 실패 및 큐브 이외의 곳 클릭
int CRubiksCubeView::WhereIClick(CPoint point)
{

	// PtIntRect함수를 사용하여서 어디를 클릭하였는지 확인
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(m_a_face[i][j].PtInRect(point))
				return (i*3+j);
		}
	}


	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(pnpoly(m_b_face[i][j], point, 1))
				return (i*3+j+9);
		}
	}

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(pnpoly(m_r_face[i][j], point, 2))
				return (i*3+j+18);
		}
	}

	return -1;
}


bool CRubiksCubeView::slice_rotation(CPoint start_point, CPoint end_point)
{
	int startPointValue = WhereIClick(start_point);
	int endPointValue = WhereIClick(end_point);

	// FACE 면 표시 (로그를 위한 변수)
	CString FaceString;
	int RowOrColNum;

	// 같은 face에서 마우스 dwon 및 up이 일어났는지 유효성 검사
	// 동시에 알바이거나 베타이거나 감마인 경우만 성립
	if(startPointValue < 9 && startPointValue >= 0 && endPointValue < 9 && endPointValue >= 0
		|| startPointValue < 18 && startPointValue >= 9 && endPointValue < 18 && endPointValue >= 9
		|| startPointValue < 27 && startPointValue >= 18 && endPointValue < 27 && endPointValue >= 18)
	{
		int faceValue = -1;
		if(startPointValue < 9 && startPointValue >= 0)
		{
			faceValue = 0;
			FaceString = _T("A");
		}else if(startPointValue < 18 && startPointValue >= 9)
		{
			faceValue = 1;
			FaceString = _T("B");
		}else if(startPointValue < 27 && startPointValue >= 18)
		{
			faceValue = 2;
			FaceString = _T("R");
		}
		
		
		// 유효성 재 확인
		if(faceValue == -1)
			MessageBox(_T("facevalueerror"));

		CRubiksCubeDoc* pDoc = GetDocument();

		int executeValue = endPointValue - startPointValue;

		switch(executeValue)
		{
		case 2:
			if(startPointValue % 9 == 0)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 4);	// Roate Face 실행
				RowOrColNum = 1;
			}
			else if(startPointValue % 9 == 3)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 5);
				RowOrColNum = 2;
			}
			else if(startPointValue % 9 == 6)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 6);
				RowOrColNum = 3;
			}

			// Mouse 로그
			MouseLogCnt++;
			MouseLog.AppendFormat(_T("%d. FACE : %s / Row : %d / Right"), MouseLogCnt, FaceString, RowOrColNum);
			MouseLog += "\r\n";

			break;
		case -2:
			if(startPointValue % 9 == 2)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 12);
				RowOrColNum = 1;
			}
			else if(startPointValue % 9 == 5)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 11);
				RowOrColNum = 2;
			}
			else if(startPointValue % 9 == 8)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 10);
				RowOrColNum = 3;
			}

			// Mouse 로그
			MouseLogCnt++;
			MouseLog.AppendFormat(_T("%d. FACE : %s / Row : %d / Left"), MouseLogCnt, FaceString, RowOrColNum);
			MouseLog += "\r\n";

			break;

		case 6:
			if(startPointValue % 9 == 0)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 9);
				RowOrColNum = 1;
			}
			else if(startPointValue % 9 == 1)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 8);
				RowOrColNum = 2;
			}
			else if(startPointValue % 9 == 2)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 7);
				RowOrColNum = 3;
			}

			// Mouse 로그
			MouseLogCnt++;
			MouseLog.AppendFormat(_T("%d. FACE : %s / Col : %d / Down"), MouseLogCnt, FaceString, RowOrColNum);
			MouseLog += "\r\n";

			break;
		case -6:
			if(startPointValue % 9 == 6)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 1);
				RowOrColNum = 1;
			}
			else if(startPointValue % 9 == 7)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 2);
				RowOrColNum = 2;
			}
			else if(startPointValue % 9 == 8)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 3);
				RowOrColNum = 3;
			}
			
			// Mouse 로그
			MouseLogCnt++;
			MouseLog.AppendFormat(_T("%d. FACE : %s / Col : %d / Up"), MouseLogCnt, FaceString, RowOrColNum);
			MouseLog += "\r\n";

			break;
		default:
			//MessageBox(_T("error"));	// 유효성 검사
			break;
		}

		return true;
	}
	else
	{
		return false;
	}


}


void CRubiksCubeView::OnFaceChangedLog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(m_bKeyboardLogOn == false)
		m_bKeyboardLogOn = true;
	else if(m_bKeyboardLogOn == true)
		m_bKeyboardLogOn = false;
	RedrawWindow();
}


void CRubiksCubeView::OnSliceRotateLog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(m_bMouseLogOn == false)
		m_bMouseLogOn = true;
	else if(m_bMouseLogOn == true)
		m_bMouseLogOn = false;
	RedrawWindow();
}


void CRubiksCubeView::OnUpdateFaceChangedLog(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_bKeyboardLogOn)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CRubiksCubeView::OnUpdateSliceRotateLog(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_bMouseLogOn)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
