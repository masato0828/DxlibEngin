#include <Dxlib.h>
#include <vector>
#include "Fream_Stage.h"
#include "../../imGui/imgui.h"


#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"
#include "../../imGui/imgui_internal.h"

#include "../Common/Utility.h"
#include "../Common/ImGuiMyCustom.h"

Fream_Stage::Fream_Stage()
{
    Init();
}

Fream_Stage::~Fream_Stage()
{
}

void Fream_Stage::Init()
{
    my_shaderData = NULL;

    modelH_ = MV1LoadModel("data/modelData/plane.mv1");

    lpShaderMng.LoadShader(
        "plane",
        "data/ShaderBinary/Vertex/planeVertexShader.vs",
        "data/ShaderBinary/Pixel/planePixelShader.ps",
        sizeof(stageGrid) * 8);

    lineNum_ = 79.8f;
    lineSize_ = 4.0f;
    color_[2] = color_[1] = color_[0] = 0.5f;
    scale_ = 8.0f;

    int x, y;
    GetWindowSize(&x, &y);
    screen_ = MakeScreen(x, y, true);

    previewData.cameraPos_ = { -243,185, -231 };
    previewData.cameraRot_ = { 0.420f,0.795f,0.0f };

    previewData.previewTypeChange_ = false;
}

void Fream_Stage::Update()
{
    //stageGrid* cbBuf = (stageGrid*)GetBufferShaderConstantBuffer(buffer_);
    stageGrid* cbBuf = (stageGrid*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle("plane"));
    cbBuf[0].lineNum = lineNum_;
    cbBuf[0].lineSize = lineSize_;
    cbBuf[0].lineColorR = color_[0];
    cbBuf[0].lineColorG = color_[1];
    cbBuf[0].lineColorB = color_[2];

    //// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
    UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle("plane"));

    //// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@��萔�o�b�t�@���W�X�^�ɃZ�b�g
    //// �����̎O�Ԗڂ̓��W�X�^�ɐݒ肵�Ă���ԍ�
    SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle("plane"), DX_SHADERTYPE_PIXEL, 5);
}

void Fream_Stage::Draw()
{
    //// �y�o�b�t�@��L���ɂ���
    SetUseZBuffer3D(false);

    //// �y�o�b�t�@�ւ̏������݂�L���ɂ���
    SetWriteZBuffer3D(false);


    MV1SetUseOrigShader(true);
    lpShaderMng.DrawBegin("plane");

    MV1SetPosition(modelH_, VGet(0, 0, 0));
    MV1SetRotationXYZ(modelH_, VGet(0, 0, 0));
    MV1SetScale(modelH_, { scale_,scale_,scale_ });
    MV1DrawModel(modelH_);

    lpShaderMng.DrawEnd();
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
    ImGui::Checkbox("previewType", &previewData.previewTypeChange_);
    
    // ���C���̃T�C�Y�̓��C���̐��܂�
    // ���������Ă��܂����ꍇ�́A���C���̐��ɍ��킹��
    lineSize_ = (std::min)(lineSize_, lineNum_);

    // ���C���̐��Ƒ傫���̍ŏ��l
    lineNum_ = (std::max)(lineNum_, 0.1f);
    lineSize_ = (std::max)(lineSize_, 0.1f);
    scale_ = (std::max)(scale_, 1.0f);

    LoadTextureFromFile(screen_, &my_shaderData, &imageSize_.x_, &imageSize_.y_);
    
    // �摜�̓ǂݍ���
    auto sizeX = ImGui::GetWindowSize().x;
    auto sizeY = ImGui::GetWindowSize().y;

    // ������
    auto x = ImGui::GetWindowSize().x / imageSize_.x_;
    // �c����
    auto y = ImGui::GetWindowSize().y / imageSize_.y_;

    // �W��
    auto factor = (std::min)(x, y);

    if (factor == 0)
    {
        ImGui::End();
        return;
    }

    ImGui::Image(
        // �摜���
        (void*)my_shaderData,
        // �摜�T�C�Y�������ŕς���
        ImVec2{ imageSize_.x_ * factor, imageSize_.y_ * factor },
        // UV1
        ImVec2{ 0,0 },
        // UV2
        ImVec2{ 1,1 },
        // �`��J���[
        ImVec4{ 1,1,1,1 },
        // �g�̃J���[
        ImVec4{ 0,0,0,0 });

    PreviewTypeChange();
}

void Fream_Stage::PreviewMake()
{
    SetDrawScreen(screen_);
    ClearDrawScreen();
    // �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
    // �J�����̃N���b�s���O������ݒ�
    SetCameraNearFar(10.0f, 300000.0f);
    // �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
    SetCameraPositionAndAngle(previewData.cameraPos_.toVECTOR(),
        previewData.cameraRot_.x_, previewData.cameraRot_.y_, previewData.cameraRot_.z_);
    Draw();
}

void Fream_Stage::PreviewTypeChange()
{
    if (!previewData.previewTypeChange_)
    {
        previewData.cameraPos_ = { -243,185, -231 };
        previewData.cameraRot_ = { 0.420f,0.795f,0.0f };
    }
    else
    {
        previewData.cameraPos_ = { 0,800+(100*scale_),0 };
        previewData.cameraRot_ = { Utility::Deg2Rad(90.0f),0.f,0.f};
    }
}
