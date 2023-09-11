struct VsBase
{
    matrix antiViewportMatrix; // �A���`�r���[�|�[�g�s��
    matrix projectionMatrix; // �r���[�@���@�v���W�F�N�V�����s��
    float4x3 viewMatrix; // ���[���h�@���@�r���[�s��
    float4x3 localWorldMatrix; // ���[�J���@���@���[���h�s��

    float4 toonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float diffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float specularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float mulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float padding;
};

cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    VsBase base;
}

//1�t���[���̉e�����󂯂钸�_
struct VertexInput
{
    float3 pos : POSITION; // ���W(���[�J�����)
    float3 norm : NORMAL; // �@��(���[�J�����)
    float4 diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 specular : COLOR1; // �X�y�L�����J���[
    float4 uv0 : TEXCOORD0; // �e�N�X�`�����W
    float4 uv1 : TEXCOORD1; // �T�u�e�N�X�`�����W
};

//�@���}�b�v�Ȃ��̏ꍇ
struct VertexToPixel
{
    float4 svPos : SV_POSITION; //�ʒu(�v���W�F�N�V����)
    float3 worldPos : POSITION0; //�ʒu(���[���h)
    float3 viewPos : POSITION1; //�ʒu(�r���[)
    float3 viewNorm : NORMAL0; //�@��
    float4 diffuse : COLOR0; //�g�U���ːF
    float4 specular : COLOR1; //���ʔ��ːF
    float2 uv : TEXCOORD; //UV�l
};

#define VS_INPUT VertexInput
#define VS_OUTPUT VertexToPixel

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT ret;
    ret.svPos = float4(input.pos,1.0f);
    
    // ���[�J�����W�n�@���@���[���h���W
    ret.worldPos = mul(ret.svPos,base.localWorldMatrix);
    ret.svPos.xyz = ret.worldPos;
    
    // ���[���h���W�n�@���@�r���[���W�n
    ret.viewPos = mul(ret.svPos,base.viewMatrix);
    ret.svPos.xyz = ret.viewPos;
    
    // �r���[���W�n�@�@���@�v���W�F�N�V�������W�n
    ret.svPos = mul(ret.svPos,base.projectionMatrix);
    
    // �@��(�r���[)���v�Z
    ret.viewNorm = normalize(mul(float4(input.norm,0.0f),base.localWorldMatrix));
    ret.viewNorm = normalize(mul(float4(ret.viewNorm,0.0f),base.viewMatrix));
    
    ret.uv = input.uv0.xy;          // UV����
    ret.diffuse = input.diffuse;    // �g�U���ːF����
    ret.specular = input.specular;  // ���ʔ��ːF����
    //
    return ret;
}