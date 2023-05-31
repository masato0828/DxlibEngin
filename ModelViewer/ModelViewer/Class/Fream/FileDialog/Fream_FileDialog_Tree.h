#pragma once
#include "../FreamBase.h"
#include "FileCommon.h"
#include <filesystem>

class Fream_FileDialog_Tree :
    public FreamBase
{
public:
	Fream_FileDialog_Tree();
	~Fream_FileDialog_Tree();

	void Init() override;
	void Update() override;
	void Update(
		FileData& fileData,
		FileData* nowSelect,
		std::filesystem::path nowSelectPath,
		std::wstring nowSelectFileName);

private:

	std::filesystem::path nowSelectPath_;

	std::wstring nowSelectFileName_;

	void Tree(std::filesystem::path directory, FileData& fileData, FileData* nowSelect);

	bool IsMatch(const std::string& filepath, const std::wstring& target);

};

