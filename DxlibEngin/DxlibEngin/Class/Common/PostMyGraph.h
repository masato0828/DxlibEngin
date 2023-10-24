#pragma once
#include <DxLib.h>
#include <array>

/// <summary>
/// ポリゴンの作成
/// </summary>
/// <param name="x">横ポリゴンサイズ</param>
/// <param name="y">縦ポリゴンサイズ</param>
/// <param name="imgHandle">画像ハンドル</param>
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
	// 左上
	verts[0].rhw = 1.0;
	verts[0].pos.x = static_cast<float>(x);
	verts[0].pos.y = static_cast<float>(y);
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;

	// 右上
	verts[1].rhw = 1.0;
	verts[1].pos.x = static_cast<float>(x + wi);
	verts[1].pos.y = static_cast<float>(y);
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;

	// 左下
	verts[2].rhw = 1.0;
	verts[2].pos.x = static_cast<float>(x);
	verts[2].pos.y = static_cast<float>(y + he);
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;

	// 右下
	verts[3].rhw = 1.0;
	verts[3].pos.x = static_cast<float>(x + wi);
	verts[3].pos.y = static_cast<float>(y + he);
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	DrawPrimitive2DToShader(verts.data(), static_cast<int>(verts.size()), DX_PRIMTYPE_TRIANGLESTRIP);
}

static void MyDrawGraph3D(int x, int y,int z, int imgHandle)
{
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
	std::array<VERTEX3DSHADER, 8> verts;
	auto wi = width;
	auto he = height;

	for (auto& v : verts)
	{
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
		v.spc = GetColorU8(0, 0, 0, 0);
		v.norm = VGet(0,0,0);
		v.tan = VGet(0, 0, 0);
		v.spos = { 0, 0, 0,0 };
		v.binorm = VGet(0, 0, 0);

	}

	auto s = 50;

	// 左上
	verts[0].pos.x = static_cast<float>(x - (wi / 2));
	verts[0].pos.y = static_cast<float>(y + (he / 2));
	verts[0].pos.z = static_cast<float>(z-s);
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;

	// 右上
	verts[1].pos.x = static_cast<float>(x + (wi / 2));
	verts[1].pos.y = static_cast<float>(y + (he / 2));
	verts[1].pos.z = static_cast<float>(z-s);
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;

	// 左下
	verts[2].pos.x = static_cast<float>(x - (wi / 2));
	verts[2].pos.y = static_cast<float>(y - (he / 2));
	verts[2].pos.z = static_cast<float>(z-s);
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;

	// 右下
	verts[3].pos.x = static_cast<float>(x + (wi / 2));
	verts[3].pos.y = static_cast<float>(y - (he / 2));
	verts[3].pos.z = static_cast<float>(z-s);
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	verts[4].pos.x = static_cast<float>(x - (wi / 2));
	verts[4].pos.y = static_cast<float>(y + (he / 2));
	verts[4].pos.z = static_cast<float>(z+s);
	verts[4].u = 0.0f;
	verts[4].v = 0.0f;

	verts[5].pos.x = static_cast<float>(x + (wi / 2));
	verts[5].pos.y = static_cast<float>(y + (he / 2));
	verts[5].pos.z = static_cast<float>(z + s);
	verts[5].u = 1.0f;
	verts[5].v = 0.0f;

	verts[6].pos.x = static_cast<float>(x - (wi / 2));
	verts[6].pos.y = static_cast<float>(y - (he / 2));
	verts[6].pos.z = static_cast<float>(z + 1);
	verts[6].u = 0.0f;
	verts[6].v = 1.0f;

	verts[7].pos.x = static_cast<float>(x + (wi / 2));
	verts[7].pos.y = static_cast<float>(y - (he / 2));
	verts[7].pos.z = static_cast<float>(z +s);
	verts[7].u = 1.0f;
	verts[7].v = 1.0f;

	DrawPrimitive3DToShader(verts.data(), static_cast<int>(verts.size()), DX_PRIMTYPE_TRIANGLESTRIP);
}