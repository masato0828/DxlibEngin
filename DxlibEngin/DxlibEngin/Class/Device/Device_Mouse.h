#pragma once
#include "../Common/Vector2.h"
class Device_Mouse
{
public:
	Device_Mouse();
	~Device_Mouse();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="screenLeftUp">ウィンドウの左上</param>
	/// <param name="screenRightDown">ウィンドウの右下</param>
	/// <param name="defaultScreenSize">ウィンドウサイズ</param>
	void Update(Vector2Flt screenLeftUp, Vector2Flt screenRightDown, Vector2 defaultScreenSize);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// マウスの位置のセット 
	/// </summary>
	/// <param name="point">マウス位置</param>
	void SetPoint(Vector2 point);
	
	/// <summary>
	/// 1フレーム間で動いた距離
	/// </summary>
	void MoveOneFreamVolume();
	
	/// <summary>
	/// マウスの位置の取得
	/// </summary>
	/// <returns>マウスの位置</returns>
	Vector2 GetNowPoint();

	/// <summary>
	/// シーン内でのマウスの位置の作成 
	/// </summary>
	void SceneMouseCreate();

	/// <summary>
	/// シーン内でのマウスの位置の取得
	/// </summary>
	/// <returns>シーン内でのマウスの位置</returns>
	Vector2Flt GetSceneMousePoint();
	
	/// <summary>
	/// スクリーン座標のマウス位置の取得
	/// </summary>
	/// <returns>スクリーン座標のマウス位置</returns>
	Vector2Flt GetScreenMousePoint();
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

