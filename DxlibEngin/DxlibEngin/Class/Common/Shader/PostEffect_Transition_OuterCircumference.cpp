#include "PostEffect_Transition_OuterCircumference.h"
#include "ShaderMng.h"
#include "../PostMyGraph.h"
#include <DxLib.h>
#include "../../../imGui/imgui.h"
#include "../../Common/Utility.h"

PostEffect_Transition_OuterCircumference::PostEffect_Transition_OuterCircumference()
{
	Init();
}

PostEffect_Transition_OuterCircumference::~PostEffect_Transition_OuterCircumference()
{
}

void PostEffect_Transition_OuterCircumference::Init()
{
	lpShaderMng.LoadShader(L"outerCircumference", "data/ShaderBinary/Vertex/OuterCircumference_vs.vs", "data/ShaderBinary/Pixel/OuterCircumference_ps.ps", sizeof(Transition_OuterCircumference) * 16);

    timespeed_ = 1.0f;

    pram_.div = 100.0f;
    pram_.color = {1.0f,1.0f,1.0f,1.0f};
    int ww, wh;
    GetWindowSize(&ww, &wh);
    pram_.screenSize = { ww,wh };
    pram_.time = 0.0f;
    pram_.direction = 0.0f;
    pram_.rotation = 0.0f;

    int num = Utility::Gcd(ww,wh);

    pram_.aspect.x_ = static_cast<float>(ww / num);
    pram_.aspect.y_ = static_cast<float>(wh / num);
    pram_.size = 1.0f;

    rotationDir_ = false;
}

void PostEffect_Transition_OuterCircumference::Update()
{
    pram_.time += timespeed_;
}

void PostEffect_Transition_OuterCircumference::Draw(std::wstring name, const int imageHnadle)
{
    if (name == L"outerCircumference")
    {
        DrawGraph(0, 0, imageHnadle, true);
        MV1SetUseOrigShader(true);
        // postEffect
        lpShaderMng.DrawBegin(name);
        lpShaderMng.SetModelTexture(SLOT_TYPE::DEFFUSE, imageHnadle);
        Transition_OuterCircumference* cbBuf = (Transition_OuterCircumference*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        cbBuf[0].color = pram_.color;
        cbBuf[0].screenSize = pram_.screenSize;
        cbBuf[0].aspect = pram_.aspect;
        cbBuf[0].time = pram_.time;
        cbBuf[0].div = pram_.div;
        cbBuf[0].direction = pram_.direction;
        cbBuf[0].rotation = pram_.rotation;
        cbBuf[0].size = pram_.size;

        //// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
        UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        //// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
        //// 引数の三番目はレジスタに設定している番号
        SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name), DX_SHADERTYPE_PIXEL, 7);
        MyDrawGraph(0, 0, imageHnadle);
        //
        lpShaderMng.DrawEnd();
        MV1SetUseOrigShader(false);
    }
}

void PostEffect_Transition_OuterCircumference::Custom()
{
    ImGuiCustom::ColorEdit4("color",&pram_.color);

    ImGui::DragFloat("time",&pram_.time,0.1f);
    ImGui::DragFloat("div", &pram_.div, 1.0f);
    static int select;
    if (ImGui::RadioButton("rotation dir [left]", &select, 0))
    {
        pram_.direction = 0.0f;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("rotation dir [right]", &select, 1))
    {
        pram_.direction = 1.0f;
    }
    ImGui::DragFloat("rotation",&pram_.rotation,0.1f);
    ImGui::DragFloat("size",&pram_.size,0.1f);
    ImGui::DragFloat("pramspeed", &timespeed_, 1.0f);
}
