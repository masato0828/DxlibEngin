#pragma once
#include "../Common/Vector2.h"
class Device_Mouse
{
public:
	Device_Mouse();
	~Device_Mouse();
	void Init();
	void Update(Vector2Flt screenLeftUp, Vector2Flt screenRightDown, Vector2 defaultScreenSize);
	void Draw();
	// マウスの位置のセット
	void SetPoint(Vector2 point);
	// 1フレーム間で動いた距離
	void MoveOneFreamVolume();
	// マウスの位置の取得
	Vector2 GetNowPoint();
	// シーン内でのマウスの位置の作成
	void SceneMouseCreate();
	// シーン内でのマウスの位置の取得
	Vector2Flt GetSceneMousePoint();
private:
	// マウスの座標
	Vector2 point_;
	// 1フレーム間で動いた距離
	Vector2Flt oneFreamMoveVolume_;
	// シーン描画の左上
	Vector2Flt screenLeftUp_;
	// シーン描画の右下
	Vector2Flt screenRightDown_;
	// シーン描画の元のサイズ
	Vector2 defaultScreenSize_;
	// シーン内でのマウスの座標
	Vector2Flt sceneMousePoint_;
};

