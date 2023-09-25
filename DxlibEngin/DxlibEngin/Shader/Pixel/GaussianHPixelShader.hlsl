#include "PixelShader2DHeader.hlsli"


// ���������ւ̂ڂ���
static const float2 PixelKernel[13] =
{
    { -6, 0 },
    { -5, 0 },
    { -4, 0 },
    { -3, 0 },
    { -2, 0 },
    { -1, 0 },
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
    { 4, 0 },
    { 5, 0 },
    { 6, 0 },
};

static const float BlurWeights[13] =
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

cbuffer args : register(b6)
{
    float sizeX;
    float sizeY;
};

// ���������ւ̂ڂ���
PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    float4 color = 0;
    //float2 TexSize = float2(1.0f / 1920.0f, 1.0f / 1080.0f);
    //float2 TexSize = float2(1.0f / 1024.0f, 1.0f / 768.0f);
    //float2 TexSize = float2(1.0f / 960.0f, 1.0f / 540.0f);
    float2 TexSize = float2(1.0f / sizeX, 1.0f / sizeY);
    
    for (int p = 0; p < 13; p++)
    {
        color += tex.Sample(texSampler, input.uv + (PixelKernel[p].xy * TexSize)) * BlurWeights[p];
    }

    output = color;
    return output;
    
    //float2 uv = input.uv;

    //// UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
    //float4 srcCol = tex.Sample(texSampler, uv);
    
    //// ����̉�f���擾����(�w�����)
    //float2 uv_dis = { -0.5f, 0.0f };
    
    //srcCol += tex.Sample(texSampler, uv + 0.5f * uv_dis * 0.25f);
    //srcCol += tex.Sample(texSampler, uv + 0.5f * uv_dis * 0.50f);
    //srcCol += tex.Sample(texSampler, uv + 0.5f * uv_dis * 0.75f);
    //srcCol += tex.Sample(texSampler, uv + 0.5f * uv_dis * 1.00f);
    
    //// �P������
    //srcCol /= 5.0f;
    
    //return srcCol;
}