#pragma once
#include <vector>
#include <string>

// �x���烉�W�A���p�x��
extern float Deg2Rad(float deg);

// LPCSTR -> string
//extern LPCSTR LPCSTRtoString(std::string str);

// vector����index�Ԗڂ̗v�f���폜����
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)
{
	vector.erase(vector.begin() + index);
}

