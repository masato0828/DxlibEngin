#pragma once
#include "../FreamBase.h"
#include "FileCommon.h"
#include <filesystem>

class Fream_FileDialog_Tree :
    public FreamBase
{
public:
	Fream_FileDialog_Tree();
	~Fream_FileDialog_Tree();

	/// <summary>
	/// ������
	/// </summary>
	void Init() override;
	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="fileData">�t�@�C���f�[�^</param>
	/// <param name="nowSelect">���݂̑I��</param>
	/// <param name="nowSelectPath">���݂̑I���t�@�C���̃t���p�X</param>
	/// <param name="nowSelectFileName">���݂̑I���t�@�C���̖��O</param>
	void Update(
		FileData& fileData,
		FileData*& nowSelect,
		std::filesystem::path& nowSelectPath,
		std::wstring& nowSelectFileName);

private:

	// ���݂̑I���p�X
	std::filesystem::path nowSelectPath_;

	// ���݂̑I���t�@�C���̖��O
	std::wstring nowSelectFileName_;

	/// <summary>
	/// �t�@�C���c���[�i�t�@�C���m�[�h�j�̍쐬
	/// </summary>
	/// <param name="directory">�t���p�X</param>
	/// <param name="fileData">�t�@�C���f�[�^</param>
	/// <param name="nowSelect">���ݑI�𒆂̃t�@�C���f�[�^</param>
	void Tree(std::filesystem::path directory, FileData& fileData, FileData*& nowSelect);

};

