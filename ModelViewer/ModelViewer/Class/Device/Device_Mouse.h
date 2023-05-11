#pragma once
#include "../Common/Vector2.h"
class Device_Mouse
{
public:
	Device_Mouse();
	~Device_Mouse();
	void Init();
	void Update(Vector2Flt screenLeftUp, Vector2Flt screenRightDown, Vector2 defaultScreenSize);
	void Draw();
	// �}�E�X�̈ʒu�̃Z�b�g
	void SetPoint(Vector2 point);
	// 1�t���[���Ԃœ���������
	void MoveOneFreamVolume();
	// �}�E�X�̈ʒu�̎擾
	Vector2 GetNowPoint();
	// �V�[�����ł̃}�E�X�̈ʒu�̍쐬
	void SceneMouseCreate();
	// �V�[�����ł̃}�E�X�̈ʒu�̎擾
	Vector2Flt GetSceneMousePoint();
private:
	// �}�E�X�̍��W
	Vector2 point_;
	// 1�t���[���Ԃœ���������
	Vector2Flt oneFreamMoveVolume_;
	// �V�[���`��̍���
	Vector2Flt screenLeftUp_;
	// �V�[���`��̉E��
	Vector2Flt screenRightDown_;
	// �V�[���`��̌��̃T�C�Y
	Vector2 defaultScreenSize_;
	// �V�[�����ł̃}�E�X�̍��W
	Vector2Flt sceneMousePoint_;
};

