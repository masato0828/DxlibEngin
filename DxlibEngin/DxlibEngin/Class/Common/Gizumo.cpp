#include "Gizumo.h"
#include <map>
#include <string>
#include "../../imGui/imgui.h"

Gizumo::Gizumo()
{
	Init();
}

Gizumo::~Gizumo()
{
}

void Gizumo::Init()
{
	pos = {0,0,0};

	float size = 2.5f;

	Vector3 redBack = { -size,-size,-size };
	Vector3 redFront = { size,size,-80 };
	Vector3 blueBack = { -size,-size,-size };
	Vector3 blueFront = { -80,size,size };
	Vector3 greenBack = { -size,-size,-size };
	Vector3 greenFront = { size,80,size };

	defoStickColor_.emplace(STICK_TYPE::X, 0xff0000);
	defoStickColor_.emplace(STICK_TYPE::Y, 0x00ff00);
	defoStickColor_.emplace(STICK_TYPE::Z, 0x0000ff);
	stick_[STICK_TYPE::X] = {redBack,redFront,defoStickColor_[STICK_TYPE::X] };
	stick_[STICK_TYPE::Y] = {greenBack,greenFront,defoStickColor_[STICK_TYPE::Y] };
	stick_[STICK_TYPE::Z] = {blueBack,blueFront,defoStickColor_[STICK_TYPE::Z] };


	// �}�E�X�֘A�ϐ��̏�����
	NowInput = 0;
	EdgeInput = 0;
	PrevInput = 0;
	Catch = 0;
	
}

void Gizumo::Update(Vector2Flt sceneMousePoint,Vector3& modelPos)
{
	pos = modelPos;

	// �}�E�X�{�^���̓��͏�Ԃ��X�V
	PrevInput = NowInput;
	NowInput = GetMouseInput();
	EdgeInput = NowInput & ~PrevInput;

	

	// ���Ƀ��f����͂�ł��邩�ǂ����ŏ����𕪊�
	if (Catch == 0)
	{
		// �͂�ł��Ȃ��ꍇ

		// ���N���b�N���ꂽ�烂�f�����N���b�N�������𒲂ׂ�
		if (EdgeInput & MOUSE_INPUT_1)
		{
			

			// �}�E�X�̉�ʍ��W�� x, y �ɃZ�b�g
			ScreenPos.x = (float)sceneMousePoint.x_;
			ScreenPos.y = (float)sceneMousePoint.y_;

			// z �����ꂼ�� 0.0f �� 1.0f �ɂ���2�� ConvScreenPosToWorldPos ���Ă�
			ScreenPos.z = 0.0f;
			start = ConvScreenPosToWorldPos(ScreenPos);

			ScreenPos.z = 1.0f;
			end = ConvScreenPosToWorldPos(ScreenPos);

			for (int i = 0; i < stick_.size(); i++)
			{
				auto result = HitCheck_Line_Cube(start, end, (modelPos + stick_[i].back).toVECTOR(), (modelPos + stick_[i].front).toVECTOR());

				if (result.HitFlag)
				{
					// �͂�ł��邩�ǂ����̃t���O�𗧂Ă�
					Catch = 1;

					// �͂񂾂Ƃ��̃X�N���[�����W��ۑ�
					CatchMouseX = ScreenPos.x;
					CatchMouseY = ScreenPos.y;

					// �͂񂾂Ƃ��̃��f���̃��[���h���W��ۑ�
					Catch3DModelPosition = modelPos.toVECTOR();

					// �͂񂾂Ƃ��̃��f���Ɛ����������������W��ۑ�( ���W���X�N���[�����W�ɕϊ��������̂��ۑ����Ă��� )
					Catch3DHitPosition = result.Position;
					Catch2DHitPosition = ConvWorldPosToScreenPos(Catch3DHitPosition);

					selectStick_ = static_cast<STICK_TYPE>(i);
					break;
				}
				selectStick_ = STICK_TYPE::MAX;
			}
		}
	}
	else
	{
		// �͂�ł���ꍇ

		// �}�E�X�̍��N���b�N��������Ă�����͂ݏ�Ԃ�����
		if ((NowInput&MOUSE_INPUT_1) == 0)
		{
			Catch = 0;
		}
		else
		{
			// �͂ݏ�Ԃ��p�����Ă�����}�E�X�J�[�\���̈ړ��ɍ��킹�ă��f�����ړ�

			float MoveX = 0, MoveY = 0, MoveZ = 0;
			VECTOR NowCatch2DHitPosition;
			VECTOR NowCatch3DHitPosition;
			VECTOR Now3DModelPosition;

			// �͂񂾂Ƃ��̃}�E�X���W���猻�݂̃}�E�X���W�܂ł̈ړ������Z�o
			MoveX = (float)(ScreenPos.x - CatchMouseX);
			MoveY = (float)(ScreenPos.y - CatchMouseY);

			// �͂񂾂Ƃ��̃��f���Ɛ����������������W���X�N���[�����W�ɕϊ��������̂Ƀ}�E�X�̈ړ����𑫂�
			NowCatch2DHitPosition.x = Catch2DHitPosition.x + MoveX;
			//NowCatch2DHitPosition.y = Catch2DHitPosition.y + MoveY;
			NowCatch2DHitPosition.y = Catch2DHitPosition.y + MoveY;
			NowCatch2DHitPosition.z = Catch2DHitPosition.z + MoveZ;

			// �͂񂾂Ƃ��̃��f���Ɛ����������������W���X�N���[�����W�ɕϊ��������̂Ƀ}�E�X�̈ړ����𑫂������W�����[���h���W�ɕϊ�
			NowCatch3DHitPosition = ConvScreenPosToWorldPos(NowCatch2DHitPosition);

			// �͂񂾂Ƃ��̃��f���̃��[���h���W�Ɂw�͂񂾂Ƃ��̃��f���Ɛ����������������W�Ƀ}�E�X�̈ړ����𑫂������W�����[���h���W��
			// �ϊ��������W�x�ƁA�w�͂񂾂Ƃ��̃��f���Ɛ����������������W�x�Ƃ̍��������Z
			Now3DModelPosition.x = Catch3DModelPosition.x + NowCatch3DHitPosition.x - Catch3DHitPosition.x;
			Now3DModelPosition.y = Catch3DModelPosition.y + NowCatch3DHitPosition.y - Catch3DHitPosition.y;
			Now3DModelPosition.z = Catch3DModelPosition.z + NowCatch3DHitPosition.z - Catch3DHitPosition.z;

			modelPos.setVECTOR(Now3DModelPosition);
		}
	}

	
	ImGui::Begin("test");
	ImGui::Text("%f;%f;%f" , modelPos.x_, modelPos.y_, modelPos.z_);
	ImGui::End();

	for (int i = 0; i < stick_.size(); i++)
	{

		if (selectStick_ == static_cast<STICK_TYPE>(i))
		{
			stick_[static_cast<STICK_TYPE>(i)].color = 0x000000;
		}
		else
		{
			stick_[static_cast<STICK_TYPE>(i)].color = defoStickColor_[static_cast<STICK_TYPE>(i)];
		}
	}

}

void Gizumo::Draw()
{
	SetUseLighting(false);

	for (auto & [back,front,color]:stick_)
	{
		DrawCube3D((pos + back).toVECTOR(), (pos + front).toVECTOR(), color, color, true);
	}

	DrawCapsule3D(start,end,0.1f,10,0xffff00, 0xffff00,true);
	

	SetUseLighting(true);
}
