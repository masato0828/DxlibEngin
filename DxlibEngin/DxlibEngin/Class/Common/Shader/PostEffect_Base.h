#pragma once
#include <string>
class PostEffect_Base
{
public:
	PostEffect_Base();
	~PostEffect_Base();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() = 0;
	
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;
	
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="name">�V�F�[�_�[�̖��O
	/// (�|�X�g�G�t�F�N�g�}�l�[�W���[��key�l�ƈ�v����K�v����)</param>
	/// <param name="imageHnadle">�摜�n���h��</param>
	virtual void Draw(std::wstring name,const int imageHnadle) = 0;

	/// <summary>
	/// �J�X�^��
	/// </summary>
	virtual void Custom() = 0;
private:
};

