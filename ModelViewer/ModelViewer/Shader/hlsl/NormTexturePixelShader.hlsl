// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 svPosition : SV_POSITION; // ���W( �v���W�F�N�V������� )
    float4 Position : POSITION0;
    float4 P : POSITION1;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // �F
};

SamplerState g_DiffuseMapSampler : register(s0); // �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D g_DiffuseMapTexture : register(t0); // �f�B�t���[�Y�}�b�v�e�N�X�`��

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    float4 TextureDiffuseColor;
    
	// �e�N�X�`���J���[�̓ǂݍ���
    TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
    
	// �o�̓J���[ = �e�N�X�`���J���[
    PSOutput.Color0 = TextureDiffuseColor;

	// �o�̓p�����[�^��Ԃ�
    return PSOutput;
}
