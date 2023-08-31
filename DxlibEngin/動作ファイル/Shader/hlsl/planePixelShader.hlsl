
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


// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
    float4 FactorColor; // アルファ値等

    float MulAlphaColor; // カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
    float AlphaTestRef; // アルファテストで使用する比較値
    float2 Padding1;

    int AlphaTestCmpMode; // アルファテスト比較モード( DX_CMP_NEVER など )
    int3 Padding2;

    float4 IgnoreTextureColor; // テクスチャカラー無視処理用カラー
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
{
    DX_D3D11_PS_CONST_BUFFER_BASE g_Base;
};


SamplerState g_DiffuseMapSampler : register(s0); // ディフューズマップテクスチャ
Texture2D g_DiffuseMapTexture : register(t0); // ディフューズマップテクスチャ

cbuffer GetLineVal : register(b5)
{
    float lineNum;
    float cnterLineSize;
    float lineColorR;
    float lineColorG;
    float lineColorB;
};




// main関数
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
    
	// 出力パラメータを返す
    return PSOutput;
}

