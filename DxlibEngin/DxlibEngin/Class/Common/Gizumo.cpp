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

	Vector3 redBack = { -size,-size,-size };
	Vector3 redFront = { -80,size,size };
	Vector3 blueBack = { -size,-size,-size };
	Vector3 blueFront = { size,size,-80 };
	Vector3 greenBack = { -size,-size,-size };
	Vector3 greenFront = { size,80,size };

	defoStickColor_.emplace(STICK_TYPE::X, 0xff0000);
	defoStickColor_.emplace(STICK_TYPE::Y, 0x00ff00);
	defoStickColor_.emplace(STICK_TYPE::Z, 0x0000ff);
	stick_[STICK_TYPE::X] = {redBack,redFront,defoStickColor_[STICK_TYPE::X] };
	stick_[STICK_TYPE::Y] = {greenBack,greenFront,defoStickColor_[STICK_TYPE::Y] };
	stick_[STICK_TYPE::Z] = {blueBack,blueFront,defoStickColor_[STICK_TYPE::Z] };


	// マウス関連変数の初期化
	NowInput = 0;
	EdgeInput = 0;
	PrevInput = 0;
	Catch = 0;

	selectStick_ = STICK_TYPE::MAX;
	
}

void Gizumo::Update(Vector2Flt sceneMousePoint,Vector3& modelPos)
{
	ImGui::Begin("test");
	pos = modelPos;

	// マウスボタンの入力状態を更新
	PrevInput = NowInput;
	NowInput = GetMouseInput();
	EdgeInput = NowInput & ~PrevInput;

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
		auto result = HitCheck_Line_Cube(start, end, (modelPos + stick_[i].back).toVECTOR(), (modelPos + stick_[i].front).toVECTOR());

		if (result.HitFlag)
		{
			if (EdgeInput & MOUSE_INPUT_1)
			{
				Catch = true;
				catchPos_ = result.Position;

				// 掴んだ時のスクリーン座標
				CatchMouseX = ScreenPos.x;
				CatchMouseY = ScreenPos.y;

				Catch3DHitPosition = result.Position;
				Catch2DHitPosition = ConvWorldPosToScreenPos(result.Position);
				selectStick_ = static_cast<STICK_TYPE>(i);
				selectStick2_ = static_cast<STICK_TYPE>(i);
			}
			break;
		}
		selectStick_ = STICK_TYPE::MAX;
	}

	if (Catch)
	{
		// 掴んでいる処理
		// マウスの左クリックが離されていたら掴み状態を解除
		if ((NowInput & MOUSE_INPUT_1) == 0)
		{
			Catch = 0;
			selectStick2_ = STICK_TYPE::MAX;
		}
		else
		{
			float moveX = 0, moveY = 0, moveZ = 0;

			// 移動分
			moveX = (float)(ScreenPos.x - CatchMouseX);
			moveY = (float)(ScreenPos.y - CatchMouseY);
			moveZ = moveX - moveY;

			ImGui::Text("move\n%f;%f;%f", moveX, moveY, moveY);

			if (selectStick2_ == STICK_TYPE::X)
			{
				modelPos.x_ = moveX + catchPos_.x;
			}
			else if (selectStick2_ == STICK_TYPE::Y)
			{
				modelPos.y_ = -(moveY + catchPos_.y);
			}
			else if (selectStick2_ == STICK_TYPE::Z)
			{
				modelPos.z_ = -(moveZ + catchPos_.z);
			}
		}
	}

	
	
	ImGui::Text("modelPos\n %f;%f;%f" , modelPos.x_, modelPos.y_, modelPos.z_);
	ImGui::Text("selectStick_\n %d", selectStick2_);

	if (Catch)
	{
		ImGui::Text("catchPos\n  :%f;%f;%f", catchPos_.x, catchPos_.y, catchPos_.z);
	}
	ImGui::End();

	for (int i = 0; i < stick_.size(); i++)
	{

		if (selectStick2_ == static_cast<STICK_TYPE>(i))
		{
			stick_[static_cast<STICK_TYPE>(i)].color = 0x000000;
		}
		else
		{
			stick_[static_cast<STICK_TYPE>(i)].color = defoStickColor_[static_cast<STICK_TYPE>(i)];
		}
	}

}

void Gizumo::Draw()
{
	//SetUseLighting(false);

	for (auto& [back, front, color] : stick_)
	{
		DrawCube3D((pos + back).toVECTOR(), (pos + front).toVECTOR(), color, color, true);

		if (selectStick2_ == STICK_TYPE::X)
		{
			DrawLine3D({ pos.x_ - (front.x_ + 10000), pos.y_, pos.z_ }, { pos.x_ + front.x_ + 10000, pos.y_, pos.z_ }, color);
		}
		else if (selectStick2_ == STICK_TYPE::Y)
		{
			DrawLine3D({ pos.x_ , pos.y_ - (front.y_ + 10000), pos.z_ }, { pos.x_ , pos.y_ + front.y_ + 10000, pos.z_ }, color);
		}
		else if (selectStick2_ == STICK_TYPE::Z)
		{
			DrawLine3D({ pos.x_, pos.y_, pos.z_ - (front.z_ + 10000) }, { pos.x_ , pos.y_, pos.z_ + front.z_ + 10000 }, color);
		}
	}



	

	//DrawCapsule3D(start,end,0.1f,10,0xffff00, 0xffff00,true);
	

	//SetUseLighting(true);
}
