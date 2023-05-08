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
    float4 Color0 : SV_TARGET0;
    float4 hdr : SV_TARGET1;
    float4 noise : SV_TARGET2;
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
Texture2D pattern : register(t6);
Texture2D g_sceneTexture : register(t7);

cbuffer OreStruct : register(b0)
{
    float time;
    float threshold;
}

// ハッシュ関数
float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

// 3次元ベクトルからシンプレックスノイズを生成する関数
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

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    
    PS_OUTPUT PSOutput;
    float4 TextureDiffuseColor;
    
    // ノイズの画像
    float b = pattern.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
    // どの軸で行うか
    float alp = (PSInput.Position.x- threshold) / 200.0;
    // ノイズの激しさ
    float noiseScale = 1.0f;
    // 軸情報 + ノイズ * ???(なくてもOK) * ノイズの激しさ
    float alb = alp + b * (1.0 - alp) * noiseScale;
    
    // それ以降のメッシュを削除
    if (alb > 0.0)
    {
        discard;
    }
    
    //　光の位置を消えてる先端へ
    alb = saturate(1.0 - abs(alb));
    
    // ディゾルブで表現される光の大きさ（大きくするたび小さくなる）
    float size = 15.0f;
    // ディゾルブで表現される光の強さ
    float lightPower = 5.0f;
    // 光データ
    float bloom = pow(alb, size) * lightPower;

    
    // 高輝度部分の色データの取得
    PSOutput.hdr = float4(bloom, bloom, bloom,1);

	// テクスチャカラーの読み込み
    TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);

	// 出力カラー = テクスチャカラー
	PSOutput.Color0 = TextureDiffuseColor ;

	// 出力アルファ = テクスチャアルファ * 不透明度
    PSOutput.Color0.a = TextureDiffuseColor.a * g_Base.FactorColor.a;
    
    // ディゾルブで表現したい色
    float3 disolveColor = float3(bloom, bloom * 0.5, bloom * 0.5);
    
    // α値
    float alphaNum = 1.0f;
    
    // デフォルトの色
    float3 defaultColor = PSInput.Diffuse.rgb;
    
    // 色データ（α値なし）
    float3 colorRGB = defaultColor + disolveColor;
    
    // 色データ
    float4 color = float4(colorRGB, alphaNum);
    
    // 色データの取得
    PSOutput.Color0 += color;
    
    
    float2 uv = PSInput.P.xy * float2(0.5f, 0.5f) + 0.5f;
    
    float uOffset = SimplexNoise(float3(uv, 0.0f) * 256.0f) * 0.02f;
    
    float4 stealth = g_sceneTexture.Sample(g_DiffuseMapSampler, uv + uOffset);
    
    PSOutput.Color0 += stealth;
   
	// 出力パラメータを返す
    return PSOutput;
}
