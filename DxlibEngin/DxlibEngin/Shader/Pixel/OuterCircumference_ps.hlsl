
// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 dif : COLOR; // ディフューズ
    float4 spc : COLOR1; // スペキュラ
    float2 uv : TEXCOORDO; // テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 color : SV_TARGET0; // 色
};

SamplerState mainSampler; // サンプラ(後述)
Texture2D mainTexture : register(t0); // テクスチャ

cbuffer args : register(b8)
{
    float time;
};

float3 palette(float t)
{
    float3 a = float3(0.5,0.5,0.5);
    float3 b = float3(0.5,0.5,0.5);
    float3 c = float3(1,1,1);
    float3 d = float3(0.263,0.416,0.557);
    
    return a + b * cos(6.28318 * (c*t+d));

}

float4 main(PS_INPUT i) : SV_TARGET
{
    float4 Tex = mainTexture.Sample(mainSampler,i.uv );
    float2 uv = (i.uv - 0.5) * 2;
    float2 uv0 = uv;
    //float3 finalCol = float3(0, 0, 0);
    
    for (int i = 0; i < 5;i++)
    {
        uv *= 1.5; // (uv*1.5)スタート
        uv = uv - floor(uv); //floor:小数点以下切り捨て
        uv -= 0.5; // 真ん中に戻す
    
        float d = length(uv) * exp(-length(uv0));
        float3 col = palette(length(uv0) + time * 0.5) /*float3(1,1,0)*/;
        d -= sin(d * 8 + time*0.1);
        d = abs(d);
        d = pow(0.05/ d,1.3);
        //d = 0.1 / d;
        //finalCol += col * d;
        Tex.rgb += col * d;

    }
    
   
    
    //if (finalCol.r == 0.0f)
    //{
    //    discard;
    //}
    
    if (Tex.r == 0.0f)
    {
        discard;
    }
    
    //float4 final = float4(finalCol, 1);
    float4 final = float4(Tex.rbg, 1);
    
    //
    
    
    
    
    return final;
}

//struct PixelInput
//{
//    float4 pos : SV_Position;
//    float4 dif : COLOR; // ディフューズ
//    float4 spc : COLOR1; // スペキュラ
//    float2 uv : TEXCOORD0;
//};


//SamplerState _MainTex_sampler;
//Texture2D _MainTex : register(t0); // テクスチャ

//cbuffer args : register(b5)
//{
//    float4 color; // 出力する色
//    float2 screenSize; // 画面のサイズ
//    float2 aspect; // 画面のアスペクト比
//    float time; // 時間
//    float div; // 四角形の分割数
//    float direction; // 回転の方向
//    float rotation; // 回転角度
//    float size; // 四角形のサイズ
//};

//// 四角形
//float rectangle(float2 p, float2 size)
//{
//    return max(abs(p.x) - size.x, abs(p.y) - size.y);
//}

//// 座標を指定された角度だけ回転させる
//float2 Rotation(float2 p, float theta)
//{
//    return float2((p.x) * cos(theta) - p.y * sin(theta), p.x * sin(theta) + p.y * cos(theta));
//}

//// 四角形の内側から外側に向かって値を増加させる領域を作成
//float trs(float2 p, float val, float div, float t)
//{
//    float mn = 0.001;
//    float u = 1.0;
//    for (float i = 0; i < t; i++)
//    {
//        u += (div * 2.0 - 4.0 * i - 2.0) * 4.0;
//    }

//    float r = (div * 2.0 - 4.0 * t - 2.0);
//    float sc = val - u;

//    float a = 1;

//    // 領域ごとに四角形を作成し、内部にいるかどうかを判定
//    float rect = rectangle(p - float2(-div + t * 2.0, -div + t * 2.0 + 1.0), float2(sc, mn));
//    a = 1 - step(1.0, rect);

//    rect = rectangle(p - float2(div - t * 2.0 - 1.0, -div + (t + 1.0) * 2.0), float2(mn, sc - r - 2.0));
//    a = max(a, 1 - step(1.0, rect));

//    rect = rectangle(p - float2(div - (t + 1) * 2.0, div - t * 2.0 - 1.0), float2(sc - r * 2.0 - 2.0, mn));
//    a = max(a, 1 - step(1.0, rect));

//    rect = rectangle(p - float2(-div + t * 2.0 + 1.0, div - (t + 1) * 2.0), float2(mn, sc - r * 3.0 - 2.0));
//    a = max(a, 1 - step(1.0, rect));

//    return a;
//}

//float4 main(PixelInput input) : SV_Target
//{
//    float4 _color = color; // 出力する色
//    float _div = div; // 四角形の分割数
//    float _dir = direction; // 回転の方向
//    float _rot = rotation; // 回転角度
//    float2 _asp = aspect; // 画面のアスペクト比
//    float _ratio = screenSize.x / screenSize.y; // 画面の縦横比
//    float _size = size; // 四角形のサイズ

//    // テクスチャ座標を-1から1の範囲に変換
//    float2 f_st = input.uv * 2.0 - 1.0;

//    //// 回転の方向に応じて座標を変換
//    f_st.x = f_st.x * (1.0 - 2.0 * _dir);
//    f_st = Rotation(f_st, radians(_rot));

//    //// 画面のアスペクト比と四角形のサイズを考慮して座標を変換
//    f_st *= _size;
//    f_st *= _div;

//    // 領域ごとに値を増加させる
//    float a = 0.0;
//    for (float i = 0; i < _div * 0.5; i++)
//    {
//        a = min(a + trs(f_st, time, _div, i), 0.5);
//    }

//    // 値が0の場合は透明にする
//    if (a == 0.0)
//    {
//        discard;
//    }

//    return _color;
//}
