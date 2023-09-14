//インクルードガード
#if !defined(COMMON_SHADER_3D_HEADER)
	#define COMMON_SHADER_3D_HEADER

	#include "../Common/CommonStructuresHeader.hlsli"

	// 頂点シェーダー・ピクセルシェーダー共通パラメータ
	cbuffer cbD3D11_CONST_BUFFER_COMMON : register(b0)
	{
	    Common common;
	};

#endif
