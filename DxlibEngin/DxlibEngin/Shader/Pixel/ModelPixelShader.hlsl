#include "PixelShader3DHeader.hlsli"

struct PsColorPram
{
    float3 diffuse;
    float3 specular;
};

cbuffer cbD3D11_CONST_BUFFER_PS_COLOR_REAM : register(b3)
{
    PsColorPram colorPram;
};

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);

    const float3 ray = normalize(input.viewPos);            //カメラ→自身のベクトル
    const float3 normal = normalize(input.viewNorm);        //法線(ビュー)

    //ライティング処理での合計色
    float3 totalDiffuse = colorPram.diffuse;
    float3 totalSpecular = colorPram.specular;

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

    //0〜1に揃えて返す
    return saturate(result);
}
