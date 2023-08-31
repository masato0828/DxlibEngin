#pragma once
#include <DxLib.h>
#include <array>

static void MyDrawGraph(int x, int y, int imgHandle)
{
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
	std::array<VERTEX2DSHADER, 4> verts;
	auto wi = width;
	auto he = height;

	for (auto& v : verts)
	{
		v.rhw = 1.0f;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		v.spc = GetColorU8(0, 0, 0, 0);

	}
	// ç∂è„
	verts[0].rhw = 1.0;
	verts[0].pos.x = static_cast<float>(x);
	verts[0].pos.y = static_cast<float>(y);
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;

	// âEè„
	verts[1].rhw = 1.0;
	verts[1].pos.x = static_cast<float>(x + wi);
	verts[1].pos.y = static_cast<float>(y);
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;

	// ç∂â∫
	verts[2].rhw = 1.0;
	verts[2].pos.x = static_cast<float>(x);
	verts[2].pos.y = static_cast<float>(y + he);
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;

	// âEâ∫
	verts[3].rhw = 1.0;
	verts[3].pos.x = static_cast<float>(x + wi);
	verts[3].pos.y = static_cast<float>(y + he);
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	//SetUsePixelShader(psHandle);
	//SetUseTextureToShader(0, imgHandle);
	//SetUseTextureToShader(1, ptnHandle);
	//SetUseTextureToShader(2, housenHandle);
	DrawPrimitive2DToShader(verts.data(), static_cast<int>(verts.size()), DX_PRIMTYPE_TRIANGLESTRIP);
}