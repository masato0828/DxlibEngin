#include "Utility.h"
#include <iostream>
#include <DxLib.h>
#include <shlwapi.h>

// �x���烉�W�A���p�x��
float Deg2Rad(float deg)
{
    return (deg * (DX_PI_F / 180.0f));
}

// �ő���񐔂����߂�
int Gcd(int x, int y)
{
	if (y == 0)
	{
		return x;
	}
	return Gcd(y, x % y);
}


std::wstring StringToWideString(const std::string& str)
{
	std::wstring wideStr;
	wideStr.resize(str.size());

	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(
		str.data(), str.data() + str.size(), wideStr.data()
	);

	return wideStr;
}

std::string WideStringToString(const std::wstring& wideString)
{
	return std::string(wideString.begin(), wideString.end());
}

std::wstring GetDefaultApplication(const std::wstring& extension)
{
	wchar_t progId[MAX_PATH];
	DWORD progIdSize = MAX_PATH;

	std::wstring messageContent;
	std::wstring errorMsg;
	std::wstring errorText;

	// �g���q�Ɋ֘A�t����ꂽ�v���O����ID���擾
	HRESULT result = AssocQueryStringW(ASSOCF_NONE, ASSOCSTR_EXECUTABLE, extension.c_str(), nullptr, progId, &progIdSize);
	if (result == S_OK)
	{
		// �v���O����ID������s�\�t�@�C���p�X���擾
		wchar_t executablePath[MAX_PATH];
		DWORD executablePathSize = MAX_PATH;

		result = AssocQueryStringW(ASSOCF_NONE, ASSOCSTR_EXECUTABLE, progId, nullptr, executablePath, &executablePathSize);
		if (result == S_OK)
		{
			return progId;
		}
		else
		{
			//MessageBox(NULL, "progID�̎��s�\�p�X�̎擾�Ɏ��s���܂����B:", "error", MB_OK);
			return L"";
		}
	}
	else
	{
		//MessageBox(NULL," �g���q��ProgID�̎擾�Ɏ��s���܂����B:", "error", MB_OK);
		return L"";
	}

	return L"";
}

bool LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath)
{
	std::wstring commandLine = L"\"";
	commandLine += applicationPath;
	commandLine += L"\" \"";
	commandLine += filePath;
	commandLine += L"\"";

	STARTUPINFOW startupInfo = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION processInfo;

	BOOL result = CreateProcessW(
		nullptr,
		&commandLine[0],
		nullptr,
		nullptr,
		FALSE,
		0,
		nullptr,
		nullptr,
		&startupInfo,
		&processInfo
	);

	if (result) {
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		return true;
	}
	else {
		return false;
	}
}

bool OpenWithDefaultApplication(const std::wstring& filePath)
{
	std::wstring extension = filePath.substr(filePath.find_last_of(L"."));
	std::wstring defaultApplication = GetDefaultApplication(extension);

	if (!defaultApplication.empty()) {
		// �K��̃A�v���P�[�V�������ݒ肳��Ă���ꍇ�A������N������
		return LaunchApplication(defaultApplication, filePath);
	}
	else {
		// �t�@�C�����J�����@��I�Ԃ��߂̃_�C�A���O��\��
		// �f�t�H�Őݒ肳��Ă���A�v��������΂�����N��
		int result = (int)ShellExecuteW(NULL, L"open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);

		if (result <= 32)
		{
			// �G���[�����������ꍇ�̏���
			DWORD error = GetLastError();
			// �G���[�R�[�h���g���ăG���[���b�Z�[�W���擾����Ȃǂ̏������s��
		}
	}
}

bool IsHeaderFile(const std::string& filename, const std::string& ext)
{
	// �g���q�̈ʒu��T��
	std::size_t dotPos = filename.find_last_of(".");
	if (dotPos == std::string::npos) {
		// �h�b�g��������Ȃ��ꍇ�͊g���q���Ȃ��Ɣ��f
		return false;
	}

	// �g���q���擾���Ĕ�r����
	std::string extension = filename.substr(dotPos + 1);
	if (extension == ext) {
		return true;
	}

	return false;
}

bool CharacterSearch(std::string showName, std::string searchFileName, ImGuiCustom::IM_COLOR color, std::string defaultTarget)
{
	if (IsHeaderFile(defaultTarget, searchFileName))
		//if (defaultTarget.find(searchFileName) != std::string::npos)
	{
		ImGui::TextColored(ImVec4(color.red, color.green, color.blue, color.alpha), showName.c_str());
		return true;
	}
	return false;
}


