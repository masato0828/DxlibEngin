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

	defoStickColor_.emplace(STICK_TYPE::CENTER, 0xffffff);
	defoStickColor_.emplace(STICK_TYPE::X, 0xff0000);
	defoStickColor_.emplace(STICK_TYPE::Y, 0x00ff00);
	defoStickColor_.emplace(STICK_TYPE::Z, 0x0000ff);

	stick_[STICK_TYPE::CENTER] = {centerBack,centerFront,defoStickColor_[STICK_TYPE::CENTER]};
	stick_[STICK_TYPE::X] = {redBack,redFront,defoStickColor_[STICK_TYPE::X] };
	stick_[STICK_TYPE::Y] = {greenBack,greenFront,defoStickColor_[STICK_TYPE::Y] };
	stick_[STICK_TYPE::Z] = {blueBack,blueFront,defoStickColor_[STICK_TYPE::Z] };

	
}

void Gizumo::Update(Vector2Flt sceneMousePoint,Vector3& modelPos)
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

	for (int i = 0; i < stick_.size(); i++)
	{
		if (i == STICK_TYPE::CENTER)
		{
			continue;
		}

		if (selectStick_ == static_cast<STICK_TYPE>(i))
		{
			stick_[static_cast<STICK_TYPE>(i)].color = 0x000000;
		}
		else
		{
			stick_[static_cast<STICK_TYPE>(i)].color = defoStickColor_[static_cast<STICK_TYPE>(i)];
		}
	}


	if (selectStick_ == STICK_TYPE::X)
	{
		stick_[STICK_TYPE::X].color = 0x000000;
	}
	else
	{
		stick_[STICK_TYPE::X].color = defoStickColor_[STICK_TYPE::X];
	}

	if (selectStick_ == STICK_TYPE::Y)
	{
		stick_[STICK_TYPE::Y].color = 0x000000;
	}
	else
	{
		stick_[STICK_TYPE::Y].color = defoStickColor_[STICK_TYPE::Y];
	}

	if (selectStick_ == STICK_TYPE::Z)
	{
		stick_[STICK_TYPE::Z].color = 0x000000;
	}
	else
	{
		stick_[STICK_TYPE::Z].color = defoStickColor_[STICK_TYPE::Z];
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
