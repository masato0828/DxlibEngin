#pragma once
#include "../FreamBase.h"
#include "FileCommon.h"
#include <filesystem>
#include <string>
#include <map>
#include "../../System/ImguiImageStb.h"
#include "../../Common/Vector2.h"
#include "../Fream_Stage.h"

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
	/// ファイルアイコンを生成する処理 
	/// </summary>
	/// <param name="name"></param>
	void MakeFileImage(std::filesystem::path name);

	/// <summary>
	/// ファイルアイコンを表示する処理 
	/// </summary>
	/// <param name="name"></param>
	/// <param name="buttonPressed"></param>
	/// <param name="buttonSize"></param>
	void FileAssignments(std::filesystem::path& name, bool& buttonPressed, Vector2Flt buttonSize);

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



	FileData* nowSelect_;
	std::filesystem::path fileFullPaht_;
	std::wstring nowSelectFileName_;
	std::wstring nowSelectFile_;

	bool contextMenuFlg_;

	// シェーダ情報の作成
	std::map<std::wstring, ID3D11ShaderResourceView*> fileImageShaderDatas_;

	std::unique_ptr<Fream_Stage> stage_;

	bool button_click_;

	std::wstring fileName_;

	bool context_renameFlg_;

	int inputHandle_;
		

	
};

