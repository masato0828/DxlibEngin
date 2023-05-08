//// 頂点シェーダーの入力
//struct VS_INPUT
//{
//    float3 Position     : POSITION;   // 座標( ローカル空間 )
//    float3 Normal       : NORMAL;     // 法線( ローカル空間 )
//    float4 Diffuse      : COLOR0;     // ディフューズカラー
//    float4 Specular     : COLOR1;     // スペキュラカラー
//    float4 TexCoords0   : TEXCOORD0;  // テクスチャ座標
//    float4 TexCoords1   : TEXCOORD1;  // サブテクスチャ座標
//};

//// 頂点シェーダーの出力
//struct VS_OUTPUT
//{
//    float4 Diffuse      : COLOR0;       // ディフューズカラー
//    float4 Specular     : COLOR1;       // スペキュラカラー
//    float2 TexCoords0   : TEXCOORD0;    // テクスチャ座標
//    float4 Position     : SV_POSITION;  // 座標( プロジェクション空間 )
//};


//// 基本パラメータ
//struct DX_D3D11_VS_CONST_BUFFER_BASE
//{
//    float4x4 AntiViewportMatrix;    // アンチビューポート行列
//    float4x4 ProjectionMatrix;      // ビュー　→　プロジェクション行列
//    float4x3 ViewMatrix;            // ワールド　→　ビュー行列
//    float4x3 LocalWorldMatrix;      // ローカル　→　ワールド行列

//    float4 ToonOutLineSize;     // トゥーンの輪郭線の大きさ
//    float DiffuseSource;        // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
//    float SpecularSource;       // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
//    float MulSpecularColor;     // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
//    float Padding;
//};

//// その他の行列
//struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
//{
//    float4 ShadowMapLightViewProjectionMatrix[3][4];    // シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
//    float4 TextureMatrix[3][2];     // テクスチャ座標操作用行列
//};

//// 基本パラメータ
//cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
//{
//    DX_D3D11_VS_CONST_BUFFER_BASE g_Base;
//};

//// その他の行列
//cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX : register(b2)
//{
//    DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX g_OtherMatrix;
//};

//// main関数
//VS_OUTPUT main(VS_INPUT VSInput)
//{
//    float3 light = normalize(float3(1, 1, 1));
//    VS_OUTPUT VSOutput;
//    float4 lLocalPosition;
//    float4 lWorldPosition;
//    float4 lViewPosition;

//    ///////////////////////////////////////////////////////////////////////////////
//	// 頂点座標変換 
//    ///////////////////////////////////////////////////////////////////////////////
	
//    // ローカル座標のセット
//    //lLocalPosition.xyz = VSInput.Position + VSInput.Normal * 8;
//    lLocalPosition.xyz = VSInput.Position + VSInput.Normal * g_Base.ToonOutLineSize.x;
//    lLocalPosition.w = 1.0f;

//	// 座標計算( ローカル→ビュー→プロジェクション )
//    lWorldPosition = float4(mul(lLocalPosition, g_Base.LocalWorldMatrix), 1.0f);

//    lViewPosition = float4(mul(lWorldPosition, g_Base.ViewMatrix), 1.0f);

//    VSOutput.Position = mul(lViewPosition, g_Base.ProjectionMatrix);



//	///////////////////////////////////////////////////////////////////////////////
//	// 出力パラメータセット 
//    ///////////////////////////////////////////////////////////////////////////////
	
//	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
//    VSOutput.TexCoords0 = VSInput.TexCoords0;
    
//    VSOutput.Diffuse = float4(1, 0.5, 0, 1);
//    //VSInput.Diffuse*(max(saturate(dot(VSInput.Normal,light)),0.3));
    
//    VSOutput.Specular = VSInput.Specular;

//	// 出力パラメータを返す
//    return VSOutput;
//}

// 頂点シェーダーの入力
struct VS_INPUT
{
    float4 Position : POSITION; // 座標( ローカル空間 )
    float3 Normal : NORMAL0; // 法線( ローカル空間 )
    float4 Diffuse : COLOR0; // ディフューズカラー
    float4 Specular : COLOR1; // スペキュラカラー
    float4 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float4 TexCoords1 : TEXCOORD1; // サブテクスチャ座標
    int4 BlendIndices0 : BLENDINDICES0; // スキニング処理用 Float型定数配列インデックス
    float4 BlendWeight0 : BLENDWEIGHT0; // スキニング処理用ウエイト値
};

// 頂点シェーダーの出力
struct VS_OUTPUT
{
    float2 TexCoords0 : TEXCOORD0;
    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
};


// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
    float4 AntiViewportMatrix[4]; // アンチビューポート行列
    float4 ProjectionMatrix[4]; // ビュー　→　プロジェクション行列
    float4 ViewMatrix[3]; // ワールド　→　ビュー行列
    float4 LocalWorldMatrix[3]; // ローカル　→　ワールド行列

    float4 ToonOutLineSize; // トゥーンの輪郭線の大きさ
    float DiffuseSource; // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
    float SpecularSource; // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
    float MulSpecularColor; // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
    float Padding;
};

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
    float4 ShadowMapLightViewProjectionMatrix[3][4]; // シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
    float4 TextureMatrix[3][2]; // テクスチャ座標操作用行列
};

// スキニングメッシュ用の　ローカル　→　ワールド行列
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	float4 imatrix[ 54 * 3 ];					// ローカル　→　ワールド行列
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    DX_D3D11_VS_CONST_BUFFER_BASE g_Base;
};

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX : register(b2)
{
    DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX g_OtherMatrix;
};

// スキニングメッシュ用の　ローカル　→　ワールド行列
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX : register(b3)
{
    DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX g_LocalWorldMatrix;
};


// main関数
VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT VSOutput;
    float4 lLocalWorldMatrix[3];
    float4 lWorldPosition;
    float4 lViewPosition;


	// 複数のフレームのブレンド行列の作成
    lLocalWorldMatrix[0] = g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.x + 0] * VSInput.BlendWeight0.x;
    lLocalWorldMatrix[1] = g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.x + 1] * VSInput.BlendWeight0.x;
    lLocalWorldMatrix[2] = g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.x + 2] * VSInput.BlendWeight0.x;

    lLocalWorldMatrix[0] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.y + 0] * VSInput.BlendWeight0.y;
    lLocalWorldMatrix[1] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.y + 1] * VSInput.BlendWeight0.y;
    lLocalWorldMatrix[2] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.y + 2] * VSInput.BlendWeight0.y;

    lLocalWorldMatrix[0] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.z + 0] * VSInput.BlendWeight0.z;
    lLocalWorldMatrix[1] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.z + 1] * VSInput.BlendWeight0.z;
    lLocalWorldMatrix[2] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.z + 2] * VSInput.BlendWeight0.z;

    lLocalWorldMatrix[0] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.w + 0] * VSInput.BlendWeight0.w;
    lLocalWorldMatrix[1] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.w + 1] * VSInput.BlendWeight0.w;
    lLocalWorldMatrix[2] += g_LocalWorldMatrix.imatrix[VSInput.BlendIndices0.w + 2] * VSInput.BlendWeight0.w;



	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

    float4 lLocalPosition;
    
    lLocalPosition.xyz = VSInput.Position.xyz + VSInput.Normal * 8;
    //lLocalPosition.xyz = VSInput.Position.xyz + VSInput.Normal * g_Base.ToonOutLineSize.x;
    lLocalPosition.w = 1.0f;
    
    ////座標計算(ローカル→ビュー→ プロジェクション)
    lWorldPosition.x = dot(lLocalPosition, lLocalWorldMatrix[0]);
    lWorldPosition.y = dot(lLocalPosition, lLocalWorldMatrix[1]);
    lWorldPosition.z = dot(lLocalPosition, lLocalWorldMatrix[2]);
    lWorldPosition.w = 1.0f;
    
    lViewPosition.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
    lViewPosition.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
    lViewPosition.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
    lViewPosition.w = 1.0f;
    
    VSOutput.Position.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
    VSOutput.Position.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
    VSOutput.Position.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
    VSOutput.Position.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);
    
    
    
	 ////ローカル座標をワールド座標に変換
  //  lWorldPosition.x = dot(VSInput.Position, lLocalWorldMatrix[0]);
  //  lWorldPosition.y = dot(VSInput.Position, lLocalWorldMatrix[1]);
  //  lWorldPosition.z = dot(VSInput.Position, lLocalWorldMatrix[2]);
  //  lWorldPosition.w = 1.0f;

  //  //ワールド座標をビュー座標に変換

  //  lViewPosition.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
  //  lViewPosition.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
  //  lViewPosition.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
  //  lViewPosition.w = 1.0f;

  //  //ビュー座標を射影座標に変換

  //  VSOutput.Position.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
  //  VSOutput.Position.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
  //  VSOutput.Position.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
  //  VSOutput.Position.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
    VSOutput.TexCoords0.x = dot(VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[0][0]);
    VSOutput.TexCoords0.y = dot(VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[0][1]);

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
    return VSOutput;
}
