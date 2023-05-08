
//// ピクセルシェーダーの入力
//struct PS_INPUT
//{
//    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
//    float4 Diffuse : COLOR0; // ディフューズカラー
//    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
//};

//// 定数バッファ
//cbuffer cbParam : register(b0)
//{
//    float4 g_color;
//}

//// 描画するテクスチャ
//Texture2D g_SrcTexture : register(t0);

//// サンプラー：適切な色を決める処理
//// UVとピクセル座標に対応した最適な色(※)を取得する
//SamplerState g_SrcSampler : register(s0);

//////→サンプラーを独自に定義する
//SamplerState TextureSampler
//{
//    Filter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
//    //Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
//};

//float4 main(PS_INPUT PSInput) : SV_TARGET
//{
//    // UV座標とテクスチャを参照して、最適な色を取得する
//    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, PSInput.TexCoords0);
//    //float4 srcCol = g_SrcTexture.Sample(TextureSampler, PSInput.TexCoords0);
    
        
//    if (srcCol.a < 0.01f)
//    {
//        // 描画しない
//        discard;
//    }
    
//    return srcCol * float4(1.0f, 0.0f, 0.0f, 1.0f);
//}

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float2 TexCoords0 : TEXCOORD0;
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


// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    float4 TextureDiffuseColor;

	// テクスチャカラーの読み込み
    //TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
    TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);

	// 出力カラー = テクスチャカラー
    PSOutput.Color0 = TextureDiffuseColor;

	// 出力アルファ = テクスチャアルファ * 不透明度
    PSOutput.Color0.a = TextureDiffuseColor.a * g_Base.FactorColor.a;
    
    if (PSOutput.Color0.a < 0.01f)
    {
        // 描画しない
        discard;
    }
    
    PSOutput.Color0.rgba = PSOutput.Color0.rgba * float4(1.0f, 1.0f, 1.0f, 1.0f);
	// 出力パラメータを返す
    return PSOutput;
}

