#include "PostEffect_Transition_Circle.h"
#include "ShaderMng.h"
#include "../PostMyGraph.h"
#include <DxLib.h>
#include "../../../imGui/imgui.h"
#include "../../Common/Utility.h"

PostEffect_Transition_Circle::PostEffect_Transition_Circle()
{
	Init();
}

PostEffect_Transition_Circle::~PostEffect_Transition_Circle()
{
}

void PostEffect_Transition_Circle::Init()
{
	lpShaderMng.LoadShader(L"circle", "data/ShaderBinary/Vertex/Circle_vs.vs", "data/ShaderBinary/Pixel/Circle_ps.ps", sizeof(Transition_Circle) * 16);

    pram_.color = {0.0f,0.0f,1.0f,1.0f};

    int ww, wh;
    GetWindowSize(&ww, &wh);
    pram_.screenSize = { ww,wh };
    pram_.dir = {1.0f,1.0f};
    pram_.div = 36;
    pram_.val = 0;
    pram_.width = 0.15f;
    pram_.revers = 0.0f;
    timeSpeed_ = 0.02f;
    
}

void PostEffect_Transition_Circle::Update()
{
    pram_.val += timeSpeed_;
}

void PostEffect_Transition_Circle::Draw(std::wstring name, const int imageHnadle)
{
    if (name == L"circle")
    {
        DrawGraph(0, 0, imageHnadle, true);
        // postEffect
        lpShaderMng.DrawBegin(name);
        lpShaderMng.SetTexture(SLOT_TYPE::DEFFUSE, imageHnadle);
        Transition_Circle* cbBuf = (Transition_Circle*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        cbBuf[0].color = pram_.color;
        cbBuf[0].screenSize = pram_.screenSize;
        cbBuf[0].dir = pram_.dir;
        cbBuf[0].div = pram_.div;
        cbBuf[0].val = pram_.val;
        cbBuf[0].width = pram_.width;
        cbBuf[0].revers = pram_.revers;

        //// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
        UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        //// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
        //// 引数の三番目はレジスタに設定している番号
        SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name), DX_SHADERTYPE_PIXEL, 7);
        MyDrawGraph(0, 0, imageHnadle);
        //
        lpShaderMng.DrawEnd();
    }
}

void PostEffect_Transition_Circle::Custom()
{
    ImGuiCustom::ColorEdit4("color", &pram_.color);
    ImGui::DragFloat("val",&pram_.val);
    ImGui::DragFloat2("dir", &pram_.dir,0.1f);
    ImGui::InputInt("div",&pram_.div);
    //ImGui::InputFloat("size",&pram_.val,1.0f);
    ImGui::InputFloat("width",&pram_.width,0.1f);
    ImGui::InputFloat("timeSpeed",&timeSpeed_,0.01f);

    static int select;
    if (ImGui::RadioButton("revers [inside]", &select, 0))
    {
        pram_.revers = 0.0f;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("revers [outside]", &select, 1))
    {
        pram_.revers = 1.0f;
    }

}
