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
	
	copy_elemenet_values = new int*[3];		// 3 x 3�� ��κ�

	//
	for(int i = 0; i < 3; i++)
	{
		copy_elemenet_values[i] = new int[3];	// 3 x 3�� ���κ�

		for(int j = 0; j < 3; j++)
		{
			// ������ m_element_values[3][3]���� ���� �����´�.
			copy_elemenet_values[i][j] = m_element_values[i][j];
		}
	}
	// new�� �������� ����Ͽ� 3 x 3 array�� �Ҵ��ϸ�
	// delete�� Set methods�� ���� �� �ۿ���(�Ҵ� ���� ������) �����ϰڴ�.
	return copy_elemenet_values;
}

void Face::SetM_element_values(int** element_values)
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_element_values[i][j] = element_values[i][j];	// Set �޼ҵ带 ����Ͽ� ���� m_element_values[3][3]�� �ִ´�.
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
