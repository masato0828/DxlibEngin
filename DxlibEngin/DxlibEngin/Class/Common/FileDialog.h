#include <filesystem>
#include <string>
#include<Windows.h>

struct OpenFileDialog
{
	std::string operator()(HWND hWnd = nullptr,std::string* title = nullptr,std::string* filter = nullptr)
	{
		//現在のカンレントディレクトリを取得
		auto directory = std::filesystem::current_path();

		//ファイルを開く為の設定用構造体
		OPENFILENAME fileStruct{};
		//指定ファイルの絶対パスが代入される
		std::string filePass;

		filePass.resize(MAX_PATH);

		//自分自身のサイズ
		fileStruct.lStructSize = sizeof(OPENFILENAME);
		//親ウィンドウのハンドル
		fileStruct.hwndOwner = hWnd;
		//表示させるファイルのフィルタリング
		if (filter == nullptr)
		{
			fileStruct.lpstrFilter = nullptr;
		}
		else
		{
			fileStruct.lpstrFilter = filter->c_str();
		}

		//パスを代入する配列
		fileStruct.lpstrFile = &filePass.at(0);
		//パスの最大文字数
		fileStruct.nMaxFile = MAX_PATH;
		//ウィンドウの名前
		if (title == nullptr)
		{
			fileStruct.lpstrTitle = nullptr;
		}
		else
		{
			fileStruct.lpstrTitle = title->c_str();
		}

		//ファイルオープンに失敗したらreturn
		if (GetOpenFileName(&fileStruct) == 0)//カレントディレクトリがずれてしまう
		{
			return "";
		}

		//元のカレントディレクトリに戻す
		std::filesystem::current_path(directory);

		return filePass;
	}
};

struct FilePathErase
{
	std::string operator()(std::string fileNameFull)
	{
		int eraseNum = static_cast<int>(fileNameFull.find_last_of('\\'));
		//// 
		std::string file = fileNameFull.erase(0, eraseNum);
		//// 
		std::string fileStrName = fileNameFull.erase(0, 1);

		return fileStrName;
	}
};



