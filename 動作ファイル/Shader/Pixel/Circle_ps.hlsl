struct PixelInput
{
    float4 dif : COLOR; // �f�B�t���[�Y
    float4 spc : COLOR1; // �X�y�L����
    float2 uv : TEXCOORD0;
};

SamplerState _MainTex_sampler;
Texture2D _MainTex : register(t0); // �e�N�X�`��

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
    // 2�����x�N�g��p�̒�����2��i p��p�̓��ρj���v�Z���ĕԂ��܂�
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
    //0����1�͈̔͂ɐ��K��(frac(input.uv))��  -1�`1�@�̒l�ɂ���;
    float2 f_st = frac(st) * 2.0 - 1.0;
    // �ϊ����ꂽ���W�i f_st�j����~�̌`���쐬
    // �~�̌`�̌v�Z�ł�, ���W�ƌ��_�Ƃ̋�����2��i ci�j���g�p�����
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
    
    // step : ��1��������2�������傫���ꍇ��1.0��Ԃ��A �����łȂ��ꍇ��0 .0��Ԃ�
    //col.a = step(v, ci);
    
    // col.a��0.0�Ɠ������ꍇ�A �܂�ci��0.1�ȉ��̏ꍇ�́A discard�������s����܂�
    // ����ɂ��A ���̃s�N�Z�����j������A�`�挋�ʂɉe����^���Ȃ��Ȃ�܂�
    // ���]
    a = revers - a * (revers * 2.0 - 1.0);
    if (a == 0.0)
    {
        discard;
    }
    
    float4 _color = color;
	return _color;
}