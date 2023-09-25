#include "PostEffect_Noise.h"
#include "ShaderMng.h"
#include "../PostMyGraph.h"
#include <DxLib.h>
#include "../../../imGui/imgui.h"

PostEffect_Noise::PostEffect_Noise()
{
    Init();
}

PostEffect_Noise::~PostEffect_Noise()
{
}

void PostEffect_Noise::Init()
{
	lpShaderMng.LoadShader(L"noise", "", "data/ShaderBinary/Pixel/screen_ps.ps", sizeof(Noise_Effect) * 8);

	pram_.time = 0.0f;
	pram_.noise1 = 8.7819f;
	pram_.noise2 = 3.255f;
	pram_.noise3 = 437.645f;
}

void PostEffect_Noise::Update()
{
	pram_.time += 0.1f;
}

void PostEffect_Noise::Draw(std::wstring name, const int imageHnadle)
{
    if (name == L"noise")
    {
        MV1SetUseOrigShader(true);
        // postEffect
        lpShaderMng.DrawBegin(name);
        lpShaderMng.SetModelTexture(SLOT_TYPE::DEFFUSE, imageHnadle);
        Noise_Effect* cbBuf = (Noise_Effect*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        cbBuf[0].time = pram_.time;
        cbBuf[0].noise1 = pram_.noise1;
        cbBuf[0].noise2 = pram_.noise2;
        cbBuf[0].noise3 = pram_.noise3;
        //// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
        UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        //// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
        //// 引数の三番目はレジスタに設定している番号
        SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name), DX_SHADERTYPE_PIXEL, 5);
        MyDrawGraph(0, 0, imageHnadle);
        //DrawGraph(0,0,screen_,true);
        lpShaderMng.DrawEnd();
        MV1SetUseOrigShader(false);
    }
}

void PostEffect_Noise::Custom()
{
    ImGui::DragFloat("noise1",&pram_.noise1,0.001f);
    ImGui::DragFloat("noise2",&pram_.noise2,0.001f);
    ImGui::DragFloat("noise3",&pram_.noise3,0.001f);
}
