// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 dif : COLOR; // �f�B�t���[�Y
    float4 spc : COLOR1; // �X�y�L����
    float2 uv : TEXCOORDO; // �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 color : SV_TARGET0; // �F
};

SamplerState mainSampler; // �T���v��(��q)
Texture2D mainTexture : register(t0); // �e�N�X�`��

cbuffer args : register(b5)
{
    float time;
    float noise1;
    float noise2;
    float noise3;
};

float WhiteNoise(float2 coord)
{
    //return frac(sin(dot(coord, float2(8.7819, 3.255))) * 437.645);
    return frac(sin(dot(coord, float2(noise1, noise2))) * noise3);
}

//float4 main(Pixel_UV pixel) : SV_TARGET
float4 main(PS_INPUT pixel) : SV_TARGET
{
    float2 samplePoint = pixel.uv;
    float4 Tex = mainTexture.Sample(mainSampler, samplePoint);
    float noise = WhiteNoise(pixel.uv * time) - 0.5;
    Tex.rgb += float3(noise, noise, noise);
    return Tex;
}