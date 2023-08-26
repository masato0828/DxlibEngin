#pragma once
#include "Geometry.h"
#include<memory>
#include "Shader\ShaderMng.h"
class Gizumo
{
public:
	Gizumo();
	~Gizumo();

	void Init();
	void Update(Vector2Flt sceneMousePoint, VECTOR cameraPos);
	void Draw();
private:
	Vector3 pos;

	VECTOR start;
	VECTOR end;

};

