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
	/// 初期化
	/// </summary>
	void Init() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="fileData">ファイルデータ</param>
	/// <param name="nowSelect">現在の選択</param>
	/// <param name="nowSelectPath">現在の選択ファイルのフルパス</param>
	/// <param name="nowSelectFileName">現在の選択ファイルの名前</param>
	void Update(
		FileData& fileData,
		FileData*& nowSelect,
		std::filesystem::path& nowSelectPath,
		std::wstring& nowSelectFileName);

private:

	// 現在の選択パス
	std::filesystem::path nowSelectPath_;

	// 現在の選択ファイルの名前
	std::wstring nowSelectFileName_;

	/// <summary>
	/// ファイルツリー（ファイルノード）の作成
	/// </summary>
	/// <param name="directory">フルパス</param>
	/// <param name="fileData">ファイルデータ</param>
	/// <param name="nowSelect">現在選択中のファイルデータ</param>
	void Tree(std::filesystem::path directory, FileData& fileData, FileData*& nowSelect);

};

