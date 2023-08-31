#pragma once
#include "Geometry.h"
#include<memory>
#include <array>
#include "Shader\ShaderMng.h"
class Gizumo
{
public:
	Gizumo();
	~Gizumo();

	void Init();
	void Update(Vector2Flt sceneMousePoint, VECTOR cameraPos,Vector3& modelPos);
	void Draw();
private:
	Vector3 pos;

	VECTOR start;
	VECTOR end;

	struct Stick
	{
		Vector3 front;
		Vector3 back;
		int color;
	};

	enum STICK_TYPE
	{
		CENTER,
		X,
		Y,
		Z,
		MAX
	};

	std::array<Stick,STICK_TYPE::MAX> stick_;

	STICK_TYPE selectStick_;
};

