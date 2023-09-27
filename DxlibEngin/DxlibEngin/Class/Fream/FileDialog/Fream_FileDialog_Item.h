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

// ファイルダイアログ作成クラス
class Fream_FileDialog_Item :
    public FreamBase
{
public:
	/// コンストラクタ
	Fream_FileDialog_Item();
	// デストラクタ
	~Fream_FileDialog_Item();

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
	/// <param name="nowselect">選択中のファイルデータ</param>
	/// <param name="fileFullPath">ファイルのフルパス</param>
	/// <param name="nowSelectFile">選択中のファイル</param>
	/// <param name="nowSelectFileName">選択中のファイルの名前</param>
	void Update(FileData*& nowselect,std::filesystem::path& fileFullPath, std::wstring& nowSelectFile, std::wstring& nowSelectFileName);
	bool& GetButton_Click();
private:

	/// <summary>
	/// コンテキストメニューの処理 
	/// </summary>
	void Popup();

	/// <summary>
	/// DockSpaceを生成する処理 
	/// </summary>
	void DokingWindow();

	/// <summary>
	/// ファイル名表示ウィンドウを生成する処理
	/// </summary>
	void FileNameWindow();

	/// <summary>
	/// ファイル履歴表示ウィンドウを生成する処理...
	/// </summary>
	void FileLogWindow();

	/// <summary>
	/// ファイル履歴を表示する処理 
	/// </summary>
	/// <param name="selectData">選択中のファイルデータ</param>
	void Recovery(FileData* selectData);

	/// <summary>
	/// ファイルの作成
	/// </summary>
	void CreateFiles(std::filesystem::path name);

	/// <summary>
	/// ファイル名変更ウィンドウを生成する処理
	/// </summary>
	void RenameWindow();

	/// <summary>
	/// ファイルの反復処理とフォルダの作成関数
	/// </summary>
	void IterateFilesAndFolders();

	/// <summary>
	/// コンテキストメニューを処理する関数
	/// </summary>
	void HandleContextMenu();

	/// <summary>
	/// ファイルのリネームウィンドウを処理する関数
	/// </summary>
	void HandleFileRenamingWindow();


	// 現在のファイルデータ
	FileData* nowSelect_;
	// ファイルのフルパス保持
	std::filesystem::path fileFullPaht_;
	// 現在の選択ファイルの名前保持
	std::wstring nowSelectFileName_;
	// 現在の選択ファイル
	std::wstring nowSelectFile_;

	// コンテキストメニューの表示状態フラグ
	bool contextMenuFlg_;

	// ボタンクリック状態
	bool button_click_;

	// ファイルの名前
	std::wstring fileName_;

	// リネームウィンドウ表示フラグ
	bool context_renameFlg_;

	std::unique_ptr<CreateIcons> icon_;
	
};

