#include "PostEffect_Led.h"
#include "ShaderMng.h"
#include "../PostMyGraph.h"
#include <DxLib.h>
#include "../../../imGui/imgui.h"

PostEffect_Led::PostEffect_Led()
{
    Init();
}

PostEffect_Led::~PostEffect_Led()
{
}

void PostEffect_Led::Init()
{
    lpShaderMng.LoadShader(L"led", "", "data/ShaderBinary/Pixel/LedPixelShader.ps",0);

    int ww, wh;
    GetWindowSize(&ww, &wh);

    pram_.wdith = ww;
    pram_.height = wh;

    
}

void PostEffect_Led::Update()
{
   
}

void PostEffect_Led::Draw(std::wstring name, const int imageHnadle)
{
    if (name == L"led")
    {
        // postEffect
        lpShaderMng.DrawBegin(name);
        lpShaderMng.SetModelTexture(SLOT_TYPE::DEFFUSE, imageHnadle);
        //Noise_Effect* cbBuf = (Noise_Effect*)GetBufferShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        //cbBuf[0].time = pram_.time;
        //cbBuf[0].noise1 = pram_.noise1;
        //cbBuf[0].noise2 = pram_.noise2;
        //cbBuf[0].noise3 = pram_.noise3;
        ////// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
        //UpdateShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name));
        ////// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@��萔�o�b�t�@���W�X�^�ɃZ�b�g
        ////// �����̎O�Ԗڂ̓��W�X�^�ɐݒ肵�Ă���ԍ�
        //SetShaderConstantBuffer(lpShaderMng.GetConstansBufferHnadle(name), DX_SHADERTYPE_PIXEL, 5);
        MyDrawGraph(0, 0, imageHnadle);
        
        //DrawGraph(0,0,screen_,true);
        lpShaderMng.DrawEnd();
    }
}

void PostEffect_Led::Custom()
{
    ImGui::DragFloat("height", &pram_.height, 1);
    ImGui::DragFloat("wdith", &pram_.wdith, 1);

    auto& noise = lpShaderMng.DataAcsess(L"led", "args");
    noise["height"].data = { pram_.height,0,0,0 };
    noise["wdith"].data = { pram_.wdith,0,0,0 };

    lpShaderMng.RegisterUpdate();
}
