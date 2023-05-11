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
    point_ = Vector2();
    oneFreamMoveVolume_ = Vector2Flt();
    screenLeftUp_ = Vector2Flt();
    screenRightDown_ = Vector2Flt();
    defaultScreenSize_ = Vector2();

    GetMousePoint(&point_.x_, &point_.y_);
}

void Device_Mouse::Update(Vector2Flt screenLeftUp, Vector2Flt screenRightDown,Vector2 defaultScreenSize)
{
    // �e���l�̎擾
    screenLeftUp_ = screenLeftUp;
    screenRightDown_ = screenRightDown;
    defaultScreenSize_ = defaultScreenSize;

    // �}�E�X�̌��݈ʒu�̎擾
    GetMousePoint(&point_.x_, &point_.y_);
    
    // �V�[�����̃}�E�X�̈ʒu�̍쐬
    SceneMouseCreate();

}

void Device_Mouse::Draw()
{

    Vector2Flt sceneMousePoint = sceneMousePoint_;
    Vector2Flt reductionScreenSize = screenRightDown_ - screenLeftUp_;
    Vector2Flt defaultScreenSize = { defaultScreenSize_.x_, defaultScreenSize_.y_ };
    sceneMousePoint /= reductionScreenSize;
    sceneMousePoint *= defaultScreenSize;

    // ���`��
    DrawCircleAA(
        sceneMousePoint.x_,
        sceneMousePoint.y_,
        10, 100, 0xff0000);
}

void Device_Mouse::SetPoint(Vector2 point)
{
    point_ = point;
}

void Device_Mouse::MoveOneFreamVolume()
{
}

Vector2 Device_Mouse::GetNowPoint()
{
    return point_;
}

void Device_Mouse::SceneMouseCreate()
{
    RECT cR;
    GetWindowClientRect(&cR);
    // ���݂̃}�E�X�̈ʒu�̎擾
    Vector2 mousePoint;
    GetMousePoint(&mousePoint.x_, &mousePoint.y_);

    sceneMousePoint_ = {
    (static_cast<float>(mousePoint.x_) + cR.left - screenLeftUp_.x_) /*/ imageSize_.x_*/,
    (static_cast<float>(mousePoint.y_) + cR.top - screenLeftUp_.y_)/* / imageSize_.y_*/
    };
}

Vector2Flt Device_Mouse::GetSceneMousePoint()
{
    return sceneMousePoint_;
}
