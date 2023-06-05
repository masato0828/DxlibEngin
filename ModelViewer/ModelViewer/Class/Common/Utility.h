#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include "../Common/ImGuiMyCustom.h"


namespace Utility
{
	// 度からラジアン角度に
	float Deg2Rad(float deg);

	// 最大公約数を求める
	int Gcd(int x, int y);

	std::wstring StringToWideString(const std::string& str);

	std::string WideStringToString(const std::wstring& wideString);

	std::string WStringToUTF8(const std::wstring& wstr);

	// マルチバイト文字列 mbStr を std::wstring
	std::wstring MultiByteToUnicode(const std::string& mbStr);

	std::wstring GetDefaultApplication(const std::wstring& extension);

	bool OpenWithDefaultApplication(const std::wstring& filePath);

	bool LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath);

	bool IsHeaderFile(const std::string& filename, const std::string& ext);

	bool CharacterSearch(std::string showName, std::string searchFileName, ImGuiCustom::IM_COLOR color, std::string defaultTarget);

    // maltbyteのエンコーディングを判別する関数
	std::string DetectMaltbyteEncoding(const std::string& maltbyte);

	bool RenameFile(const std::filesystem::path& filePath, const std::filesystem::path& newFileName);

}
// vectorからindex番目の要素を削除する
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)
{
	vector.erase(vector.begin() + index);
}

