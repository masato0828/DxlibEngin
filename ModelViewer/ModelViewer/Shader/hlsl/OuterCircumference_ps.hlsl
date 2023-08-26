struct PixelInput
{
    float4 pos : SV_Position;
    float4 dif : COLOR; // �f�B�t���[�Y
    float4 spc : COLOR1; // �X�y�L����
    float2 uv : TEXCOORD0;
};

SamplerState _MainTex_sampler;
Texture2D _MainTex : register(t0); // �e�N�X�`��

cbuffer args : register(b7)
{
    float4 color; // �o�͂���F
    float2 screenSize; // ��ʂ̃T�C�Y
    float2 aspect; // ��ʂ̃A�X�y�N�g��
    float time; // ����
    float div; // �l�p�`�̕�����
    float direction; // ��]�̕���
    float rotation; // ��]�p�x
    float size; // �l�p�`�̃T�C�Y
};

// �l�p�`
float rectangle(float2 p, float2 size)
{
    return max(abs(p.x) - size.x, abs(p.y) - size.y);
}

// ���W���w�肳�ꂽ�p�x������]������
float2 Rotation(float2 p, float theta)
{
    return float2((p.x) * cos(theta) - p.y * sin(theta), p.x * sin(theta) + p.y * cos(theta));
}

// �l�p�`�̓�������O���Ɍ������Ēl�𑝉�������̈���쐬
float trs(float2 p, float val, float div, float t)
{
    float mn = 0.001;
    float u = 1.0;
    for (float i = 0; i < t; i++)
    {
        u += (div * 2.0 - 4.0 * i - 2.0) * 4.0;
    }

    float r = (div * 2.0 - 4.0 * t - 2.0);
    float sc = val - u;

    float a = 1;

    // �̈悲�ƂɎl�p�`���쐬���A�����ɂ��邩�ǂ����𔻒�
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
    float4 _color = color; // �o�͂���F
    float _div = div; // �l�p�`�̕�����
    float _dir = direction; // ��]�̕���
    float _rot = rotation; // ��]�p�x
    float2 _asp = aspect; // ��ʂ̃A�X�y�N�g��
    float _ratio = screenSize.x / screenSize.y; // ��ʂ̏c����
    float _size = size; // �l�p�`�̃T�C�Y

    // �e�N�X�`�����W��-1����1�͈̔͂ɕϊ�
    float2 f_st = input.uv * 2.0 - 1.0;

    //// ��]�̕����ɉ����č��W��ϊ�
    f_st.x = f_st.x * (1.0 - 2.0 * _dir);
    f_st = Rotation(f_st, radians(_rot));

    //// ��ʂ̃A�X�y�N�g��Ǝl�p�`�̃T�C�Y���l�����č��W��ϊ�
    f_st *= _size;
    f_st *= _div;

    // �̈悲�Ƃɒl�𑝉�������
    float a = 0.0;
    for (float i = 0; i < _div * 0.5; i++)
    {
        a = min(a + trs(f_st, time, _div, i), 0.5);
    }

    // �l��0�̏ꍇ�͓����ɂ���
    if (a == 0.0)
    {
        discard;
    }

    return _color;
}
