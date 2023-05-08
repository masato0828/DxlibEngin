#pragma once
#include "../Common/Vector2.h"

struct MousePoint 
{
	struct windowImageData
	{
		Vector2 imageSize;
		Vector2Flt reftUpCornor;
		Vector2Flt rightDownCornor;
	};

	Vector2Flt Create(windowImageData data);
};