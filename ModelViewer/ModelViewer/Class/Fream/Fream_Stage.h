#pragma once
#include "FreamBase.h"
#include "../System/ImguiImageStb.h"
#include "../Common/Vector2.h"
#include "../Common/Geometry.h"

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

    void PreviewMake();

    void PreviewTypeChange();
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

    // シェーダ情報の作成
    ID3D11ShaderResourceView* my_shaderData;
    Vector2 imageSize_;
    
    struct PreviewData
    {
        Vector3 cameraPos_;
        Vector3 cameraRot_;
        bool previewTypeChange_;
    } previewData;


};

