#include "Utility.h"
#include <DxLib.h>

float Deg2Rad(float deg)
{
    return (deg * (DX_PI_F / 180.0f));
}

int Gcd(int x, int y)
{
	if (y == 0)
	{
		return x;
	}
	return Gcd(y, x % y);
}
