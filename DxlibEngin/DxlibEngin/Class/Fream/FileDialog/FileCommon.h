#pragma once
#include <string>
#include <unordered_map>

struct FileData
{
    //FileData(FileData*, std::string); 
    FileData(FileData* parent, std::wstring name)
    {
        parentFile_ = parent;
        myName = name;
    }
    std::unordered_map<std::string, FileData> fileMap_;
    FileData* parentFile_;
    std::wstring myName;
};