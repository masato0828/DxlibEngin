struct VsBase
{
    matrix antiViewportMatrix; // アンチビューポート行列
    matrix projectionMatrix; // ビュー　→　プロジェクション行列
    float4x3 viewMatrix; // ワールド　→　ビュー行列
    float4x3 localWorldMatrix; // ローカル　→　ワールド行列

    float4 toonOutLineSize; // トゥーンの輪郭線の大きさ
    float diffuseSource; // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
    float specularSource; // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
    float mulSpecularColor; // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
    float padding;
};

cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    VsBase base;
}

//1フレームの影響を受ける頂点
struct VertexInput
{
    float3 pos : POSITION; // 座標(ローカル空間)
    float3 norm : NORMAL; // 法線(ローカル空間)
    float4 diffuse : COLOR0; // ディフューズカラー
    float4 specular : COLOR1; // スペキュラカラー
    float4 uv0 : TEXCOORD0; // テクスチャ座標
    float4 uv1 : TEXCOORD1; // サブテクスチャ座標
};

//法線マップなしの場合
struct VertexToPixel
{
    float4 svPos : SV_POSITION; //位置(プロジェクション)
    float3 worldPos : POSITION0; //位置(ワールド)
    float3 viewPos : POSITION1; //位置(ビュー)
    float3 viewNorm : NORMAL0; //法線
    float4 diffuse : COLOR0; //拡散反射色
    float4 specular : COLOR1; //鏡面反射色
    float2 uv : TEXCOORD; //UV値
};

#define VS_INPUT VertexInput
#define VS_OUTPUT VertexToPixel

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT ret;
    ret.svPos = float4(input.pos,1.0f);
    
    // ローカル座標系　→　ワールド座標
    ret.worldPos = mul(ret.svPos,base.localWorldMatrix);
    ret.svPos.xyz = ret.worldPos;
    
    // ワールド座標系　→　ビュー座標系
    ret.viewPos = mul(ret.svPos,base.viewMatrix);
    ret.svPos.xyz = ret.viewPos;
    
    // ビュー座標系　　→　プロジェクション座標系
    ret.svPos = mul(ret.svPos,base.projectionMatrix);
    
    // 法線(ビュー)を計算
    ret.viewNorm = normalize(mul(float4(input.norm,0.0f),base.localWorldMatrix));
    ret.viewNorm = normalize(mul(float4(ret.viewNorm,0.0f),base.viewMatrix));
    
    ret.uv = input.uv0.xy;          // UVを代入
    ret.diffuse = input.diffuse;    // 拡散反射色を代入
    ret.specular = input.specular;  // 鏡面反射色を代入
    //
    return ret;
}