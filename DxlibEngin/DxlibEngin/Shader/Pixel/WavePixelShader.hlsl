#include "PixelShader3DHeader.hlsli"

#define DRAG_MULT 0.28
#define WATER_DEPTH 1.0
#define CAMERA_HEIGHT 1.5
#define ITERATIONS_RAYMARCH 12
#define ITERATIONS_NORMAL 40

float2 Wavesdx(float2 position,float2 direction,float frequency,float timeshift)
{
    float x = dot(direction,position)*frequency+ timeshift;
    float wave = exp(sin(x)*1.0);
    float dx = wave * cos(x);
    return float2(wave,-dx);
}

float GetWaves(float2 position,int iterations)
{
    float iter = 0.0f;
    float frequency = 1.0f;
    float timeMultiplier = 2.0f;
    float weight = 1.0f;
    float sumOfValues = 0.0f;
    float sumOfWeoghts = 0.0f;
    
    for (int i = 0; i < iterations;i++)
    {
        float2 p = float2(sin(iter), cos(iter));
        float2 res = Wavesdx(position,p,frequency,/*time*/1.0f*timeMultiplier);
        
        position += p * res.y * weight * DRAG_MULT;
        
        sumOfValues += res.x * weight;
        sumOfWeoghts += weight;
        
        weight *= 0.82f;
        frequency *= 1.18f;
        timeMultiplier *= 1.07f;
        
        iter += 1232.399963f;
    }
    
    return sumOfValues / sumOfWeoghts;

}

float RayMarchWater(float3 camera,float3 start,float3 end, float depth)
{
    float3 pos = start;
    float3 dir = normalize(end - start);
    for (int i = 0; i < 64; i++)
    {
    // the height is from 0 to -depth
        float height = GetWaves(pos.xz, ITERATIONS_RAYMARCH) * depth - depth;
    // if the waves height almost nearly matches the ray height, assume its a hit and return the hit distance
        if (height + 0.01 > pos.y)
        {
            return distance(pos, camera);
        }
    // iterate forwards according to the height mismatch
        pos += dir * (pos.y - height);
    }
    return distance(start,camera);
}

float IntersectPlane(float3 origin , float3 direction,float3 points,float3 normal)
{
    return clamp(dot(points - origin, normal) / dot(direction,normal),-1.0f,999199.0f);
}

float3 Normal(float2 pos,float e,float depth)
{
    float2 ex = float2(e,0);
    float H = GetWaves(pos.xy,ITERATIONS_NORMAL)* depth;
    float3 a = float3(pos.x,H,pos.y);
    return normalize(cross(
    a - float3(pos.x - e, GetWaves(pos.xy-ex.xy, ITERATIONS_NORMAL)*depth, pos.y),
    a - float3(pos.x, GetWaves(pos.xy+ex.xy, ITERATIONS_NORMAL)*depth, pos.y + e)));
}

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
    //return saturate(result+cubeColor);
    
    float3 waterPlaneHight = float3(0.0f,0.0f,0.0f);
    float3 waterPlaneLow = float3(0.0f,-WATER_DEPTH,0.0f);
    
    float3 origin = float3( /*time*/1.0f,CAMERA_HEIGHT, /*time*/1.0f);
    
    float highPlaneHit = IntersectPlane(origin, ray, waterPlaneHight, float3(0.0f,1.0f,0.0f));
    float lowPlaneHit = IntersectPlane(origin, ray, waterPlaneLow, float3(0.0f,1.0f,0.0f));
    float3 highHitPos = origin * highPlaneHit;
    float3 lowHitPos = origin * lowPlaneHit;
    
    float dist = RayMarchWater(origin, highHitPos, lowHitPos, WATER_DEPTH);
    float3 waterHitPos = origin  * dist;
    
    float3 N = Normal(waterHitPos.xy, 0.01, WATER_DEPTH);
    N = lerp(N, float3(0.0f, 1.0f, 0.0f), 0.8f * min(1.0f, sqrt(dist * 0.01f) * 1.1f));
    return float4(N, 1.0f);
}
