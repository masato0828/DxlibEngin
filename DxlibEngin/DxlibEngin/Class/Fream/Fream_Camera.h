#pragma once
#include "FreamBase.h"
#include "../Common/Geometry.h"
class Fream_Camera :
    public FreamBase
{
public:
    Fream_Camera();
    ~Fream_Camera();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="mousePoint">マウスの位置</param>
	/// <param name="windowHlfeSize">ウィンドウの半分のサイズ</param>
	/// <param name="centerPoint">画面の中央</param>
	void Update(Vector2 mousePoint,Vector2Flt windowHlfeSize,Vector2 centerPoint);
	
	/// <summary>
	/// カメラの回転をボタンで操作
	/// </summary>
	void Angle();

	/// <summary>
	/// カメラの移動をボタンで操作
	/// </summary>
	void Move();

	/// <summary>
	/// カメラのニア＆ファーをセット
	/// </summary>
	/// <param name="cameraNear">ニア値</param>
	/// <param name="cameraFar">ファー値</param>
	void Set(int cameraNear = 10.0f,int cameraFar = 300000.0f);
	
	/// <summary>
	/// マウスの左が押されているときにマウスによるカメラの回転
	/// </summary>
	void PushMouseAngle();

	/// <summary>
	/// マウスによるカメラの回転
	/// </summary>
	/// <param name="centerPoint">画面の真ん中</param>
	void MouseAngle(Vector2 centerPoint);

	/// <summary>
	/// カスタムパラメータの変更
	/// </summary>
	void Custom();

	/// <summary>
	/// 画面上のマウスの位置の取得
	/// </summary>
	/// <returns>マウスの位置</returns>
	Vector2 GetScreenMouse();

	/// <summary>
	/// カメラの位置の取得
	/// </summary>
	/// <returns>カメラの位置</returns>
	Vector3 GetCameraPos();
private:
	// 位置
	Vector3 pos_;
	// 回転
	Vector3 rot_;

	// 注視点位置情報
	Vector3 targetPos_;

	// マウス用
	Vector2Dbl aftorMousePoint_;	// 更新位置
	Vector2Dbl beforMousePoint_;	// 更新前位置
	Vector2Dbl mousePointDiff_;		// 更新差分
	float sens_;	// マウス感度
	int mouseX_;	// マウスのX座標
	int mouseY_;	// マウスのY座標
	int input_;		// マウスの押下状態

	// 移動スピード
	float moveSpeed_;
	// 移動スピード（速度早い）
	int moveSpeedBoost_;

	// ウィンドウの左上
	Vector2 correctWindowLeftTop_;
	// ウィンドウの半分のサイズ
	Vector2 windowHlfeSize_;

	// マウスの移動フラグ
	bool mouseMove_;
	// テキスト表示点滅カウント
	int cnt_;
	// マウスの位置
	Vector2 mousePoint_;
};

