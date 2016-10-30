#include "stdafx.h"
#include "Face.h"


Face::Face(void)
	: m_face_name(0)
{
	
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_element_values[i][j] = NULL;
		}
	}
	p_Next = NULL;
	p_Up = NULL;
}


Face::~Face(void)
{
}

Face::Face(char face_name)
{

	m_face_name = face_name;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_element_values[i][j] = NULL;
		}
	}
	p_Next = NULL;
	p_Up = NULL;
}

char Face::GetM_face_name()
{
	return m_face_name;
}


void Face::SetM_face_name(char face_name)
{
	m_face_name = face_name;
}


int** Face::GetM_element_values()
{
	int** copy_elemenet_values;
	
	copy_elemenet_values = new int*[3];		// 3 x 3의 행부분

	//
	for(int i = 0; i < 3; i++)
	{
		copy_elemenet_values[i] = new int[3];	// 3 x 3의 열부분

		for(int j = 0; j < 3; j++)
		{
			// 기존의 m_element_values[3][3]에서 값을 가져온다.
			copy_elemenet_values[i][j] = m_element_values[i][j];
		}
	}
	// new를 이중으로 사용하여 3 x 3 array를 할당하며
	// delete는 Set methods를 수행 후 밖에서(할당 받은 곳에서) 시행하겠다.
	return copy_elemenet_values;
}

void Face::SetM_element_values(int** element_values)
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_element_values[i][j] = element_values[i][j];	// Set 메소드를 사용하여 값을 m_element_values[3][3]에 넣는다.
		}
	}
}


Face* Face::GetP_Next(void)
{
	return p_Next;
}


void Face::SetP_Next(Face* InutP_Next)
{
	this->p_Next = InutP_Next;
}


Face* Face::GetP_Up(void)
{
	return p_Up;
}



void Face::SetP_Up(Face* InputP_Up)
{
	this->p_Up = InputP_Up;
}
