#include <filesystem>
#include <string>
#include<Windows.h>

struct OpenFileDialog
{
	std::string operator()(HWND hWnd = nullptr,std::string* title = nullptr,std::string* filter = nullptr)
	{
		//���݂̃J�������g�f�B���N�g�����擾
		auto directory = std::filesystem::current_path();

		//�t�@�C�����J���ׂ̐ݒ�p�\����
		OPENFILENAME fileStruct{};
		//�w��t�@�C���̐�΃p�X����������
		std::string filePass;

		filePass.resize(MAX_PATH);

		//�������g�̃T�C�Y
		fileStruct.lStructSize = sizeof(OPENFILENAME);
		//�e�E�B���h�E�̃n���h��
		fileStruct.hwndOwner = hWnd;
		//�\��������t�@�C���̃t�B���^�����O
		if (filter == nullptr)
		{
			fileStruct.lpstrFilter = nullptr;
		}
		else
		{
			fileStruct.lpstrFilter = filter->c_str();
		}

		//�p�X��������z��
		fileStruct.lpstrFile = &filePass.at(0);
		//�p�X�̍ő啶����
		fileStruct.nMaxFile = MAX_PATH;
		//�E�B���h�E�̖��O
		if (title == nullptr)
		{
			fileStruct.lpstrTitle = nullptr;
		}
		else
		{
			fileStruct.lpstrTitle = title->c_str();
		}

		//�t�@�C���I�[�v���Ɏ��s������return
		if (GetOpenFileName(&fileStruct) == 0)//�J�����g�f�B���N�g��������Ă��܂�
		{
			return "";
		}

		//���̃J�����g�f�B���N�g���ɖ߂�
		std::filesystem::current_path(directory);

		return filePass;
	}
};

struct FilePathErase
{
	std::string operator()(std::string fileNameFull)
	{
		int eraseNum = static_cast<int>(fileNameFull.find_last_of('\\'));
		//// 
		std::string file = fileNameFull.erase(0, eraseNum);
		//// 
		std::string fileStrName = fileNameFull.erase(0, 1);

		return fileStrName;
	}
};



