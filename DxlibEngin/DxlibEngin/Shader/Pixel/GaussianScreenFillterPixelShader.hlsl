#include "PixelShader2DHeader.hlsli"

Texture2D decaltTex : register(t1); //�e�N�X�`��
SamplerState decalTexSampler : register(s1); //�T���v���[

// ���������ւ̂ڂ���
PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    float4 defaultColor = tex.Sample(texSampler, input.uv);
    float4 decalColor = decaltTex.Sample(decalTexSampler, input.uv);
    
    float4 blend = (1.0f - ((1.0f - defaultColor) * (1.0f - (decalColor * 0.8f))));
    
    output = blend;
    
    return output;
}
