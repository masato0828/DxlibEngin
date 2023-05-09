#include "..\Vector2.h"
//#include "Vector2.h"
// Vector2.cpp�ɂ͏����Ȃ����߃w�b�_�[�ɂ���K�v������

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
//������Z�q
//============================================================
template<class T>
Vector2Template<T>& Vector2Template<T>::operator=(const Vector2Template<T>& vec)
{
	x_ = vec.x_;
	y_ = vec.y_;

	return *this;		//�������g��Ԃ�
}

//============================================================
//�Y�������Z�q
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
//�P�����Z�q
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
	//�O�Ōv�Z���Ȃ��悤�ɂ���
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
Vector2Template<T> Vector2Template<T>::operator-() const//�}�C�i�X��t�^����
{
	return { -x_,-y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator*=(double scale)
{
	x_ = x_ * scale;
	y_ = y_ * scale;

	return { x_,y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator/=(double scale)
{
	x_ = x_ / scale;
	y_ = y_ / scale;

	return { x_,y_};
}

//============================================================
//�񍀉��Z�q
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

template<class T>
Vector2Template<T> Vector2Template<T>::operator/(T scale)
{
	return { x_ / scale, y_ / scale };
}

//============================================================
//��r���Z�q
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
//�Q�Ɠn��
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
