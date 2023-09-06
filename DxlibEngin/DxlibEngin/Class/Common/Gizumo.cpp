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
	
}

void Gizumo::Update(Vector2Flt sceneMousePoint,Vector3& modelPos)
{
	pos = modelPos;

	// マウスボタンの入力状態を更新
	PrevInput = NowInput;
	NowInput = GetMouseInput();
	EdgeInput = NowInput & ~PrevInput;

	

	// 既にモデルを掴んでいるかどうかで処理を分岐
	if (Catch == 0)
	{
		// 掴んでいない場合

		// 左クリックされたらモデルをクリックしたかを調べる
		if (EdgeInput & MOUSE_INPUT_1)
		{
			

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
					// 掴んでいるかどうかのフラグを立てる
					Catch = 1;

					// 掴んだときのスクリーン座標を保存
					CatchMouseX = ScreenPos.x;
					CatchMouseY = ScreenPos.y;

					// 掴んだときのモデルのワールド座標を保存
					Catch3DModelPosition = modelPos.toVECTOR();

					// 掴んだときのモデルと線分が当たった座標を保存( 座標をスクリーン座標に変換したものも保存しておく )
					Catch3DHitPosition = result.Position;
					Catch2DHitPosition = ConvWorldPosToScreenPos(Catch3DHitPosition);

					selectStick_ = static_cast<STICK_TYPE>(i);
					break;
				}
				selectStick_ = STICK_TYPE::MAX;
			}
		}
	}
	else
	{
		// 掴んでいる場合

		// マウスの左クリックが離されていたら掴み状態を解除
		if ((NowInput&MOUSE_INPUT_1) == 0)
		{
			Catch = 0;
		}
		else
		{
			// 掴み状態が継続していたらマウスカーソルの移動に合わせてモデルも移動

			float MoveX = 0, MoveY = 0, MoveZ = 0;
			VECTOR NowCatch2DHitPosition;
			VECTOR NowCatch3DHitPosition;
			VECTOR Now3DModelPosition;

			// 掴んだときのマウス座標から現在のマウス座標までの移動分を算出
			MoveX = (float)(ScreenPos.x - CatchMouseX);
			MoveY = (float)(ScreenPos.y - CatchMouseY);

			// 掴んだときのモデルと線分が当たった座標をスクリーン座標に変換したものにマウスの移動分を足す
			NowCatch2DHitPosition.x = Catch2DHitPosition.x + MoveX;
			//NowCatch2DHitPosition.y = Catch2DHitPosition.y + MoveY;
			NowCatch2DHitPosition.y = Catch2DHitPosition.y + MoveY;
			NowCatch2DHitPosition.z = Catch2DHitPosition.z + MoveZ;

			// 掴んだときのモデルと線分が当たった座標をスクリーン座標に変換したものにマウスの移動分を足した座標をワールド座標に変換
			NowCatch3DHitPosition = ConvScreenPosToWorldPos(NowCatch2DHitPosition);

			// 掴んだときのモデルのワールド座標に『掴んだときのモデルと線分が当たった座標にマウスの移動分を足した座標をワールド座標に
			// 変換した座標』と、『掴んだときのモデルと線分が当たった座標』との差分を加算
			Now3DModelPosition.x = Catch3DModelPosition.x + NowCatch3DHitPosition.x - Catch3DHitPosition.x;
			Now3DModelPosition.y = Catch3DModelPosition.y + NowCatch3DHitPosition.y - Catch3DHitPosition.y;
			Now3DModelPosition.z = Catch3DModelPosition.z + NowCatch3DHitPosition.z - Catch3DHitPosition.z;

			modelPos.setVECTOR(Now3DModelPosition);
		}
	}

	
	ImGui::Begin("test");
	ImGui::Text("%f;%f;%f" , modelPos.x_, modelPos.y_, modelPos.z_);
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
