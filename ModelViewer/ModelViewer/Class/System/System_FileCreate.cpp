#include "System_FileCreate.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <shobjidl.h>
#include "../Common/Utility.h"

#include "../../imGui/imgui.h"

System_FileCreate::System_FileCreate()
{
    basePath_ = L"D:/UseEngin";
    fileDialog_open = false;
    systemFullPath_ = basePath_;
}

System_FileCreate::~System_FileCreate()
{
}

void System_FileCreate::Is_CreateFile(const std::wstring& filePath)
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
    // フォルダ名


    ImGui::Begin(u8"データの読み込み");

    if(fileDialog_open)
    {
        basePath_ = L"UseEngin";
        // UseEnginフォルダがあるかどうか
        std::ifstream file(basePath_);
        if (file.is_open())
        {
            // UseEnginフォルダがある
            //return true;
        }
        else
        {
            // UseEnginフォルダがない
            // ファイルダイアログの表示
            auto result = CreateDirectoryFromFileDialog();

            if (result == "Null")
            {
                return false;
            }
           
            // UseEnginフォルダの作成
            result /= basePath_;
            std::filesystem::create_directories(result);
            systemFullPath_ = result;

            //return true;
        }
        

        fileDialog_open = false;
    }
    std::string utf8Str = Utility::WStringToUTF8(systemFullPath_);

    ImGui::Text(u8"ファイルの参照");
    char buffer[256]; // 適切なサイズに調整してください
    strncpy_s(buffer, sizeof(buffer), utf8Str.c_str(), _TRUNCATE);
    ImGui::InputText("##label", buffer, 1060);

    std::string spath = buffer;
    systemFullPath_ = Utility::StringToWideString(spath);
    ImGui::SameLine();
    if (ImGui::SmallButton(u8"参照"))
    {
        fileDialog_open = true;
    }

    ImGui::End();

    //再度開くときのため、
    //systemFullPath_　を取得しておかなければならない


    userFileName_ = L"/BaseDirectory";


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
    Is_CreateFile(mainFilePath);

    // System
    std::wstring systemPath = directoryPath + L"/System";
    std::filesystem::create_directory(systemPath);
    std::wstring systemFilePath = systemPath + L"/System.txt";
    Is_CreateFile(systemFilePath);

    // Data
    std::wstring dataPath = directoryPath + L"/Data";
    std::filesystem::create_directory(dataPath);
    std::wstring dataFilePath = dataPath + L"/Data.json";
    Is_CreateFile(dataFilePath);

    // Assets
    std::wstring assetsPath = directoryPath + L"/Assets";
    std::filesystem::create_directory(assetsPath);
    std::wstring assetsFilePath = assetsPath + L"/Assets.txt";
    Is_CreateFile(assetsFilePath);
}

std::filesystem::path System_FileCreate::CreateDirectoryFromFileDialog()
{
    // Common Item Dialogのインスタンスを作成
    IFileDialog* pFileDialog;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (FAILED(hr))
    {
        // エラーメッセージを表示するか、エラーログに記録するなどの処理を行う
        return "Null";
    }

    // ファイルダイアログをフォルダ選択モードに設定
    DWORD options;
    pFileDialog->GetOptions(&options);
    pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

    // ファイルダイアログを表示
    hr = pFileDialog->Show(NULL);
    if (FAILED(hr))
    {
        // エラーメッセージを表示するか、エラーログに記録するなどの処理を行う
        pFileDialog->Release();
        return "Null";
    }

    // 選択されたフォルダのパスを取得
    IShellItem* pShellItem;
    hr = pFileDialog->GetResult(&pShellItem);
    if (FAILED(hr))
    {
        // エラーメッセージを表示するか、エラーログに記録するなどの処理を行う
        pFileDialog->Release();
        return "Null";
    }

    PWSTR pszFolderPath;
    hr = pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
    if (FAILED(hr))
    {
        // エラーメッセージを表示するか、エラーログに記録するなどの処理を行う
        pShellItem->Release();
        pFileDialog->Release();
        return "Null";
    }

    std::filesystem::path folderPath(pszFolderPath);
    CoTaskMemFree(pszFolderPath);
    pShellItem->Release();
    pFileDialog->Release();

    return folderPath;
}

