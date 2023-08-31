#pragma once
#include "../Common/Vector2.h"
class Device_Mouse
{
public:
	Device_Mouse();
	~Device_Mouse();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="screenLeftUp">�E�B���h�E�̍���</param>
	/// <param name="screenRightDown">�E�B���h�E�̉E��</param>
	/// <param name="defaultScreenSize">�E�B���h�E�T�C�Y</param>
	void Update(Vector2Flt screenLeftUp, Vector2Flt screenRightDown, Vector2 defaultScreenSize);
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �}�E�X�̈ʒu�̃Z�b�g 
	/// </summary>
	/// <param name="point">�}�E�X�ʒu</param>
	void SetPoint(Vector2 point);
	
	/// <summary>
	/// 1�t���[���Ԃœ���������
	/// </summary>
	void MoveOneFreamVolume();
	
	/// <summary>
	/// �}�E�X�̈ʒu�̎擾
	/// </summary>
	/// <returns>�}�E�X�̈ʒu</returns>
	Vector2 GetNowPoint();

	/// <summary>
	/// �V�[�����ł̃}�E�X�̈ʒu�̍쐬 
	/// </summary>
	void SceneMouseCreate();

	/// <summary>
	/// �V�[�����ł̃}�E�X�̈ʒu�̎擾
	/// </summary>
	/// <returns>�V�[�����ł̃}�E�X�̈ʒu</returns>
	Vector2Flt GetSceneMousePoint();
	
	/// <summary>
	/// �X�N���[�����W�̃}�E�X�ʒu�̎擾
	/// </summary>
	/// <returns>�X�N���[�����W�̃}�E�X�ʒu</returns>
	Vector2Flt GetScreenMousePoint();
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

