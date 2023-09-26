#include "PixelShader3DHeader.hlsli"

//エントリーポイント
PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{    
    float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    const float3 ray = normalize(input.viewPos); //カメラ→自身のベクトル
    
     ////テクスチャがある場合はそれを考慮した処理を行う
    uint2 diffuseMapSize;
    diffuseMapTexture.GetDimensions(diffuseMapSize.x, diffuseMapSize.y);
    if (diffuseMapSize.x * diffuseMapSize.y > 0)
    {
        float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, input.uv);
        if (texColor.a == 0.0f)
        {
            discard;
        }
        
        result *= texColor;
    }

    //法線マップから色を取得し、0～1を-1～1に変換
    const float3 tanNormal = normalize(Sub0Texture.Sample(Sub0Sampler, input.uv).xyz * 2 - 1);
   
    const float3 normal = normalize(input.viewNorm+tanNormal); //法線(ビュー)
   
    float3 cubeRef = reflect(-ray, normal);
    
    float4 cubeColor = 0.5f * cubeMapTexture.Sample(cubeMapSampler,cubeRef);
    
    //ライティング処理での合計色
    float3 totalDiffuse = (0, 0, 0);
    float3 totalSpecular = (0, 0, 0);
    
   
    //ライトごとにライティング処理
    for (int i = 0; i < DX_D3D11_COMMON_CONST_LIGHT_NUM; i++)
    {
        ProcessLight(input.viewPos, ray, normal, common.light[i], common.material.power, totalDiffuse, totalSpecular);
    }

    //環境光を適用
    totalDiffuse += common.material.ambientEmissive.rgb;

    //明度にマテリアルの色を適用
    totalDiffuse *= common.material.diffuse.rgb;
    totalSpecular *= common.material.specular.rgb;

    //元の色にマテリアルを適用
    result.rgb *= totalDiffuse;
    result.rgb += totalSpecular;
    result.a *= common.material.diffuse.a * base.factorColor.a;

    //加算色を適用
    result.rgb += base.drawAddColor.rgb;
    
    //0～1に揃えて返す
    return saturate(result+cubeColor);
}
