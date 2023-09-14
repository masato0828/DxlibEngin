#include "PixelShader3DHeader.hlsli"

struct PsColorPram
{
    float3 diffuse;
    float3 specular;
};

cbuffer cbD3D11_CONST_BUFFER_PS_COLOR_REAM : register(b3)
{
    PsColorPram colorPram;
};

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);

    const float3 ray = normalize(input.viewPos);            //�J���������g�̃x�N�g��
    const float3 normal = normalize(input.viewNorm);        //�@��(�r���[)

    //���C�e�B���O�����ł̍��v�F
    float3 totalDiffuse = colorPram.diffuse;
    float3 totalSpecular = colorPram.specular;

    //���C�g���ƂɃ��C�e�B���O����
    for (int i = 0; i < DX_D3D11_COMMON_CONST_LIGHT_NUM; i++)
    {
        ProcessLight(input.viewPos, ray, normal, common.light[i], common.material.power, totalDiffuse, totalSpecular);
    }

    //������K�p
    totalDiffuse += common.material.ambientEmissive.rgb;

    //���x�Ƀ}�e���A���̐F��K�p
    totalDiffuse *= common.material.diffuse.rgb;
    totalSpecular *= common.material.specular.rgb;

    //���̐F�Ƀ}�e���A����K�p
    result.rgb *= totalDiffuse;
    result.rgb += totalSpecular;
    result.a *= common.material.diffuse.a * base.factorColor.a;

    //���Z�F��K�p
    result.rgb += base.drawAddColor.rgb;

    //0�`1�ɑ����ĕԂ�
    return saturate(result);
}
