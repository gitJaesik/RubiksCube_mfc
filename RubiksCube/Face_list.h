/*
마우스 입력으로 인한 Cube rotate는 face가 3면 중 하나로만 선택 됨 (첫번째 파라미터는 a,b,r 3개 중 하나)
(두번째 파라미터는 움직이는 방향)
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

