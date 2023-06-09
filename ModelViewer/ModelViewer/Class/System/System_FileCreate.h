#pragma once
#include <string>
#include <filesystem>
class System_FileCreate
{
public:
	System_FileCreate();
	~System_FileCreate();
	void CreateFilesInDirectory(const std::wstring& filePath);
	void Is_CreateFile(const std::wstring& directoryPath);

	bool IsMainFile();
private:
	std::filesystem::path CreateDirectoryFromFileDialog();

	bool SeachFolder();

	void InputFolderName(std::wstring& text,bool& seacthFolderOpen);

	std::wstring basePath_;
	std::wstring userFileName_;

	bool fileDialog_open;

	std::wstring systemFullPath_;
};

