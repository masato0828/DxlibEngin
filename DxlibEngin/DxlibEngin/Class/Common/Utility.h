#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <map>
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

	bool RenameFile(const std::string& filePath, const std::string& newFileName);

	bool IsMatch(const std::string& filepath, const std::wstring& target);
}
// vectorからindex番目の要素を削除する
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)
{
	vector.erase(vector.begin() + index);
}


// 指定された位置（インデックス）の要素を取得する関数
template<typename T,typename N>
std::pair<T, N> GetElementNum(const std::map<T, N>& myMap, size_t index) {
	if (index >= myMap.size()) {
		throw std::out_of_range("インデックスが範囲外です。");
	}

	auto it = myMap.begin();
	std::advance(it, index); // インデックスまでイテレータを進める

	return *it;
}

