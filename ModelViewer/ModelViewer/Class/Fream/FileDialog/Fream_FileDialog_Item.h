#pragma once
#include "../FreamBase.h"
#include "FileCommon.h"
#include <filesystem>
#include <string>
#include "../../System/ImguiImageStb.h"

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

	void RenameWindow();

	FileData* nowSelect_;
	std::filesystem::path fileFullPaht_;
	std::wstring nowSelectFileName_;
	std::wstring nowSelectFile_;

	bool contextMenuFlg_;

	// シェーダ情報の作成
	ID3D11ShaderResourceView* my_shaderData = NULL;

	bool button_click_;

	std::wstring fileName_;

	bool context_renameFlg_;

	int inputHandle_;

	// キーボードの入力状態を保存する変数
	char prevKeyState[KEY_NUM] = {};
	char currentKeyState[KEY_NUM] = {};
		
};

