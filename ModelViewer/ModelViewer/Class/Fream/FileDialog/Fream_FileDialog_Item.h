#pragma once
#include "../FreamBase.h"
#include "FileCommon.h"
#include <filesystem>
#include <string>
#include <map>
#include "../../System/ImguiImageStb.h"
#include "../../Common/Vector2.h"
#include "../Fream_Stage.h"

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
	/// �t�@�C���A�C�R���𐶐����鏈�� 
	/// </summary>
	/// <param name="name"></param>
	void MakeFileImage(std::filesystem::path name);

	/// <summary>
	/// �t�@�C���A�C�R����\�����鏈�� 
	/// </summary>
	/// <param name="name"></param>
	/// <param name="buttonPressed"></param>
	/// <param name="buttonSize"></param>
	void FileAssignments(std::filesystem::path& name, bool& buttonPressed, Vector2Flt buttonSize);

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



	FileData* nowSelect_;
	std::filesystem::path fileFullPaht_;
	std::wstring nowSelectFileName_;
	std::wstring nowSelectFile_;

	bool contextMenuFlg_;

	// �V�F�[�_���̍쐬
	std::map<std::wstring, ID3D11ShaderResourceView*> fileImageShaderDatas_;

	std::unique_ptr<Fream_Stage> stage_;

	bool button_click_;

	std::wstring fileName_;

	bool context_renameFlg_;

	int inputHandle_;
		

	
};

