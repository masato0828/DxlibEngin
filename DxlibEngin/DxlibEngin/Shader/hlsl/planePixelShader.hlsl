
// woald
// �o���������Ŋ���
// ����ɋ߂��Ƃ���`��
// �ȊOdiscard

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 pos : VS_POSITION;
    float3 wPos : Position;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // �F
};


// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
    float4 FactorColor; // �A���t�@�l��

    float MulAlphaColor; // �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
    float AlphaTestRef; // �A���t�@�e�X�g�Ŏg�p�����r�l
    float2 Padding1;

    int AlphaTestCmpMode; // �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
    int3 Padding2;

    float4 IgnoreTextureColor; // �e�N�X�`���J���[���������p�J���[
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
{
    DX_D3D11_PS_CONST_BUFFER_BASE g_Base;
};


SamplerState g_DiffuseMapSampler : register(s0); // �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D g_DiffuseMapTexture : register(t0); // �f�B�t���[�Y�}�b�v�e�N�X�`��

cbuffer GetLineVal : register(b5)
{
    float lineNum;
    float cnterLineSize;
    float lineColorR;
    float lineColorG;
    float lineColorB;
};




// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    
    float3 abs_ = abs(PSInput.wPos);
    float3 fmod_ = fmod(abs_, lineNum);
    float3 step_ = step(fmod_, cnterLineSize);
    
    
    step_.y = 0.0f;
    
    if (step_.x == 0.0f && step_.y == 0.0f && step_.z == 0.0f)
    {
        discard;
    }
   
    //PSOutput.Color0 = float4(step_ * float3(0.5f, 1.0f, 0.5f), 1.0f);
    //PSOutput.Color0 = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (step_.x == 1.0f)
    {
        
    }
    
    
    //PSOutput.Color0 = float4(
    //lineColor.x * (1 / 255),
    //lineColor.y * (1 / 255),
    //lineColor.z * (1 / 255), 
    //1.0f);
    
    PSOutput.Color0 = float4(
    lineColorR,
    lineColorG,
    lineColorB,
    1.0f);
    
	// �o�̓p�����[�^��Ԃ�
    return PSOutput;
}

