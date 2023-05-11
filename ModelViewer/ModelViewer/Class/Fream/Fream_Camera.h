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
	void Update(Vector2 mousePoint,Vector2Flt windowHlfeSize,Vector2 centerPoint);
	void Angle();
	void Move();
	void Set();
	void PushMouseMove();
	void MouseMove(Vector2 centerPoint);

	void Custom();
private:
	Vector3 pos_;
	Vector3 rot_;

	// 注視点位置情報
	Vector3 targetPos_;

	// マウス用
	Vector2Dbl aftorMousePoint_;
	Vector2Dbl beforMousePoint_;
	Vector2Dbl mousePointDiff_;
	float sens_;
	int mouseX_;
	int mouseY_;
	int input_;

	bool isTarget_;

	int mauseHWheel_;

	float moveSpeed_;
	int moveSpeedBoost_;

	Vector2 correctWindowLeftTop_;
	Vector2 windowHlfeSize_;

	bool mouseMove_;
	int cnt_;
	Vector2 mousePoint_;
};

