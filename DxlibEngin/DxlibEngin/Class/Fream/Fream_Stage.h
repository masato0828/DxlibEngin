#pragma once
#include "FreamBase.h"
#include "../System/ImguiImageStb.h"
#include "../Common/Vector2.h"
#include "../Common/Geometry.h"

#include "../Common/Shader/ShaderMng.h"

class Fream_Stage :
    public FreamBase
{
public:
    Fream_Stage();
    ~Fream_Stage();

    /// <summary>
    /// 初期化
    /// </summary>
    void Init();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// パラメータの更新
    /// </summary>
    void Custom();

    /// <summary>
    /// 上から見た画像の作成
    /// </summary>
    void PreviewMake();

    /// <summary>
    /// プレビュータイプの変更
    /// </summary>
    void PreviewTypeChange();
private:

    /// <summary>
    /// 
    /// </summary>
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

