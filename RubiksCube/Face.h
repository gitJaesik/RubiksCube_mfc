#pragma once
class Face
{
public:
	Face(void);
	Face(char face_name);
	~Face(void);
private:
	char m_face_name;
	int m_element_values[3][3];
	Face *p_Next;
	Face *p_Up;
public:
	char GetM_face_name();
	void SetM_face_name(char face_name);
	int** GetM_element_values();
	void SetM_element_values(int** element_values);
	Face* GetP_Next(void);
	void SetP_Next(Face* InutP_Next);
	Face* GetP_Up(void);
	void SetP_Up(Face* InputP_Up);
};