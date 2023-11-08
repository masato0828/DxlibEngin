#pragma once
#include "FreamBase.h"
#include "../Common/Geometry.h"
class Fream_Camera :
    public FreamBase
{
public:
    Fream_Camera();
    ~Fream_Camera();
	
	/// <summary>
	/// ������
	/// </summary>
	void Init()override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;
	
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="mousePoint">�}�E�X�̈ʒu</param>
	/// <param name="windowHlfeSize">�E�B���h�E�̔����̃T�C�Y</param>
	/// <param name="centerPoint">��ʂ̒���</param>
	void Update(Vector2 mousePoint,Vector2Flt windowHlfeSize,Vector2 centerPoint);
	
	/// <summary>
	/// �J�����̉�]���{�^���ő���
	/// </summary>
	void Angle();

	/// <summary>
	/// �J�����̈ړ����{�^���ő���
	/// </summary>
	void Move();

	/// <summary>
	/// �J�����̃j�A���t�@�[���Z�b�g
	/// </summary>
	/// <param name="cameraNear">�j�A�l</param>
	/// <param name="cameraFar">�t�@�[�l</param>
	void Set(int cameraNear = 10.0f,int cameraFar = 300000.0f);
	
	/// <summary>
	/// �}�E�X�̍���������Ă���Ƃ��Ƀ}�E�X�ɂ��J�����̉�]
	/// </summary>
	void PushMouseAngle();

	/// <summary>
	/// �}�E�X�ɂ��J�����̉�]
	/// </summary>
	/// <param name="centerPoint">��ʂ̐^��</param>
	void MouseAngle(Vector2 centerPoint);

	/// <summary>
	/// �J�X�^���p�����[�^�̕ύX
	/// </summary>
	void Custom();

	/// <summary>
	/// ��ʏ�̃}�E�X�̈ʒu�̎擾
	/// </summary>
	/// <returns>�}�E�X�̈ʒu</returns>
	Vector2 GetScreenMouse();

	/// <summary>
	/// �J�����̈ʒu�̎擾
	/// </summary>
	/// <returns>�J�����̈ʒu</returns>
	Vector3 GetCameraPos();
private:
	// �ʒu
	Vector3 pos_;
	// ��]
	Vector3 rot_;

	// �����_�ʒu���
	Vector3 targetPos_;

	// �}�E�X�p
	Vector2Dbl aftorMousePoint_;	// �X�V�ʒu
	Vector2Dbl beforMousePoint_;	// �X�V�O�ʒu
	Vector2Dbl mousePointDiff_;		// �X�V����
	float sens_;	// �}�E�X���x
	int mouseX_;	// �}�E�X��X���W
	int mouseY_;	// �}�E�X��Y���W
	int input_;		// �}�E�X�̉������

	// �ړ��X�s�[�h
	float moveSpeed_;
	// �ړ��X�s�[�h�i���x�����j
	int moveSpeedBoost_;

	// �E�B���h�E�̍���
	Vector2 correctWindowLeftTop_;
	// �E�B���h�E�̔����̃T�C�Y
	Vector2 windowHlfeSize_;

	// �}�E�X�̈ړ��t���O
	bool mouseMove_;
	// �e�L�X�g�\���_�ŃJ�E���g
	int cnt_;
	// �}�E�X�̈ʒu
	Vector2 mousePoint_;
};

