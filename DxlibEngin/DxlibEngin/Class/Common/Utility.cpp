#include "Utility.h"
#include <iostream>
#include <DxLib.h>
#include <shlwapi.h>
#include <fstream>
#include <regex>
#include <Shlwapi.h>

// �x���烉�W�A���p�x��
float Utility::Deg2Rad(float deg)
{
    return (deg * (DX_PI_F / 180.0f));
}

// �ő���񐔂����߂�
int Utility::Gcd(int x, int y)
{
	if (y == 0)
	{
		return x;
	}
	return Gcd(y, x % y);
}


std::wstring Utility::StringToWideString(const std::string& str)
{
	std::wstring wideStr;
	wideStr.resize(str.size());

	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(
		str.data(), str.data() + str.size(), wideStr.data()
	);

	return wideStr;
}

std::string Utility::WideStringToString(const std::wstring& wideString)
//{
//	auto wbe = wideString.begin();
//	auto wen = wideString.end();
//	auto string = std::string(wbe, wen);
//
//	return string;
//}
{
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (bufferSize == 0) {
		// �G���[�n���h�����O
		return std::string();
	}

	std::string result(bufferSize, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, &result[0], bufferSize, nullptr, nullptr);

	return result;
}

std::string Utility::WStringToUTF8(const std::wstring& wstr)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string utf8Str(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], size, nullptr, nullptr);
	return utf8Str;
}

std::wstring Utility::MultiByteToUnicode(const std::string& mbStr)
{
	// �}���`�o�C�g������̃T�C�Y���擾
	int size = MultiByteToWideChar(CP_ACP, 0, mbStr.c_str(), -1, nullptr, 0);

	// �T�C�Y�Ɋ�Â��� Unicode ��������i�[���邽�߂� wstring ���쐬
	std::wstring unicodeStr(size, 0);

	// �}���`�o�C�g������� Unicode ������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mbStr.c_str(), -1, &unicodeStr[0], size);

	// �ϊ����ꂽ Unicode �������Ԃ�
	return unicodeStr;
}

std::wstring Utility::GetDefaultApplication(const std::wstring& extension)
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

bool Utility::LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath)
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

bool Utility::OpenWithDefaultApplication(const std::wstring& filePath)
{
	std::wstring extension = filePath.substr(filePath.find_last_of(L"."));
	std::wstring defaultApplication = Utility::GetDefaultApplication(extension);

	if (!defaultApplication.empty()) {
		// �K��̃A�v���P�[�V�������ݒ肳��Ă���ꍇ�A������N������
		return Utility::LaunchApplication(defaultApplication, filePath);
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

	return false;
}

bool Utility::IsHeaderFile(const std::string& filename, const std::string& ext)
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

bool Utility::CharacterSearch(std::string showName, std::string searchFileName, ImGuiCustom::IM_COLOR color, std::string defaultTarget)
{
	if (Utility::IsHeaderFile(defaultTarget, searchFileName))
		//if (defaultTarget.find(searchFileName) != std::string::npos)
	{
		ImGui::TextColored(ImVec4(color.red, color.green, color.blue, color.alpha), showName.c_str());
		return true;
	}
	return false;
}

// maltbyte�̃G���R�[�f�B���O�𔻕ʂ���֐�
std::string Utility::DetectMaltbyteEncoding(const std::string& maltbyte)
{
	// UTF-8���ʂ̂��߂̃o�C�g�p�^�[��
	std::string utf8Pattern = "\xEF\xBB\xBF";

	// UTF-16 (LE) ���ʂ̂��߂̃o�C�g�p�^�[��
	std::string utf16LEPattern = "\xFF\xFE";

	// UTF-16 (BE) ���ʂ̂��߂̃o�C�g�p�^�[��
	std::string utf16BEPattern = "\xFE\xFF";

	// Shift-JIS ���ʂ̂��߂̃o�C�g�p�^�[��
	std::string shiftJISPattern = "\x82\xA0-\x82\xF1\x83\x40-\x83\x96\x83\x9F-\x83\xB6\x83\xBF-\x83\xD6\x83\xF5-\x83\xFC";

	// maltbyte�Ɋ܂܂��o�C�g�p�^�[�����`�F�b�N���ăG���R�[�f�B���O�𔻕ʂ���
	if (maltbyte.compare(0, utf8Pattern.size(), utf8Pattern) == 0)
	{
		return "UTF-8";
	}
	else if (maltbyte.compare(0, utf16LEPattern.size(), utf16LEPattern) == 0)
	{
		return "UTF-16 (LE)";
	}
	else if (maltbyte.compare(0, utf16BEPattern.size(), utf16BEPattern) == 0)
	{
		return "UTF-16 (BE)";
	}
	else if(std::regex_search(maltbyte, std::regex(shiftJISPattern)))
	{
		return "SHIFT-JIS";
	}


	return "Unknown";
}

bool Utility::RenameFile(const std::string& filePath, const std::string& newFileName)
{
	// ���݂̃t�@�C���p�X�ƐV�����t�@�C��������V�����t�@�C���p�X���쐬
	std::string newFilePath = filePath;
	size_t lastSlash = newFilePath.find_last_of("/\\");
	if (lastSlash != std::string::npos)
	{
		newFilePath = newFilePath.substr(0, lastSlash + 1) + newFileName;
	}
	else
	{
		newFilePath = newFileName;
	}

	// �t�@�C���̖��O��ύX����
	if (std::rename(filePath.c_str(), newFilePath.c_str()) != 0)
	{
		return false;
	}

	// �t�@�C�����̕ύX�ɐ��������ꍇ�̏���
	return true;
}

bool Utility::IsMatch(const std::string& filepath, const std::wstring& target)
{
	{
		std::filesystem::path pathObj(filepath);
		std::string filename = pathObj.filename().string();

		if (target.empty())
		{
			return false;
		}

		// �啶���Ə���������ʂ����ɔ�r����ꍇ
		/*if (filename == target) {
			return true;
		}*/

		// �啶���Ə���������ʂ��Ĕ�r����ꍇ
		// if (filename.compare(target) == 0) {
		//     return true;
		// }

		// �t�@�C�����Ɏw�肵�������񂪊܂܂�Ă��邩�𔻒肷��
		if (filename.find(Utility::WStringToUTF8(target)) != std::string::npos) {
			return true;
		}

		return false;
	}
}