struct VertexInput
{
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 dif : COLOR; // ディフューズ
    float4 spc : COLOR1; // スペキュラ
    float2 uv : TEXCOORD0;
};

PixelInput main(VertexInput v)
{
    PixelInput o;
    o.uv = v.uv;
    return o;
}