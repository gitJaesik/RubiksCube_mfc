#include "stdafx.h"
#include "Face_list.h"


Face_list::Face_list(void)
{
	Face_Header = NULL;
	Init();
}


Face_list::~Face_list(void)
{
}


int InitiateNumber(Face* pPos, int counter)
{
	int** copy_element_values = pPos->GetM_element_values();

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			copy_element_values[i][j] = counter;
			//pPos->m_unique_value[i][j] = counter;
			counter++;
		}
	}

	pPos->SetM_element_values(copy_element_values);		// 해당 face에 복사한 3 x 3 배열을 통째로 넣는다.

	for(int i = 0; i < 3; i++)
	{
		delete[] copy_element_values[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	}
	delete[] copy_element_values;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	copy_element_values = 0;	// 초기 값을 0으로 준다.

	return counter;
}


void Face_list::Init()
{
	// Header가 NULL이 아니면 동적 메모리를 모두 제거
	if (Face_Header != NULL)
	{
		Clean();
	}

	// 큐브의 6개 면을 생성
	Face* a = new Face('a');
	Face* b = new Face('b');
	Face* c = new Face('c');
	Face* d = new Face('d');
	Face* e = new Face('e');
	Face* f = new Face('f');

	// 노드 연결 초기화
	Face_Header = a;
	a->SetP_Next(b);
	b->SetP_Next(c);
	c->SetP_Next(d);
	d->SetP_Next(a);
	a->SetP_Up(e);
	e->SetP_Up(c);
	c->SetP_Up(f);
	f->SetP_Up(a);

	// 번호 초기화 : raster order
	Face* pPos = Face_Header;
	int counter = 1;
	//순서 : a->b->c->d->e->f;
	counter = InitiateNumber(pPos, counter);
	counter = InitiateNumber(pPos->GetP_Next(), counter);
	counter = InitiateNumber(pPos->GetP_Next()->GetP_Next(), counter);
	counter = InitiateNumber(pPos->GetP_Next()->GetP_Next()->GetP_Next(), counter);
	counter = InitiateNumber(pPos->GetP_Up(), counter);
	counter = InitiateNumber(pPos->GetP_Up()->GetP_Up()->GetP_Up(), counter);
}


void Face_list::Clean()
{
	// 에러 처리 : Header가 NULL인 경우는 동적 메모리를 지울 필요가 없음
	if (Face_Header == NULL)
	{
		//MessageBox(NULL, _T("There is no dynamic memory to remove!"), _T("error"), MB_ICONERROR | MB_OK);
		return;
	}

	// 삭제 시 시작을 a로 가정하며 실행
	// cube 방식으로 삭제
	// while문을 사용할 필요 없으며, cube에 맞추어 삭제
	// 여기서 사용할 삭제 순서는 right->right->right->top->top->top

	Face* pPos = Face_Header;
	delete pPos->GetP_Next()->GetP_Next()->GetP_Next();
	delete pPos->GetP_Up()->GetP_Up()->GetP_Up();
	delete pPos->GetP_Next()->GetP_Next();
	delete pPos->GetP_Next();
	delete pPos->GetP_Up();

	Face_Header = NULL;

	//AfxGetMainWnd()->MessageBox(_T("Clean well!"));
}

bool Face_list::RotateFace(int iSelectedFace, int iDirection)
{
	// error 처리
	if (iSelectedFace > 3 || iSelectedFace < 0 || iDirection > 12 || iDirection < 0)
	{
		AfxGetMainWnd()->MessageBox(_T("error 발생"));
		return false;
	}

	switch (iSelectedFace)
	{
	case 0:
		// a (알파) Face를 선택한 경우, Header를 옮겨서 재 설정하여 진행 할 필요 없음
		RotateFaceAllDirection(iDirection);
		break;
	case 1:
		// b (베타) Face를 선택한 경우, Keyboard ↓를 누른 상태로 바꾼 후 다시 Keyboard ↑를 누른 경우로 바꾸기
		// RotateCube : (↑ ↓ ← → 순서대로 0, 1, 2, 3)
		RotateCube(1);
		RotateFaceAllDirection(iDirection);
		RotateCube(0);
		break;
	case 2:
		// c (감마) Face를 선택한 경우, Keyboard ←를 누른 상태로 바꾼 후 다시 Keyboard →를 누른 경우로 바꾸기
		RotateCube(2);
		RotateFaceAllDirection(iDirection);
		RotateCube(3);
		break;
	default :
		AfxGetMainWnd()->MessageBox(_T("error 발생"));
		break;

	}

	return true;
}


// ↑ ↓ ← → 순서대로 0, 1, 2, 3
bool Face_list::RotateCube(int iCubeDirection)
{
	switch (iCubeDirection)
	{
	case 0: // ↑ (위쪽 방향)
		RotateCubeUp();
		break;
	case 1:	// ↓ (아래쪽 방향)
		RotateCubeUp();
		RotateCubeUp();
		RotateCubeUp();
		break;
	case 2:	// ← (왼쪽 방향)
		RotateCubeRight();
		RotateCubeRight();
		RotateCubeRight();
		break;
	case 3: // → (오른쪽 방향)
		RotateCubeRight();
		break;
	default:
		break;
	}
	return true;
}


bool Face_list::RotateCubeUp()
{
	// a를 기준이었을 시 f가 새로운 Header가 되는 상황
	//   e
	//   a  b c d
	//   f
	//  모두 Spin상태는 정상

	//   a
	//   f b e d
	//   c 
	//  Spin 상태 정상 : f, a, e  // 왼쪽으로 회전 : d, 오른쪽으로 회전 : b // 180도로 회전 : e, c

	// 설정 부분
	Face* pNewHeader = Face_Header->GetP_Up()->GetP_Up()->GetP_Up();		// f를 새로운 Header로 선택
	
	pNewHeader->SetP_Next(Face_Header->GetP_Next());		// f->p_Next = b;
	Face_Header->SetP_Next(NULL);						// a->p_Next = NULL;

	pNewHeader->GetP_Next()->SetP_Next(pNewHeader->GetP_Up()->GetP_Up());		// f->p_Next->p_Next = e

	// f->p_Next->p_Next->p_Next = d
	pNewHeader->GetP_Next()->GetP_Next()->SetP_Next(Face_Header->GetP_Up()->GetP_Up()->GetP_Next());	
	Face_Header->GetP_Up()->GetP_Up()->SetP_Next(NULL);		// a->p_Next->p_Next->p_Next = NULL; c->p_Next = NULL;

	// d->p_Next = f
	pNewHeader->GetP_Next()->GetP_Next()->GetP_Next()->SetP_Next(pNewHeader);

	Face_Header = pNewHeader;
	
	// 회전 부분
	SpinLeft(Face_Header->GetP_Next()->GetP_Next()->GetP_Next());	// d를 왼쪽으로 회전
	SpinRight(Face_Header->GetP_Next());		// b를 오른쪽으로 회전 
	Spin180(Face_Header->GetP_Up()->GetP_Up());			// e를 180도 회전
	Spin180(Face_Header->GetP_Up()->GetP_Up()->GetP_Up());			//  c를 180도 회전
	
	return true;
}


bool Face_list::SpinLeft(Face* SelectedFace)
{
	// 에러 처리
	if (SelectedFace->GetM_face_name() < 'a' && SelectedFace->GetM_face_name() > 'f')
	{
		AfxGetMainWnd()->MessageBox(_T("error 발생 : wrong face name or wrong face pointer"));
		return false;
	}

	int** spin_element_values_to_insert_later = SelectedFace->GetM_element_values(); // Spin 후 넣을 데아타 Array Pointer 필요 (데이타 구조 복사)
	int** copy_element_values = SelectedFace->GetM_element_values(); // 기존의 데이타를 가져온다. Array의 포인터를 사용

	/* 스핀 시작 */
	// Left
	spin_element_values_to_insert_later[2][0] = copy_element_values[0][0];
	spin_element_values_to_insert_later[1][0] = copy_element_values[0][1];
	spin_element_values_to_insert_later[0][0] = copy_element_values[0][2];

	// Bottom
	spin_element_values_to_insert_later[2][2] = copy_element_values[2][0];
	spin_element_values_to_insert_later[2][1] = copy_element_values[1][0];
	spin_element_values_to_insert_later[2][0] = copy_element_values[0][0];

	// Right
	spin_element_values_to_insert_later[0][2] = copy_element_values[2][2];
	spin_element_values_to_insert_later[1][2] = copy_element_values[2][1];
	spin_element_values_to_insert_later[2][2] = copy_element_values[2][0];

	// Top
	spin_element_values_to_insert_later[0][0] = copy_element_values[0][2];
	spin_element_values_to_insert_later[0][1] = copy_element_values[1][2];
	spin_element_values_to_insert_later[0][2] = copy_element_values[2][2];


	SelectedFace->SetM_element_values(spin_element_values_to_insert_later);		// 해당 face에 Spin하여 복사한 3 x 3 배열을 통째로 넣는다.


	/* 생성한 동적 메모리를 모두 삭제한다. */
	for(int i = 0; i < 3; i++)
	{
		delete[] copy_element_values[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] spin_element_values_to_insert_later[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	}
	delete[] copy_element_values;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] spin_element_values_to_insert_later;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	copy_element_values = 0;	// 초기 값을 0으로 준다.
	spin_element_values_to_insert_later = 0;	// 초기 값을 0으로 준다.

	return true;
}


bool Face_list::SpinRight(Face* SelectedFace)
{
	SpinLeft(SelectedFace);
	SpinLeft(SelectedFace);
	SpinLeft(SelectedFace);
	return true;
}


bool Face_list::Spin180(Face* SelectedFace)
{
	SpinLeft(SelectedFace);
	SpinLeft(SelectedFace);
	return true;
}


bool Face_list::RotateCubeRight()
{
	// a 를 기준 시 b가 새로운 Header가 되는 상황
	//   e
	// d a b c
	//   f
	// 에서
	//   e
	// c d a b
	//   f
	// 로 바뀜
	//  Spin 상태 정상 : a, b, c, d  // 왼쪽으로 회전 : f, 오른쪽으로 회전 : e 

	// 설정 부분
	Face* pNewHeader = Face_Header->GetP_Next()->GetP_Next()->GetP_Next();		// d를 새로운 헤더로 선택
	
	pNewHeader->SetP_Up(Face_Header->GetP_Up());		// d의 위를 e로 설정
	Face_Header->SetP_Up(NULL);						// a의 up을 NULL로 설정

	pNewHeader->GetP_Up()->SetP_Up(pNewHeader->GetP_Next()->GetP_Next());	// d의 위는 e, e의 위는 b

	pNewHeader->GetP_Next()->GetP_Next()->SetP_Up(Face_Header->GetP_Next()->GetP_Next()->GetP_Up()); // b의 위는 c의 위였던 f
	Face_Header->GetP_Next()->GetP_Next()->SetP_Up(NULL);			// c의 위를 null로 설정

	pNewHeader->GetP_Up()->GetP_Up()->GetP_Up()->SetP_Up(pNewHeader);		// f의 위를 d로 설정

	Face_Header = pNewHeader;		// Header를 b로 설정

	// 회전 부분
	SpinRight(Face_Header->GetP_Up()->GetP_Up()->GetP_Up());	// f를 오른쪽으로 회전
	SpinLeft(Face_Header->GetP_Up());		 // e를 왼쪽으로 회전

	return true;
}


bool Face_list::RotateFaceAllDirection(int iDirection)
{
	switch(iDirection)
	{
	case 1: case 2: case 3:
		RotateFaceToUp(iDirection);
		break;
	case 4: case 5: case 6:
		RotateFaceToRight(iDirection);
		break;
	case 7:
		// RotateFaceToUp(3)을 3번 실행한 것과 같다.
		RotateFaceToUp(3);
		RotateFaceToUp(3);
		RotateFaceToUp(3);
		break;
	case 8:
		RotateFaceToUp(2);
		RotateFaceToUp(2);
		RotateFaceToUp(2);
		break;
	case 9:
		RotateFaceToUp(1);
		RotateFaceToUp(1);
		RotateFaceToUp(1);
		break;
	case 10:
		// RotateFaceToRight(6)을 3번 수행한 것과 같다.
		RotateFaceToRight(6);
		RotateFaceToRight(6);
		RotateFaceToRight(6);
		break;
	case 11:
		RotateFaceToRight(5);
		RotateFaceToRight(5);
		RotateFaceToRight(5);
		break;
	case 12:
		RotateFaceToRight(4);
		RotateFaceToRight(4);
		RotateFaceToRight(4);
		break;
	default:
		AfxGetMainWnd()->MessageBox(_T("Error come up! wrong parameter"));
		break;
	}

	return true;
}

// 2015.06.14
// c에대한 오류 디버깅
bool Face_list::RotateFaceToUp(int iDirection)
{
	// 현재 Face_Header에서 작업을 진행 (미리 Cube의 회전으로 항상 Header에서 Rotate를 하도록 설계)
	// 1, 2, 3 중에서만 입력을 받으며 1 혹은 3일 경우 왼쪽이 spinleft 혹은 오른쪽이 spinright를 진행
	if(iDirection < 1 || iDirection > 3)
	{
		AfxGetMainWnd()->MessageBox(_T("Error come up! wrong parameter"));
		return false;
	}

	// Face가 a일 경우를 기준으로 a, e, c, f가 값이 변함
	
	// a, e, c, f 값을 모두 복사함
	int** shift_element_valuesA = Face_Header->GetM_element_values();
	int** shift_element_valuesE = Face_Header->GetP_Up()->GetM_element_values();
	int** shift_element_valuesC = Face_Header->GetP_Up()->GetP_Up()->GetM_element_values();
	int** shift_element_valuesF = Face_Header->GetP_Up()->GetP_Up()->GetP_Up()->GetM_element_values();

	int** shift_element_valuesACopy = Face_Header->GetM_element_values();		// 복사를 위한 element_values
	
	// F -> A 로 값 Shift
	shift_element_valuesA[0][iDirection-1] = shift_element_valuesF[0][iDirection-1];
	shift_element_valuesA[1][iDirection-1] = shift_element_valuesF[1][iDirection-1];
	shift_element_valuesA[2][iDirection-1] = shift_element_valuesF[2][iDirection-1];

	// C -> F 로 값 Shift
	shift_element_valuesF[0][iDirection-1] = shift_element_valuesC[0][2-(iDirection-1)];
	shift_element_valuesF[1][iDirection-1] = shift_element_valuesC[1][2-(iDirection-1)];
	shift_element_valuesF[2][iDirection-1] = shift_element_valuesC[2][2-(iDirection-1)];

	// E -> C 로 값 Shift
	shift_element_valuesC[0][2-(iDirection-1)] = shift_element_valuesE[0][iDirection-1];
	shift_element_valuesC[1][2-(iDirection-1)] = shift_element_valuesE[1][iDirection-1];
	shift_element_valuesC[2][2-(iDirection-1)] = shift_element_valuesE[2][iDirection-1];

	// A -> E 로 값 Shift
	shift_element_valuesE[0][iDirection-1] = shift_element_valuesACopy[0][iDirection-1];
	shift_element_valuesE[1][iDirection-1] = shift_element_valuesACopy[1][iDirection-1];
	shift_element_valuesE[2][iDirection-1] = shift_element_valuesACopy[2][iDirection-1];


	// A, E, C, F 재 설정
	Face_Header->SetM_element_values(shift_element_valuesA);
	Face_Header->GetP_Up()->SetM_element_values(shift_element_valuesE);
	Face_Header->GetP_Up()->GetP_Up()->SetM_element_values(shift_element_valuesC);
	Face_Header->GetP_Up()->GetP_Up()->GetP_Up()->SetM_element_values(shift_element_valuesF);

	// 만약 iDirection이 1이면 D를 SpinLeft iDirection이 3이면 B를 SpinRight
	if(iDirection == 1)
		SpinLeft(Face_Header->GetP_Next()->GetP_Next()->GetP_Next());
	else if(iDirection == 3)
		SpinRight(Face_Header->GetP_Next());

	//* 생성한 동적 메모리를 모두 삭제한다. */
	for(int i = 0; i < 3; i++)
	{
		delete[] shift_element_valuesA[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] shift_element_valuesE[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] shift_element_valuesC[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] shift_element_valuesF[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] shift_element_valuesACopy[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	}
	delete[] shift_element_valuesA;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] shift_element_valuesE;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] shift_element_valuesC;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] shift_element_valuesF;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] shift_element_valuesACopy;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	shift_element_valuesA = 0;	// 초기 값을 0으로 준다.
	shift_element_valuesE = 0;	// 초기 값을 0으로 준다.
	shift_element_valuesC = 0;	// 초기 값을 0으로 준다.
	shift_element_valuesF = 0;	// 초기 값을 0으로 준다.
	shift_element_valuesACopy = 0;	// 초기 값을 0으로 준다.

	return true;
}





bool Face_list::RotateFaceToRight(int iDirection)
{
	// 현재 Face_Header에서 작업을 진행 (미리 Cube의 회전으로 항상 Header에서 Rotate를 하도록 설계)
	// 1, 2, 3 중에서만 입력을 받으며 1 혹은 3일 경우 왼쪽이 spinleft 혹은 오른쪽이 spinright를 진행
	if(iDirection < 4 || iDirection > 6)
	{
		AfxGetMainWnd()->MessageBox(_T("Error come up! wrong parameter"));
		return false;
	}

	// Face가 a일 경우를 기준으로 a, b, c, d가 값이 변함
	
	// a, b, c, d 값을 모두 복사함
	int** shift_element_valuesA = Face_Header->GetM_element_values();
	int** shift_element_valuesB = Face_Header->GetP_Next()->GetM_element_values();
	int** shift_element_valuesC = Face_Header->GetP_Next()->GetP_Next()->GetM_element_values();
	int** shift_element_valuesD = Face_Header->GetP_Next()->GetP_Next()->GetP_Next()->GetM_element_values();

	int** shift_element_valuesACopy = Face_Header->GetM_element_values();		// 복사를 위한 element_values
	
	// D -> A 로 값 Shift
	shift_element_valuesA[iDirection-4][0] = shift_element_valuesD[iDirection-4][0];
	shift_element_valuesA[iDirection-4][1] = shift_element_valuesD[iDirection-4][1];
	shift_element_valuesA[iDirection-4][2] = shift_element_valuesD[iDirection-4][2];

	// C -> D 로 값 Shift
	shift_element_valuesD[iDirection-4][0] = shift_element_valuesC[iDirection-4][0];
	shift_element_valuesD[iDirection-4][1] = shift_element_valuesC[iDirection-4][1];
	shift_element_valuesD[iDirection-4][2] = shift_element_valuesC[iDirection-4][2];

	// B -> C 로 값 Shift
	shift_element_valuesC[iDirection-4][0] = shift_element_valuesB[iDirection-4][0];
	shift_element_valuesC[iDirection-4][1] = shift_element_valuesB[iDirection-4][1];
	shift_element_valuesC[iDirection-4][2] = shift_element_valuesB[iDirection-4][2];

	// A -> B 로 값 Shift
	shift_element_valuesB[iDirection-4][0] = shift_element_valuesACopy[iDirection-4][0];
	shift_element_valuesB[iDirection-4][1] = shift_element_valuesACopy[iDirection-4][1];
	shift_element_valuesB[iDirection-4][2] = shift_element_valuesACopy[iDirection-4][2];



	// A, B, C, D 재 설정
	Face_Header->SetM_element_values(shift_element_valuesA);
	Face_Header->GetP_Next()->SetM_element_values(shift_element_valuesB);
	Face_Header->GetP_Next()->GetP_Next()->SetM_element_values(shift_element_valuesC);
	Face_Header->GetP_Next()->GetP_Next()->GetP_Next()->SetM_element_values(shift_element_valuesD);

	// 만약 iDirection이 4이면 E를 SpinLeft iDirection이 6이면 F를 SpinRight
	if(iDirection == 4)
		SpinLeft(Face_Header->GetP_Up());
	else if(iDirection == 6)
		SpinRight(Face_Header->GetP_Up()->GetP_Up()->GetP_Up());

	//* 생성한 동적 메모리를 모두 삭제한다. */
	for(int i = 0; i < 3; i++)
	{
		delete[] shift_element_valuesA[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] shift_element_valuesB[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] shift_element_valuesC[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] shift_element_valuesD[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
		delete[] shift_element_valuesACopy[i];	// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	}
	delete[] shift_element_valuesA;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] shift_element_valuesB;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] shift_element_valuesC;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] shift_element_valuesD;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	delete[] shift_element_valuesACopy;		// 복사가 끝났으니 GetM_element_values 메소드에서 생성한 동적 메모리를 다시 삭제한다.
	shift_element_valuesA = 0;	// 초기 값을 0으로 준다.
	shift_element_valuesB = 0;	// 초기 값을 0으로 준다.
	shift_element_valuesC = 0;	// 초기 값을 0으로 준다.
	shift_element_valuesD = 0;	// 초기 값을 0으로 준다.
	shift_element_valuesACopy = 0;	// 초기 값을 0으로 준다.

	return true;
}


Face* Face_list::GetFace_Header(void)
{
	// 외부 접근이 가능하게 하기 위하여 만든 Get 메소드 
	return Face_Header;
}
