#pragma once
#include <string>
#include <filesystem>

#include "../../nlohmann/json.hpp"



class System_FileCreate
{
public:
	System_FileCreate();
	~System_FileCreate();
	void CreateFilesInDirectory(const std::wstring& filePath);
	void Is_CreateFile(const std::wstring& directoryPath);

	void IsMainFile();

	bool GetAppOpenFlg();
private:
	std::filesystem::path CreateDirectoryFromFileDialog();

	bool SeachFolder();

	void InputFolderName(std::wstring& text,bool& seacthFolderOpen);

	bool CreateFolderCheck(const std::wstring folderPath);

	void DataCreate();

	void FileLoad(const std::wstring fileName);

	std::wstring basePath_;
	std::wstring userFileName_;

	bool fileDialog_open;

	std::wstring systemFullPath_;


	bool appOpenFlg_;

	std::wstring mainFilePath_;
};

