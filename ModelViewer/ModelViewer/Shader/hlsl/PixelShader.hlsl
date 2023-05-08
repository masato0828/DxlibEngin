// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 svPosition : SV_POSITION; // ���W( �v���W�F�N�V������� )
    float4 Position : POSITION0;
    float4 P : POSITION1;
};


// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0;
    float4 hdr : SV_TARGET1;
    float4 noise : SV_TARGET2;
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
Texture2D pattern : register(t6);
Texture2D g_sceneTexture : register(t7);

cbuffer OreStruct : register(b0)
{
    float time;
    float threshold;
}

// �n�b�V���֐�
float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

// 3�����x�N�g������V���v���b�N�X�m�C�Y�𐶐�����֐�
float SimplexNoise(float3 x)
{
    // The noise function returns a value in the range -1.0f -> 1.0f
    float3 p = floor(x);
    float3 f = frac(x);

    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + 113.0 * p.z;

    return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
                     lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
                lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
                     lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
    
    PS_OUTPUT PSOutput;
    float4 TextureDiffuseColor;
    
    // �m�C�Y�̉摜
    float b = pattern.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
    // �ǂ̎��ōs����
    float alp = (PSInput.Position.x- threshold) / 200.0;
    // �m�C�Y�̌�����
    float noiseScale = 1.0f;
    // ����� + �m�C�Y * ???(�Ȃ��Ă�OK) * �m�C�Y�̌�����
    float alb = alp + b * (1.0 - alp) * noiseScale;
    
    // ����ȍ~�̃��b�V�����폜
    if (alb > 0.0)
    {
        discard;
    }
    
    //�@���̈ʒu�������Ă��[��
    alb = saturate(1.0 - abs(alb));
    
    // �f�B�]���u�ŕ\���������̑傫���i�傫�����邽�я������Ȃ�j
    float size = 15.0f;
    // �f�B�]���u�ŕ\���������̋���
    float lightPower = 5.0f;
    // ���f�[�^
    float bloom = pow(alb, size) * lightPower;

    
    // ���P�x�����̐F�f�[�^�̎擾
    PSOutput.hdr = float4(bloom, bloom, bloom,1);

	// �e�N�X�`���J���[�̓ǂݍ���
    TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);

	// �o�̓J���[ = �e�N�X�`���J���[
	PSOutput.Color0 = TextureDiffuseColor ;

	// �o�̓A���t�@ = �e�N�X�`���A���t�@ * �s�����x
    PSOutput.Color0.a = TextureDiffuseColor.a * g_Base.FactorColor.a;
    
    // �f�B�]���u�ŕ\���������F
    float3 disolveColor = float3(bloom, bloom * 0.5, bloom * 0.5);
    
    // ���l
    float alphaNum = 1.0f;
    
    // �f�t�H���g�̐F
    float3 defaultColor = PSInput.Diffuse.rgb;
    
    // �F�f�[�^�i���l�Ȃ��j
    float3 colorRGB = defaultColor + disolveColor;
    
    // �F�f�[�^
    float4 color = float4(colorRGB, alphaNum);
    
    // �F�f�[�^�̎擾
    PSOutput.Color0 += color;
    
    
    float2 uv = PSInput.P.xy * float2(0.5f, 0.5f) + 0.5f;
    
    float uOffset = SimplexNoise(float3(uv, 0.0f) * 256.0f) * 0.02f;
    
    float4 stealth = g_sceneTexture.Sample(g_DiffuseMapSampler, uv + uOffset);
    
    PSOutput.Color0 += stealth;
   
	// �o�̓p�����[�^��Ԃ�
    return PSOutput;
}
