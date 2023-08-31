#pragma once
#include <map>
#include <memory>
#include <string>
#include <filesystem>
#include "../../../System/ImguiImageStb.h"
#include "../../Fream_Stage.h"
class CreateIcons
{
public:
	CreateIcons();
	~CreateIcons();

	void Init();
	void Update();

	/// <summary>
	/// �t�@�C���A�C�R���𐶐����鏈�� 
	/// </summary>
	/// <param name="name"></param>
	void MakeFileImage(std::filesystem::path name, std::filesystem::path fileFullPath);

	

private:
	/// <summary>
	/// �t�@�C���A�C�R����\�����鏈�� 
	/// </summary>
	/// <param name="name"></param>
	/// <param name="buttonPressed"></param>
	/// <param name="buttonSize"></param>
	void FileAssignments(std::filesystem::path& name, bool& buttonPressed, Vector2Flt buttonSize, std::filesystem::path fileFullPath);

	/// <summary>
	/// �A�C�R���ݒ�̏���
	/// </summary>
	/// <param name="name"></param>
	/// <param name="buttonPressed"></param>
	/// <param name="buttonSize"></param>
	/// <param name="ext"></param>
	/// <returns></returns>
	bool SettingIcon(std::wstring& name, bool& buttonPressed, Vector2Flt buttonSize, std::wstring ext);

	/// <summary>
	/// 3D���f���̃A�C�R���𐶐����鏈��
	/// </summary>
	/// <param name="path"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	bool CreateModelIcon(std::filesystem::path path, std::wstring key);

	/// <summary>
	/// �QD�摜�̃A�C�R���𐶐����鏈��
	/// </summary>
	/// <param name="path"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	bool CreateImageIcon(std::filesystem::path path, std::wstring key);


	// �V�F�[�_���̍쐬
	std::map<std::wstring, ID3D11ShaderResourceView*> fileImageShaderDatas_;

	std::unique_ptr<Fream_Stage> stage_;
};

