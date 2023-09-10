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
	Vector3 redFront = { size,size,-80 };
	Vector3 blueBack = { -size,-size,-size };
	Vector3 blueFront = { -80,size,size };
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

	resultMovePos_ = VECTOR();
	
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
			}
			selectStick_ = static_cast<STICK_TYPE>(i);
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
		}
		else
		{

			float moveX = 0, moveY = 0, moveZ = 0;

			// 移動分
			moveX = (float)(ScreenPos.x - CatchMouseX-modelPos.x_);
			moveY = (float)(ScreenPos.y - CatchMouseY);
			ImGui::Text("move :%f;%f;%f", moveX, moveY, moveY);

			if (modelPos == Vector3(0,0,0))
			{
				ImGui::End();
				return;
				
			}

			resultMovePos_.x = modelPos.x_ + moveX;

			modelPos.y_ = 0;
			modelPos.z_ = 0;

			modelPos.x_ = resultMovePos_.x+ catchPos_.x;
		}
	}

	
	
	ImGui::Text("modelPos: %f;%f;%f" , modelPos.x_, modelPos.y_, modelPos.z_);

	if (Catch)
	{
		ImGui::Text("catchPos  :%f;%f;%f", catchPos_.x, catchPos_.y, catchPos_.z);
	}
	ImGui::End();

	for (int i = 0; i < stick_.size(); i++)
	{

		if (selectStick_ == static_cast<STICK_TYPE>(i))
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
	SetUseLighting(false);

	for (auto & [back,front,color]:stick_)
	{
		DrawCube3D((pos + back).toVECTOR(), (pos + front).toVECTOR(), color, color, true);
	}

	DrawCapsule3D(start,end,0.1f,10,0xffff00, 0xffff00,true);
	

	SetUseLighting(true);
}
