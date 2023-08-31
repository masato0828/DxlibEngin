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


	// directX�̃Z�b�g�A�b�v�iDxlib_Init�̑O�ɂ���K�v����j
	static void ShaderSetUp(int DX_DIRECT3D_);
	// �V�F�[�_���g�����߂̏�����
	void Init(int modelHandle,std::string vertFileName, std::string pixelFileName);
	// �萔�o�b�t�@�[�̊m�ہAinit��ɋL�q
	void CreateBuffer4(int& IntegerBuffer);
	void CreateBuffer8(int &IntegerBuffer);
	// �萔�o�b�t�@�̐ݒ�
	// �m�ۂ�����̂݁A�K���X�V���������邱��
	void SetConstantBuffer(
		int& IntegerBuffer,
		float registerFloat1, 
		float registerFloat2 = NULL,
		float registerFloat3 = NULL,
		float registerFloat4 = NULL);
	// �萔�o�b�t�@�̊m�ہA�ݒ��������̂�
	// �o�b�t�@�̍X�V
	void Update(int IntegerBuffer,int registerNo);
	// �w�胂�f���̂ݕ`�悷��ۂɕK�v
	// ���ׂĂɓK�p����ۂ͂��̕`��O�ɋL�q
	void Draw(void);
	// ���f���̕`���ɋL�q
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

	// ���_�V�F�[�_�̃Z�b�g
	void VertexSet(std::string vertFileName);
	void VertexType(std::string& vertFileName);
	// �s�N�Z���V�F�[�_�̃Z�b�g
	void PixelSet(std::string pixelFileName);

	void AddImageLoad(std::string& addImageName, std::string& addImageName2, std::string& addImageName3, std::string& addImageName4);
	void AddImage(int& imageHandle, int& imageHandle2, int& imageHandle3, int& imageHandle4);

};

