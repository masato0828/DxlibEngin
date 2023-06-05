#include "Fream_FileDialog_Tree.h"
#include "../../../imGui/imgui.h"
#include "../../../imGui/ImGuiFileDialog.h"
#include "../../../imGui/CustomFont.h"
#include "../../Common/FileDialog.h"
#include "../../../imGui/imgui_internal.h"
#include "../../Common/ImGuiMyCustom.h"
#include "../../Common/Utility.h"


Fream_FileDialog_Tree::Fream_FileDialog_Tree()
{
}

Fream_FileDialog_Tree::~Fream_FileDialog_Tree()
{
}

void Fream_FileDialog_Tree::Init()
{
}

void Fream_FileDialog_Tree::Update()
{
}

void Fream_FileDialog_Tree::Update(
	FileData& fileData,
	FileData*& nowSelect,
	std::filesystem::path& nowSelectPath,
	std::wstring& nowSelectFileName)
{
	nowSelectPath_ = nowSelectPath;
	nowSelectFileName_ = nowSelectFileName;

	if (ImGui::Begin("FileTree"))
	{
		//現在のカンレントディレクトリを取得
		auto directory = std::filesystem::current_path();

		ImGuiTreeNodeFlags node_flags =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		if (&fileData == nowSelect)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (nowSelect->parentFile_ != NULL)
		{
			if (IsMatch(directory.string(), Utility::StringToWideString(nowSelect->parentFile_->myName)))
			{
				// ツリーを開く
				ImGui::SetNextItemOpen(true);
			}
		}

		bool is_open = ImGui::TreeNodeEx((void*)&fileData, node_flags, fileData.myName.c_str());

		// ツリーを選択
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// 現在選択中
			nowSelect = &fileData;
		}
		if (is_open)
		{
			Tree(directory, fileData,nowSelect);
			ImGui::TreePop();
		}

		ImGui::End();
	}

	nowSelectPath = nowSelectPath_;
	nowSelectFileName = nowSelectFileName_;
}

void Fream_FileDialog_Tree::Tree(std::filesystem::path directory, FileData& fileData, FileData*& nowSelect)
{
	// イテレータの作成
	std::filesystem::directory_iterator itr(Utility::WideStringToString((Utility::StringToWideString(directory.string()))));
	std::filesystem::directory_iterator end{};

	for (; itr != end; ++itr)
	{
		// ツリーデータの設定
		ImGuiTreeNodeFlags node_flags = 0;

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
			Tree(itr->path(), data->second,nowSelect);

			// ツリーの終了
			ImGui::TreePop();
		}
	}


}

bool Fream_FileDialog_Tree::IsMatch(const std::string& filepath, const std::wstring& target)
{
	std::filesystem::path pathObj(filepath);
	std::string filename = pathObj.filename().string();

	if (target.empty())
	{
		return false;
	}

	// 大文字と小文字を区別せずに比較する場合
	/*if (filename == target) {
		return true;
	}*/

	// 大文字と小文字を区別して比較する場合
	// if (filename.compare(target) == 0) {
	//     return true;
	// }

	// ファイル名に指定した文字列が含まれているかを判定する
	if (filename.find(Utility::WideStringToString(target)) != std::string::npos) {
		return true;
	}

	return false;
}