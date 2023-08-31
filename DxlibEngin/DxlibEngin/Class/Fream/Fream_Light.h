#pragma once
#include "FreamBase.h"
class Fream_Light :
    public FreamBase
{
public:
    Fream_Light();
    ~Fream_Light();

    void Init()override;
    void Update()override;
    void Draw();
private:

};

