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
        // �t�@�C���̍쐬�ɐ��������ꍇ�̏���
       // std::cout << "File created: " << filePath << std::endl;
    }
    else
    {
        // �t�@�C���̍쐬�Ɏ��s�����ꍇ�̏���
        //std::cout << "Failed to create file: " << filePath << std::endl;
    }
}

void System_FileCreate::IsMainFile()
{
    // �t�H���_��
    if (appOpenFlg_)
    {
        return;
    }

    if (basePath_ == mainFilePath_)
    {
        return;
    }

    // ����N�����Ɏw�肵���t�@�C���̒���useEngin�������
#ifdef _DEBUG
    if (systemFullPath_ != L"")
    {
        appOpenFlg_ = true;
        return;
    }
#endif // DEBUG

    

    // ��ʂ̃T�C�Y���擾����
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); // �������̉�ʃT�C�Y
    int screenHeight = GetSystemMetrics(SM_CYSCREEN); // �c�����̉�ʃT�C�Y

    //screenSize
    ImVec2 windowSize = { 500,200 };
    ImGui::SetNextWindowSize(windowSize);
    // �E�B���h�E�̈ʒu���v�Z���Đݒ肷��
    ImVec2 windowPos((screenWidth - windowSize.x) * 0.5f, (screenHeight - windowSize.y) * 0.5f);
    ImGui::SetNextWindowPos(windowPos);

    if (ImGui::Begin(u8"�Ǘ��f�[�^�쐬"), NULL,ImGuiWindowFlags_NoCollapse)
    {
        SeachFolder();

        ImGui::Text(u8"�t�@�C���̎Q��");

        InputFolderName(systemFullPath_, fileDialog_open);

        if (ImGui::Button("OK"))
        {
            // �w�肵���ꏊ�Ƀt�H���_�̍쐬
            //std::filesystem::create_directories(systemFullPath_);

            CreateFolderCheck(systemFullPath_);

            appOpenFlg_ = true;
        }

        ImGui::End();

    }
   

    //�ēx�J���Ƃ��̂��߁A
    //systemFullPath_�@���擾���Ă����Ȃ���΂Ȃ�Ȃ�


    userFileName_ = L"/BaseDirectory";


    //// �����̃f�[�^�܂��̓f�[�^�̍쐬���s�����𕷂�
    //if (true)
    //{
    //    // �V�K�̏ꍇ�ǂ��ɍ�邩�𕷂�
    //    // �ǂ��ɍ�邩
    //    CreateFilesInDirectory(basePath_);
    //}
    //else
    //{
    //    // �����̃f�[�^�̓ǂݍ��ݏ���
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
    // Common Item Dialog�̃C���X�^���X���쐬
    IFileDialog* pFileDialog;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (FAILED(hr))
    {
        // �G���[���b�Z�[�W��\�����邩�A�G���[���O�ɋL�^����Ȃǂ̏���
        return "Error";
    }

    // �t�@�C���_�C�A���O���t�H���_�I�����[�h�ɐݒ�
    DWORD options;
    pFileDialog->GetOptions(&options);
    pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

    // �t�@�C���_�C�A���O��\��
    hr = pFileDialog->Show(NULL);
    if (FAILED(hr))
    {
        // �L�����Z���������̓o�c���������ꍇ�̏���
        if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
        {
            pFileDialog->Release();
            return ""; // �󕶎����Ԃ��i�L�����Z��or�o�c�j
        }
        else
        {
            // �G���[���b�Z�[�W��\���A�G���[���O�ɋL�^����Ȃǂ̏���
            pFileDialog->Release();
            return "Error";
        }
    }

    // �I�����ꂽ�t�H���_�̃p�X���擾
    IShellItem* pShellItem;
    hr = pFileDialog->GetResult(&pShellItem);
    if (FAILED(hr))
    {
        // �G���[���b�Z�[�W��\�����邩�A�G���[���O�ɋL�^����Ȃǂ̏���
        pFileDialog->Release();
        return "Error";
    }

    PWSTR pszFolderPath;
    hr = pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
    if (FAILED(hr))
    {
        // �G���[���b�Z�[�W��\�����邩�A�G���[���O�ɋL�^����Ȃǂ̏���
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
        // UseEngin�t�H���_�����邩�ǂ���
        std::ifstream file(basePath_);
        if (file.is_open())
        {
            // UseEngin�t�H���_�����݊J���Ă���
            return true;
        }
        else
        {
            // UseEngin�t�H���_�����݊J���Ă��Ȃ�
            // �t�@�C���_�C�A���O�̕\��
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
                MessageBox(NULL,"�t�@�C���̎w��Ɏ��s���܂���","�t�@�C���f�[�^�쐬",MB_OK);
                return false;
            }

            // UseEngin�t�H���_�̍쐬
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
    char buffer[256]; // �K�؂ȃT�C�Y�ɒ���
    strncpy_s(buffer, sizeof(buffer), utf8Str.c_str(), _TRUNCATE);
    ImGui::InputText("##label", buffer, 1060);

    std::string spath = buffer;
    text = Utility::MultiByteToUnicode(spath);
    ImGui::SameLine();
    if (ImGui::SmallButton(u8"�Q��"))
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

