#pragma once
#include "FreamBase.h"
#include "../Common/Vector2.h"
#include "../Common/Geometry.h"
class Fream_Model :
    public FreamBase
{
public:
    Fream_Model();
    ~Fream_Model();
    void Init()override;
    void Update()override;

    void Draw();
private:

};

