#pragma once
#include "FreamBase.h"
class Fream_Stage :
    public FreamBase
{
public:
    Fream_Stage();
    ~Fream_Stage();

    void Init();
    void Update();
    void Draw();

    void Custom();

private:
    struct  stageGrid
    {
        float lineNum;
        float lineSize;
        float lineColorR;
        float lineColorG;
        float lineColorB;
    };

    int modelH_;
    int pixelShaderH_;
    int vertexShaderH_;
    int buffer_;
    float lineNum_;
    float lineSize_;
    float color_[3];
    float scale_;
    int screen_;
};

