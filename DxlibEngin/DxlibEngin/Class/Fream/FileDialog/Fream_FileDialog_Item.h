#pragma once
#include "../FreamBase.h"
#include "FileCommon.h"
#include <filesystem>
#include <string>
#include <map>
#include "../../System/ImguiImageStb.h"
#include "../../Common/Vector2.h"
#include "../Fream_Stage.h"
#include "CreateIcon\CreateIcons.h"

// �t�@�C���_�C�A���O�쐬�N���X
class Fream_FileDialog_Item :
    public FreamBase
{
public:
	/// �R���X�g���N�^
	Fream_FileDialog_Item();
	// �f�X�g���N�^
	~Fream_FileDialog_Item();

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
	/// <param name="nowselect">�I�𒆂̃t�@�C���f�[�^</param>
	/// <param name="fileFullPath">�t�@�C���̃t���p�X</param>
	/// <param name="nowSelectFile">�I�𒆂̃t�@�C��</param>
	/// <param name="nowSelectFileName">�I�𒆂̃t�@�C���̖��O</param>
	void Update(FileData*& nowselect,std::filesystem::path& fileFullPath, std::wstring& nowSelectFile, std::wstring& nowSelectFileName);
	bool& GetButton_Click();
private:

	/// <summary>
	/// �R���e�L�X�g���j���[�̏��� 
	/// </summary>
	void Popup();

	/// <summary>
	/// DockSpace�𐶐����鏈�� 
	/// </summary>
	void DokingWindow();

	/// <summary>
	/// �t�@�C�����\���E�B���h�E�𐶐����鏈��
	/// </summary>
	void FileNameWindow();

	/// <summary>
	/// �t�@�C������\���E�B���h�E�𐶐����鏈��...
	/// </summary>
	void FileLogWindow();

	/// <summary>
	/// �t�@�C��������\�����鏈�� 
	/// </summary>
	/// <param name="selectData">�I�𒆂̃t�@�C���f�[�^</param>
	void Recovery(FileData* selectData);

	/// <summary>
	/// �t�@�C���̍쐬
	/// </summary>
	void CreateFiles(std::filesystem::path name);

	/// <summary>
	/// �t�@�C�����ύX�E�B���h�E�𐶐����鏈��
	/// </summary>
	void RenameWindow();

	/// <summary>
	/// �t�@�C���̔��������ƃt�H���_�̍쐬�֐�
	/// </summary>
	void IterateFilesAndFolders();

	/// <summary>
	/// �R���e�L�X�g���j���[����������֐�
	/// </summary>
	void HandleContextMenu();

	/// <summary>
	/// �t�@�C���̃��l�[���E�B���h�E����������֐�
	/// </summary>
	void HandleFileRenamingWindow();


	// ���݂̃t�@�C���f�[�^
	FileData* nowSelect_;
	// �t�@�C���̃t���p�X�ێ�
	std::filesystem::path fileFullPaht_;
	// ���݂̑I���t�@�C���̖��O�ێ�
	std::wstring nowSelectFileName_;
	// ���݂̑I���t�@�C��
	std::wstring nowSelectFile_;

	// �R���e�L�X�g���j���[�̕\����ԃt���O
	bool contextMenuFlg_;

	// �{�^���N���b�N���
	bool button_click_;

	// �t�@�C���̖��O
	std::wstring fileName_;

	// ���l�[���E�B���h�E�\���t���O
	bool context_renameFlg_;

	std::unique_ptr<CreateIcons> icon_;
	
};

