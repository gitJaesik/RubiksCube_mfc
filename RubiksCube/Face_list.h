/*
���콺 �Է����� ���� Cube rotate�� face�� 3�� �� �ϳ��θ� ���� �� (ù��° �Ķ���ʹ� a,b,r 3�� �� �ϳ�)
(�ι�° �Ķ���ʹ� �����̴� ����)
*/
#include "Face.h"

#pragma once
class Face_list
{
public:
	Face_list(void);
	~Face_list(void);
	
private:
	Face* Face_Header;
public:
	void Init();
	void Clean();
	bool RotateFace(int iSelectedFace, int iDirection);
	bool RotateCube(int iCubeDirection);
	bool RotateCubeUp();
	bool SpinLeft(Face* SelectedFace);
	bool SpinRight(Face* SelectedFace);
	bool Spin180(Face* SelectedFace);
	bool RotateCubeRight();
	bool RotateFaceAllDirection(int iDirection);
	bool RotateFaceToUp(int iDirection);
	bool RotateFaceToRight(int iDirection);
	Face* GetFace_Header(void);
};

