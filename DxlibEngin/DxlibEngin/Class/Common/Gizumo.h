#pragma once
#include "Geometry.h"
#include<memory>
#include <array>
#include <map>
#include "Shader\ShaderMng.h"
class Gizumo
{
public:
	Gizumo();
	~Gizumo();

	

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="sceneMousePoint">�}�E�X�̃X�N���[�����W</param>
	/// <param name="modelPos">���f���̈ʒuor���f���̃t���[���ʒu</param>
	void Update(Vector2Flt sceneMousePoint,Vector3& modelPos);
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
private:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	Vector3 pos;	// �ʒu
	VECTOR start;	// �J�������C�n�_
	VECTOR end;		// �J�������C�I�_

	// �e���W�̖_�̍\����
	struct Stick
	{
		Vector3 front;	// �ő咸�_
		Vector3 back;	// �ŏ����_
		int color;		// �F
	};

	// �_�̃^�C�v
	enum STICK_TYPE
	{
		X,
		Y,
		Z,
		MAX
	};

	// �_�̊Ǘ��ϐ�
	std::array<Stick,STICK_TYPE::MAX> stick_;

	// �I�����Ă���_
	STICK_TYPE selectStick_;

	// �_�̃f�t�H���g�J���[
	std::map<STICK_TYPE,int> defoStickColor_;

	// �}�E�X�֘A
	int NowInput, EdgeInput, PrevInput;
	int Catch;
	int CatchMouseX, CatchMouseY;
	VECTOR catchPos_;

	// �ʒu�֘A
	VECTOR Catch3DModelPosition;
	VECTOR Catch3DHitPosition;
	VECTOR Catch2DHitPosition;

	VECTOR ScreenPos;

	VECTOR resultMovePos_;


	// �I�����Ă���_
	STICK_TYPE selectStick2_;
};

