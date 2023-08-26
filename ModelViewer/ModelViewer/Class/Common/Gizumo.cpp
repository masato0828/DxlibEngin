#include "Gizumo.h"
#include <map>
#include <string>
#include "../../imGui/imgui.h"

Gizumo::Gizumo()
{
}

Gizumo::~Gizumo()
{
}

void Gizumo::Init()
{
	//lpShaderMng.LoadShader();
	pos = {0,0,0};
}

void Gizumo::Update(Vector2Flt sceneMousePoint,VECTOR cameraPos)
{
	VECTOR Start3DPos, End3DPos;
	VECTOR ScreenPos;
	int MouseX, MouseY;

	// マウスの画面座標を x, y にセット
	ScreenPos.x = (float)sceneMousePoint.x_;
	ScreenPos.y = (float)sceneMousePoint.y_;

	// z をそれぞれ 0.0f と 1.0f にして2回 ConvScreenPosToWorldPos を呼ぶ
	ScreenPos.z = 0.0f;
	start = ConvScreenPosToWorldPos(ScreenPos);

	ScreenPos.z = 1.0f;
	end = ConvScreenPosToWorldPos(ScreenPos);

	ImGui::Begin("gizumo");
	ImGui::Text("ScreenPos:%f:%f:%f", ScreenPos.x, ScreenPos.y, ScreenPos.z);
	ImGui::Text("start:%d:%d:%d",start.x, start.y, start.z);
	ImGui::Text("end:%d:%d:%d", end.x, end.y, end.z);
	ImGui::End();
}

void Gizumo::Draw()
{
	float size = 2.5f;

	Vector3 centerBack = { -5,-5,-5 };
	Vector3 centerFront = { 5,5,5 };
	DrawCube3D((pos + centerBack).toVECTOR(), (pos+centerFront).toVECTOR(), 0xffffff, 0xffffff, true);

	Vector3 redBack = {-size,-size,-size };
	Vector3 redFront = { size,size,-80};
	DrawCube3D((pos + redBack).toVECTOR(), (pos + redFront).toVECTOR(),0xff0000,0xff0000,true);

	Vector3 blueBack = { -size,-size,-size };
	Vector3 blueFront = { -80,size,size };
	DrawCube3D((pos + blueBack).toVECTOR(), (pos + blueFront).toVECTOR(), 0x0000ff, 0x0000ff, true);

	Vector3 greenBack = { -size,-size,-size };
	Vector3 greenFront = { size,80,size };
	DrawCube3D((pos + greenBack).toVECTOR(), (pos + greenFront).toVECTOR(), 0x00ff00, 0x00ff00, true);
}
