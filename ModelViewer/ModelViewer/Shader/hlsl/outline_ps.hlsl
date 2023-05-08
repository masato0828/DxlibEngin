
//// �s�N�Z���V�F�[�_�[�̓���
//struct PS_INPUT
//{
//    float4 Position : SV_POSITION; // ���W( �v���W�F�N�V������� )
//    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
//    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
//};

//// �萔�o�b�t�@
//cbuffer cbParam : register(b0)
//{
//    float4 g_color;
//}

//// �`�悷��e�N�X�`��
//Texture2D g_SrcTexture : register(t0);

//// �T���v���[�F�K�؂ȐF�����߂鏈��
//// UV�ƃs�N�Z�����W�ɑΉ������œK�ȐF(��)���擾����
//SamplerState g_SrcSampler : register(s0);

//////���T���v���[��Ǝ��ɒ�`����
//SamplerState TextureSampler
//{
//    Filter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
//    //Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
//};

//float4 main(PS_INPUT PSInput) : SV_TARGET
//{
//    // UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
//    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, PSInput.TexCoords0);
//    //float4 srcCol = g_SrcTexture.Sample(TextureSampler, PSInput.TexCoords0);
    
        
//    if (srcCol.a < 0.01f)
//    {
//        // �`�悵�Ȃ�
//        discard;
//    }
    
//    return srcCol * float4(1.0f, 0.0f, 0.0f, 1.0f);
//}

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float2 TexCoords0 : TEXCOORD0;
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


// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    float4 TextureDiffuseColor;

	// �e�N�X�`���J���[�̓ǂݍ���
    //TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
    TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);

	// �o�̓J���[ = �e�N�X�`���J���[
    PSOutput.Color0 = TextureDiffuseColor;

	// �o�̓A���t�@ = �e�N�X�`���A���t�@ * �s�����x
    PSOutput.Color0.a = TextureDiffuseColor.a * g_Base.FactorColor.a;
    
    if (PSOutput.Color0.a < 0.01f)
    {
        // �`�悵�Ȃ�
        discard;
    }
    
    PSOutput.Color0.rgba = PSOutput.Color0.rgba * float4(1.0f, 1.0f, 1.0f, 1.0f);
	// �o�̓p�����[�^��Ԃ�
    return PSOutput;
}

