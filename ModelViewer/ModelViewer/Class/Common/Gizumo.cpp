#include "Gizumo.h"

Gizumo::Gizumo()
{
}

Gizumo::~Gizumo()
{
}

void Gizumo::Init()
{
}

void Gizumo::Update()
{
}

void Gizumo::Draw()
{
	float size = 2.5f;

	Vector3 centerBack = { -5,-5,-5 };
	Vector3 centerFront = { 5,5,5 };
	DrawCube3D(centerBack.toVECTOR(), centerFront.toVECTOR(), 0xffffff, 0xffffff, true);

	Vector3 redBack = {-size,-size,-size };
	Vector3 redFront = { size,size,-80};
	DrawCube3D(redBack.toVECTOR(),redFront.toVECTOR(),0xff0000,0xff0000,true);

	Vector3 blueBack = { -size,-size,-size };
	Vector3 blueFront = { -80,size,size };
	DrawCube3D(blueBack.toVECTOR(), blueFront.toVECTOR(), 0x0000ff, 0x0000ff, true);

	Vector3 greenBack = { -size,-size,-size };
	Vector3 greenFront = { size,80,size };
	DrawCube3D(greenBack.toVECTOR(), greenFront.toVECTOR(), 0x00ff00, 0x00ff00, true);
}
