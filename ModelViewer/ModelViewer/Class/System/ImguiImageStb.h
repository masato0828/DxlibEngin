#pragma once
#include <d3d11.h>

/**
//class ID3D11ShaderResourceView;
struct LoadTextureFromFile
{
	bool operator()(
		const char* filename,
		ID3D11ShaderResourceView** out_srv,
		int* out_width,
		int* out_height);

};
/*/
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
/**/