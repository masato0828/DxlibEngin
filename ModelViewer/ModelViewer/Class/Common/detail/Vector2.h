//#include "Vector2.h"
// Vector2.cppには書けないためヘッダーにする必要がある

template<class T>
Vector2Template<T>::Vector2Template()
{
	x_ = 0;
	y_ = 0;
}
template<class T>
Vector2Template<T>::Vector2Template(T x, T y)
{
	x_ = x;
	y_ = y;
}
template<class T>
inline Vector2Template<T>::~Vector2Template()
{

}

//============================================================
//代入演算子
//============================================================
template<class T>
Vector2Template<T>& Vector2Template<T>::operator=(const Vector2Template<T>& vec)
{
	x_ = vec.x_;
	y_ = vec.y_;

	return *this;		//自分自身を返す
}

//============================================================
//添え字演算子
//============================================================
template<class T>
int& Vector2Template<T>::operator[](int j)
{
	switch (j)
	{
	case 0:
		return x_;
		//break;
	case 1:
		return y_;
		//break;
	default:
		return x_;
		//break;
	}
}

//============================================================
//単項演算子
//============================================================
template<class T>
Vector2Template<T>& Vector2Template<T>::operator+=(const Vector2Template<T>& vec)
{
	x_ += vec.x_;
	y_ += vec.y_;

	return *this;
}

template<class T>
Vector2Template<T>& Vector2Template<T>::operator-=(const Vector2Template<T>& vec)
{
	x_ -= vec.x_;
	y_ -= vec.y_;

	return *this;
}

template<class T>
Vector2Template<T>& Vector2Template<T>::operator*=(const Vector2Template<T>& vec)
{
	x_ *= vec.x_;
	y_ *= vec.y_;

	return *this;
}

template<class T>
Vector2Template<T>& Vector2Template<T>::operator/=(const Vector2Template<T>& vec)
{
	//０で計算しないようにする
	if (vec.x_ != 0)
	{
		x_ /= vec.x_;
	}
	else
	{
		x_ = 0;
	}
	if (vec.y_ != 0)
	{
		y_ /= vec.y_;
	}
	else
	{
		y_ = 0;
	}
	return *this;
}

template<class T>
Vector2Template<T>& Vector2Template<T>::operator%=(const Vector2Template<T>& vec)
{
	if (vec.x_ != 0)
	{
		x_ %= vec.x_;
	}
	else
	{
		x_ = 0;
	}
	if (vec.y_ != 0)
	{
		y_ %= vec.y_;
	}
	else
	{
		y_ = 0;
	}
	return *this;
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator+() const
{
	return *this;
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator-() const//マイナスを付与する
{
	return { -x_,-y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator*=(double scale)
{
	return { x_ * scale,y_ * scale };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator/=(double scale)
{
	return { x_ / scale,y_ / scale };
}

//============================================================
//二項演算子
//============================================================
template<class T>
Vector2Template<T> Vector2Template<T>::operator+(Vector2Template<T> vec)
{

	return { x_ + vec.x_, y_ + vec.y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator-(Vector2Template<T> vec)
{
	return { x_ - vec.x_, y_ - vec.y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator*(Vector2Template<T> vec)
{
	return { x_ * vec.x_, y_ * vec.y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator/(Vector2Template<T> vec)
{
	return { x_ / vec.x_, y_ / vec.y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator%(Vector2Template<T> vec)
{
	return { x_ % vec.x_, y_ % vec.y_ };
}

//============================================================
//比較演算子
//============================================================
template<class T>
bool Vector2Template<T>::operator==(const Vector2Template<T>& vec) const
{
	return (x_ == vec.x_ && y_ == vec.y_);
}

template<class T>
bool Vector2Template<T>::operator!=(const Vector2Template<T>& vec) const
{
	return (x_ != vec.x_ && y_ != vec.y_);
}

template<class T>
bool Vector2Template<T>::operator<(const Vector2Template<T>& vec) const
{
	return (x_ < vec.x_&& y_ < vec.y_);
}

template<class T>
bool Vector2Template<T>::operator>(const Vector2Template<T>& vec) const
{
	return (x_ > vec.x_ && y_ > vec.y_);
}

template<class T>
bool Vector2Template<T>::operator<=(const Vector2Template<T>& vec) const
{
	return (x_ <= vec.x_ && y_ <= vec.y_);
}

template<class T>
bool Vector2Template<T>::operator>=(const Vector2Template<T>& vec) const
{
	return (x_ >= vec.x_ && y_ >= vec.y_);
}

//============================================================
//参照渡し
//============================================================
template<class T>
Vector2Template<T> operator*(const Vector2Template<T>& u, T k)
{
	return { u.x_ * k,u.y_ * k };
}

//template<class T>
//Vector2Template<T> operator*(int k, const Vector2Template<T>& u)
//{
//	return {k * u.x_,k * u.y_};
//}
