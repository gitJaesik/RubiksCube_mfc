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

	pPos->SetM_element_values(copy_element_values);		// �ش� face�� ������ 3 x 3 �迭�� ��°�� �ִ´�.

	for(int i = 0; i < 3; i++)
	{
		delete[] copy_element_values[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	}
	delete[] copy_element_values;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	copy_element_values = 0;	// �ʱ� ���� 0���� �ش�.

	return counter;
}


void Face_list::Init()
{
	// Header�� NULL�� �ƴϸ� ���� �޸𸮸� ��� ����
	if (Face_Header != NULL)
	{
		Clean();
	}

	// ť���� 6�� ���� ����
	Face* a = new Face('a');
	Face* b = new Face('b');
	Face* c = new Face('c');
	Face* d = new Face('d');
	Face* e = new Face('e');
	Face* f = new Face('f');

	// ��� ���� �ʱ�ȭ
	Face_Header = a;
	a->SetP_Next(b);
	b->SetP_Next(c);
	c->SetP_Next(d);
	d->SetP_Next(a);
	a->SetP_Up(e);
	e->SetP_Up(c);
	c->SetP_Up(f);
	f->SetP_Up(a);

	// ��ȣ �ʱ�ȭ : raster order
	Face* pPos = Face_Header;
	int counter = 1;
	//���� : a->b->c->d->e->f;
	counter = InitiateNumber(pPos, counter);
	counter = InitiateNumber(pPos->GetP_Next(), counter);
	counter = InitiateNumber(pPos->GetP_Next()->GetP_Next(), counter);
	counter = InitiateNumber(pPos->GetP_Next()->GetP_Next()->GetP_Next(), counter);
	counter = InitiateNumber(pPos->GetP_Up(), counter);
	counter = InitiateNumber(pPos->GetP_Up()->GetP_Up()->GetP_Up(), counter);
}


void Face_list::Clean()
{
	// ���� ó�� : Header�� NULL�� ���� ���� �޸𸮸� ���� �ʿ䰡 ����
	if (Face_Header == NULL)
	{
		//MessageBox(NULL, _T("There is no dynamic memory to remove!"), _T("error"), MB_ICONERROR | MB_OK);
		return;
	}

	// ���� �� ������ a�� �����ϸ� ����
	// cube ������� ����
	// while���� ����� �ʿ� ������, cube�� ���߾� ����
	// ���⼭ ����� ���� ������ right->right->right->top->top->top

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
	// error ó��
	if (iSelectedFace > 3 || iSelectedFace < 0 || iDirection > 12 || iDirection < 0)
	{
		AfxGetMainWnd()->MessageBox(_T("error �߻�"));
		return false;
	}

	switch (iSelectedFace)
	{
	case 0:
		// a (����) Face�� ������ ���, Header�� �Űܼ� �� �����Ͽ� ���� �� �ʿ� ����
		RotateFaceAllDirection(iDirection);
		break;
	case 1:
		// b (��Ÿ) Face�� ������ ���, Keyboard �鸦 ���� ���·� �ٲ� �� �ٽ� Keyboard �踦 ���� ���� �ٲٱ�
		// RotateCube : (�� �� �� �� ������� 0, 1, 2, 3)
		RotateCube(1);
		RotateFaceAllDirection(iDirection);
		RotateCube(0);
		break;
	case 2:
		// c (����) Face�� ������ ���, Keyboard �縦 ���� ���·� �ٲ� �� �ٽ� Keyboard �渦 ���� ���� �ٲٱ�
		RotateCube(2);
		RotateFaceAllDirection(iDirection);
		RotateCube(3);
		break;
	default :
		AfxGetMainWnd()->MessageBox(_T("error �߻�"));
		break;

	}

	return true;
}


// �� �� �� �� ������� 0, 1, 2, 3
bool Face_list::RotateCube(int iCubeDirection)
{
	switch (iCubeDirection)
	{
	case 0: // �� (���� ����)
		RotateCubeUp();
		break;
	case 1:	// �� (�Ʒ��� ����)
		RotateCubeUp();
		RotateCubeUp();
		RotateCubeUp();
		break;
	case 2:	// �� (���� ����)
		RotateCubeRight();
		RotateCubeRight();
		RotateCubeRight();
		break;
	case 3: // �� (������ ����)
		RotateCubeRight();
		break;
	default:
		break;
	}
	return true;
}


bool Face_list::RotateCubeUp()
{
	// a�� �����̾��� �� f�� ���ο� Header�� �Ǵ� ��Ȳ
	//   e
	//   a  b c d
	//   f
	//  ��� Spin���´� ����

	//   a
	//   f b e d
	//   c 
	//  Spin ���� ���� : f, a, e  // �������� ȸ�� : d, ���������� ȸ�� : b // 180���� ȸ�� : e, c

	// ���� �κ�
	Face* pNewHeader = Face_Header->GetP_Up()->GetP_Up()->GetP_Up();		// f�� ���ο� Header�� ����
	
	pNewHeader->SetP_Next(Face_Header->GetP_Next());		// f->p_Next = b;
	Face_Header->SetP_Next(NULL);						// a->p_Next = NULL;

	pNewHeader->GetP_Next()->SetP_Next(pNewHeader->GetP_Up()->GetP_Up());		// f->p_Next->p_Next = e

	// f->p_Next->p_Next->p_Next = d
	pNewHeader->GetP_Next()->GetP_Next()->SetP_Next(Face_Header->GetP_Up()->GetP_Up()->GetP_Next());	
	Face_Header->GetP_Up()->GetP_Up()->SetP_Next(NULL);		// a->p_Next->p_Next->p_Next = NULL; c->p_Next = NULL;

	// d->p_Next = f
	pNewHeader->GetP_Next()->GetP_Next()->GetP_Next()->SetP_Next(pNewHeader);

	Face_Header = pNewHeader;
	
	// ȸ�� �κ�
	SpinLeft(Face_Header->GetP_Next()->GetP_Next()->GetP_Next());	// d�� �������� ȸ��
	SpinRight(Face_Header->GetP_Next());		// b�� ���������� ȸ�� 
	Spin180(Face_Header->GetP_Up()->GetP_Up());			// e�� 180�� ȸ��
	Spin180(Face_Header->GetP_Up()->GetP_Up()->GetP_Up());			//  c�� 180�� ȸ��
	
	return true;
}


bool Face_list::SpinLeft(Face* SelectedFace)
{
	// ���� ó��
	if (SelectedFace->GetM_face_name() < 'a' && SelectedFace->GetM_face_name() > 'f')
	{
		AfxGetMainWnd()->MessageBox(_T("error �߻� : wrong face name or wrong face pointer"));
		return false;
	}

	int** spin_element_values_to_insert_later = SelectedFace->GetM_element_values(); // Spin �� ���� ����Ÿ Array Pointer �ʿ� (����Ÿ ���� ����)
	int** copy_element_values = SelectedFace->GetM_element_values(); // ������ ����Ÿ�� �����´�. Array�� �����͸� ���

	/* ���� ���� */
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


	SelectedFace->SetM_element_values(spin_element_values_to_insert_later);		// �ش� face�� Spin�Ͽ� ������ 3 x 3 �迭�� ��°�� �ִ´�.


	/* ������ ���� �޸𸮸� ��� �����Ѵ�. */
	for(int i = 0; i < 3; i++)
	{
		delete[] copy_element_values[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] spin_element_values_to_insert_later[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	}
	delete[] copy_element_values;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] spin_element_values_to_insert_later;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	copy_element_values = 0;	// �ʱ� ���� 0���� �ش�.
	spin_element_values_to_insert_later = 0;	// �ʱ� ���� 0���� �ش�.

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
	// a �� ���� �� b�� ���ο� Header�� �Ǵ� ��Ȳ
	//   e
	// d a b c
	//   f
	// ����
	//   e
	// c d a b
	//   f
	// �� �ٲ�
	//  Spin ���� ���� : a, b, c, d  // �������� ȸ�� : f, ���������� ȸ�� : e 

	// ���� �κ�
	Face* pNewHeader = Face_Header->GetP_Next()->GetP_Next()->GetP_Next();		// d�� ���ο� ����� ����
	
	pNewHeader->SetP_Up(Face_Header->GetP_Up());		// d�� ���� e�� ����
	Face_Header->SetP_Up(NULL);						// a�� up�� NULL�� ����

	pNewHeader->GetP_Up()->SetP_Up(pNewHeader->GetP_Next()->GetP_Next());	// d�� ���� e, e�� ���� b

	pNewHeader->GetP_Next()->GetP_Next()->SetP_Up(Face_Header->GetP_Next()->GetP_Next()->GetP_Up()); // b�� ���� c�� ������ f
	Face_Header->GetP_Next()->GetP_Next()->SetP_Up(NULL);			// c�� ���� null�� ����

	pNewHeader->GetP_Up()->GetP_Up()->GetP_Up()->SetP_Up(pNewHeader);		// f�� ���� d�� ����

	Face_Header = pNewHeader;		// Header�� b�� ����

	// ȸ�� �κ�
	SpinRight(Face_Header->GetP_Up()->GetP_Up()->GetP_Up());	// f�� ���������� ȸ��
	SpinLeft(Face_Header->GetP_Up());		 // e�� �������� ȸ��

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
		// RotateFaceToUp(3)�� 3�� ������ �Ͱ� ����.
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
		// RotateFaceToRight(6)�� 3�� ������ �Ͱ� ����.
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
// c������ ���� �����
bool Face_list::RotateFaceToUp(int iDirection)
{
	// ���� Face_Header���� �۾��� ���� (�̸� Cube�� ȸ������ �׻� Header���� Rotate�� �ϵ��� ����)
	// 1, 2, 3 �߿����� �Է��� ������ 1 Ȥ�� 3�� ��� ������ spinleft Ȥ�� �������� spinright�� ����
	if(iDirection < 1 || iDirection > 3)
	{
		AfxGetMainWnd()->MessageBox(_T("Error come up! wrong parameter"));
		return false;
	}

	// Face�� a�� ��츦 �������� a, e, c, f�� ���� ����
	
	// a, e, c, f ���� ��� ������
	int** shift_element_valuesA = Face_Header->GetM_element_values();
	int** shift_element_valuesE = Face_Header->GetP_Up()->GetM_element_values();
	int** shift_element_valuesC = Face_Header->GetP_Up()->GetP_Up()->GetM_element_values();
	int** shift_element_valuesF = Face_Header->GetP_Up()->GetP_Up()->GetP_Up()->GetM_element_values();

	int** shift_element_valuesACopy = Face_Header->GetM_element_values();		// ���縦 ���� element_values
	
	// F -> A �� �� Shift
	shift_element_valuesA[0][iDirection-1] = shift_element_valuesF[0][iDirection-1];
	shift_element_valuesA[1][iDirection-1] = shift_element_valuesF[1][iDirection-1];
	shift_element_valuesA[2][iDirection-1] = shift_element_valuesF[2][iDirection-1];

	// C -> F �� �� Shift
	shift_element_valuesF[0][iDirection-1] = shift_element_valuesC[0][2-(iDirection-1)];
	shift_element_valuesF[1][iDirection-1] = shift_element_valuesC[1][2-(iDirection-1)];
	shift_element_valuesF[2][iDirection-1] = shift_element_valuesC[2][2-(iDirection-1)];

	// E -> C �� �� Shift
	shift_element_valuesC[0][2-(iDirection-1)] = shift_element_valuesE[0][iDirection-1];
	shift_element_valuesC[1][2-(iDirection-1)] = shift_element_valuesE[1][iDirection-1];
	shift_element_valuesC[2][2-(iDirection-1)] = shift_element_valuesE[2][iDirection-1];

	// A -> E �� �� Shift
	shift_element_valuesE[0][iDirection-1] = shift_element_valuesACopy[0][iDirection-1];
	shift_element_valuesE[1][iDirection-1] = shift_element_valuesACopy[1][iDirection-1];
	shift_element_valuesE[2][iDirection-1] = shift_element_valuesACopy[2][iDirection-1];


	// A, E, C, F �� ����
	Face_Header->SetM_element_values(shift_element_valuesA);
	Face_Header->GetP_Up()->SetM_element_values(shift_element_valuesE);
	Face_Header->GetP_Up()->GetP_Up()->SetM_element_values(shift_element_valuesC);
	Face_Header->GetP_Up()->GetP_Up()->GetP_Up()->SetM_element_values(shift_element_valuesF);

	// ���� iDirection�� 1�̸� D�� SpinLeft iDirection�� 3�̸� B�� SpinRight
	if(iDirection == 1)
		SpinLeft(Face_Header->GetP_Next()->GetP_Next()->GetP_Next());
	else if(iDirection == 3)
		SpinRight(Face_Header->GetP_Next());

	//* ������ ���� �޸𸮸� ��� �����Ѵ�. */
	for(int i = 0; i < 3; i++)
	{
		delete[] shift_element_valuesA[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] shift_element_valuesE[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] shift_element_valuesC[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] shift_element_valuesF[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] shift_element_valuesACopy[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	}
	delete[] shift_element_valuesA;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] shift_element_valuesE;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] shift_element_valuesC;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] shift_element_valuesF;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] shift_element_valuesACopy;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	shift_element_valuesA = 0;	// �ʱ� ���� 0���� �ش�.
	shift_element_valuesE = 0;	// �ʱ� ���� 0���� �ش�.
	shift_element_valuesC = 0;	// �ʱ� ���� 0���� �ش�.
	shift_element_valuesF = 0;	// �ʱ� ���� 0���� �ش�.
	shift_element_valuesACopy = 0;	// �ʱ� ���� 0���� �ش�.

	return true;
}





bool Face_list::RotateFaceToRight(int iDirection)
{
	// ���� Face_Header���� �۾��� ���� (�̸� Cube�� ȸ������ �׻� Header���� Rotate�� �ϵ��� ����)
	// 1, 2, 3 �߿����� �Է��� ������ 1 Ȥ�� 3�� ��� ������ spinleft Ȥ�� �������� spinright�� ����
	if(iDirection < 4 || iDirection > 6)
	{
		AfxGetMainWnd()->MessageBox(_T("Error come up! wrong parameter"));
		return false;
	}

	// Face�� a�� ��츦 �������� a, b, c, d�� ���� ����
	
	// a, b, c, d ���� ��� ������
	int** shift_element_valuesA = Face_Header->GetM_element_values();
	int** shift_element_valuesB = Face_Header->GetP_Next()->GetM_element_values();
	int** shift_element_valuesC = Face_Header->GetP_Next()->GetP_Next()->GetM_element_values();
	int** shift_element_valuesD = Face_Header->GetP_Next()->GetP_Next()->GetP_Next()->GetM_element_values();

	int** shift_element_valuesACopy = Face_Header->GetM_element_values();		// ���縦 ���� element_values
	
	// D -> A �� �� Shift
	shift_element_valuesA[iDirection-4][0] = shift_element_valuesD[iDirection-4][0];
	shift_element_valuesA[iDirection-4][1] = shift_element_valuesD[iDirection-4][1];
	shift_element_valuesA[iDirection-4][2] = shift_element_valuesD[iDirection-4][2];

	// C -> D �� �� Shift
	shift_element_valuesD[iDirection-4][0] = shift_element_valuesC[iDirection-4][0];
	shift_element_valuesD[iDirection-4][1] = shift_element_valuesC[iDirection-4][1];
	shift_element_valuesD[iDirection-4][2] = shift_element_valuesC[iDirection-4][2];

	// B -> C �� �� Shift
	shift_element_valuesC[iDirection-4][0] = shift_element_valuesB[iDirection-4][0];
	shift_element_valuesC[iDirection-4][1] = shift_element_valuesB[iDirection-4][1];
	shift_element_valuesC[iDirection-4][2] = shift_element_valuesB[iDirection-4][2];

	// A -> B �� �� Shift
	shift_element_valuesB[iDirection-4][0] = shift_element_valuesACopy[iDirection-4][0];
	shift_element_valuesB[iDirection-4][1] = shift_element_valuesACopy[iDirection-4][1];
	shift_element_valuesB[iDirection-4][2] = shift_element_valuesACopy[iDirection-4][2];



	// A, B, C, D �� ����
	Face_Header->SetM_element_values(shift_element_valuesA);
	Face_Header->GetP_Next()->SetM_element_values(shift_element_valuesB);
	Face_Header->GetP_Next()->GetP_Next()->SetM_element_values(shift_element_valuesC);
	Face_Header->GetP_Next()->GetP_Next()->GetP_Next()->SetM_element_values(shift_element_valuesD);

	// ���� iDirection�� 4�̸� E�� SpinLeft iDirection�� 6�̸� F�� SpinRight
	if(iDirection == 4)
		SpinLeft(Face_Header->GetP_Up());
	else if(iDirection == 6)
		SpinRight(Face_Header->GetP_Up()->GetP_Up()->GetP_Up());

	//* ������ ���� �޸𸮸� ��� �����Ѵ�. */
	for(int i = 0; i < 3; i++)
	{
		delete[] shift_element_valuesA[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] shift_element_valuesB[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] shift_element_valuesC[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] shift_element_valuesD[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
		delete[] shift_element_valuesACopy[i];	// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	}
	delete[] shift_element_valuesA;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] shift_element_valuesB;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] shift_element_valuesC;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] shift_element_valuesD;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	delete[] shift_element_valuesACopy;		// ���簡 �������� GetM_element_values �޼ҵ忡�� ������ ���� �޸𸮸� �ٽ� �����Ѵ�.
	shift_element_valuesA = 0;	// �ʱ� ���� 0���� �ش�.
	shift_element_valuesB = 0;	// �ʱ� ���� 0���� �ش�.
	shift_element_valuesC = 0;	// �ʱ� ���� 0���� �ش�.
	shift_element_valuesD = 0;	// �ʱ� ���� 0���� �ش�.
	shift_element_valuesACopy = 0;	// �ʱ� ���� 0���� �ش�.

	return true;
}


Face* Face_list::GetFace_Header(void)
{
	// �ܺ� ������ �����ϰ� �ϱ� ���Ͽ� ���� Get �޼ҵ� 
	return Face_Header;
}
