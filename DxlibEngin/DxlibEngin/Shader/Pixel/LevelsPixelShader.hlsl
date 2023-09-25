#include "PixelShader2DHeader.hlsli"

cbuffer args : register(b5)
{
    float rblack;
    float rwhite;
    float gblack;
    float gwhite;
    float bblack;
    float bwhite;
};

// mainŠÖ”
PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    float4 defaultColor = tex.Sample(texSampler, input.uv);
    float2 uv = input.uv;
	
    // ƒŒƒxƒ‹•â³(Œ³‚ÌF*255-•)/(”’-•)
    float r = (defaultColor.r * 255.0f - rblack) / (rwhite - rblack);
    float g = (defaultColor.g * 255.0f - gblack) / (gwhite - gblack);
    float b = (defaultColor.b * 255.0f - bblack) / (bwhite - bblack);
    float a = (defaultColor.a * 255.0f - 110.0f) / (155.0f - 110.0f);
    
    output = float4(r, g, b, a);
    
    return output;
}