struct VertexInput
{
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 dif : COLOR; // �f�B�t���[�Y
    float4 spc : COLOR1; // �X�y�L����
    float2 uv : TEXCOORD0;
};

PixelInput main(VertexInput v)
{
    PixelInput o;
    o.uv = v.uv;
    return o;
}