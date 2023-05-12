#include "Utility.h"
#include <DxLib.h>

float Deg2Rad(float deg)
{
    return (deg * (DX_PI_F / 180.0f));
}