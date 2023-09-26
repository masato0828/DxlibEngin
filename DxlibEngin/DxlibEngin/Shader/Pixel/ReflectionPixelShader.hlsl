#include "PixelShader3DHeader.hlsli"

//�G���g���[�|�C���g
PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{    
    float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    const float3 ray = normalize(input.viewPos); //�J���������g�̃x�N�g��
    
     ////�e�N�X�`��������ꍇ�͂�����l�������������s��
    uint2 diffuseMapSize;
    diffuseMapTexture.GetDimensions(diffuseMapSize.x, diffuseMapSize.y);
    if (diffuseMapSize.x * diffuseMapSize.y > 0)
    {
        float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, input.uv);
        if (texColor.a == 0.0f)
        {
            discard;
        }
        
        result *= texColor;
    }

    //�@���}�b�v����F���擾���A0�`1��-1�`1�ɕϊ�
    const float3 tanNormal = normalize(Sub0Texture.Sample(Sub0Sampler, input.uv).xyz * 2 - 1);
   
    const float3 normal = normalize(input.viewNorm+tanNormal); //�@��(�r���[)
   
    float3 cubeRef = reflect(-ray, normal);
    
    float4 cubeColor = 0.5f * cubeMapTexture.Sample(cubeMapSampler,cubeRef);
    
    //���C�e�B���O�����ł̍��v�F
    float3 totalDiffuse = (0, 0, 0);
    float3 totalSpecular = (0, 0, 0);
    
   
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
    return saturate(result+cubeColor);
}
