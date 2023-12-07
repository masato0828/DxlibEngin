#include "Utility.h"
#include <iostream>
#include <DxLib.h>
#include <shlwapi.h>
#include <fstream>
#include <regex>
#include <Shlwapi.h>

// 度からラジアン角度に
float Utility::Deg2Rad(float deg)
{
    return (deg * (DX_PI_F / 180.0f));
}

float Utility::Rad2DegF(float rad)
{
	return rad * (180.0f / DX_PI_F);
}

// 最大公約数を求める
int Utility::Gcd(int x, int y)
{
	if (y == 0)
	{
		return x;
	}
	return Gcd(y, x % y);
}


std::wstring Utility::StringToWideString(const std::string& str)
{
	std::wstring wideStr;
	wideStr.resize(str.size());

	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(
		str.data(), str.data() + str.size(), wideStr.data()
	);

	return wideStr;
}

std::string Utility::WideStringToString(const std::wstring& wideString)
{
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (bufferSize == 0) {
		// エラーハンドリング
		return std::string();
	}

	std::string result(bufferSize, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, &result[0], bufferSize, nullptr, nullptr);

	return result;
}

std::string Utility::WStringToUTF8(const std::wstring& wstr)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string utf8Str(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], size, nullptr, nullptr);
	return utf8Str;
}

std::wstring Utility::MultiByteToUnicode(const std::string& mbStr)
{
	// マルチバイト文字列のサイズを取得
	int size = MultiByteToWideChar(CP_ACP, 0, mbStr.c_str(), -1, nullptr, 0);

	// サイズに基づいて Unicode 文字列を格納するための wstring を作成
	std::wstring unicodeStr(size, 0);

	// マルチバイト文字列を Unicode 文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mbStr.c_str(), -1, &unicodeStr[0], size);

	// 変換された Unicode 文字列を返す
	return unicodeStr;
}

std::wstring Utility::GetDefaultApplication(const std::wstring& extension)
{
	wchar_t progId[MAX_PATH];
	DWORD progIdSize = MAX_PATH;

	std::wstring messageContent;
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
			//MessageBox(NULL, "progIDの実行可能パスの取得に失敗しました。:", "error", MB_OK);
			return L"";
		}
	}
	else
	{
		//MessageBox(NULL," 拡張子のProgIDの取得に失敗しました。:", "error", MB_OK);
		return L"";
	}

	return L"";
}

bool Utility::LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath)
{
	std::wstring commandLine = L"\"";
	commandLine += applicationPath;
	commandLine += L"\" \"";
	commandLine += filePath;
	commandLine += L"\"";

	STARTUPINFOW startupInfo = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION processInfo;

	BOOL result = CreateProcessW(
		nullptr,
		&commandLine[0],
		nullptr,
		nullptr,
		FALSE,
		0,
		nullptr,
		nullptr,
		&startupInfo,
		&processInfo
	);

	if (result) {
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		return true;
	}
	else {
		return false;
	}
}

bool Utility::OpenWithDefaultApplication(const std::wstring& filePath)
{
	std::wstring extension = filePath.substr(filePath.find_last_of(L"."));
	std::wstring defaultApplication = Utility::GetDefaultApplication(extension);

	if (!defaultApplication.empty()) {
		// 規定のアプリケーションが設定されている場合、それを起動する
		return Utility::LaunchApplication(defaultApplication, filePath);
	}
	else {
		// ファイルを開く方法を選ぶためのダイアログを表示
		// デフォで設定されているアプリがあればそれを起動
		int result = (int)ShellExecuteW(NULL, L"open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);

		if (result <= 32)
		{
			// エラーが発生した場合の処理
			DWORD error = GetLastError();
			// エラーコードを使ってエラーメッセージを取得するなどの処理を行う
		}
	}

	return false;
}

bool Utility::ComparisonExtensionFile(const std::wstring& filename, const std::wstring& ext)
{
	// 拡張子の位置を探す
	std::size_t dotPos = filename.find_last_of(L".");
	if (dotPos == std::string::npos) {
		// ドットが見つからない場合は拡張子がないと判断
		return false;
	}

	// 拡張子を取得して比較する
	std::wstring extension = filename.substr(dotPos + 1);
	if (extension == ext) {
		return true;
	}

	return false;
}

bool Utility::CharacterSearch(std::string showName, std::wstring searchFileName, ImGuiCustom::IM_COLOR color, std::wstring defaultTarget)
{
	if (Utility::ComparisonExtensionFile(defaultTarget, searchFileName))
	{
		ImGui::TextColored(ImVec4(color.red, color.green, color.blue, color.alpha), showName.c_str());
		return true;
	}
	return false;
}

// maltbyteのエンコーディングを判別する関数
std::string Utility::DetectMaltbyteEncoding(const std::string& maltbyte)
{
	// UTF-8判別のためのバイトパターン
	std::string utf8Pattern = "\xEF\xBB\xBF";

	// UTF-16 (LE) 判別のためのバイトパターン
	std::string utf16LEPattern = "\xFF\xFE";

	// UTF-16 (BE) 判別のためのバイトパターン
	std::string utf16BEPattern = "\xFE\xFF";

	// Shift-JIS 判別のためのバイトパターン
	std::string shiftJISPattern = "\x82\xA0-\x82\xF1\x83\x40-\x83\x96\x83\x9F-\x83\xB6\x83\xBF-\x83\xD6\x83\xF5-\x83\xFC";

	// maltbyteに含まれるバイトパターンをチェックしてエンコーディングを判別する
	if (maltbyte.compare(0, utf8Pattern.size(), utf8Pattern) == 0)
	{
		return "UTF-8";
	}
	else if (maltbyte.compare(0, utf16LEPattern.size(), utf16LEPattern) == 0)
	{
		return "UTF-16 (LE)";
	}
	else if (maltbyte.compare(0, utf16BEPattern.size(), utf16BEPattern) == 0)
	{
		return "UTF-16 (BE)";
	}
	else if(std::regex_search(maltbyte, std::regex(shiftJISPattern)))
	{
		return "SHIFT-JIS";
	}


	return "Unknown";
}

bool Utility::RenameFile(const std::string& filePath, const std::string& newFileName)
{
	// 現在のファイルパスと新しいファイル名から新しいファイルパスを作成
	std::string newFilePath = filePath;
	size_t lastSlash = newFilePath.find_last_of("/\\");
	if (lastSlash != std::string::npos)
	{
		newFilePath = newFilePath.substr(0, lastSlash + 1) + newFileName;
	}
	else
	{
		newFilePath = newFileName;
	}

	// ファイルの名前を変更する
	if (std::rename(filePath.c_str(), newFilePath.c_str()) != 0)
	{
		return false;
	}

	// ファイル名の変更に成功した場合の処理
	return true;
}

bool Utility::IsMatch(const std::string& filepath, const std::wstring& target)
{
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
		if (filename.find(Utility::WStringToUTF8(target)) != std::string::npos) {
			return true;
		}

		return false;
	}
}

std::string Utility::StringToInt(const int& val)
{
	return std::to_string(val);
}

int Utility::IntToString(const std::string& str)
{
	return stoi(str);
}
