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
    /// ������
    /// </summary>
    void Init();

    /// <summary>
    /// �X�V
    /// </summary>
    void Update();

    /// <summary>
    /// �`��
    /// </summary>
    void Draw();

    /// <summary>
    /// �p�����[�^�̍X�V
    /// </summary>
    void Custom();

    /// <summary>
    /// �ォ�猩���摜�̍쐬
    /// </summary>
    void PreviewMake();

    /// <summary>
    /// �v���r���[�^�C�v�̕ύX
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

    // �V�F�[�_���̍쐬
    ID3D11ShaderResourceView* my_shaderData;
    Vector2 imageSize_;
    
    struct PreviewData
    {
        Vector3 cameraPos_;
        Vector3 cameraRot_;
        bool previewTypeChange_;
    } previewData;



};

