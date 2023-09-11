// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
    float3 Position : POSITION; // ���W( ���[�J����� )
    float3 Normal : NORMAL; // �@��( ���[�J����� )
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float4 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 TexCoords1 : TEXCOORD1; // �T�u�e�N�X�`�����W
};

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 Position : SV_POSITION; // ���W( �v���W�F�N�V������� )
    float3 wPos : Position;
};


// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
    float4 AntiViewportMatrix[4]; // �A���`�r���[�|�[�g�s��
    float4 ProjectionMatrix[4]; // �r���[�@���@�v���W�F�N�V�����s��
    float4 ViewMatrix[3]; // ���[���h�@���@�r���[�s��
    float4 LocalWorldMatrix[3]; // ���[�J���@���@���[���h�s��

    float4 ToonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float DiffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float SpecularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float MulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float Padding;
};

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
    float4 ShadowMapLightViewProjectionMatrix[3][4]; // �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
    float4 TextureMatrix[3][2]; // �e�N�X�`�����W����p�s��
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    DX_D3D11_VS_CONST_BUFFER_BASE g_Base;
};

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX : register(b2)
{
    DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX g_OtherMatrix;
};



// main�֐�
VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT VSOutput;
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
    
    VSOutput.Diffuse = float4(0,0,0,0);
    VSOutput.Specular = float4(0, 0, 0, 0);
    VSOutput.TexCoords0 = float4(0, 0, 0, 0);


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�̃Z�b�g
    lLocalPosition.xyz = VSInput.Position;
    lLocalPosition.w = 1.0f;

	// ���W�v�Z( ���[�J�����r���[���v���W�F�N�V���� )
    lWorldPosition.x = dot(lLocalPosition, g_Base.LocalWorldMatrix[0]);
    lWorldPosition.y = dot(lLocalPosition, g_Base.LocalWorldMatrix[1]);
    lWorldPosition.z = dot(lLocalPosition, g_Base.LocalWorldMatrix[2]);
    lWorldPosition.w = 1.0f;
    
    // ���[���h���W�̎擾
    VSOutput.wPos.x = lWorldPosition.x;
    VSOutput.wPos.y = lWorldPosition.y;
    VSOutput.wPos.z = lWorldPosition.z;

    lViewPosition.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
    lViewPosition.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
    lViewPosition.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
    lViewPosition.w = 1.0f;

    VSOutput.Position.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
    VSOutput.Position.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
    VSOutput.Position.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
    VSOutput.Position.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	// �o�̓p�����[�^��Ԃ�
    return VSOutput;
}

