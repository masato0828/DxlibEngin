#include "Fream_FileDialog_Item.h"
#include "../../../imGui/imgui.h"
#include "../../../imGui/ImGuiFileDialog.h"
#include "../../../imGui/CustomFont.h"
#include "../../Common/FileDialog.h"
#include "../../../imGui/imgui_internal.h"
#include "../../Common/ImGuiMyCustom.h"
#include "../../Common/Utility.h"

Fream_FileDialog_Item::Fream_FileDialog_Item()
{
	Init();
}

Fream_FileDialog_Item::~Fream_FileDialog_Item()
{
}

void Fream_FileDialog_Item::Init()
{
	button_click_ = false;
	contextMenuFlg_ = false;
	context_renameFlg_ = false;

	fileImageShaderDatas_ = {
		{"non",nullptr},
		{"cpp",nullptr},
		{"h",nullptr},
		{"hlsl",nullptr},
		{"hlsli",nullptr},
		{"txt",nullptr},
		{"hpp",nullptr},
		{"vs",nullptr},
		{"ps",nullptr},
		{"file",nullptr},
		{"test",nullptr},
	};



	int textureSizeX;
	int textureSizeY;
	ImguiSup::LoadBackGroundTextureFromFile(&fileImageShaderDatas_.at("non"),&textureSizeX,&textureSizeY);

	ImguiSup::LoadTextureFromFile("data/iconData/CPP.png", &fileImageShaderDatas_.at("cpp"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/h.png", &fileImageShaderDatas_.at("h"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/hlsl.png", &fileImageShaderDatas_.at("hlsl"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/hlsli.png", &fileImageShaderDatas_.at("hlsli"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/txt.png", &fileImageShaderDatas_.at("txt"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/file.png", &fileImageShaderDatas_.at("file"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/HPP.png", &fileImageShaderDatas_.at("hpp"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/vs.png", &fileImageShaderDatas_.at("vs"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/ps.png", &fileImageShaderDatas_.at("ps"), &textureSizeX, &textureSizeY);



	stage_ = std::make_unique<Fream_Stage>();

	CreateIcon("data/Bomber_2.mv1","test");
	
}
void Fream_FileDialog_Item::Update()
{
}

void Fream_FileDialog_Item::Update(FileData*& nowselect,std::filesystem::path& fileFullPath, std::wstring& nowSelectFile, std::wstring& nowSelectFileName)
{
	nowSelect_ = nowselect;
	fileFullPaht_ = fileFullPath;
	nowSelectFile_ = nowSelectFile;
	nowSelectFileName_ = nowSelectFileName;

	DokingWindow();

	FileNameWindow();

	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);

	if (ImGui::Begin("ITEM"))
	{
		//ImGui::Separator();
		if (nowSelect_ != nullptr)
		{
			FileLogWindow();

			auto current_path = std::filesystem::current_path();

			// イテレータの作成
			std::filesystem::directory_iterator itr(fileFullPaht_);
			std::filesystem::directory_iterator end{};
			std::wstring directoryName;

			auto copySelectPath = nowSelectFileName_;
			
			for (; itr != end; ++itr)
			{
				directoryName = itr->path().filename();

				MakeFileImage(itr->path()/*現在フォルダの下の階層のファイル及びフォルダの名前*/);

				if (nowSelectFileName_ != copySelectPath)
				{
					fileFullPaht_ /= nowSelectFileName_;
					copySelectPath = nowSelectFileName_;

					// フォルダの場合
					if (std::filesystem::is_directory(fileFullPaht_))
					{
						directoryName = fileFullPaht_.filename();

						nowSelect_->fileMap_.try_emplace(Utility::WideStringToString(directoryName), FileData(nowSelect_, Utility::WideStringToString(directoryName)));
						nowSelect_ = &nowSelect_->fileMap_.at(Utility::WideStringToString(directoryName));
					}
					// ファイルの場合
					else
					{
						Utility::OpenWithDefaultApplication(fileFullPaht_.wstring());
					}
				}
			}
		}
		
		if (contextMenuFlg_)
		{
			// ボタン上で右クリックされたときの処理
			ImGui::OpenPopup("ContextMenu");

			Popup();
		}

		if (context_renameFlg_)
		{
			RenameWindow();
		}


		ImGui::End();
	}

	
	fileFullPath = fileFullPaht_;
	nowSelectFile = nowSelectFile_;
	nowSelectFileName = nowSelectFileName_;
	nowselect = nowSelect_;
}

void Fream_FileDialog_Item::DokingWindow()
{
	if (ImGui::Begin("##ItemWindow"))
	{
		// 強制ドッキング設定
		ImGuiID dockspace_id = ImGui::GetID("ItemDock");
		if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
		{
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

			ImGuiID dock_main_id = dockspace_id;
			ImGuiID dock_id_FileDialog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.01, NULL, &dock_main_id);
			ImGuiID dock_id_FileLog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.01, NULL, &dock_main_id);

			ImGui::DockBuilderSetNodeSize(dock_main_id, ImVec2(300, 400));
			ImGui::DockBuilderSetNodeSize(dock_id_FileDialog, ImVec2(ImGui::GetWindowWidth(), ImGui::GetTextLineHeight()*2));
			ImGui::DockBuilderSetNodeSize(dock_id_FileLog, ImVec2(ImGui::GetWindowWidth(), ImGui::GetTextLineHeight()+10));

			ImGui::DockBuilderDockWindow("FileNameLog", dock_id_FileDialog);
			ImGui::DockBuilderDockWindow("FileLog", dock_id_FileLog);
			ImGui::DockBuilderDockWindow("ITEM", dock_main_id);

			ImGui::DockBuilderFinish(dockspace_id);
		}
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		ImGui::End();
	}
}

void Fream_FileDialog_Item::FileNameWindow()
{
	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar| ImGuiDockNodeFlags_NoResizeFlagsMask_;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	if (ImGui::Begin("FileNameLog"))
	{
		// ウィンドウの背景色を設定
		ImGui::Text(Utility::WStringToUTF8(nowSelectFile_).c_str());
		ImGui::End();
	}
	ImGui::PopStyleColor();
}

void Fream_FileDialog_Item::FileLogWindow()
{
	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = 
		ImGuiDockNodeFlags_NoWindowMenuButton |
		ImGuiDockNodeFlags_NoTabBar | 
		ImGuiDockNodeFlags_NoResizeFlagsMask_
		;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::Begin("FileLog",nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

	ImGui::Separator();
	fileFullPaht_.clear();
	Recovery(nowSelect_);
	fileFullPaht_ = fileFullPaht_.empty() ? std::filesystem::current_path() :
		std::filesystem::current_path() /= fileFullPaht_;

	ImGui::End();
}

void Fream_FileDialog_Item::Recovery(FileData* selectData)
{
	if (selectData->parentFile_ != NULL)
	{
		Recovery(selectData->parentFile_);
		fileFullPaht_ += selectData->myName + "\\";
		ImGui::SameLine();
		ImGui::SmallButton("/");
	}
	ImGui::SameLine();
	// ボタンのサイズと文字の配置を設定する
	if (ImGui::SmallButton(selectData->myName.c_str()))
	{
		nowSelect_ = selectData;
		return;
	}

}

void Fream_FileDialog_Item::MakeFileImage(
	std::filesystem::path name)
{
	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(80, 80);
	float buttonSpacing = 10.0f; // ボタン間のスペース
	
	ImGui::BeginGroup();
	{
		// ユニークなIDを作成
		ImGuiID buttonID = ImGui::GetID(name.c_str());

		// ボタンを生成
		ImGui::PushID(buttonID);
		bool buttonPressed;

		FileAssignments(name, buttonPressed, {buttonSize.x,buttonSize.y});
		
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			// ボタンがダブルクリックされたときの処理
			nowSelectFileName_ = name.c_str();
		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			//if (nowSelectFile_ != name.c_str())
			{
				button_click_ = true;
			}
			// ボタンが押されたときの処理
			nowSelectFile_ = name.c_str();
			
		}
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
		{
			fileName_ = name.c_str();
			contextMenuFlg_ = true;
		}

		// 他の場所をクリックしたらコンテキストメニューを閉じる
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			if (!ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			{
				contextMenuFlg_ = false;
			}
		}

		ImGui::PopID();

		

		// テキストの幅を計算
		std::wstring fileName = name.filename();
		ImVec2 textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());

		// ボタンの幅と文字列の長さを比較し、ボタンの幅を超える場合は文字列を切り詰めて "..." を追加
		if (textSize.x > buttonSize.x)
		{
			while (textSize.x > buttonSize.x && fileName.length() > 0)
			{
				fileName.pop_back();
				// テキストのサイズを毎回更新する
				textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());
			}
			fileName += L"...";
			// 合成後のサイズを取得
			textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());
		}

		float textWidth = textSize.x;
		// テキストの中央揃えを行うためにカーソルの位置を調整
		float posX = (buttonSize.x - textWidth) * 0.5f;
		ImGui::SetCursorPosX(posX + buttonPos.x);
		//ImGui::Text(fileName.c_str());

		// 名前の色変更
		if (!Utility::CharacterSearch(Utility::WStringToUTF8(fileName).c_str(), "cpp", { 0,1,1,1 }, Utility::WideStringToString(name)) &&
			!Utility::CharacterSearch(Utility::WStringToUTF8(fileName).c_str(), "h", { 1,0,1,1 }, Utility::WideStringToString(name)))
		{
			ImGui::Text(Utility::WStringToUTF8(fileName).c_str());
		}

		ImGui::EndGroup();
	}


	// ボタンの右端がウィンドウの幅を超える場合は改行する
	if (buttonPos.x + buttonSize.x + buttonSpacing <= ImGui::GetWindowWidth() - 100)
	{
		ImGui::SameLine();
	}
}

void Fream_FileDialog_Item::FileAssignments(std::filesystem::path& name, bool& buttonPressed, Vector2Flt buttonSize)
{
	ImGuiCustom::SetCustomButtonStyle(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGuiCustom::SetCustomButtonStyle(ImGuiCol_ButtonHovered, ImVec4(0.5, 0.5, 0.7, 1));

	
	auto u8name = name.u8string();

	if (std::filesystem::is_directory(name))
	{
		buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at("file"), ImVec2(buttonSize.x_, buttonSize.y_));
	}
	else
	{
		auto ext = name.extension().u8string().substr(1);
		if (fileImageShaderDatas_.count(ext))
		{
			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(ext), ImVec2(buttonSize.x_, buttonSize.y_));
		}
		else if (fileImageShaderDatas_.count(u8name))
		{
			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(u8name), ImVec2(buttonSize.x_, buttonSize.y_));
		}
		else if (ext == "mv1")
		{
			fileImageShaderDatas_.emplace(u8name, nullptr);
			auto IconDataPath = fileFullPaht_ / name;
			CreateIcon(IconDataPath, u8name);

			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(u8name), ImVec2(buttonSize.x_, buttonSize.y_));
		}
		else
		{
			ImGuiCustom::SetCustomButtonStyle(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.4, 1));
			buttonPressed = ImGui::ImageButton((void*)my_shaderData, ImVec2(buttonSize.x_, buttonSize.y_));
		}
	}


	// カスタムスタイルを元に戻す
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();
}

void Fream_FileDialog_Item::RenameWindow()
{
	auto path = fileFullPaht_ /= fileName_;

	static char buffer[256] = "";

	// ImGuiのウィンドウを作成
	ImGui::Begin(u8"文字入力");

	ImGui::Text(u8"新しい名前の入力");
	// ImGuiのInputTextを使用してテキスト入力を処理
	ImGui::InputText("##テキスト入力", buffer, sizeof(buffer));
	ImGui::SameLine();
	ImGuiCustom::HelpMarker(
	u8"すいません日本語入力対応してないです、なるべく実装できるようにします"
	);

	if (ImGui::Button(u8"変更の確定"))
	{
		// 入力文字列が変更された場合の処理を行う
		std::string newFileName = buffer;

		// ファイル名の変更処理を実行する
		bool success = Utility::RenameFile(path.u8string(), newFileName);
		if (success)
		{
			// ファイル名の変更が成功した場合の処理を行う
			MessageBox(NULL,"ファイル名の変更に成功しました。","ファイル名の変更",MB_OK);
			context_renameFlg_ = false;
		}
		else
		{
			// ファイル名の変更が失敗した場合の処理を行う
			MessageBox(NULL, "ファイル名の変更に失敗しました。", "ファイル名の変更", MB_OK);
			context_renameFlg_ = false;
		}
	}
	
	// ImGuiのウィンドウの終了
	ImGui::End();
}

bool Fream_FileDialog_Item::SettingIcon(std::wstring& name, bool& buttonPressed, Vector2Flt buttonSize, std::string ext)
{
	if (Utility::IsHeaderFile(Utility::WStringToUTF8(name).c_str(),ext))
	{
		buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(ext), ImVec2(buttonSize.x_, buttonSize.y_));
		return true;
	}

	return false;
}

bool Fream_FileDialog_Item::CreateIcon(std::filesystem::path path, std::string key)
{
	std::wstring wstrPath = path;
	
	auto model = MV1LoadModel(Utility::WStringToUTF8(wstrPath).c_str());

	MV1_REF_POLYGONLIST RefPoly;

	// モデルの全フレームのポリゴンの情報を取得
	RefPoly = MV1GetReferenceMesh(model, -1, TRUE);

	RefPoly.MaxPosition;
	RefPoly.MaxPosition;

	/*for ()
	{
		if (CheckCameraViewClip(RefPoly.MaxPosition) || CheckCameraViewClip(RefPoly.MaxPosition))
		{

		}
	}*/


	// アンチエイリアシング処理
	SetCreateDrawValidGraphMultiSample(4, 4);
	// 20x20サイズのアルファチャンネルなしの描画可能画像を作成する
	auto handle = MakeScreen(256, 256, true);

	// 作成した画像を描画対象にする
	SetDrawScreen(handle);

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	// カメラのクリッピング距離を設定
	SetCameraNearFar(10.0f, 300000.0f);
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraPositionAndAngle(VGet(-293.486, 185, -279.488), 0.264, 0.808, 0.0f);

	DrawBox(0, 0, 1000, 1000, 0xffffff, true);
	stage_->Update();
	stage_->Draw();
	MV1SetScale(model, VGet(1, 1, 1));
	MV1DrawModel(model);

	int textureSizeX;
	int textureSizeY;
	ImguiSup::LoadTextureFromFile(handle, &fileImageShaderDatas_.at(key), &textureSizeX, &textureSizeY);

	MV1DeleteModel(model);
	DeleteGraph(handle);
	return false;
}

bool& Fream_FileDialog_Item::GetButton_Click()
{
	return button_click_;
}

void Fream_FileDialog_Item::Popup()
{
	if (ImGui::BeginPopup("ContextMenu"))
	{
		// コンテキストメニューの内容をここに記述

		if (ImGui::MenuItem(u8"切り取り")) // ユニークなIDを指定する
		{
			// オプション1が選択されたときの処理
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"コピー")) // ユニークなIDを指定する
		{
			// オプション2が選択されたときの処理
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"貼り付け")) // ユニークなIDを指定する
		{
			// オプション2が選択されたときの処理
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"名前の変更")) // ユニークなIDを指定する
		{
			context_renameFlg_ = true;
			// オプション2が選択されたときの処理
			contextMenuFlg_ = false;
		}
		ImGui::EndPopup();
	}
}
