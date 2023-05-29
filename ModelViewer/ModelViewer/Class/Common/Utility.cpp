#include "Utility.h"
#include <DxLib.h>

// 度からラジアン角度に
float Deg2Rad(float deg)
{
    return (deg * (DX_PI_F / 180.0f));
}

// 最大公約数を求める
int Gcd(int x, int y)
{
	if (y == 0)
	{
		return x;
	}
	return Gcd(y, x % y);
}
