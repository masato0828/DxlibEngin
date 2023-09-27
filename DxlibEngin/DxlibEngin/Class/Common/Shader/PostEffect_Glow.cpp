#include "PostEffect_Glow.h"
#include "ShaderMng.h"
#include "../PostMyGraph.h"
#include <DxLib.h>
#include "../../../imGui/imgui.h"

PostEffect_Glow::PostEffect_Glow()
{
    Init();
}

PostEffect_Glow::~PostEffect_Glow()
{
}

void PostEffect_Glow::Init()
{
	lpShaderMng.LoadShader(L"level", "", "data/ShaderBinary/Pixel/LevelsPixelShader.ps", sizeof(LevelParam) * 8);
    lpShaderMng.LoadShader(L"gaussH", "", "data/ShaderBinary/Pixel/GaussianHPixelShader.ps", sizeof(Blur) * 8);
    lpShaderMng.LoadShader(L"gaussV", "", "data/ShaderBinary/Pixel/GaussianVPixelShader.ps", sizeof(Blur) * 8);
    lpShaderMng.LoadShader(L"glow", "", "data/ShaderBinary/Pixel/GaussianScreenFillterPixelShader.ps",0);

	levelPram_.rblack = 110.0f;
    levelPram_.rwhite = 155.0f;
    levelPram_.gblack = 110.0f;
    levelPram_.gwhite = 155.0f;
    levelPram_.bblack = 110.0f;
    levelPram_.bwhite = 155.0f;

    blurPram_.sizeX = 1920;
    blurPram_.sizeY = 1080;

    int ww, wh;
    GetWindowSize(&ww, &wh);
    screen_levels_ = MakeScreen(ww, wh);
    screen_gaussH_ = MakeScreen(ww,wh);
    screen_gaussV_ = MakeScreen(ww,wh);
    mainScreen_ = MakeScreen(ww,wh);
}

void PostEffect_Glow::Update()
{
}

void PostEffect_Glow::Draw(std::wstring name, const int imageHnadle)
{
    if (name == L"glow")
    {
        SetDrawScreen(screen_levels_);
        ClearDrawScreen();
        {
            lpShaderMng.DrawBegin(L"level");
            lpShaderMng.SetModelTexture(SLOT_TYPE::DEFFUSE, imageHnadle);
            LevelParam* cbBuf = (LevelParam*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"level"));
            cbBuf[0].rblack = levelPram_.rblack;
            cbBuf[0].rwhite = levelPram_.rwhite;
            cbBuf[0].gblack = levelPram_.gblack;
            cbBuf[0].gwhite = levelPram_.gwhite;
            cbBuf[0].bblack = levelPram_.bblack;
            cbBuf[0].bwhite = levelPram_.bwhite;
            //// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
            UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"level"));
            //// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
            //// 引数の三番目はレジスタに設定している番号
            SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"level"), DX_SHADERTYPE_PIXEL, 5);
            MyDrawGraph(0, 0, imageHnadle);
            lpShaderMng.EndTextere(SLOT_TYPE::DEFFUSE);
            lpShaderMng.DrawEnd();
        }

        SetDrawScreen(screen_gaussH_);
        ClearDrawScreen();
        {
            lpShaderMng.DrawBegin(L"gaussH");
            lpShaderMng.SetModelTexture(SLOT_TYPE::DEFFUSE, screen_levels_);
            Blur* cbBuf = (Blur*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"gaussH"));
            cbBuf[0].sizeX = blurPram_.sizeX;
            cbBuf[0].sizeY = blurPram_.sizeY;
            MyDrawGraph(0, 0, screen_levels_);
            UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"gaussH"));
            SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"gaussH"), DX_SHADERTYPE_PIXEL,6);
            lpShaderMng.EndTextere(SLOT_TYPE::DEFFUSE);
            lpShaderMng.DrawEnd();
        }

        SetDrawScreen(screen_gaussV_);
        ClearDrawScreen();
        {
            MV1SetUseOrigShader(true);
            lpShaderMng.DrawBegin(L"gaussV");
            lpShaderMng.SetModelTexture(SLOT_TYPE::DEFFUSE, screen_gaussH_);
            Blur* cbBuf = (Blur*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"gaussV"));
            cbBuf[0].sizeX = blurPram_.sizeX;
            cbBuf[0].sizeY = blurPram_.sizeY;
            MyDrawGraph(0, 0, screen_gaussH_);
            UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"gaussV"));
            SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(L"gaussV"), DX_SHADERTYPE_PIXEL, 6);
            lpShaderMng.EndTextere(SLOT_TYPE::DEFFUSE);
            lpShaderMng.DrawEnd();
            MV1SetUseOrigShader(false);
        }

        SetDrawScreen(DX_SCREEN_BACK);
        ClearDrawScreen();

        DrawGraph(0,0, screen_gaussV_,true);

        /*lpShaderMng.DrawBegin(name);
        SetUseTextureToShader(0, imageHnadle);
        SetUseTextureToShader(1, screen_gaussV_);
        MyDrawGraph(0, 0, imageHnadle);
        SetUseTextureToShader(0, -1);
        SetUseTextureToShader(1, -1);
        lpShaderMng.DrawEnd();*/
    }
}

void PostEffect_Glow::Custom()
{
    ImGui::Text("level");
    ImGui::DragFloat("rblack", &levelPram_.rblack, 1.f, 0.0f,255.0f);
    ImGui::DragFloat("rwhite", &levelPram_.rwhite, 1.f, 0.0f, 255.0f);
    ImGui::DragFloat("gblack", &levelPram_.gblack, 1.f, 0.0f, 255.0f);
    ImGui::DragFloat("gwhite", &levelPram_.gwhite, 1.f, 0.0f, 255.0f);
    ImGui::DragFloat("bblack", &levelPram_.bblack, 1.f, 0.0f, 255.0f);
    ImGui::DragFloat("bwhite", &levelPram_.bwhite, 1.f, 0.0f, 255.0f);
    ImGui::Text("Blue");
    ImGui::DragFloat("sizeX", &blurPram_.sizeX, 1.f, 0.0f);
    ImGui::DragFloat("sizeY", &blurPram_.sizeY, 1.f, 0.0f);


}
