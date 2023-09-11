#include "PixelShader3DHeader.hlsli"

//�@���}�b�v�Ȃ��̏ꍇ
struct VertexToPixel
{
    float4 svPos : SV_POSITION; //�ʒu(�v���W�F�N�V����)
    float3 worldPos : POSITION0; //�ʒu(���[���h)
    float3 viewPos : POSITION1; //�ʒu(�r���[)
    float3 viewNorm : NORMAL0; //�@��
    float4 diffuse : COLOR0; //�g�U���ːF
    float4 specular : COLOR1; //���ʔ��ːF
    float2 uv : TEXCOORD; //UV�l
};

#define PS_INPUT VertexToPixel
#define PS_OUTPUT float4

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    const float3 ray = normalize(input.viewPos); //�J���������g�̃x�N�g��
    const float3 normal = normalize(input.viewNorm); // �@��(�r���[)
    
    // ���C�e�B���O�����ł̍��v�F
    float3 totalDiffuse = float3(0,0,0);
    float3 totalSpecular = float3(0,0,0);
    
    // ���C�g���ƂɃ��C�e�B���O����
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