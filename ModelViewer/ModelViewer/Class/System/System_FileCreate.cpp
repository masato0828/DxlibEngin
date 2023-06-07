#include "System_FileCreate.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../../imGui/imgui.h"

System_FileCreate::System_FileCreate()
{
    // ファイル名
    basePath_ = L"D:/UseEngin";
    
    std::ifstream file(basePath_);
    if (file.is_open())
    {
        return;
    }
    else
    {
        userFileName_ = L"/BaseDirectory";

        std::filesystem::create_directories(basePath_);
        std::wstring systemPath = basePath_ +userFileName_;
        std::filesystem::create_directory(systemPath);
        
        return;
    }
}

System_FileCreate::~System_FileCreate()
{
}

void System_FileCreate::CreateFile(const std::wstring& filePath)
{
    std::ofstream file(filePath);
    if (file)
    {
        // ファイルの作成に成功した場合の処理
       // std::cout << "File created: " << filePath << std::endl;
    }
    else
    {
        // ファイルの作成に失敗した場合の処理
        //std::cout << "Failed to create file: " << filePath << std::endl;
    }
}

bool System_FileCreate::IsMainFile()
{
    ImGui::Begin(u8"データの読み込み");



    ImGui::End();


    //// 既存のデータまたはデータの作成を行うかを聞く
    //if (true)
    //{
    //    // 新規の場合どこに作るかを聞く
    //    // どこに作るか
    //    CreateFilesInDirectory(basePath_);
    //}
    //else
    //{
    //    // 既存のデータの読み込み処理
    //}

    return true;
}

void System_FileCreate::CreateFilesInDirectory(const std::wstring& directoryPath)
{
    std::filesystem::create_directories(directoryPath);

    // MainFile
    std::wstring mainFilePath = directoryPath + L"/MainFile";
    CreateFile(mainFilePath);

    // System
    std::wstring systemPath = directoryPath + L"/System";
    std::filesystem::create_directory(systemPath);
    std::wstring systemFilePath = systemPath + L"/System.txt";
    CreateFile(systemFilePath);

    // Data
    std::wstring dataPath = directoryPath + L"/Data";
    std::filesystem::create_directory(dataPath);
    std::wstring dataFilePath = dataPath + L"/Data.json";
    CreateFile(dataFilePath);

    // Assets
    std::wstring assetsPath = directoryPath + L"/Assets";
    std::filesystem::create_directory(assetsPath);
    std::wstring assetsFilePath = assetsPath + L"/Assets.txt";
    CreateFile(assetsFilePath);
}
