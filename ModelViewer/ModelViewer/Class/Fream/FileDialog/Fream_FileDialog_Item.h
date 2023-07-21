#pragma once
#include "../FreamBase.h"
#include "FileCommon.h"
#include <filesystem>
#include <string>
#include <map>
#include "../../System/ImguiImageStb.h"
#include "../../Common/Vector2.h"
#include "../Fream_Stage.h"

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

	void MakeFileImage(std::filesystem::path name);

	void FileAssignments(std::filesystem::path& name, bool& buttonPressed, Vector2Flt buttonSize);

	void RenameWindow();

	bool SettingIcon(std::wstring& name, bool& buttonPressed, Vector2Flt buttonSize,std::string ext);

	bool CreateIcon(std::filesystem::path path,std::string key);


	FileData* nowSelect_;
	std::filesystem::path fileFullPaht_;
	std::wstring nowSelectFileName_;
	std::wstring nowSelectFile_;

	bool contextMenuFlg_;

	// シェーダ情報の作成
	ID3D11ShaderResourceView* my_shaderData = NULL;
	std::map<std::string, ID3D11ShaderResourceView*> fileImageShaderDatas_;

	std::unique_ptr<Fream_Stage> stage_;

	bool button_click_;

	std::wstring fileName_;

	bool context_renameFlg_;

	int inputHandle_;
		
};

