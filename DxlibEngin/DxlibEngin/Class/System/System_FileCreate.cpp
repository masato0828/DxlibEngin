#include "System_FileCreate.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <shobjidl.h>
#include "../Common/Utility.h"

#include "../../imGui/imgui.h"

using  json = nlohmann::json;

System_FileCreate::System_FileCreate()
{
    basePath_ = L"D:/UseEngin";
    fileDialog_open = false;
    systemFullPath_ = basePath_;
    enginFile_ = L"D:/";

    appOpenFlg_ = false;
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

void System_FileCreate::IsMainFile()
{
    // フォルダ名
    if (appOpenFlg_)
    {
        return;
    }

    if (basePath_ == mainFilePath_)
    {
        return;
    }

    // 初回起動時に指定したファイルの中にuseEnginがあれば
#ifdef _DEBUG
    if (systemFullPath_ != L"")
    {
        appOpenFlg_ = true;
        return;
    }
#endif // DEBUG

    

    // 画面のサイズを取得する
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); // 横方向の画面サイズ
    int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 縦方向の画面サイズ

    //screenSize
    ImVec2 windowSize = { 500,200 };
    ImGui::SetNextWindowSize(windowSize);
    // ウィンドウの位置を計算して設定する
    ImVec2 windowPos((screenWidth - windowSize.x) * 0.5f, (screenHeight - windowSize.y) * 0.5f);
    ImGui::SetNextWindowPos(windowPos);

    if (ImGui::Begin(u8"管理データ作成"), NULL,ImGuiWindowFlags_NoCollapse)
    {
        SeachFolder();

        ImGui::Text(u8"ファイルの参照");

        InputFolderName(systemFullPath_, fileDialog_open);

        if (ImGui::Button("OK"))
        {
            // 指定した場所にフォルダの作成
            //std::filesystem::create_directories(systemFullPath_);

            CreateFolderCheck(systemFullPath_);

            appOpenFlg_ = true;
        }

        ImGui::End();

    }
   

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
}

bool System_FileCreate::GetAppOpenFlg()
{
    return appOpenFlg_;
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
        // エラーメッセージを表示するか、エラーログに記録するなどの処理
        return "Error";
    }

    // ファイルダイアログをフォルダ選択モードに設定
    DWORD options;
    pFileDialog->GetOptions(&options);
    pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

    // ファイルダイアログを表示
    hr = pFileDialog->Show(NULL);
    if (FAILED(hr))
    {
        // キャンセルもしくはバツを押した場合の処理
        if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
        {
            pFileDialog->Release();
            return ""; // 空文字列を返す（キャンセルorバツ）
        }
        else
        {
            // エラーメッセージを表示、エラーログに記録するなどの処理
            pFileDialog->Release();
            return "Error";
        }
    }

    // 選択されたフォルダのパスを取得
    IShellItem* pShellItem;
    hr = pFileDialog->GetResult(&pShellItem);
    if (FAILED(hr))
    {
        // エラーメッセージを表示するか、エラーログに記録するなどの処理
        pFileDialog->Release();
        return "Error";
    }

    PWSTR pszFolderPath;
    hr = pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
    if (FAILED(hr))
    {
        // エラーメッセージを表示するか、エラーログに記録するなどの処理
        pShellItem->Release();
        pFileDialog->Release();
        return "Error";
    }

    std::filesystem::path folderPath(pszFolderPath);
    CoTaskMemFree(pszFolderPath);
    pShellItem->Release();
    pFileDialog->Release();

    return folderPath;
}

bool System_FileCreate::SeachFolder()
{
    if (fileDialog_open)
    {
        basePath_ = L"UseEngin";
        // UseEnginフォルダがあるかどうか
        std::ifstream file(basePath_);
        if (file.is_open())
        {
            // UseEnginフォルダが現在開いている
            return true;
        }
        else
        {
            // UseEnginフォルダが現在開いていない
            // ファイルダイアログの表示
            auto result = CreateDirectoryFromFileDialog();
            

            if (result == ""&&addFileName_ == L"")
            {
                result = enginFile_;
            }
            else
            {
                addFileName_ = result;
            }

            if (result == "Error")
            {
                MessageBox(NULL,"ファイルの指定に失敗しました","ファイルデータ作成",MB_OK);
                return false;
            }

            // UseEnginフォルダの作成
            result /= basePath_;
            systemFullPath_ = result;

            //return true;
            fileDialog_open = false;
        }


       
    }

    return false;
}

void System_FileCreate::InputFolderName(std::wstring& text, bool& seacthFolderOpen)
{
    std::string utf8Str = Utility::WStringToUTF8(text);
    char buffer[256]; // 適切なサイズに調整
    strncpy_s(buffer, sizeof(buffer), utf8Str.c_str(), _TRUNCATE);
    ImGui::InputText("##label", buffer, 1060);

    std::string spath = buffer;
    text = Utility::MultiByteToUnicode(spath);
    ImGui::SameLine();
    if (ImGui::SmallButton(u8"参照"))
    {
        seacthFolderOpen = true;
    }
}

bool System_FileCreate::CreateFolderCheck(const std::wstring folderPath)
{
    if (std::filesystem::create_directories(folderPath))
    {
        return true;
    }
    return false;
}

void System_FileCreate::DataCreate()
{
    std::string fileName = Utility::WideStringToString(basePath_);

    std::ofstream writing_file;
    writing_file.open(fileName, std::ios::out);

    json j;

    j["MainFolderPath"] = basePath_;

    writing_file << std::setw(4) << j << std::endl;

    writing_file.close();
}

void System_FileCreate::FileLoad(const std::wstring fileName)
{

}

