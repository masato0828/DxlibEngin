#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include "../Common/ImGuiMyCustom.h"


namespace Utility
{
	// �x���烉�W�A���p�x��
	float Deg2Rad(float deg);

	// �ő���񐔂����߂�
	int Gcd(int x, int y);

	std::wstring StringToWideString(const std::string& str);

	std::string WideStringToString(const std::wstring& wideString);

	std::string WStringToUTF8(const std::wstring& wstr);

	// �}���`�o�C�g������ mbStr �� std::wstring
	std::wstring MultiByteToUnicode(const std::string& mbStr);

	std::wstring GetDefaultApplication(const std::wstring& extension);

	bool OpenWithDefaultApplication(const std::wstring& filePath);

	bool LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath);

	bool IsHeaderFile(const std::string& filename, const std::string& ext);

	bool CharacterSearch(std::string showName, std::string searchFileName, ImGuiCustom::IM_COLOR color, std::string defaultTarget);

    // maltbyte�̃G���R�[�f�B���O�𔻕ʂ���֐�
	std::string DetectMaltbyteEncoding(const std::string& maltbyte);

	bool RenameFile(const std::filesystem::path& filePath, const std::filesystem::path& newFileName);

}
// vector����index�Ԗڂ̗v�f���폜����
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)
{
	vector.erase(vector.begin() + index);
}

