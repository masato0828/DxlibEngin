#pragma once
#include <vector>
#include <string>
#include "../Common/ImGuiMyCustom.h"

// �x���烉�W�A���p�x��
extern float Deg2Rad(float deg);

// �ő���񐔂����߂�
extern int Gcd(int x, int y);

extern std::wstring StringToWideString(const std::string& str);

extern std::string WideStringToString(const std::wstring& wideString);

extern std::wstring GetDefaultApplication(const std::wstring& extension);

extern bool OpenWithDefaultApplication(const std::wstring& filePath);

extern bool LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath);

extern bool IsHeaderFile(const std::string& filename, const std::string& ext);

extern bool CharacterSearch(std::string showName, std::string searchFileName, ImGuiCustom::IM_COLOR color, std::string defaultTarget);
// vector����index�Ԗڂ̗v�f���폜����
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)
{
	vector.erase(vector.begin() + index);
}

