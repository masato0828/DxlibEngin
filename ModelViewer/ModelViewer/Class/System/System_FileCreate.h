#pragma once
#include <string>
class System_FileCreate
{
public:
	System_FileCreate();
	~System_FileCreate();
	void CreateFilesInDirectory(const std::wstring& filePath);
	void CreateFile(const std::wstring& directoryPath);

	bool IsMainFile();
private:
	std::wstring basePath_;
	std::wstring userFileName_;
};

