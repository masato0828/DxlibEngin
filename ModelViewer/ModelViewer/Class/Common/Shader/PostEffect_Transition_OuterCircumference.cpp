#include "PostEffect_Transition_OuterCircumference.h"
#include "ShaderMng.h"
#include "../PostMyGraph.h"
#include <DxLib.h>
#include "../../../imGui/imgui.h"
#include "../../Common/ImGuiMyCustom.h"

PostEffect_Transition_OuterCircumference::PostEffect_Transition_OuterCircumference()
{
	Init();
}

PostEffect_Transition_OuterCircumference::~PostEffect_Transition_OuterCircumference()
{
}

void PostEffect_Transition_OuterCircumference::Init()
{
	lpShaderMng.LoadShader("outerCircumference", "Shader/vs/OuterCircumference_vs.vs", "Shader/ps/OuterCircumference_ps.ps", sizeof(Transition_OuterCircumference) * 8);

    pram_.size = {0.1f,0.1f };
    pram_.color = {0.0f,0.0f,1.0f,0.5f};
    pram_.time = 0.0f;
    pram_._rotation = 0;
    pram_._aspect = 1;
    int ww, wh;
    GetWindowSize(&ww,&wh);

    pram_._screenSize = {ww,wh};
}

void PostEffect_Transition_OuterCircumference::Update()
{
}

void PostEffect_Transition_OuterCircumference::Draw(std::string name, const int imageHnadle)
{
    if (name == "outerCircumference")
    {
        DrawGraph(0, 0, imageHnadle, true);
        MV1SetUseOrigShader(true);
        // postEffect
        lpShaderMng.Draw(name);
        lpShaderMng.SetTexture(0, imageHnadle);
        Transition_OuterCircumference* cbBuf = (Transition_OuterCircumference*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        cbBuf[0].size = pram_.size;
        cbBuf[0].color = pram_.color;
        cbBuf[0].time = pram_.time;
        cbBuf[0]._aspect = pram_._aspect;
        cbBuf[0]._rotation = pram_._rotation;
        cbBuf[0]._screenSize = pram_._screenSize;
        //// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
        UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        //// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
        //// 引数の三番目はレジスタに設定している番号
        SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name), DX_SHADERTYPE_PIXEL, 5);
        MyDrawGraph(0, 0, imageHnadle);
        //
        lpShaderMng.DrawEnd();
        MV1SetUseOrigShader(false);
    }
}

void PostEffect_Transition_OuterCircumference::Custom()
{
    ImGui::DragFloat2("size",&pram_.size,0.01f);
    ImGui::DragFloat2("screen",&pram_._screenSize,1.0f);
    ImGui::DragFloat("color.r", &pram_.color.r, 0.1f);
    ImGui::DragFloat("color.g", &pram_.color.g, 0.1f);
    ImGui::DragFloat("color.b", &pram_.color.b, 0.1f);
    ImGui::DragFloat("color.a", &pram_.color.a, 0.1f);
    ImGui::DragFloat("time",&pram_.time,0.1f);
    ImGui::DragFloat("aspect",&pram_._aspect,1.0f);
    ImGui::DragFloat("rotation", &pram_._rotation, 1.0f);
}
