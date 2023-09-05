#pragma once
#include "PostEffect_Base.h"
#include "../../Common/Vector2.h"
#include "../../Common/ImGuiMyCustom.h"
class PostEffect_Transition_Circle :
    public PostEffect_Base
{
public:
	PostEffect_Transition_Circle();
	~PostEffect_Transition_Circle();
	void Init() override final;
	void Update() override final;
	void Draw(std::string name, const int imageHnadle) override final;
	void Custom()override final;
private:

	struct Transition_Circle
	{
		ImGuiCustom::IM_COLOR color;// �F
		Vector2Flt screenSize;// �X�N���[���T�C�Y
		Vector2Flt dir;// �傫���Ȃ����
		int div;// �~�̐�
		float val;// �o�ߎ���
		float width;// �~���傫���Ȃ�^�C�~���O�����p

		// 1.0�F��ʂ̃|�X�g�G�t�F�N�g������
		// 0.0�F��ʂ̃|�X�g�G�t�F�N�g�������Ȃ�
		float revers;
	};

	// �p�����[�^�Ǘ�
	Transition_Circle pram_;

	// ����
	float timeSpeed_;
};

