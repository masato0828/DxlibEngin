#include <filesystem>
#include <string>
#include<Windows.h>

struct OpenFileDialog
{
	std::filesystem::path operator()(HWND hWnd = nullptr,std::string* title = nullptr,std::string* filter = nullptr)
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


struct ExpoterFileDialog
{
	std::filesystem::path operator()(HWND hWnd = nullptr, std::string* fileName = nullptr)
	{
		OPENFILENAME			ofn{};
		static TCHAR            szPath[MAX_PATH];
		static std::string		filePass;
		filePass.resize(MAX_PATH);
		if (fileName == nullptr)
		{
			filePass = (LPSTR)"outputfile.json";
		}
		else
		{
			filePass = &fileName->at(0);       // 選択ファイル格納
		}

		if (szPath[0] == TEXT('\0')) {
			GetCurrentDirectory(MAX_PATH, szPath);
		}
		if (ofn.lStructSize == 0) {
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrInitialDir = szPath;       // 初期フォルダ位置
			//ofn.lpstrFile = szFile;       // 選択ファイル格納
			ofn.lpstrFile = &filePass.at(0);       // 選択ファイル格納
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = TEXT(".json");
			ofn.lpstrFilter = TEXT("jsonファイル(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0");
			ofn.lpstrTitle = TEXT("ファイル名を付けて保存");
			ofn.Flags = OFN_OVERWRITEPROMPT;
		}
		if (GetSaveFileName(&ofn)) {
			MessageBox(hWnd, &filePass.at(0), TEXT("ファイル名を付けて保存"), MB_OK);
		}
		return &filePass.at(0);
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



