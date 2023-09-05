#pragma once
#include "PostEffect_Base.h"
#include "../../Common/Vector2.h"
#include "../../Common/ImGuiMyCustom.h"
class PostEffect_Transition_OuterCircumference :
    public PostEffect_Base
{
public:
	PostEffect_Transition_OuterCircumference();
	~PostEffect_Transition_OuterCircumference();
	void Init() override final;
	void Update() override final;
	void Draw(std::string name, const int imageHnadle) override final;
	void Custom()override final;
private:

	struct Transition_OuterCircumference
	{
		
		ImGuiCustom::IM_COLOR color;// �F
		Vector2Flt screenSize;// �X�N���[���̃T�C�Y
		Vector2Flt aspect;// �A�X�y�N�g��
		float time;// ����
		float div;//
		float direction;//
		float rotation;//��]
		float size;//�T�C�Y
	};

	// �p�����[�^�Ǘ�
	Transition_OuterCircumference pram_;

	// ���Ԃ̃X�s�[�h
	float timespeed_;

	// ��]����
	bool rotationDir_;
};

