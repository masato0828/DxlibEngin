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
	rot_ = { 0.420f,0.795f,0.0f };

	targetPos_ = { 0,0,0 };

	isTarget_ = false;

	aftorMousePoint_ = beforMousePoint_ = mousePointDiff_ = Vector2Dbl(0.0, 0.0);

	mouseX_ = 0;
	mouseY_ = 0;
	input_ = false;

	sens_ = 0.46f;
	// マウス初期位置の設定
	//SetMousePoint(1600 / 2 - 80, 720 / 2);



	mauseHWheel_ = 0;

	moveSpeed_ = 5.0f;
	moveSpeedBoost_ = 2;

	mouseMove_ = false;
	cnt_ = 0;
}

void Fream_Camera::Update()
{
}

void Fream_Camera::Update(Vector2 mousePoint, Vector2Flt windowHlfeSize,Vector2 centerPoint)
{
	mousePoint_ = mousePoint;
	windowHlfeSize_ = { windowHlfeSize.x_,windowHlfeSize.y_ };

	// 移動
	Move();
	if (mouseMove_)
	{
		cnt_++;
		MouseMove(centerPoint);
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			mouseMove_ = false;
		}
	}
	else
	{
		// キーボードカメラ角度変更
		//Angle();
		cnt_ = 0;

		// マウスのボタンを押しながらマウス操作
		PushMouseMove();
	}
	

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
		rot_.x_ -= Utility::Deg2Rad(angleSpeed);
	}
	// 下
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		rot_.x_ += Utility::Deg2Rad(angleSpeed);
	}
	// 左
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		rot_.y_ -= Utility::Deg2Rad(angleSpeed);
	}
	// 右
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		rot_.y_ += Utility::Deg2Rad(angleSpeed);
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
		rad = (0.0f * (DX_PI_F / 180.0f));
		pos_.x_ += sinf(rot_.y_ + rad) * moveSpeed;
		pos_.z_ += cosf(rot_.y_ + rad) * moveSpeed;
	}
	// 後方へ進む
	if (CheckHitKey(KEY_INPUT_S)) {
		rad = (180.0f * (DX_PI_F / 180.0f));
		pos_.x_ += sinf(rot_.y_ + rad) * moveSpeed;
		pos_.z_ += cosf(rot_.y_ + rad) * moveSpeed;
	}
	// 左方向
	if (CheckHitKey(KEY_INPUT_A)) {
		rad = (-90.0f * (DX_PI_F / 180.0f));
		pos_.x_ += sinf(rot_.y_ + rad) * moveSpeed;
		pos_.z_ += cosf(rot_.y_ + rad) * moveSpeed;
	}
	// 右方向
	if (CheckHitKey(KEY_INPUT_D)) {
		rad = (90.0f * (DX_PI_F / 180.0f));
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
	

	ImGui::Checkbox("camera move", &mouseMove_);
	ImGui::SliderFloat("sens", &sens_, 0.0f, 8.0f);
	ImGui::SliderFloat("move speed", &moveSpeed_, 1.0f, 20.0f);
	ImGui::SliderInt("move speed boost", &moveSpeedBoost_, 0, 10);
	ImGui::DragFloat3("pos", &pos_);
	ImGui::DragFloat3("rot", &rot_, Utility::Deg2Rad(1.0f));

	ImGui::Text("windowHlfeSize_ = %f,%f", windowHlfeSize_.x_,windowHlfeSize_.y_);
	ImGui::Text("beforMousePoint_ = %f:%f", beforMousePoint_.x_,beforMousePoint_.y_);
	ImGui::Text("aftorMousePoint_ = %f:%f",aftorMousePoint_.x_,aftorMousePoint_.y_);
	ImGui::Text("mousePointDiff_ = %f:%f",mousePointDiff_.x_,mousePointDiff_.y_);
	ImGui::Text("mouse = %d:%d",mouseX_,mouseY_);

	int ww, wh;
	GetWindowSize(&ww,&wh);
	ImGui::Text("windowSize = %d:%d",ww,wh);

	int mx, my;
	GetMousePoint(&mx,&my);
	ImGui::Text("mousePoint = %d:%d",mx,my);




	
	if (cnt_ & 8)
	{
		ImGui::Text("<<PUSH TO SPACE MOUSE MOVE RELESE>>");
	}


}

Vector2 Fream_Camera::GetScreenMouse()
{
	return {mouseX_,mouseY_};
}

Vector3 Fream_Camera::GetCameraPos()
{
	return pos_;
}

void Fream_Camera::Set()
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	// カメラのクリッピング距離を設定
	SetCameraNearFar(10.0f, 300000.0f);
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraPositionAndAngle(pos_.toVECTOR(), rot_.x_, rot_.y_, rot_.z_);
}

void Fream_Camera::PushMouseMove()
{
	// 一フレーム前のマウス位置
	beforMousePoint_ = {static_cast<double>(mouseX_),static_cast<double>(mouseY_) };
	// sceneViewのマウス位置取得
	mouseX_ = static_cast<int>(mousePoint_.x_);
	mouseY_ = static_cast<int>(mousePoint_.y_);
	// 現在のマウス位置
	aftorMousePoint_ = { static_cast<double>(mouseX_),static_cast<double>(mouseY_)  };
	// １フレーム前の座標と現在の座標を比べた差を求める
	mousePointDiff_ = aftorMousePoint_ - beforMousePoint_ ;

	// マウスの左が押されているとき
	input_ = GetMouseInput();
	if (input_&MOUSE_INPUT_2)
	{
		// 先ほどの差をを視点移動量とする
		rot_.x_ += static_cast<float>(mousePointDiff_.y_) * sens_ / 180;
		rot_.y_ += static_cast<float>(mousePointDiff_.x_) * sens_ / 180;
	}
}

void Fream_Camera::MouseMove(Vector2 centerPoint)
{
	// 真ん中を0とする
	Vector2Flt nowPoint = { mousePoint_.x_ - windowHlfeSize_.x_ ,mousePoint_.y_ - windowHlfeSize_.y_ };

	// 真ん中から動いた距離を視点移動とする
	rot_.x_ += nowPoint.y_ * sens_ / 180;
	rot_.y_ += nowPoint.x_ * sens_ / 180;

	// マウスの位置の固定
	SetMousePoint(static_cast<int>(centerPoint.x_), static_cast<int>(centerPoint.y_));
}
