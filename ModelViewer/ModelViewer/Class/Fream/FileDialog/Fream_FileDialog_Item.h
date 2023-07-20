#pragma once
#include "../FreamBase.h"
#include "FileCommon.h"
#include <filesystem>
#include <string>
#include <map>
#include "../../System/ImguiImageStb.h"
#include "../../Common/Vector2.h"

class Fream_FileDialog_Item :
    public FreamBase
{
public:
	Fream_FileDialog_Item();
	~Fream_FileDialog_Item();

	void Init() override;
	void Update() override;
	void Update(FileData*& nowselect,std::filesystem::path& fileFullPath, std::wstring& nowSelectFile, std::wstring& nowSelectFileName);
	bool& GetButton_Click();
private:
	void Popup();

	void DokingWindow();

	void FileNameWindow();

	void FileLogWindow();

	void Recovery(FileData* selectData);

	void MakeFileImage(std::wstring name);

	void FileAssignments(std::wstring& name, bool& buttonPressed, Vector2Flt buttonSize);

	void RenameWindow();

	bool SettingIcon(std::wstring& name, bool& buttonPressed, Vector2Flt buttonSize,std::string ext);

	FileData* nowSelect_;
	std::filesystem::path fileFullPaht_;
	std::wstring nowSelectFileName_;
	std::wstring nowSelectFile_;

	bool contextMenuFlg_;

	// �V�F�[�_���̍쐬
	ID3D11ShaderResourceView* my_shaderData = NULL;
	std::map<std::string, ID3D11ShaderResourceView*> fileImageShaderDatas_;


	bool button_click_;

	std::wstring fileName_;

	bool context_renameFlg_;

	int inputHandle_;
		
};

