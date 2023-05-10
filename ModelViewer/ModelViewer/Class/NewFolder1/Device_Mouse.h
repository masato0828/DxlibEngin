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

	void SetPoint(Vector2Flt point);
	// 1ÉtÉåÅ[ÉÄä‘Ç≈ìÆÇ¢ÇΩãóó£
	void MoveOneFreamVolume();
	Vector2Flt GetNowPoint();
private:
	Vector2Flt point_;
	Vector2Flt oneFreamMoveVolume_;

	Vector2Flt screenLeftUp_;
	Vector2Flt screenRightDown_;
	Vector2 defaultScreenSize_;
};

