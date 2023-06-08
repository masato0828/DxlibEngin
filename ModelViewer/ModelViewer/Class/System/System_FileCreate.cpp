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
        // �t�@�C���̍쐬�ɐ��������ꍇ�̏���
       // std::cout << "File created: " << filePath << std::endl;
    }
    else
    {
        // �t�@�C���̍쐬�Ɏ��s�����ꍇ�̏���
        //std::cout << "Failed to create file: " << filePath << std::endl;
    }
}

bool System_FileCreate::IsMainFile()
{
    // �t�H���_��


    ImGui::Begin(u8"�f�[�^�̓ǂݍ���");

    if(fileDialog_open)
    {
        basePath_ = L"UseEngin";
        // UseEngin�t�H���_�����邩�ǂ���
        std::ifstream file(basePath_);
        if (file.is_open())
        {
            // UseEngin�t�H���_������
            //return true;
        }
        else
        {
            // UseEngin�t�H���_���Ȃ�
            // �t�@�C���_�C�A���O�̕\��
            auto result = CreateDirectoryFromFileDialog();

            if (result == "Null")
            {
                return false;
            }
           
            // UseEngin�t�H���_�̍쐬
            result /= basePath_;
            std::filesystem::create_directories(result);
            systemFullPath_ = result;

            //return true;
        }
        

        fileDialog_open = false;
    }
    std::string utf8Str = Utility::WStringToUTF8(systemFullPath_);

    ImGui::Text(u8"�t�@�C���̎Q��");
    char buffer[256]; // �K�؂ȃT�C�Y�ɒ������Ă�������
    strncpy_s(buffer, sizeof(buffer), utf8Str.c_str(), _TRUNCATE);
    ImGui::InputText("##label", buffer, 1060);

    std::string spath = buffer;
    systemFullPath_ = Utility::StringToWideString(spath);
    ImGui::SameLine();
    if (ImGui::SmallButton(u8"�Q��"))
    {
        fileDialog_open = true;
    }

    ImGui::End();

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
    // Common Item Dialog�̃C���X�^���X���쐬
    IFileDialog* pFileDialog;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (FAILED(hr))
    {
        // �G���[���b�Z�[�W��\�����邩�A�G���[���O�ɋL�^����Ȃǂ̏������s��
        return "Null";
    }

    // �t�@�C���_�C�A���O���t�H���_�I�����[�h�ɐݒ�
    DWORD options;
    pFileDialog->GetOptions(&options);
    pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

    // �t�@�C���_�C�A���O��\��
    hr = pFileDialog->Show(NULL);
    if (FAILED(hr))
    {
        // �G���[���b�Z�[�W��\�����邩�A�G���[���O�ɋL�^����Ȃǂ̏������s��
        pFileDialog->Release();
        return "Null";
    }

    // �I�����ꂽ�t�H���_�̃p�X���擾
    IShellItem* pShellItem;
    hr = pFileDialog->GetResult(&pShellItem);
    if (FAILED(hr))
    {
        // �G���[���b�Z�[�W��\�����邩�A�G���[���O�ɋL�^����Ȃǂ̏������s��
        pFileDialog->Release();
        return "Null";
    }

    PWSTR pszFolderPath;
    hr = pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
    if (FAILED(hr))
    {
        // �G���[���b�Z�[�W��\�����邩�A�G���[���O�ɋL�^����Ȃǂ̏������s��
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

