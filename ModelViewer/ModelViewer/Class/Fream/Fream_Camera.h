#pragma once
#include "FreamBase.h"
#include "../Common/Geometry.h"
class Fream_Camera :
    public FreamBase
{
public:
    Fream_Camera();
    ~Fream_Camera();
	void Init()override;
	void Update()override;
	void Update(Vector2Flt mousePoint);
	void Angle();
	void Move();
	void Set();
	void PushMouseMove(Vector2Flt mousePoint);
	void MouseMove();

	void Custom();
private:
	Vector3 pos_;
	Vector3 rot_;

	// 注視点位置情報
	Vector3 targetPos_;

	// マウス用
	Vector2Dbl ma_;
	Vector2Dbl mb_;
	Vector2Dbl mc_;
	float sens_;
	int mouseX_;
	int mouseY_;
	int input_;

	bool isTarget_;

	int mauseHWheel_;

	float moveSpeed_;
	int moveSpeedBoost_;
};

