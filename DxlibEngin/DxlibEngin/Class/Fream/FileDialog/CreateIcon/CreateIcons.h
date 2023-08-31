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
	/// ファイルアイコンを生成する処理 
	/// </summary>
	/// <param name="name"></param>
	void MakeFileImage(std::filesystem::path name, std::filesystem::path fileFullPath);

	

private:
	/// <summary>
	/// ファイルアイコンを表示する処理 
	/// </summary>
	/// <param name="name"></param>
	/// <param name="buttonPressed"></param>
	/// <param name="buttonSize"></param>
	void FileAssignments(std::filesystem::path& name, bool& buttonPressed, Vector2Flt buttonSize, std::filesystem::path fileFullPath);

	/// <summary>
	/// アイコン設定の処理
	/// </summary>
	/// <param name="name"></param>
	/// <param name="buttonPressed"></param>
	/// <param name="buttonSize"></param>
	/// <param name="ext"></param>
	/// <returns></returns>
	bool SettingIcon(std::wstring& name, bool& buttonPressed, Vector2Flt buttonSize, std::wstring ext);

	/// <summary>
	/// 3Dモデルのアイコンを生成する処理
	/// </summary>
	/// <param name="path"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	bool CreateModelIcon(std::filesystem::path path, std::wstring key);

	/// <summary>
	/// ２D画像のアイコンを生成する処理
	/// </summary>
	/// <param name="path"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	bool CreateImageIcon(std::filesystem::path path, std::wstring key);


	// シェーダ情報の作成
	std::map<std::wstring, ID3D11ShaderResourceView*> fileImageShaderDatas_;

	std::unique_ptr<Fream_Stage> stage_;
};

