#include "PixelShader2DHeader.hlsli"


// …•½•ûŒü‚Ö‚Ì‚Ú‚©‚µ
static const float2 PixelKernel[13] =
{
    { 0, -6 },
    { 0, -5 },
    { 0, -4 },
    { 0, -3 },
    { 0, -2 },
    { 0, -1 },
    { 0, 0 },
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 0, 4 },
    { 0, 5 },
    { 0, 6 },
};

static const float BlurVerticals[13] =
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

// …•½•ûŒü‚Ö‚Ì‚Ú‚©‚µ
PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    float4 color = 0;
    float2 TexSize = float2(1.0f / sizeX, 1.0f / sizeY);

    for (int p = 0; p < 13; p++)
    {
        color += tex.Sample(texSampler, input.uv + (PixelKernel[p].xy * TexSize)) * BlurVerticals[p];
    }

    output = color;
    return output;
}