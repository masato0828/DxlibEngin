
// woald
// 出したい幅で割る
// ぜろに近いとこを描画
// 以外discard

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // ディフューズカラー
    float4 Specular : COLOR1; // スペキュラカラー
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float4 pos : VS_POSITION;
    float3 wPos : Position;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // 色
};

cbuffer GetLineVal : register(b5)
{
    float4 lineColor;
    float lineNum;
    float cnterLineSize;
};

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    
    float3 abs_ = abs(PSInput.wPos);
    //float3 fmod_ = fmod(abs_, lineNum);
    //float3 step_ = step(fmod_, cnterLineSize);
    float3 fmod_ = fmod(abs_, 79.8f);
    float3 step_ = step(fmod_, 4.0f);
    
    
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
    lineColor.r,
    lineColor.g,
    lineColor.b,
    1.0f);
    
	// 出力パラメータを返す
    return PSOutput;
}
