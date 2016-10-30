
// RubiksCubeView.cpp : CRubiksCubeView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CRubiksCubeView ����/�Ҹ�

CRubiksCubeView::CRubiksCubeView()
	: m_bIsColorOnly(true)
	, m_bKeyboardLogOn(false)
	, m_bMouseLogOn(false)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

	ptChild.x = 500;
	ptChild.y = 300;

	//m_a_face[3][3];			// a
	//m_b_face[3][3][4];		// b�̸� Polygon�̱� ������ �ڿ� 4���� ���� �ʿ��ϴ�. (��ٸ����� �������� 4���̱� ����)
	//m_r_face[3][3][4];		// r�̸� Polygon�̱� ������ �ڿ� 4���� ���� �ʿ��ϴ�. (��ٸ����� �������� 4���̱� ����)

	// a �κ� �ʱ�ȭ 
	// j�κ��� x �� ���� (column), i�κ��� y�� ����(row)�� �׸��� ���Ͽ� ���
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_a_face[i][j] = CRect(ptChild.x + j*64, ptChild.y + i*64, ptChild.x + 64 + j*64, ptChild.y + 64 + i*64);
		}
	}

	// b (��Ÿ)
	// Polygon ���� (�迭����)
	// 0 : ���� ��, 1 : ������ ��, 2 : ������ �Ʒ�, 3 : ���� �Ʒ� (����)
	// �������� ptChiuld.x + 90�� ptChild.y -90. �� ���� m_b_face[0][0]�� ����
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

	// r (����)
	// Polygon ���� (�迭����)
	// 0 : ���� ��, 1 : ������ ��, 2 : ������ �Ʒ�, 3 : ���� �Ʒ� (����)
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			// ���� (ptChild.x + 64*3, ptChild.y)
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CRubiksCubeView �׸���

void CRubiksCubeView::OnDraw(CDC* pDC)
{
	CRubiksCubeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	// �׸� �׸��� �������� ����
	// 1. Document�� �����ϴ� m_RubiksCube�� ���� �����´�.
	// 2. m_RubiksCube�� a�� �ش��ϴ� Face_Header�� ��� b�� �ش��ϴ� Face_Header->GetP_Up()�� r�� �ش��ϴ� Face_Header->GetP_Next()�� unique value�� �����´�.
	// 3. �� ���� ������ �׸��� �׸� �� ���� ��ȯ�Ѵ�.
	// * �߰������� ��� ���� �޸𸮸� �Ҵ� ������ �� �ֵ��� �Ѵ�.

	// Header�� a�� ��Ī�ϱ� ���Ͽ� pDoc���� �ڷḦ �����´�.
	int** a_element_values = pDoc->m_RubiksCube.GetFace_Header()->GetM_element_values();

	// a �κ� ���
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			CBrush rectangle_colora = color_mapping(a_element_values[i][j]);
			CBrush* pOldBrush = pDC->SelectObject(&rectangle_colora);

			pDC->Rectangle(m_a_face[i][j]);		// �簢���� ����ϴ� �κ�
			
			if(m_bIsColorOnly == false)
			{
				// ������ background color�� transparent�� �ٲ�
				pDC->SetBkMode(TRANSPARENT);
				// ���ڸ� �����ϴ� �κ�, ���� ��� ��ġ�� CRect�� (Left + 25, Top + 25)���� ����
				CString numberString;
				numberString.Format(_T("%d"), a_element_values[i][j]);
				pDC->TextOutW(m_a_face[i][j].left + 25, m_a_face[i][j].top + 25, numberString);
			}
			// Brush �� ����
			pDC->SelectObject(pOldBrush);

		}
	}

	// ���� �޸� ����
	for(int i = 0; i < 3; i++)
	{
		delete[] a_element_values[i];
	}
	delete a_element_values;
	a_element_values = 0;




	// Header->Up�� b�� ��Ī�ϱ� ���Ͽ� pDoc���� �ڷḦ �����´�.
	int** b_element_values = pDoc->m_RubiksCube.GetFace_Header()->GetP_Up()->GetM_element_values();
	// b �κ� ���
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			CBrush polygon_colorb = color_mapping(b_element_values[i][j]);
			CBrush* pOldBrush = pDC->SelectObject(&polygon_colorb);

			pDC->Polygon(m_b_face[i][j], 4);

			if(m_bIsColorOnly == false)
			{
				// ������ background color�� transparent�� �ٲ�
				pDC->SetBkMode(TRANSPARENT);
				// ���ڸ� �����ϴ� �κ�, ���� ��� ��ġ�� ���� �� ���� (x + 8, y + 8)�� ����
				CString numberString;
				numberString.Format(_T("%d"), b_element_values[i][j]);
				pDC->TextOutW(m_b_face[i][j][0].x + 8, m_b_face[i][j][0].y + 8, numberString);
			}

			// Brush �� ����
			pDC->SelectObject(pOldBrush);
		}
	}

	// ���� �޸� ����
	for(int i = 0; i < 3; i++)
	{
		delete[] b_element_values[i];
	}
	delete b_element_values;
	b_element_values = 0;


	// Header->Next�� r�� ��Ī�ϱ� ���Ͽ� pDoc���� �ڷḦ �����´�.
	int** r_element_values = pDoc->m_RubiksCube.GetFace_Header()->GetP_Next()->GetM_element_values();
	// r �κ� ���
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			CBrush polygon_colorr = color_mapping(r_element_values[i][j]);
			CBrush* pOldBrush = pDC->SelectObject(&polygon_colorr);

			pDC->Polygon(m_r_face[i][j], 4);

			if(m_bIsColorOnly == false)
			{
				// ������ background color�� transparent�� �ٲ�
				pDC->SetBkMode(TRANSPARENT);
				// ���ڸ� �����ϴ� �κ�, ���� ��� ��ġ�� ���� �� ���� (x + 8, y + 8)�� ����
				CString numberString;
				numberString.Format(_T("%d"), r_element_values[i][j]);
				pDC->TextOutW(m_r_face[i][j][0].x + 8, m_r_face[i][j][0].y + 8, numberString);
			}

			// Brush �� ����
			pDC->SelectObject(pOldBrush);
		}
	}


	// ���� �޸� ����
	for(int i = 0; i < 3; i++)
	{
		delete[] r_element_values[i];
	}
	delete r_element_values;
	r_element_values = 0;
	

	// Keydown�����
	pDC->SetBkMode(TRANSPARENT);
	if(m_bKeyboardLogOn)
		pDC->DrawTextW(keydownLog, CRect(800, 210, 900, 500), DT_NOCLIP);

	// Mouse Drag log �����
	if(m_bMouseLogOn)
		pDC->DrawTextW(MouseLog, CRect(900, 210, 1300, 500), DT_NOCLIP);
}


// CRubiksCubeView �μ�

BOOL CRubiksCubeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CRubiksCubeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CRubiksCubeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CRubiksCubeView ����

#ifdef _DEBUG
void CRubiksCubeView::AssertValid() const
{
	CView::AssertValid();
}

void CRubiksCubeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRubiksCubeDoc* CRubiksCubeView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRubiksCubeDoc)));
	return (CRubiksCubeDoc*)m_pDocument;
}
#endif //_DEBUG


// CRubiksCubeView �޽��� ó����


void CRubiksCubeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	// �α� ������ FraME�κп��� ������ �ϳ�?

	// Document�� �����´�.
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

	// �׸��� �ٽ� �׷� �ش�.
	RedrawWindow();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CRubiksCubeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// ��𿡼� LButtonDown Ȥ��  LButtonUp�� ȣ��Ǿ����� ��ġ�� ã���ִ� �޼ҵ� ȣ�� : �޼ҵ�� RubiksCubeView���� ����
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnMouseMove(nFlags, point);
}


void CRubiksCubeView::OnColorOnly()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// TODO : ���⿡�� ���ڸ� �Ⱥ��̵��� ���� �� �׸��� �ٽ� �׸����� ȣ��
	//MessageBox(_T("print color only"));
	m_bIsColorOnly = true;

	RedrawWindow();		// ���� ������ �ٷ� ���̵��� RedrawWindow() �ǽ�
}


void CRubiksCubeView::OnColorWithNumber()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//MessageBox(_T("print color & number"));
	m_bIsColorOnly = false;
	RedrawWindow();		// ���� ������ �ٷ� ���̵��� RedrawWindow() �ǽ�
}


void CRubiksCubeView::OnUpdateColorOnly(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	
	// �޴����� ���� ��� üũ �������� �ƴ����� Ȯ�� �ϱ� ���� ���ǹ�
	if(m_bIsColorOnly)	// ���� ����� ���� �Ǿ�����
		pCmdUI->SetCheck(1);	// ���� ��� �޴��� üũ
	else // ���� ����� ���� �Ǿ����� ����
		pCmdUI->SetCheck(0);	// ���� ��� �޴��� üũ ����
}



void CRubiksCubeView::OnUpdateColorWithNumber(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
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

// PtInPoly �Լ� ����
int pnpoly(CPoint* polygonPoint, CPoint point, int iFaceValue)	// ù ��° ���ڴ� ������ 4���� ����Ʈ�̸�, �� ��° ���ڴ� Ŭ���� ����Ʈ, �� ��° ���ڴ� ���� �����ϰ� �ִ� ��(face)��
{
	/* Ŭ���� �Ǿ����� ��ġ�� Ȯ���ϴ� ��� */
	// 1. ���� �簢�� (������ cpoint�� ������ �Ʒ� cpoint)�ȿ� �����ϸ� (CRect.PtInRect���� Ȯ��)  ������ ���� 1
	// Polygon ���� (�迭����)
	// 0 : ���� ��, 1 : ������ ��, 2 : ������ �Ʒ�, 3 : ���� �Ʒ� (����)
	// ���� �簢��, ���� ��, ������ �Ʒ�
	CRect smallRect (polygonPoint[0], polygonPoint[2]);
	if(smallRect.PtInRect (point))
	{
		return 1 ;
	}

	// 2. ū �簢�� �ȿ� �����ϸ�
	//CRect largeRect(polygonPoint[3], polygonPoint[1]);
	CRect largeRect( polygonPoint[3].x, polygonPoint[1].y, polygonPoint[1].x, polygonPoint[3].y);
	if(largeRect.PtInRect(point))
	{
		// ���⼭ �����ϸ� �ﰢ���� ��ʽ����� Ȯ��
		if ( iFaceValue == 1)    // B ��Ÿ
		{
			// ū �簢���� ���� �κ����� ������ �κ����� Ȯ��
			//CRect leftRect( polygonPoint[3], polygonPoint[0]);
			CRect leftRect( polygonPoint[3].x, polygonPoint[0].y, polygonPoint[0].x, polygonPoint[3].y);
			//CRect rightRect( polygonPoint[1], polygonPoint[2]);
			CRect rightRect( polygonPoint[2].x, polygonPoint[1].y, polygonPoint[1].x, polygonPoint[2].y);

			if (leftRect.PtInRect(point))
			{
				// ���ʿ� ���� ��� : point ���� �����ʿ� ������ �� �簢�� ���� ����
				// ���� �簢�� ���� ������ �� �ﰢ���� ��ʽ��� ����Ͽ� �ش� ���� ���� �����ϴ��� Ȯ��
				// �غ� : �غ����� = ���� : ���� ���� <= �̿� ���� ������ ����Ͽ� ���̸� ���ϰ� 
				// ���̺��� ũ�� �ۿ� ����, ������ ���� �������� Ȯ��
				// ���� ���� = ���� x �غ� ���� / �غ�

				double boundaryLineHeight;
				boundaryLineHeight = (double)(point.x  - polygonPoint[3].x )*(polygonPoint[3].y - polygonPoint[0].y)/(double)(polygonPoint[0].x - polygonPoint[3].x);
				if(polygonPoint[3].y- point.y < boundaryLineHeight)
					return 1;
			}
			else if (rightRect.PtInRect( point ))
			{
				// �����ʿ� ���� ���
				double boundaryLineHeight;
				boundaryLineHeight = (double)(point.x  - polygonPoint[2].x )*(polygonPoint[2].y - polygonPoint[1].y)/(double)(polygonPoint[1].x - polygonPoint[2].x);
				if(polygonPoint[2].y- point.y > boundaryLineHeight)
					return 1;
			}

		}
		else if (iFaceValue == 2 ) // R ����
		{
			// 2.1 ����(or ����) ���� �簢���� �����ϴ��� Ȯ�� or ������(or �Ʒ���) ���� �簢���� �����ϴ��� Ȯ��
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
			// 2.2 ���� �˻� ���� �� �����ϸ� return 1, �������� ������ return 0
			// * ���� �˻�� �ﰢ���� ��ʽ����� ����
			// ū �簢�� �ȿ� �������� ������ return 0
		}
	}

	return 0;


}

// ��𿡼� LButtonDown Ȥ��  LButtonUp�� ȣ��Ǿ����� ��ġ�� ã���ִ� �޼ҵ�
//0~8�� ���� // 9~17�� ��Ÿ // 18~26�� �����̸�, ���� 012�� 1�� 1��2��3�� ������� ���� ����
// -1�� ���� �� ť�� �̿��� �� Ŭ��
int CRubiksCubeView::WhereIClick(CPoint point)
{

	// PtIntRect�Լ��� ����Ͽ��� ��� Ŭ���Ͽ����� Ȯ��
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

	// FACE �� ǥ�� (�α׸� ���� ����)
	CString FaceString;
	int RowOrColNum;

	// ���� face���� ���콺 dwon �� up�� �Ͼ���� ��ȿ�� �˻�
	// ���ÿ� �˹��̰ų� ��Ÿ�̰ų� ������ ��츸 ����
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
		
		
		// ��ȿ�� �� Ȯ��
		if(faceValue == -1)
			MessageBox(_T("facevalueerror"));

		CRubiksCubeDoc* pDoc = GetDocument();

		int executeValue = endPointValue - startPointValue;

		switch(executeValue)
		{
		case 2:
			if(startPointValue % 9 == 0)
			{
				pDoc->m_RubiksCube.RotateFace(faceValue, 4);	// Roate Face ����
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

			// Mouse �α�
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

			// Mouse �α�
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

			// Mouse �α�
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
			
			// Mouse �α�
			MouseLogCnt++;
			MouseLog.AppendFormat(_T("%d. FACE : %s / Col : %d / Up"), MouseLogCnt, FaceString, RowOrColNum);
			MouseLog += "\r\n";

			break;
		default:
			//MessageBox(_T("error"));	// ��ȿ�� �˻�
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if(m_bKeyboardLogOn == false)
		m_bKeyboardLogOn = true;
	else if(m_bKeyboardLogOn == true)
		m_bKeyboardLogOn = false;
	RedrawWindow();
}


void CRubiksCubeView::OnSliceRotateLog()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if(m_bMouseLogOn == false)
		m_bMouseLogOn = true;
	else if(m_bMouseLogOn == true)
		m_bMouseLogOn = false;
	RedrawWindow();
}


void CRubiksCubeView::OnUpdateFaceChangedLog(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	if(m_bKeyboardLogOn)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CRubiksCubeView::OnUpdateSliceRotateLog(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	if(m_bMouseLogOn)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
