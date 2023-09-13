#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include "../Common/ImGuiMyCustom.h"

// 色んな所で使用される
namespace Utility
{
	/// <summary>
	/// 度からラジアン角度に 
	/// </summary>
	/// <param name="deg">デグリー角度</param>
	/// <returns>ラジアン角度</returns>
	float Deg2Rad(float deg);

	/// <summary>
	/// 最大公約数を求める
	/// </summary>
	/// <param name="x">数値１</param>
	/// <param name="y">数値２</param>
	/// <returns>最大公約数</returns>
	int Gcd(int x, int y);

	/// <summary>
	/// string型からwstring型への変換
	/// </summary>
	/// <param name="str">string</param>
	/// <returns>変換後</returns>
	std::wstring StringToWideString(const std::string& str);

	/// <summary>
	/// wstring型からstring型への変換
	/// </summary>
	/// <param name="wideString">wstring</param>
	/// <returns>変換後</returns>
	std::string WideStringToString(const std::wstring& wideString);

	/// <summary>
	/// wstring型からu8string型への変換
	/// </summary>
	/// <param name="wstr">wstring</param>
	/// <returns>変換後</returns>
	std::string WStringToUTF8(const std::wstring& wstr);

	/// <summary>
	/// マルチバイト文字列をwstring型に変換
	/// </summary>
	/// <param name="mbStr">string</param>
	/// <returns>変換後</returns>
	std::wstring MultiByteToUnicode(const std::string& mbStr);

	/// <summary>
	/// 規定アプリのパス取得
	/// </summary>
	/// <param name="extension">動作させたいファイルの拡張子</param>
	/// <returns>規定アプリのパス</returns>
	std::wstring GetDefaultApplication(const std::wstring& extension);

	/// <summary>
	/// 既定のアプリを開く、既定のアプリが無ければ選ぶ
	/// </summary>
	/// <param name="filePath">アプリのパス</param>
	/// <returns>アプリを開けた：true　　エラー：false</returns>
	bool OpenWithDefaultApplication(const std::wstring& filePath);

	/// <summary>
	/// アプリを開く 
	/// </summary>
	/// <param name="applicationPath">アプリのパス</param>
	/// <param name="filePath">開きたいファイルのパス</param>
	/// <returns>成功：true　失敗：false</returns>
	bool LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath);

	/// <summary>
	/// 拡張子の比較
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	/// <param name="ext">比較したい拡張子</param>
	/// <returns>ファイル名と拡張子が一致してる：true  ファイル名と拡張子が一致してる：false</returns>
	bool ComparisonExtensionFile(const std::wstring& filename, const std::wstring& ext);

	/// <summary>
	/// 文字の拡張子が指定された拡張子であればテキストの色を変える
	/// </summary>
	/// <param name="showName">表示する名前</param>
	/// <param name="searchFileName">拡張子</param>
	/// <param name="color">変更色</param>
	/// <param name="defaultTarget">表示したい文字のフルパス</param>
	/// <returns></returns>
	bool CharacterSearch(std::string showName, std::wstring searchFileName, ImGuiCustom::IM_COLOR color, std::wstring defaultTarget);

	/// <summary>
	/// maltbyteのエンコーディングを判別する関数
	/// </summary>
	/// <param name="maltbyte">文字</param>
	/// <returns>マルチバイトのエンコーディング</returns>
	std::string DetectMaltbyteEncoding(const std::string& maltbyte);

	/// <summary>
	/// ファイル名の変更処理
	/// </summary>
	/// <param name="filePath">ファイル名</param>
	/// <param name="newFileName">新しい名前</param>
	/// <returns>成功：true　失敗：false</returns>
	bool RenameFile(const std::string& filePath, const std::string& newFileName);

	/// <summary>
	/// ファイル名が指定文字と一致しているかどうか
	/// </summary>
	/// <param name="filepath">ファイルパス</param>
	/// <param name="target">指定文字</param>
	/// <returns>一致：true　不一致：false</returns>
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


