struct PixelInput
{
    float4 dif : COLOR; // ディフューズ
    float4 spc : COLOR1; // スペキュラ
    float2 uv : TEXCOORD0;
};

SamplerState _MainTex_sampler; 
Texture2D _MainTex : register(t0); // テクスチャ

cbuffer args : register(b5)
{
    float2 size;
    float4 color;
    float time;
    float rotation;
    float _aspect;
    float2 _screenSize;
    float _drection;
};

//rectangle
float rectangle(float2 p, float2 size)
{
    return max(abs(p.x) - size.x, abs(p.y) - size.y);
}

float2 rotation(float2 p, float theta)
{
    return float2((p.x) * cos(theta) - p.y * sin(theta), p.x * sin(theta) + p.y * cos(theta));
}

float trs(float2 p, float val, float div, float t)
{
    float mn = 0.001;
    float u = 1.0;
    for (int i = 0; i < t; i++)
    {
        u += (div * 2.0 - 4.0 * i - 2.0) * 4.0;
    }
 
    float r = (div * 2.0 - 4.0 * t - 2.0);
    float sc = val - u;
 
    float a = 1;
 
    float rect = rectangle(p - float2(-div + t * 2.0, -div + t * 2.0 + 1.0), float2(sc, mn));
    a = 1 - step(1.0, rect);
 
    rect = rectangle(p - float2(div - t * 2.0 - 1.0, -div + (t + 1.0) * 2.0), float2(mn, sc - r - 2.0));
    a = max(a, 1 - step(1.0, rect));
 
    rect = rectangle(p - float2(div - (t + 1) * 2.0, div - t * 2.0 - 1.0), float2(sc - r * 2.0 - 2.0, mn));
    a = max(a, 1 - step(1.0, rect));
 
    rect = rectangle(p - float2(-div + t * 2.0 + 1.0, div - (t + 1) * 2.0), float2(mn, sc - r * 3.0 - 2.0));
    a = max(a, 1 - step(1.0, rect));
 
    return a;
}

float4 main(PixelInput input) : SV_Target
{
    
    float div = 10;
    float2 _size = size;
    int asp = _aspect;
    float ratio = _screenSize.x / _screenSize.y;
    float dir = _drection;
    float _rotation = rotation;
    
    float val = time * div * div * 2.0;
    // -1 ～ 1
    float2 f_st = input.uv * 2.0 - 1.0;
    
    
    //アスペクト比で調整
    f_st.x *= (ratio * asp + (1.0 - asp));
    
    //座標の回転
    //f_st = rotation(f_st, radians(rotation));
    
    //回転方向
    f_st.x = f_st.x * (1.0 - 2.0 * dir);
    
    // -分割数 ～ 分割数
    f_st *= div;
    f_st *= _size;
    
    float a = 0.0f;
    
    for (int i = 0; i < div * 0.5; i++)
    {
        a = min(a + trs(f_st, val, div, i), 1);
    }
    
    
    if (a == 0.0)
    {
        discard;
    }
    
    return color;

}