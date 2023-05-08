#pragma once
#include <string>
#include<vector>


class Shader3D
{
public:
	enum class PixelShaderType
	{
		Material,
		Texture,
	};


	Shader3D(PixelShaderType type,int model);
	Shader3D(
		int modelHandle, 
		std::string vertFileName,
		std::string pixelFileName,
		std::string addImageName ="",
		std::string addImageName2 = "",
		std::string addImageName3 = "",
		std::string addImageName4 = "");

	~Shader3D();


	// directXのセットアップ（Dxlib_Initの前にする必要あり）
	static void ShaderSetUp(int DX_DIRECT3D_);
	// シェーダを使うための初期化
	void Init(int modelHandle,std::string vertFileName, std::string pixelFileName);
	// 定数バッファーの確保、init後に記述
	void CreateBuffer4(int& IntegerBuffer);
	void CreateBuffer8(int &IntegerBuffer);
	// 定数バッファの設定
	// 確保した後のみ、必ず更新処理を入れること
	void SetConstantBuffer(
		int& IntegerBuffer,
		float registerFloat1, 
		float registerFloat2 = NULL,
		float registerFloat3 = NULL,
		float registerFloat4 = NULL);
	// 定数バッファの確保、設定をした後のみ
	// バッファの更新
	void Update(int IntegerBuffer,int registerNo);
	// 指定モデルのみ描画する際に必要
	// すべてに適用する際はその描画前に記述
	void Draw(void);
	// モデルの描画後に記述
	void DrawEnd(void);

	
	void ShaderTexture();

private:

	int addImage_;
	int addImage2_;
	int addImage3_;
	int addImage4_;
	int orgShader_;
	int vertexShaderHandle_;
	int pixelShaderHandle_;

	int modelListNum_;
	int modelShaderType_;

	// 頂点シェーダのセット
	void VertexSet(std::string vertFileName);
	void VertexType(std::string& vertFileName);
	// ピクセルシェーダのセット
	void PixelSet(std::string pixelFileName);

	void AddImageLoad(std::string& addImageName, std::string& addImageName2, std::string& addImageName3, std::string& addImageName4);
	void AddImage(int& imageHandle, int& imageHandle2, int& imageHandle3, int& imageHandle4);

};

