#pragma once
#include <d3d11.h>

inline namespace ImguiSup
{
	extern bool LoadTextureFromFile(
		const char* filename,
		ID3D11ShaderResourceView** out_srv,
		int* out_width,
		int* out_height);

	extern bool LoadBackGroundTextureFromFile(
		ID3D11ShaderResourceView** out_srv,
		int* out_width,
		int* out_height);

	extern bool LoadTextureFromFile(
		int handle,
		ID3D11ShaderResourceView** out_srv,
		int* out_width,
		int* out_height);
}