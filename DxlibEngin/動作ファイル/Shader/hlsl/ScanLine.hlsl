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
    float pram1;
    float pram2;
    float pram3;
    float pram4;
    float pram5;
    float pram6;
    float pram7;
};

//float4 main(Pixel_UV pixel) : SV_TARGET
float4 main(PS_INPUT pixel) : SV_TARGET
{
    float2 samplePoint = pixel.uv;
    float4 Tex = mainTexture.Sample(mainSampler, samplePoint);
    float sinv = sin(pixel.uv.y * pram1 + time * -pram2);
    float steped = step(pram3, sinv * sinv);
    Tex.rgb -= (1 - steped) * abs(sin(pixel.uv.y * pram4 + time * 1.0)) * pram6;
    Tex.rgb -= (1 - steped) * abs(sin(pixel.uv.y * pram5 - time * 2.0)) * pram7;
    Tex.rgb += steped * 0.1;
    return Tex;
}