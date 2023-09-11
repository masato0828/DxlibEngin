//�C���N���[�h�K�[�h
#if !defined(PIXEL_SHADER_3D_HEADER)
#define PIXEL_SHADER_3D_HEADER

    #include "../Common/CommonShader3DHeader.hlsli"

    #include "PixelStructuresHeader.hlsli"
	#include "PixelFunctionHeader.hlsli"

    // ��{�p�����[�^
    cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
    {
        PsBase base;
    };

    SamplerState diffuseMapSampler : register(s0); // �f�B�t���[�Y�}�b�v�e�N�X�`��
    Texture2D diffuseMapTexture : register(t0); // �f�B�t���[�Y�}�b�v�e�N�X�`��
#endif