#pragma once
#include <vector>
#include <string>

// 度からラジアン角度に
extern float Deg2Rad(float deg);

// LPCSTR -> string
//extern LPCSTR LPCSTRtoString(std::string str);

// vectorからindex番目の要素を削除する
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)
{
	vector.erase(vector.begin() + index);
}

