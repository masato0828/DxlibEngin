#include "Fream_FileDialog.h"
#include "../../imGui/ImGuiFileDialog.h"
#include "../../imGui/CustomFont.h"
#include "../Common/FileDialog.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"
#include <codecvt>
#include <shlwapi.h>
#include <Windows.h>
#include <string>

Fream_FileDialog::Fream_FileDialog():fileData_(nullptr, (--std::filesystem::current_path().end())->u8string())
{
	Init();
}

Fream_FileDialog::~Fream_FileDialog()
{
}

void Fream_FileDialog::Init()
{
	// 選択している数
	nowSelect = nullptr;
	selected_ = 0;
	// 選択用マスク
	//selection_mask_ = (1 << 0);
	nowSelect = &fileData_;

	nowSelectFileName_ = "";
}

void Fream_FileDialog::Update()
{
	if (ImGui::Begin("Project"))
	{
		// 強制ドッキング設定
		ImGuiID dockspace_id = ImGui::GetID("projectDock");
		if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
		{
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

			ImGuiID dock_main_id = dockspace_id;
			ImGuiID dock_id_FileDialog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 1.f, NULL, &dock_main_id);

			ImGui::DockBuilderSetNodeSize(dock_main_id, ImVec2(300, 200));
			ImGui::DockBuilderSetNodeSize(dock_id_FileDialog, ImVec2(293, 200));

			ImGui::DockBuilderDockWindow("fileDialog", dock_id_FileDialog);
			ImGui::DockBuilderDockWindow("ITEM", dock_main_id);

			ImGui::DockBuilderFinish(dockspace_id);
		}
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		mainWindowSize_ = { ImGui::GetWindowSize().x,ImGui::GetWindowSize().y};

		// Imgui用ウィンドウクラスの作成
		ImGuiWindowClass window_class;
		// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("fileDialog", 0))
		{
			//現在のカンレントディレクトリを取得
			auto directory = std::filesystem::current_path();
			// プログラム起動時にファイルの出力
			// その位置からツリーを開く

			//if (ImGui::TreeNode("file"))
			//{
			int eraseNum = directory.string().find_last_of('\\');
			//// 
			std::string file = directory.string().erase(0, eraseNum);
			//// 
			std::string fileN = file.erase(0, 1);

			//if (ImGui::TreeNode(fileN.c_str()))
			//{
			//	selection_mask_vector_.push_back((0 << 1));
				//Tree(directory,0);
			ImGuiTreeNodeFlags node_flags =
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_OpenOnDoubleClick
				| ImGuiTreeNodeFlags_SpanAvailWidth;

			if (&fileData_ == nowSelect)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}
			
			if (nowSelect->parentFile_ != NULL)
			{
				if (IsMatch(directory.string(), nowSelect->parentFile_->myName))
				{
					// ツリーを開く
					ImGui::SetNextItemOpen(true);
				}
			}


			bool is_open = ImGui::TreeNodeEx((void*)&fileData_, node_flags, fileData_.myName.c_str());

			

			// ツリーを選択
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				// 現在選択中
				nowSelect = &fileData_;

			}
			if (is_open)
			{
				Tree(directory, fileData_);
				ImGui::TreePop();
			}





			//	ImGui::TreePop();
			//}

		/*	ImGui::TreePop();
		}*/

			ImGui::End();
		}

		ImGui::SetNextWindowClass(&window_class);
		ItemWindow();
		
		ImGui::End();
	}
}

void Fream_FileDialog::Tree(std::filesystem::path directory, FileData& fileData)
{
	// イテレータの作成
	std::filesystem::directory_iterator itr(directory.u8string());
	std::filesystem::directory_iterator end{};

	for (; itr != end; ++itr) 
	{	
		// ツリーデータの設定
		ImGuiTreeNodeFlags node_flags = 0;//=
			
			//ImGuiTreeNodeFlags_OpenOnDoubleClick |
			//ImGuiTreeNodeFlags_SpanAvailWidth;

		// ディレクトリがない場合
		if (!itr->is_directory())
		{
			// もう一度
			continue;
		}


		// イメージ
		// --------------------path-----------------------------
		// C: file1 // file2 // file3 // file4 // file5 // end{};
		// --------------------path-----------------------------の状態から
		// １ファイル分つまり//end{}を引かれ
		// C: file1 // file2 // file3 // file4 // file5となる
		// そこからitr->path().end()で一番最後のパスをとり
		// file5がとれる
		
		// この場合は(--itr->path().end()) でfile5が入る
		// ↓
		// フォルダ名の取得　（フルパスからの切り離し）
		auto directoryName = (--itr->path().end())->u8string();
		
		// データの作成　自分の
		//auto[dataItr,empFlag] = fileData.fileMap_.try_emplace(directoryName, FileData());
		auto data = fileData.fileMap_.try_emplace(directoryName.c_str(), FileData(&fileData, directoryName.c_str())).first;

		// 選択しているかどうか
		//const bool is_selected = &dataItr->second == nowSelect;
		const bool is_selected = &data->second == nowSelect;

		// 選択中
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		std::filesystem::recursive_directory_iterator subItr(itr->path());

		for (auto& sub : subItr)
		{
			if (sub.is_directory())
			{ 
				node_flags = node_flags & ~ImGuiTreeNodeFlags_Leaf;
				break;
			} 
			node_flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (IsMatch(directoryName.c_str(), nowSelectFileName_))
		{
			// ツリーを開く
			ImGui::SetNextItemOpen(true);
		}

		// ノードを開いているかどうか
		//auto is_open = ImGui::TreeNodeEx((void*)&dataItr->second, node_flags, directoryName.c_str());
		auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags, directoryName.c_str());

		// ツリーを選択
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// 現在選択中
			//nowSelect = &dataItr->second;
			nowSelect = &data->second;
			nowSelectPath_ = itr->path();
		}

		// ノードが開かれている
		if (is_open)
		{
			// 再起処理
			//Tree2(itr->path(), dataItr->second);
			Tree(itr->path(), data->second);

			// ツリーの終了
			ImGui::TreePop();
		}
	}


}

bool Fream_FileDialog::CharacterSearch(std::string showName,std::string searchFileName,Color color, std::string defaultTarget)
{
	if(IsHeaderFile(defaultTarget,searchFileName))
	//if (defaultTarget.find(searchFileName) != std::string::npos)
	{
		ImGui::TextColored(ImVec4(color.r, color.g,color.b, color.a), showName.c_str());
		return true;
	}
	return false;
}

void Fream_FileDialog::ItemWindow()
{
	//// 強制ドッキング設定
	ImGuiID dockspace_id = ImGui::GetID("ShowFileNameLog");

	if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	{
		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
		

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_id_FileNameLog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.001f, NULL, &dock_main_id);

		ImGui::DockBuilderDockWindow("FileNameLog", dock_main_id);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	if (ImGui::Begin("ITEM"))
	{
		
		ImGui::DockSpace(dockspace_id, ImVec2(ImGui::GetWindowWidth(), 50.0f));

		ImGui::Separator();
		if (nowSelect != nullptr)
		{
			fileFullPaht_.clear();
			Recovery(nowSelect);
			fileFullPaht_ = fileFullPaht_.empty() ? std::filesystem::current_path() :
				std::filesystem::current_path() /= fileFullPaht_;
			ImGui::Separator();

			auto current_path = std::filesystem::current_path();

			// イテレータの作成
			std::filesystem::directory_iterator itr(fileFullPaht_);
			std::filesystem::directory_iterator end{};
			std::string directoryName;

			auto copySelectPath = nowSelectFileName_;
			for (; itr != end; ++itr)
			{
				directoryName = (--itr->path().end())->u8string();

				MakeFileImage(directoryName/*現在フォルダの下の階層のファイル及びフォルダの名前*/);

				if (nowSelectFileName_ != copySelectPath)
				{
					fileFullPaht_ /= nowSelectFileName_;
					copySelectPath = nowSelectFileName_;

					// フォルダの場合
					if (std::filesystem::is_directory(fileFullPaht_))
					{
						//std::filesystem::path path(fileFullPaht_);
						//auto directoryName = (--path.end())->u8string();
						directoryName = fileFullPaht_.filename().u8string();
						
						nowSelect->fileMap_.try_emplace(directoryName.c_str(), FileData(nowSelect, directoryName.c_str()));
						nowSelect = &nowSelect->fileMap_.at(directoryName.c_str());
					}
					// ファイルの場合
					else
					{
						if (IsHeaderFile(fileFullPaht_.string(), "cpp")||
							IsHeaderFile(fileFullPaht_.string(), "h")||
							IsHeaderFile(fileFullPaht_.string(), "txt"))
						{
							AppOpen();
						}
					}
				}
			}

			

		}

		ImGui::End();
	}

	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	ImGui::SetNextWindowSize(ImVec2(50, 10), ImGuiCond_Once);
	if (ImGui::Begin("FileNameLog"))
	{
		if (!std::filesystem::is_directory(StringToWideString(nowSelectFileName_)))
		{
			// ウィンドウの背景色を設定
			ImGui::Text(nowSelectFileName_.c_str());
		}
		ImGui::End();
	}
	ImGui::PopStyleColor();
}

void Fream_FileDialog::Recovery(FileData* selectData)
{
	if (selectData->parentFile_ != NULL)
	{
		Recovery(selectData->parentFile_);
		fileFullPaht_ += selectData->myName + "\\";
		ImGui::SameLine();
		ImGui::Button("/");
	}
	ImGui::SameLine();
	if (ImGui::Button(selectData->myName.c_str()))
	{
		nowSelect = selectData;
		//fileFullPaht_ += selectData->myName + "/";
		return;
	}
}

void Fream_FileDialog::MakeFileImage(std::string name)
{
	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(90, 100);
	float buttonSpacing = 10.0f; // ボタン間のスペース

	ImGui::BeginGroup();

	// ユニークなIDを作成
	ImGuiID buttonID = ImGui::GetID(name.c_str());

	// ボタンを生成
	ImGui::PushID(buttonID);
	bool buttonPressed = ImGui::ImageButton((void*)my_shaderData, buttonSize);
	ImGui::PopID();

	if (buttonPressed)
	{
		// ボタンが押されたときの処理
		nowSelectFileName_ = name.c_str();
	}

	// テキストの幅を計算
	std::string fileName = name;
	ImVec2 textSize = ImGui::CalcTextSize(fileName.c_str());

	// ボタンの幅と文字列の長さを比較し、ボタンの幅を超える場合は文字列を切り詰めて "..." を追加
	if (textSize.x > buttonSize.x)
	{
		while (textSize.x > buttonSize.x && fileName.length() > 0)
		{
			fileName.pop_back();
			// テキストのサイズを毎回更新する
			textSize = ImGui::CalcTextSize(fileName.c_str());
		}
		fileName += "...";
		// 合成後のサイズを取得
		textSize = ImGui::CalcTextSize(fileName.c_str());
	}

	float textWidth = textSize.x;
	// テキストの中央揃えを行うためにカーソルの位置を調整
	float posX = (buttonSize.x - textWidth) * 0.5f;
	ImGui::SetCursorPosX(posX + buttonPos.x);
	//ImGui::Text(fileName.c_str());
	if (!CharacterSearch(fileName, "cpp", { 0,1,1,1 }, name)&&
		!CharacterSearch(fileName, "h", { 1,0,1,1 }, name))
	{
		ImGui::Text(fileName.c_str());
	}
	ImGui::EndGroup();

	// ボタンの右端がウィンドウの幅を超える場合は改行する
	if (buttonPos.x + buttonSize.x + buttonSpacing <= ImGui::GetWindowWidth() - 100)
	{
		ImGui::SameLine();
	}
}

void Fream_FileDialog::CreateImage(std::string filePath)
{
	
	// 画像の読み込み
	auto defaultImageSize_ = Vector2();
	//bool ret = LoadBackGroundTextureFromFile(&my_shaderData, &defaultImageSize_.x_, &defaultImageSize_.y_);
	bool ret = false; 
	
	if (my_shaderData_vector_.size() != NULL)
	{
		ret = LoadTextureFromFile(
			filePath.c_str(),// 画像ファイルの場所
			&my_shaderData_vector_.at(my_shaderData_vector_.size() - 1),	// シェーダのデータ作成用の変数
			&defaultImageSize_.x_,// 画像のサイズ取得変数
			&defaultImageSize_.y_);

		if (!ret)
		{
			my_shaderData_vector_.at(my_shaderData_vector_.size() - 1) = NULL;
		}

		MakeFileImage("fileName");
	}

	
}

void Fream_FileDialog::AppOpen()
{
	std::wstring commandLine;

	// コマンドライン引数（開くソリューションファイルのパスなど）
	std::wstring commandLineArgs = fileFullPaht_.wstring();

	// アプリの実行可能ファイルパス
	//const wchar_t* programPath = L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\Common7\\IDE\\devenv.exe";
	std::wstring programPath = GetAssociatedApplicationPath(fileFullPaht_.extension().wstring());

	// プロセス情報構造体の初期化
	STARTUPINFOW startupInfo = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION processInfo;

	if (fileFullPaht_.extension() == ".cpp"|| fileFullPaht_.extension() == ".h")
	{
		commandLine = programPath;
		commandLine += L" /edit ";
		commandLine += commandLineArgs;

		programPath.clear();
	}
	else if(fileFullPaht_.extension() == ".txt")
	{
		//commandLine = commandLineArgs;
		commandLine = programPath;
		commandLine += L"  ";
		commandLine += commandLineArgs;

		programPath.clear();
	}

	LPCWSTR appcation = NULL;

	if (programPath == L"")
	{
		appcation = NULL;
	}
	else
	{
		appcation = programPath.c_str();
	}


	BOOL result = CreateProcessW(
		appcation,		// 実行するプログラムのパス
		&commandLine[0],			// コマンドライン引数
		NULL,                       // セキュリティ属性
		NULL,                       // スレッドのセキュリティ属性
		FALSE,                      // ハンドルの継承オプション
		0,                          // 作成フラグ
		NULL,                       // 環境変数ブロック
		NULL,                       // カレントディレクトリ
		&startupInfo,               // スタートアップ情報
		&processInfo                // プロセス情報
	);

	if (result) {
		// プロセスが正常に起動した場合の処理
		//std::cout << "Visual Studioが起動しました。" << std::endl;

		// プロセスの終了を待つ場合は、WaitForSingleObject関数などを使用
		// WaitForSingleObject(processInfo.hProcess, INFINITE);

		// プロセスハンドルの解放
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	else {
		// プロセス起動が失敗した場合の処理
		DWORD errorCode = GetLastError();
		std::cout << "Visual Studioの起動に失敗しました。エラーコード: " << errorCode << std::endl;
	}

}

// std::stringをwchar_t*に変換する関数
std::wstring Fream_FileDialog::ConvertToWideString(const std::string& str) 
{
	int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::vector<wchar_t> buffer(length);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.data(), length);
	return std::wstring(buffer.data());
}

std::wstring Fream_FileDialog::GetAssociatedApplicationPath(const std::wstring& extension)
{
	wchar_t progId[MAX_PATH];
	DWORD progIdSize = MAX_PATH;

	std::string messageContent;
	std::wstring errorMsg;
	std::wstring errorText;

	// 拡張子に関連付けられたプログラムIDを取得
	HRESULT result = AssocQueryStringW(ASSOCF_NONE, ASSOCSTR_EXECUTABLE, extension.c_str(), nullptr, progId, &progIdSize);
	if (result == S_OK)
	{
		// プログラムIDから実行可能ファイルパスを取得
		wchar_t executablePath[MAX_PATH];
		DWORD executablePathSize = MAX_PATH;

		result = AssocQueryStringW(ASSOCF_NONE, ASSOCSTR_EXECUTABLE, progId, nullptr, executablePath, &executablePathSize);
		if (result == S_OK)
		{
			return progId;
		}
		else
		{
			messageContent = "progIDの実行可能パスの取得に失敗しました。:";
			errorMsg = progId;
			errorText = StringToWideString(messageContent) + errorMsg;
			MessageBox(NULL, (LPCSTR)(errorText.c_str()), "error", MB_OK);
		}
	}
	else
	{
		messageContent = "拡張子のProgIDの取得に失敗しました。:";
		errorMsg = extension.c_str();
		errorText = StringToWideString(messageContent) + errorMsg;
		MessageBox(NULL,(LPCSTR)(errorText.c_str()), "error", MB_OK);
	}

	MessageBox(NULL,"その他エラー","error",MB_OK);
	return L"その他エラー";
}

Fream_FileDialog::FileData::FileData(FileData* parent, std::string name)
{
	parentFile_ = parent;
	myName = name;
}

bool Fream_FileDialog::IsHeaderFile(const std::string& filename, const std::string& ext)
{	
	// 拡張子の位置を探す
	std::size_t dotPos = filename.find_last_of(".");
	if (dotPos == std::string::npos) {
		// ドットが見つからない場合は拡張子がないと判断
		return false;
	}

	// 拡張子を取得して比較する
	std::string extension = filename.substr(dotPos + 1);
	if (extension == ext) {
		return true;
	}

	return false;
}

bool Fream_FileDialog::IsMatch(const std::string& filepath, const std::string& target) {
	std::filesystem::path pathObj(filepath);
	std::string filename = pathObj.filename().string();

	// 大文字と小文字を区別せずに比較する場合
	/*if (filename == target) {
		return true;
	}*/

	// 大文字と小文字を区別して比較する場合
	// if (filename.compare(target) == 0) {
	//     return true;
	// }

	// ファイル名に指定した文字列が含まれているかを判定する
	if (filename.find(target) != std::string::npos) {
		return true;
	}

	return false;
}

std::wstring Fream_FileDialog::StringToWideString(const std::string& str)
{
	std::wstring wideStr;
	wideStr.resize(str.size());

	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(
		str.data(), str.data() + str.size(), wideStr.data()
	);

	return wideStr;
}