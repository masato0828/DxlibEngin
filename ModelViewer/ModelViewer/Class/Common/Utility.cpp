#include "Utility.h"
#include <iostream>
#include <DxLib.h>
#include <shlwapi.h>

// 度からラジアン角度に
float Deg2Rad(float deg)
{
    return (deg * (DX_PI_F / 180.0f));
}

// 最大公約数を求める
int Gcd(int x, int y)
{
	if (y == 0)
	{
		return x;
	}
	return Gcd(y, x % y);
}


std::wstring StringToWideString(const std::string& str)
{
	std::wstring wideStr;
	wideStr.resize(str.size());

	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(
		str.data(), str.data() + str.size(), wideStr.data()
	);

	return wideStr;
}

std::string WideStringToString(const std::wstring& wideString)
{
	return std::string(wideString.begin(), wideString.end());
}

std::wstring GetDefaultApplication(const std::wstring& extension)
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

bool LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath)
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

bool OpenWithDefaultApplication(const std::wstring& filePath)
{
	std::wstring extension = filePath.substr(filePath.find_last_of(L"."));
	std::wstring defaultApplication = GetDefaultApplication(extension);

	if (!defaultApplication.empty()) {
		// 規定のアプリケーションが設定されている場合、それを起動する
		return LaunchApplication(defaultApplication, filePath);
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
}

bool IsHeaderFile(const std::string& filename, const std::string& ext)
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

bool CharacterSearch(std::string showName, std::string searchFileName, ImGuiCustom::IM_COLOR color, std::string defaultTarget)
{
	if (IsHeaderFile(defaultTarget, searchFileName))
		//if (defaultTarget.find(searchFileName) != std::string::npos)
	{
		ImGui::TextColored(ImVec4(color.red, color.green, color.blue, color.alpha), showName.c_str());
		return true;
	}
	return false;
}


