#pragma once
#include <filesystem>
class CodeReader
{
public:
	CodeReader();
	~CodeReader();

	void Read(std::filesystem::path filePath);
	void Update();
	void Draw();
private:
	void Init();

};

