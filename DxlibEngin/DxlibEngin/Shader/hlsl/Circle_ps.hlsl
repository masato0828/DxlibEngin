struct PixelInput
{
    float4 dif : COLOR; // ディフューズ
    float4 spc : COLOR1; // スペキュラ
    float2 uv : TEXCOORD0;
};

SamplerState _MainTex_sampler;
Texture2D _MainTex : register(t0); // テクスチャ

cbuffer args : register(b7)
{
    float4 color;
    float2 screenSize;
    float2 dir;
    int div;
    float val;
    float width;
    float revers;
};

float circle(float2 p)
{
    // 2次元ベクトルpの長さの2乗（ pとpの内積）を計算して返します
    return dot(p, p);
}

float4 main(PixelInput input) : SV_TARGET
{
    float2 _dir = normalize(dir);
    //float2 sg = sign(_dir);
    
    
    float2 _screenSize = screenSize;
    float _val = val * (dot(div - 1.0, abs(dir)) * width + 2.0);;
    float2 _div = float2(div, div * _screenSize.y / _screenSize.x);
    float2 st = input.uv * _div;
    //0から1の範囲に正規化(frac(input.uv))後  -1～1　の値にする;
    float2 f_st = frac(st) * 2.0 - 1.0;
    // 変換された座標（ f_st）から円の形を作成
    // 円の形の計算では, 座標と原点との距離の2乗（ ci）が使用される
    //float ci = circle(f_st);
    
    float2 i_st = floor(st);
    //float2 f = (div - 1) * (0.5 - sg * 0.5) + i_st * sg;
   // float v = val - dot(f, abs(_dir)) * width;
    
    
    float4 col = 0.0;
    
    float a = 1;
    
    float2 sg = sign(_dir);
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            float2 f = (div - 1) * (0.5 - sg * 0.5) + (i_st + float2(i, j)) * sg;
            float v = val - dot(f, abs(_dir)) * width;
 
            float ci = circle(f_st - float2(2.0 * i, 2.0 * j));
            a = min(a, step(v, ci));
        }
    }
    
    // step : 第1引数が第2引数より大きい場合に1.0を返し、 そうでない場合に0 .0を返す
    //col.a = step(v, ci);
    
    // col.aが0.0と等しい場合、 つまりciが0.1以下の場合は、 discard文が実行されます
    // これにより、 そのピクセルが破棄され、描画結果に影響を与えなくなります
    // 反転
    a = revers - a * (revers * 2.0 - 1.0);
    if (a == 0.0)
    {
        discard;
    }
    
    float4 _color = color;
	return _color;
}