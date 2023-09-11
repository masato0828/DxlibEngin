#pragma once
#include "Geometry.h"
#include<memory>
#include <array>
#include <map>
#include "Shader\ShaderMng.h"
class Gizumo
{
public:
	Gizumo();
	~Gizumo();

	

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="sceneMousePoint">マウスのスクリーン座標</param>
	/// <param name="modelPos">モデルの位置orモデルのフレーム位置</param>
	void Update(Vector2Flt sceneMousePoint,Vector3& modelPos);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
private:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	Vector3 pos;	// 位置
	VECTOR start;	// カメラレイ始点
	VECTOR end;		// カメラレイ終点

	// 各座標の棒の構造体
	struct Stick
	{
		Vector3 front;	// 最大頂点
		Vector3 back;	// 最小頂点
		int color;		// 色
	};

	// 棒のタイプ
	enum STICK_TYPE
	{
		X,
		Y,
		Z,
		MAX
	};

	// 棒の管理変数
	std::array<Stick,STICK_TYPE::MAX> stick_;

	// 選択している棒
	STICK_TYPE selectStick_;

	// 棒のデフォルトカラー
	std::map<STICK_TYPE,int> defoStickColor_;

	// マウス関連
	int NowInput, EdgeInput, PrevInput;
	int Catch;
	int CatchMouseX, CatchMouseY;
	VECTOR catchPos_;

	// 位置関連
	VECTOR Catch3DModelPosition;
	VECTOR Catch3DHitPosition;
	VECTOR Catch2DHitPosition;

	VECTOR ScreenPos;

	VECTOR resultMovePos_;


	// 選択している棒
	STICK_TYPE selectStick2_;
};

