#include "PostEffect_ScanLine.h"
#include "ShaderMng.h"
#include "../PostMyGraph.h"
#include <DxLib.h>
#include "../../../imGui/imgui.h"

PostEffect_ScanLine::PostEffect_ScanLine()
{
	Init();
}

PostEffect_ScanLine::~PostEffect_ScanLine()
{
}

void PostEffect_ScanLine::Init()
{
	lpShaderMng.LoadShader(L"scanLine", "", "data/ShaderBinary/Pixel/ScanLine.ps", sizeof(ScanLine) * 8);

    pram_.time = 0.0f;
    pram_.pram1 = 2.0f;
    pram_.pram2 = 0.1f;
    pram_.pram3 = 0.99f;
    pram_.pram4 = 50.0f;
    pram_.pram5 = 100.0f;
    pram_.pram6 = 0.05f;
    pram_.pram7 = 0.08f;

    speed_ = 0.1f;
}

void PostEffect_ScanLine::Update()
{
    pram_.time += 0.1f;
}

void PostEffect_ScanLine::Draw(std::wstring name, const int imageHnadle)
{
    if (name == L"scanLine")
    {
        // postEffect
        lpShaderMng.DrawBegin(name);
        lpShaderMng.SetTexture(SLOT_TYPE::DEFFUSE, imageHnadle);
        ScanLine* cbBuf = (ScanLine*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        cbBuf[0].time = pram_.time;
        cbBuf[0].pram1 = pram_.pram1;
        cbBuf[0].pram2 = pram_.pram2;
        cbBuf[0].pram3 = pram_.pram3;
        cbBuf[0].pram4 = pram_.pram4;
        cbBuf[0].pram5 = pram_.pram5;
        cbBuf[0].pram6 = pram_.pram6;
        cbBuf[0].pram7 = pram_.pram7;
        //// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
        UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        //// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
        //// 引数の三番目はレジスタに設定している番号
        SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name), DX_SHADERTYPE_PIXEL, 5);
        MyDrawGraph(0, 0, imageHnadle);
        //DrawGraph(0,0,screen_,true);
        lpShaderMng.DrawEnd();
    }
}

void PostEffect_ScanLine::Custom()
{
    ImGui::DragFloat("speed",&speed_,0.01f);
    ImGui::DragFloat("pram1",&pram_.pram1,speed_);
    ImGui::DragFloat("pram2",&pram_.pram2, speed_);
    ImGui::DragFloat("pram3",&pram_.pram3, speed_);
    ImGui::DragFloat("pram4",&pram_.pram4, speed_);
    ImGui::DragFloat("pram5",&pram_.pram5, speed_);
    ImGui::DragFloat("pram6",&pram_.pram6, speed_);
    ImGui::DragFloat("pram7",&pram_.pram7, speed_);
}
