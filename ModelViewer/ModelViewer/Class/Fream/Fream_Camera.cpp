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

	mouseMove_ = false;
	cnt_ = 0;
}

void Fream_Camera::Update()
{
}

void Fream_Camera::Update(Vector2Flt mousePoint, Vector2Flt windowSize, Vector2Flt correctLeftTop, float factor)
{
	windowHlfeSize_ = { windowSize.x_,windowSize.y_ };
	correctWindowLeftTop_ = {correctLeftTop.x_,correctLeftTop.y_ };

	// 移動
	Move();
	if (mouseMove_)
	{
		cnt_++;
		MouseMove(mousePoint,factor);
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
		PushMouseMove(mousePoint);
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
	

	//ImGui::Begin("CameraInfo", NULL, ImGuiWindowFlags_NoCollapse);
	if (ImGui::Checkbox("camera move", &mouseMove_))
	{

		//SetMousePoint(lpFreamMng.GetSceneViewSize().x_ - lpFreamMng.GetImageSize().x_, lpFreamMng.GetSceneViewSize().y_ - lpFreamMng.GetImageSize().x_ / 2);
	}
	ImGui::SliderFloat("sens", &sens_, 0.0f, 8.0f);
	ImGui::SliderFloat("move speed", &moveSpeed_, 1.0f, 20.0f);
	ImGui::SliderInt("move speed boost", &moveSpeedBoost_, 0, 10);
	ImGui::DragFloat3("pos", &pos_);
	ImGui::DragFloat3("rot", &rot_, Deg2Rad(1.0f));

	ImGui::Text("%f,%f", windowHlfeSize_.x_,windowHlfeSize_.y_);
	
	ImGui::Text("ma = %f:%f",ma_.x_,ma_.y_);
	ImGui::Text("mb = %f:%f",mb_.x_,mb_.y_);
	ImGui::Text("mc = %f:%f",mc_.x_,mc_.y_);
	ImGui::Text("mouseX&Y = %d:%d",mouseX_,mouseY_);

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

void Fream_Camera::Set()
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	// カメラのクリッピング距離を設定
	SetCameraNearFar(10.0f, 300000.0f);
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraPositionAndAngle(pos_.toVECTOR(), rot_.x_, rot_.y_, rot_.z_);
	
	
	int wh, ww;
	GetWindowSize(&ww, &wh);
	Vector2Flt windowSize = {
	   ww / 2,
	   wh / 2
	};
	DrawCircle(windowSize.x_, windowSize.y_,5,0xffff00,true);
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

void Fream_Camera::MouseMove(Vector2Flt mousePoint, float factor)
{
	Vector2Flt nowPoint = { mousePoint.x_ - windowHlfeSize_.x_ ,mousePoint.y_ - windowHlfeSize_.y_ };
	Vector2 centerPoint = correctWindowLeftTop_+windowHlfeSize_;

	auto move =  nowPoint;


	

	//mb_ = { static_cast<double>(mouseX_),static_cast<double>(mouseY_) };
	//// sceneViewのマウス位置取得
	//mouseX_ = mousePoint.x_;
	//mouseY_ = mousePoint.y_;
	//ma_ = { static_cast<double>(mouseX_),static_cast<double>(mouseY_) };
	//mc_ = ma_ - mb_;

	ImGui::SetNextWindowSize(ImVec2{300,100});
	ImGui::Begin("debug");
	ImGui::Text("move %f:%f",move.x_,move.y_);
	ImGui::End();

	rot_.x_ += move.y_ * sens_ / 180;
	rot_.y_ += move.x_ * sens_ / 180;

	SetMousePoint(static_cast<int>(centerPoint.x_), static_cast<int>(centerPoint.y_));
}
