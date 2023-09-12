#define VERTEX_INPUT_TYPE (VERTEX_INPUT_TYPE_8FRAME)
#include "VertexShader3DHeader.hlsli"

//エントリーポイント
VS_OUTPUT main(VS_INPUT input)
{
    //ローカルワールド行列を計算
    const float4x3 lwMatrix = CalculateLocalWorldMatrix(
    input.blendIndices0,
    input.blendWeight0,
    input.blendIndices1,
    input.blendWeight1,
    localWorldMatrix);

	VS_OUTPUT ret;
    ret.svPos = float4(input.pos, 1.0f);

    //ローカル座標系→ワールド座標系
    ret.worldPos = mul(ret.svPos, lwMatrix);
    ret.svPos.xyz = ret.worldPos;

    //ワールド座標系→ビュー座標系
    ret.viewPos = mul(ret.svPos, base.viewMatrix);
    ret.svPos.xyz = ret.viewPos;

    //ビュー座標系→プロジェクション座標系
    ret.svPos = mul(ret.svPos, base.projectionMatrix);

    //法線(ビュー)を計算
    ret.viewNorm = normalize(mul(float4(input.norm, 0.0f), lwMatrix));
    ret.viewNorm = normalize(mul(float4(ret.viewNorm, 0.0f), base.viewMatrix));

    //UV値を計算
    ret.uv = mul(input.uv0, otherMatrix.textureMatrix[0]);

    ret.diffuse = input.diffuse;        //拡散反射色を代入
    ret.specular = input.specular;      //鏡面反射色を代入

    return ret;
}
