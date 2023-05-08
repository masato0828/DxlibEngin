#include <Dxlib.h>
#include "Fream_Camera.h"
#include "../Common/Utility.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"

Fream_Camera::Fream_Camera()
{
	Init();
}

Fream_Camera::~Fream_Camera()
{
}

void Fream_Camera::Init()
{
	//pos_ = { 0,180, -300 };
	//rot_ = { Deg2Rad(15.0f),0.0f,0.0f };
	
	
	pos_ = { -243,185, -231 };
	rot_ = { 0.420,0.795f,0.0f };


	targetPos_ = { 0,0,0 };

	isTarget_ = false;

	ma_ = mb_ = mc_ = Vector2Dbl(0.0, 0.0);

	mouseX_ = 0;
	mouseY_ = 0;
	input_ = false;

	sens_ = 0.46;
	// マウス位置の設定
	SetMousePoint(1600 / 2 - 80, 720 / 2);



	mauseHWheel_ = 0;

	moveSpeed_ = 5.0f;
	moveSpeedBoost_ = 2;
}

void Fream_Camera::Update()
{
}

void Fream_Camera::Update(Vector2Flt mousePoint)
{
	// 移動
	Move();

	// マウスのボタンを押しながらマウス操作

	PushMouseMove(mousePoint);

	//MouseMove();


	// マウスホイール取得
	mauseHWheel_ = DxLib::GetMouseWheelRotVol();

	// 拡大（要修正）
	pos_.z_ += mauseHWheel_ * 30;
}

void Fream_Camera::Angle()
{
	// カメラ角度変更
	float angleSpeed = sens_;
	// 上
	if (CheckHitKey(KEY_INPUT_UP))
	{
		rot_.x_ -= Deg2Rad(angleSpeed);
	}
	// 下
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		rot_.x_ += Deg2Rad(angleSpeed);
	}
	// 左
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		rot_.y_ -= Deg2Rad(angleSpeed);
	}
	// 右
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		rot_.y_ += Deg2Rad(angleSpeed);
	}
}

void Fream_Camera::Move()
{
	float moveSpeed = moveSpeed_;
	float rad = 0.0f;

	// スピードアップ
	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		moveSpeed = moveSpeed_ * static_cast<float>(moveSpeedBoost_);
	}

	// 前方へ進む
	if (CheckHitKey(KEY_INPUT_W)) {
		rad = (0.0f * (DX_PI / 180.0f));
		pos_.x_ += sinf(rot_.y_ + rad) * moveSpeed;
		pos_.z_ += cosf(rot_.y_ + rad) * moveSpeed;
	}
	// 後方へ進む
	if (CheckHitKey(KEY_INPUT_S)) {
		rad = (180.0f * (DX_PI / 180.0f));
		pos_.x_ += sinf(rot_.y_ + rad) * moveSpeed;
		pos_.z_ += cosf(rot_.y_ + rad) * moveSpeed;
	}
	// 左方向
	if (CheckHitKey(KEY_INPUT_A)) {
		rad = (-90.0f * (DX_PI / 180.0f));
		pos_.x_ += sinf(rot_.y_ + rad) * moveSpeed;
		pos_.z_ += cosf(rot_.y_ + rad) * moveSpeed;
	}
	// 右方向
	if (CheckHitKey(KEY_INPUT_D)) {
		rad = (90.0f * (DX_PI / 180.0f));
		pos_.x_ += sinf(rot_.y_ + rad) * moveSpeed;
		pos_.z_ += cosf(rot_.y_ + rad) * moveSpeed;
	}

	// 上方向へ移動
	if (CheckHitKey(KEY_INPUT_Q)) {
		pos_.y_ += moveSpeed;
	}
	// 下方向へ移動
	if (CheckHitKey(KEY_INPUT_E)) {
		pos_.y_ -= moveSpeed;
	}


}

void Fream_Camera::Custom()
{
	static bool mouseMove = false;
	static int cnt = 0;

	//ImGui::Begin("CameraInfo", NULL, ImGuiWindowFlags_NoCollapse);
	if (ImGui::Checkbox("camera move", &mouseMove))
	{

		//SetMousePoint(lpFreamMng.GetSceneViewSize().x_ - lpFreamMng.GetImageSize().x_, lpFreamMng.GetSceneViewSize().y_ - lpFreamMng.GetImageSize().x_ / 2);
	}
	if (mouseMove)
	{
		cnt++;
		MouseMove();
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			mouseMove = false;
		}
	}
	else
	{
		// キーボードカメラ角度変更
		//Angle();
		cnt = 0;
	}
	ImGui::SliderFloat("sens", &sens_, 0.0f, 8.0f);
	ImGui::SliderFloat("move speed", &moveSpeed_, 1.0f, 20.0f);
	ImGui::SliderInt("move speed boost", &moveSpeedBoost_, 0, 10);
	ImGui::DragFloat3("pos", &pos_);
	ImGui::DragFloat3("rot", &rot_, Deg2Rad(1.0f));


	if (cnt & 8)
	{
		ImGui::Text("<<PUSH TO SPACE MOUSE MOVE RELESE>>");
	}
}

void Fream_Camera::Set()
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	// カメラのクリッピング距離を設定
	SetCameraNearFar(10.0f, 300000.0f);
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraPositionAndAngle(pos_.toVECTOR(), rot_.x_, rot_.y_, rot_.z_);
}

void Fream_Camera::PushMouseMove(Vector2Flt mousePoint)
{
	mb_ = {static_cast<double>(mouseX_),static_cast<double>(mouseY_) };
	// Dxlibのマウス位置取得
	//GetMousePoint(&mouseX_,&mouseY_);
	// sceneViewのマウス位置取得
	mouseX_ = mousePoint.x_;
	mouseY_ = mousePoint.y_;
	ma_ = { static_cast<double>(mouseX_),static_cast<double>(mouseY_)  };
	mc_ = ma_ - mb_ ;

	input_ = GetMouseInput();
	if (input_&MOUSE_INPUT_2)
	{
		rot_.x_ += mc_.y_ * sens_ / 180;
		rot_.y_ += mc_.x_ * sens_ / 180;
	}
	mc_ *= 0.9;
}

void Fream_Camera::MouseMove()
{
	mb_ = { static_cast<double>(mouseX_),static_cast<double>(mouseY_) };
	// Dxlibのマウス位置取得
	GetMousePoint(&mouseX_, &mouseY_);
	// sceneViewのマウス位置取得
	//mouseX_ = lpFreamMng.GetSceneViewMousePoint().x_;
	//mouseY_ = lpFreamMng.GetSceneViewMousePoint().y_;
	//
	ma_ = { static_cast<double>(mouseX_),static_cast<double>(mouseY_) };
	mc_ = ma_ - mb_;
	rot_.x_ += mc_.y_ * sens_ / 180;
	rot_.y_ += mc_.x_ * sens_ / 180;
	//mc_ *= 0.9;

	int DesktopW, DesktopH;
	GetDefaultState(&DesktopW, &DesktopH, NULL);
	SetMousePoint(DesktopW / 2, DesktopH / 2);
	//SetMousePoint(lpFreamMng.GetSceneViewSize().x_ - lpFreamMng.GetImageSize().x_, lpFreamMng.GetSceneViewSize().y_ - lpFreamMng.GetImageSize().y_ / 2);
}
