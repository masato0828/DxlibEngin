#include <Dxlib.h>
#include "Device_Mouse.h"

Device_Mouse::Device_Mouse()
{
    Init();
}

Device_Mouse::~Device_Mouse()
{
}

void Device_Mouse::Init()
{
    point_ = Vector2Flt();
    oneFreamMoveVolume_ = Vector2Flt();
    screenLeftUp_ = Vector2Flt();
    screenRightDown_ = Vector2Flt();
    defaultScreenSize_ = Vector2();
}

void Device_Mouse::Update(Vector2Flt screenLeftUp, Vector2Flt screenRightDown,Vector2 defaultScreenSize)
{
    screenLeftUp_ = screenLeftUp;
    screenRightDown_ = screenLeftUp;
    defaultScreenSize_ = defaultScreenSize;
}

void Device_Mouse::Draw()
{

    RECT cR;
    GetWindowClientRect(&cR);
    // 現在のマウスの位置の取得
    Vector2 mousePoint_;
    GetMousePoint(&mousePoint_.x_, &mousePoint_.y_);

    Vector2Flt mousePointFactor = {
    (static_cast<float>(mousePoint_.x_) + cR.left - screenLeftUp_.x_) /*/ imageSize_.x_*/,
    (static_cast<float>(mousePoint_.y_) + cR.top - screenLeftUp_.y_)/* / imageSize_.y_*/
    };

    Vector2Flt reductionScreenSize_ = screenRightDown_ - screenLeftUp_;
    Vector2Flt defaultScreenSize = { defaultScreenSize_.x_, defaultScreenSize_.y_ };
    mousePointFactor /= reductionScreenSize_;
    mousePointFactor *= defaultScreenSize;

    DrawCircleAA(
        mousePointFactor.x_,
        mousePointFactor.y_,
        10, 100, 0xff0000);
}

void Device_Mouse::SetPoint(Vector2Flt point)
{
    point_ = point;
}

void Device_Mouse::MoveOneFreamVolume()
{
}

Vector2Flt Device_Mouse::GetNowPoint()
{
    return point_;
}
