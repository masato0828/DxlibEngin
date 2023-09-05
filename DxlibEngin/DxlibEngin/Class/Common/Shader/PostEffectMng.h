#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "PostEffect_Base.h"
class PostEffectMng
{
public:
	PostEffectMng();
	~PostEffectMng();
	
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="screen">�w��X�N���[��</param>
	void Draw(const int screen);
	
	/// <summary>
	/// �J�X�^��
	/// </summary>
	void Custom();
private:

	// �|�X�g�G�t�F�N�g�Ǘ�(key�l,�|�X�g�G�t�F�N�g)
	std::unordered_map<std::string,std::unique_ptr<PostEffect_Base>> postEffect_;

	// �I�𒆂̃|�X�g�G�t�F�N�g��key�l
	std::string selectName_;

	
};

