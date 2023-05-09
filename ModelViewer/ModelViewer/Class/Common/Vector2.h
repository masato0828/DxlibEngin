#pragma once
//template<class T> class ev[gNXΌ

template<class T> class Vector2Template
{
public:

	Vector2Template();
	Vector2Template(T x, T y);
	template<class F>
	Vector2Template(F x, F y);
	~Vector2Template();

	T x_;
	T y_;

	//γόZq
	Vector2Template& operator = (const Vector2Template& vec);

	//Y¦Zq
	int& operator[](int j);

	//PZq
	Vector2Template& operator+=(const Vector2Template& vec);
	Vector2Template& operator-=(const Vector2Template& vec);
	Vector2Template& operator*=(const Vector2Template& vec);
	Vector2Template& operator/=(const Vector2Template& vec);
	Vector2Template& operator%=(const Vector2Template& vec);
	Vector2Template operator+()const;
	Vector2Template operator-()const;
	Vector2Template operator*=(double scale);
	Vector2Template operator/=(double scale);



	//ρZq
	Vector2Template operator+(Vector2Template vec);	//ZqπpκΕoperator ‘ρΝ«ZΎ©ηA{πγλΙ­
	Vector2Template operator-(Vector2Template vec);
	Vector2Template operator*(Vector2Template vec);
	Vector2Template operator/(Vector2Template vec);
	Vector2Template operator%(Vector2Template vec);
	Vector2Template operator/(T scale);

	//δrZq
	bool operator==(const Vector2Template& vec) const;
	bool operator!=(const Vector2Template& vec) const;
	bool operator<(const Vector2Template& vec) const;
	bool operator>(const Vector2Template& vec) const;
	bool operator<=(const Vector2Template& vec) const;
	bool operator>=(const Vector2Template& vec) const;

};
template <class T>
Vector2Template<T> operator*(const Vector2Template<T>& u, T k);		//QΖn΅:const ^& ψ
//template <class T>
//Vector2Template<T> operator*(int k,const Vector2Template<T>& u);

using Vector2 = Vector2Template<int>;
using Vector2Flt = Vector2Template<float>;
using Vector2Dbl = Vector2Template<double>;

#include "detail/Vector2.h"

template<class T>
template<class F>
inline Vector2Template<T>::Vector2Template(F x, F y)
{
	x_ = (T)x;
	y_ = (T)y;
}
