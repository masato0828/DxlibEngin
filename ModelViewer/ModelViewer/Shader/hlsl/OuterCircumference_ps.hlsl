struct PixelInput
{
    float4 dif : COLOR; // ??????
    float4 spc : COLOR1; // ?????
    float2 uv : TEXCOORD0;
};

SamplerState _MainTex_sampler;
Texture2D _MainTex : register(t0); // ?????

cbuffer args : register(b7)
{
    float4 color;
    float2 screenSize;
    float2 aspect;
    float time;
    float div;
    float direction;
    float rotation;
    float size;
};

//rectangle
float rectangle(float2 p, float2 size)
{
    return max(abs(p.x) - size.x, abs(p.y) - size.y);
}

float2 Rotation(float2 p, float theta)
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
    // ???
    float4 _color = color;
    // ???
    float _div = div;
    // ????
    float _dir = direction;
    // ??
    float _rot = rotation;
    // ??????
    float2 _asp = aspect;
    // ????
    float _ratio = screenSize.x / screenSize.y;
    // ???
    float _size = size;
    
    // -1 ? 1
    float2 f_st = input.uv * 2.0 - 1.0;
    
    // ????
    f_st.x = f_st.x * (1.0 - 2.0 * _dir);
    // ??
    f_st = Rotation(f_st, radians(_rot));
    
   // f_st.x *= (_ratio * _asp.x + (1.0 - _asp.x));
    //f_st.y *= (_ratio * _asp.y + (1.0 - _asp.y));
    
    f_st *= _size;
    // -1 ? 1?->?-div ? div
    f_st *= _div;
    
    // ???
    float a = 0.0f;
    // ?????
    for (int i = 0; i < _div * 0.5; i++)
    {
        a = min(a + trs(f_st, time, _div, i), 1);
    }
    // ??????????????
    if (a == 0.0)
    {
        discard;
    }
   
    return _color;
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);

}