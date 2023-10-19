#include "PixelShader2DHeader.hlsli"

cbuffer args : register(b7)
{
    float wdith;
    float height;
};

float mod(float x, float y)
{
    return x - y * floor(x / y);
}

//エントリーポイント
PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    //HSVで基準色を設定
    float4 result = float4(1.f, 1.f, 1.f, 1.f);
    
    float3 tc = tex.Sample(texSampler, input.uv).rgb;
    
    // 1920,1080
    
    ///float vlines = floor(input.uv.x * 1920 * 0.5f);
    float vlines = floor(input.uv.x * 1920*5*0.5);
    float vrep = fmod(vlines,3.0f);
    
    tc += float3(0.15, 0.3, 0.45);
    tc += float3(0.5, 0., 0.) * float(vrep == 0.);
    tc += float3(0.0, 0.5, 0.) * float(vrep == 1.);
    tc += float3(0.0, 0., 0.5) * float(vrep == 2.);
    
    float hlines = floor(input.uv.y * 1080*5);
    float hrep = mod(hlines, 4.0f);
    
    tc *= step(hrep,2.0f);
    
    result = float4(tc,1.0f);
    
    return result;
}
