#pragma once

class FreamMng
{
public:
	FreamMng();
	~FreamMng();

	// �X�V����
	void Update();
	// ImGui�`��I������
	void Render();
	// ImGui�I������
	void ShutDown();

private:
	// ������
	void Init();
	// ImGui�̃_�C���N�gX�ƃE�B���h�E�̍X�V
	void SysNewFream();
	// �E�B���h�E�̃X�^�C��
	void Style();


};

