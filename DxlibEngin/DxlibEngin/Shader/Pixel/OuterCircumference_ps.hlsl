
// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 dif : COLOR; // �f�B�t���[�Y
    float4 spc : COLOR1; // �X�y�L����
    float2 uv : TEXCOORDO; // �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 color : SV_TARGET0; // �F
};

SamplerState mainSampler; // �T���v��(��q)
Texture2D mainTexture : register(t0); // �e�N�X�`��

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
        uv *= 1.5; // (uv*1.5)�X�^�[�g
        uv = uv - floor(uv); //floor:�����_�ȉ��؂�̂�
        uv -= 0.5; // �^�񒆂ɖ߂�
    
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
//    float4 dif : COLOR; // �f�B�t���[�Y
//    float4 spc : COLOR1; // �X�y�L����
//    float2 uv : TEXCOORD0;
//};


//SamplerState _MainTex_sampler;
//Texture2D _MainTex : register(t0); // �e�N�X�`��

//cbuffer args : register(b5)
//{
//    float4 color; // �o�͂���F
//    float2 screenSize; // ��ʂ̃T�C�Y
//    float2 aspect; // ��ʂ̃A�X�y�N�g��
//    float time; // ����
//    float div; // �l�p�`�̕�����
//    float direction; // ��]�̕���
//    float rotation; // ��]�p�x
//    float size; // �l�p�`�̃T�C�Y
//};

//// �l�p�`
//float rectangle(float2 p, float2 size)
//{
//    return max(abs(p.x) - size.x, abs(p.y) - size.y);
//}

//// ���W���w�肳�ꂽ�p�x������]������
//float2 Rotation(float2 p, float theta)
//{
//    return float2((p.x) * cos(theta) - p.y * sin(theta), p.x * sin(theta) + p.y * cos(theta));
//}

//// �l�p�`�̓�������O���Ɍ������Ēl�𑝉�������̈���쐬
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

//    // �̈悲�ƂɎl�p�`���쐬���A�����ɂ��邩�ǂ����𔻒�
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
//    float4 _color = color; // �o�͂���F
//    float _div = div; // �l�p�`�̕�����
//    float _dir = direction; // ��]�̕���
//    float _rot = rotation; // ��]�p�x
//    float2 _asp = aspect; // ��ʂ̃A�X�y�N�g��
//    float _ratio = screenSize.x / screenSize.y; // ��ʂ̏c����
//    float _size = size; // �l�p�`�̃T�C�Y

//    // �e�N�X�`�����W��-1����1�͈̔͂ɕϊ�
//    float2 f_st = input.uv * 2.0 - 1.0;

//    //// ��]�̕����ɉ����č��W��ϊ�
//    f_st.x = f_st.x * (1.0 - 2.0 * _dir);
//    f_st = Rotation(f_st, radians(_rot));

//    //// ��ʂ̃A�X�y�N�g��Ǝl�p�`�̃T�C�Y���l�����č��W��ϊ�
//    f_st *= _size;
//    f_st *= _div;

//    // �̈悲�Ƃɒl�𑝉�������
//    float a = 0.0;
//    for (float i = 0; i < _div * 0.5; i++)
//    {
//        a = min(a + trs(f_st, time, _div, i), 0.5);
//    }

//    // �l��0�̏ꍇ�͓����ɂ���
//    if (a == 0.0)
//    {
//        discard;
//    }

//    return _color;
//}
