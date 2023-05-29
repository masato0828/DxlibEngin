#pragma once
#include <vector>
#include <string>

// 度からラジアン角度に
extern float Deg2Rad(float deg);

// 最大公約数を求める
extern int Gcd(int x, int y);

// vectorからindex番目の要素を削除する
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)
{
	vector.erase(vector.begin() + index);
}

