#pragma once
#include "Geometry.h"
class Gizumo
{
public:
	Gizumo();
	~Gizumo();

	void Init();
	void Update();
	void Draw();
private:
	Vector3 pos;
};

