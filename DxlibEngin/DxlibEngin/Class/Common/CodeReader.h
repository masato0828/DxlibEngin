#pragma once
#include <filesystem>
class CodeReader
{
public:
	CodeReader();
	~CodeReader();

	/// <summary>
	/// コードの読み込み
	/// </summary>
	/// <param name="filePath">読み込みファイルのパス</param>
	void Read(std::filesystem::path filePath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
private:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

};

