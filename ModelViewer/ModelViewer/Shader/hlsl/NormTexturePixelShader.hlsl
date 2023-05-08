// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // ディフューズカラー
    float4 Specular : COLOR1; // スペキュラカラー
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float4 svPosition : SV_POSITION; // 座標( プロジェクション空間 )
    float4 Position : POSITION0;
    float4 P : POSITION1;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // 色
};

SamplerState g_DiffuseMapSampler : register(s0); // ディフューズマップテクスチャ
Texture2D g_DiffuseMapTexture : register(t0); // ディフューズマップテクスチャ

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    float4 TextureDiffuseColor;
    
	// テクスチャカラーの読み込み
    TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
    
	// 出力カラー = テクスチャカラー
    PSOutput.Color0 = TextureDiffuseColor;

	// 出力パラメータを返す
    return PSOutput;
}
