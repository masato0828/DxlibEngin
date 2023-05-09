#pragma once
#include "FreamBase.h"
#include "../Common/Vector2.h"

class Fream_SceneView :
    public FreamBase
{
public:
    Fream_SceneView();
    ~Fream_SceneView();
    void Init() override;
    void Update() override;
    void Create();

    float GetFactor();
    Vector2 GetDefaultImageSize();
    Vector2Flt GetImageLeftUpCornor();
    Vector2Flt GetImageRightDownCornor();
private:

    float factor_;

    Vector2 defaultImageSize_;

    Vector2Flt imageLeftUpCornor_;
    Vector2Flt imageRightDownCornor_;
};

