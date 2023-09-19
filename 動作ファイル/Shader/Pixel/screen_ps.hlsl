// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 dif : COLOR; // ディフューズ
    float4 spc : COLOR1; // スペキュラ
    float2 uv : TEXCOORDO; // テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 color : SV_TARGET0; // 色
};

SamplerState mainSampler; // サンプラ(後述)
Texture2D mainTexture : register(t0); // テクスチャ

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