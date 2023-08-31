#include "Gizumo.h"
#include <map>
#include <string>
#include "../../imGui/imgui.h"

Gizumo::Gizumo()
{
	Init();
}

Gizumo::~Gizumo()
{
}

void Gizumo::Init()
{
	pos = {0,0,0};

	float size = 2.5f;

	Vector3 centerBack = { -5,-5,-5 };
	Vector3 centerFront = { 5,5,5 };
	Vector3 redBack = { -size,-size,-size };
	Vector3 redFront = { size,size,-80 };
	Vector3 blueBack = { -size,-size,-size };
	Vector3 blueFront = { -80,size,size };
	Vector3 greenBack = { -size,-size,-size };
	Vector3 greenFront = { size,80,size };

	stick_[STICK_TYPE::CENTER] = {centerBack,centerFront,0xffffff};
	stick_[STICK_TYPE::X] = {redBack,redFront,0xff0000};
	stick_[STICK_TYPE::Y] = {greenBack,greenFront,0x00ff00};
	stick_[STICK_TYPE::Z] = {blueBack,blueFront,0x0000ff};
}

void Gizumo::Update(Vector2Flt sceneMousePoint,VECTOR cameraPos,Vector3& modelPos)
{
	pos = modelPos;

	VECTOR ScreenPos;

	// マウスの画面座標を x, y にセット
	ScreenPos.x = (float)sceneMousePoint.x_;
	ScreenPos.y = (float)sceneMousePoint.y_;

	// z をそれぞれ 0.0f と 1.0f にして2回 ConvScreenPosToWorldPos を呼ぶ
	ScreenPos.z = 0.0f;
	start = ConvScreenPosToWorldPos(ScreenPos);

	ScreenPos.z = 1.0f;
	end = ConvScreenPosToWorldPos(ScreenPos);
	
	for (int i = 0; i < stick_.size(); i++)
	{
		if (HitCheck_Line_Cube(start, end, stick_[i].back.toVECTOR(), stick_[i].front.toVECTOR()).HitFlag)
		{
			selectStick_ = static_cast<STICK_TYPE>(i);
			break;
		}
		selectStick_ = STICK_TYPE::MAX;
	}

	if (selectStick_ == STICK_TYPE::X)
	{
		stick_[STICK_TYPE::X].color = 0x000000;
	}
	else
	{
		stick_[STICK_TYPE::X].color = 0xff0000;
	}
}

void Gizumo::Draw()
{
	SetUseLighting(false);

	for (auto & [back,front,color]:stick_)
	{
		DrawCube3D((pos + back).toVECTOR(), (pos + front).toVECTOR(), color, color, true);
	}

	DrawCapsule3D(start,end,0.1f,10,0xffff00, 0xffff00,true);
	

	SetUseLighting(true);
}
