#include <Dxlib.h>
#include "Fream_Stage.h"
#include "../../imGui/imgui.h"

Fream_Stage::Fream_Stage()
{
    Init();
}

Fream_Stage::~Fream_Stage()
{
}

void Fream_Stage::Init()
{
    modelH_ = MV1LoadModel("data/modelData/plane.mv1");
    pixelShaderH_ = LoadPixelShader("Shader/ps/planePixelShader.ps");
    vertexShaderH_ = LoadVertexShader("Shader/vs/planeVertexShader.vs");

    buffer_ = CreateShaderConstantBuffer(sizeof(stageGrid) * 8);
    lineNum_ = 79.8f;
    lineSize_ = 2.0f;
    color_[2] = color_[1] = color_[0] =  0.5f;
    scale_ = 8.0f;

    int x, y;
    GetWindowSize(&x, &y);
    screen_ = MakeScreen(x, y);
}

void Fream_Stage::Update()
{
    stageGrid* cbBuf = (stageGrid*)GetBufferShaderConstantBuffer(buffer_);
    cbBuf[0].lineNum = lineNum_;
    cbBuf[0].lineSize = lineSize_;
    cbBuf[0].lineColorR = color_[0];
    cbBuf[0].lineColorG = color_[1];
    cbBuf[0].lineColorB = color_[2];

    // �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
    UpdateShaderConstantBuffer(buffer_);

    // �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@��萔�o�b�t�@���W�X�^�ɃZ�b�g
    // �����̎O�Ԗڂ̓��W�X�^�ɐݒ肵�Ă���ԍ�
    SetShaderConstantBuffer(buffer_, DX_SHADERTYPE_PIXEL, 5);
}

void Fream_Stage::Draw()
{
    //// �y�o�b�t�@��L���ɂ���
    SetUseZBuffer3D(false);

    //// �y�o�b�t�@�ւ̏������݂�L���ɂ���
    SetWriteZBuffer3D(false);


    MV1SetUseOrigShader(true);
    SetUseVertexShader(vertexShaderH_);
    SetUsePixelShader(pixelShaderH_);

    MV1SetPosition(modelH_, VGet(0, 0, 0));
    MV1SetRotationXYZ(modelH_, VGet(0, 0, 0));
    MV1SetScale(modelH_, { scale_,scale_,scale_ });
    MV1DrawModel(modelH_);

    MV1SetUseOrigShader(false);

    //// �y�o�b�t�@��L���ɂ���
    SetUseZBuffer3D(true);

    //// �y�o�b�t�@�ւ̏������݂�L���ɂ���
    SetWriteZBuffer3D(true);
}

void Fream_Stage::Custom()
{
    ImGui::DragFloat("scale", &scale_);
    ImGui::DragFloat("lineNum", &lineNum_);
    ImGui::DragFloat("lineSize", &lineSize_);
    ImGui::ColorEdit3("lineColor", color_);

    // ���C���̃T�C�Y�̓��C���̐��܂�
    // ���������Ă��܂����ꍇ�́A���C���̐��ɍ��킹��
    if (lineSize_ >= lineNum_)
    {
        lineSize_ = lineNum_;
    }
}
