#include <Dxlib.h>
#include "MousePointCreate.h"
#include "../../imGui/imgui.h"

Vector2Flt MousePoint::Create(windowImageData data)
{
    // 画像がウィンドウに作られた後取れる値
    Vector2 imageSize_ = {
       data.rightDownCornor.x_ - data.reftUpCornor.x_,
       data.rightDownCornor.y_ - data.reftUpCornor.y_ };

    RECT cR;
    GetWindowClientRect(&cR);
    // 現在のマウスの位置の取得
    Vector2 mousePoint_;
    GetMousePoint(&mousePoint_.x_, &mousePoint_.y_);

    Vector2Flt mousePointFactor = {
    (static_cast<float>(mousePoint_.x_) + cR.left - data.reftUpCornor.x_) /*/ imageSize_.x_*/,
    (static_cast<float>(mousePoint_.y_) + cR.top - data.reftUpCornor.y_)/* / imageSize_.y_*/
    };

    /*mousePointFactor.x_ *= data.imageSize.x_;
    mousePointFactor.y_ *= data.imageSize.y_;*/

    // マウスから画像サイズを引く
    return mousePointFactor;
}