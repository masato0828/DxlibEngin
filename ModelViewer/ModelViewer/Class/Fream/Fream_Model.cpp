#include "Fream_Model.h"
#include "../../imGui/imgui.h"
#include "../Common/Utility.h"

Fream_Model::Fream_Model():freamData_(nullptr,L"")
{
	Init();
}

Fream_Model::~Fream_Model()
{
}

void Fream_Model::Init()
{
	// �I�����Ă��鐔
	nowSelect = nullptr;

	nowSelect = &freamData_;

	nowSelectFreamName_ = L"";

	freamNumber_ = -1;
}

void Fream_Model::Update()
{
	// �v�f�I��p�E�B���h�E
	if (ImGui::Begin("Items", NULL, ImGuiWindowFlags_NoCollapse))
	{
		// �t�@�C���̖��O����
		for (auto& handleM : handleMap_)
		{
			Tree(handleM);
		}
	}
	ImGui::End();
}

void Fream_Model::SetModelHandle(std::map<std::wstring, int> handle)
{
	handleMap_ = handle;
	for (auto & handleM:handleMap_)
	{
		if (sclMap_.count(handleM.first))
		{
			continue;
		}
		sclMap_.emplace(handleM.first,Vector3(1,1,1));
		posMap_.emplace(handleM.first,Vector3(0,0,0));
		rotMap_.emplace(handleM.first,Vector3(0,0,0));

		freamData_.freamMap_.try_emplace(handleM.first, 
			FreamData(&freamData_, handleM.first));

		//freamData_.freamMap_ = handleM.first;
	}

	
	

}

void Fream_Model::Draw()
{
	if (handleMap_.empty())
	{
		return;
	}

	if (nowSelect->myName == L"")
	{
		return;
	}
	//auto selectHandle = GetElementNum(handleMap_, selected);

    ////MV1SetScale(modelHandle.second, scl.toVECTOR());
    

    for (auto& modelHandle : handleMap_)
    {
        MV1_REF_POLYGONLIST RefPoly;

        // ���f���̑S�t���[���̃|���S���̏����擾
        RefPoly = MV1GetReferenceMesh(modelHandle.second, -1, TRUE);

        auto minRP = RefPoly.MinPosition;
        auto maxRP = RefPoly.MaxPosition;

        VECTOR a1 = { minRP.x,minRP.y,minRP.z };
        VECTOR a2 = { maxRP.x,minRP.y,minRP.z };
        VECTOR a3 = { minRP.x,maxRP.y,minRP.z };
        VECTOR a4 = { maxRP.x,maxRP.y,minRP.z };
        VECTOR a5 = { minRP.x,minRP.y,maxRP.z };
        VECTOR a6 = { maxRP.x,minRP.y,maxRP.z };
        VECTOR a7 = { minRP.x,maxRP.y,maxRP.z };
        VECTOR a8 = { maxRP.x,maxRP.y,maxRP.z };

        int lineCol = 0x000000;
        DrawLine3D(a1, a3, lineCol);
        DrawLine3D(a1, a2, lineCol);
        DrawLine3D(a1, a5, lineCol);
        DrawLine3D(a2, a4, lineCol);
        DrawLine3D(a2, a6, lineCol);
        DrawLine3D(a3, a7, lineCol);
        DrawLine3D(a3, a4, lineCol);
        DrawLine3D(a4, a8, lineCol);
        DrawLine3D(a5, a6, lineCol);
        DrawLine3D(a6, a8, lineCol);
        DrawLine3D(a7, a8, lineCol);
        DrawLine3D(a7, a5, lineCol);

        //DrawSphere3D(minRP, 10, 10, lineCol, lineCol, true);
        //DrawSphere3D(maxRP, 10, 10, lineCol, lineCol, true);
		MV1SetScale(modelHandle.second, sclMap_.at(modelHandle.first).toVECTOR());
		MV1SetPosition(modelHandle.second, posMap_.at(modelHandle.first).toVECTOR());
		MV1SetRotationXYZ(modelHandle.second, rotMap_.at(modelHandle.first).toVECTOR());
        
        MV1DrawModel(modelHandle.second);
        MV1RefreshReferenceMesh(modelHandle.second, -1, TRUE);
    }
}

void Fream_Model::CustomStatus()
{
	if (handleMap_.empty())
	{
		return;
	}

	if (nowSelect->myName == L"")
	{
		return;
	}

	if (freamNumber_ == -1)
	{

		ImGui::SetCursorPos(ImVec2(30, 50));
		ImGui::Text("scl");

		ImGui::SetCursorPos(ImVec2(30, 90));
		ImGui::Text("pos");

		ImGui::SetCursorPos(ImVec2(30, 130));
		ImGui::Text("rot");

		ImGui::SetCursorPos(ImVec2(70, 40));
		ImGui::DragFloat3("##scl", &sclMap_.at(nowSelect->myName));

		ImGui::SetCursorPos(ImVec2(70, 80));
		ImGui::DragFloat3("##pos", &posMap_.at(nowSelect->myName));

		ImGui::SetCursorPos(ImVec2(70, 120));
		ImGui::DragFloat3("##rot", &rotMap_.at(nowSelect->myName));
	}
	else
	{
		ImGui::SetCursorPos(ImVec2(30, 50));
		ImGui::Text("scl");

		ImGui::SetCursorPos(ImVec2(30, 90));
		ImGui::Text("pos");

		ImGui::SetCursorPos(ImVec2(30, 130));
		ImGui::Text("rot");

		ImGui::SetCursorPos(ImVec2(70, 40));
		ImGui::DragFloat3("##scl", &freamSclVec_.at(freamNumber_));

		ImGui::SetCursorPos(ImVec2(70, 80));
		ImGui::DragFloat3("##pos", &freamPosVec_.at(freamNumber_));

		ImGui::SetCursorPos(ImVec2(70, 120));
		ImGui::DragFloat3("##rot", &freamRotVec_.at(freamNumber_));

		Fream();
	}

	
}

void Fream_Model::Fream()
{
	auto selectHandle = handleMap_.at(nowSelect->parentFream_->myName);
	MATRIX mat = MV1GetFrameLocalMatrix(selectHandle,freamNumber_);

	// �t���[���̍s�񂩂�g�嗦�s��̎擾
	auto scl = MGetSize(mat);

	// �t���[���̍s�񂩂��]�s��̎擾
	auto rot = MGetRotElem(mat);

	// �t���[���̍s�񂩂畽�s�ړ��s��̎擾
	auto pos = MGetTranslateElem(mat);

	// �g�嗦�p�P�ʍs��̎擾
	MATRIX mixMat = MGetIdent();

	//////////�g�嗦///////////////
	{
		auto scale = freamSclVec_.at(freamNumber_);

		// �g�嗦�s��̏�Z
		mixMat = MMult(mixMat, MGetScale(scale.toVECTOR()));
	}

	////////// ��]///////////////
	{
		// ��]�p�P�ʍs��̎擾
		MATRIX mixRot = MGetIdent();
		//// ��]�s��O�v�f�̏�Z�@
		mixRot = MMult(mixRot, MGetRotX(freamRotVec_.at(freamNumber_).x_));
		mixRot = MMult(mixRot, MGetRotY(freamRotVec_.at(freamNumber_).y_));
		mixRot = MMult(mixRot, MGetRotZ(freamRotVec_.at(freamNumber_).z_));
		// ��]�s��Ƃ̏�Z(�g�嗦�A��])
		mixMat = MMult(mixMat, mixRot);
	}
	
	//////////�ʒu///////////////
	{
		auto position = freamPosVec_.at(freamNumber_);
		// ���s�ړ��s��Ƃ̏�Z(�g�嗦�A��]�A���s�ړ�)
		mixMat = MMult(mixMat, MGetTranslate(position.toVECTOR()));
	}

	// �t���[���̍s��Ƃ���
	MV1SetFrameUserLocalMatrix(selectHandle, freamNumber_, mixMat);
}

void Fream_Model::Tree(std::pair<const std::wstring, int>& handleData)
{
	ImGuiTreeNodeFlags node_flags = 0;

	auto data = freamData_.freamMap_.try_emplace(handleData.first, FreamData(&freamData_, handleData.first)).first;

	bool is_selected = &data->second.myName == &handleData.first;

	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	auto freamNum = MV1GetFrameNum(handleData.second);

	if (freamNum <= 0)
	{
		node_flags = node_flags |= ImGuiTreeNodeFlags_Leaf;
	}
	else
	{
		node_flags & ~ImGuiTreeNodeFlags_Leaf;
	}

	auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags,Utility::WideStringToString(handleData.first).c_str());
	
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		// ���ݑI��
		nowSelect = &data->second;
		nowSelectFreamName_ = handleData.first;
		freamNumber_ = -1;
	}

	if (is_open)
	{
		nowSelect->parentFream_->myName = handleData.first;

		ImGuiTreeNodeFlags node_fream_flags = 0;

		int freamNum = MV1GetFrameNum(handleData.second);
		for (auto itr = 0; itr < freamNum; itr++)
		{
			auto freamName = MV1GetFrameName(handleData.second, itr);
			std::filesystem::path fileNameP = freamName;

			auto data = freamData_.freamMap_.try_emplace(fileNameP,
				FreamData(&freamData_, fileNameP)).first;

			bool is_fream_selected = freamNumber_ == itr;

			if (is_fream_selected)
				node_fream_flags |= ImGuiTreeNodeFlags_Selected;

			auto childNum = MV1GetFrameChildNum(handleData.second, itr);

			if (childNum <= 0)
			{
				node_fream_flags = node_fream_flags |= ImGuiTreeNodeFlags_Leaf;
			}
			else
			{
				node_fream_flags & ~ImGuiTreeNodeFlags_Leaf;
			}

			auto is = ImGui::TreeNodeEx((void*)&data->second, node_fream_flags, fileNameP.u8string().c_str());
			
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				nowSelect = &data->second;
				nowSelectFreamName_ = fileNameP;
				freamNumber_ = itr;
			}

			// ���[�J���s����擾
		MATRIX matrix = MV1GetFrameLocalMatrix(handleData.second, itr);

		// �ʒu���擾
		Vector3 position;
		position.x_ = matrix.m[3][0];
		position.y_ = matrix.m[3][1];
		position.z_ = matrix.m[3][2];

		// ��]���擾
		Vector3 rotation;
		rotation.x_ = atan2f(matrix.m[2][1], matrix.m[2][2]);
		rotation.y_ = atan2f(-matrix.m[2][0], sqrtf(matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]));
		rotation.z_ = atan2f(matrix.m[1][0], matrix.m[0][0]);

		// �g�嗦���擾
		Vector3 scale;
		scale.x_ = sqrtf(matrix.m[0][0] * matrix.m[0][0] + matrix.m[0][1] * matrix.m[0][1] + matrix.m[0][2] * matrix.m[0][2]);
		scale.y_ = sqrtf(matrix.m[1][0] * matrix.m[1][0] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[1][2] * matrix.m[1][2]);
		scale.z_ = sqrtf(matrix.m[2][0] * matrix.m[2][0] + matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]);
	
		freamSclVec_.push_back(scale);
		freamPosVec_.push_back(position);
		freamRotVec_.push_back(rotation);

			if (is)
			{
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

}


//// �m�[�h���J���Ă��邩�ǂ���
	//		// �c���[�f�[�^�̐ݒ�
	//ImGuiTreeNodeFlags node_flags = 0;

	//auto data = freamData_.freamMap_.try_emplace(handleData.first,
	//	FreamData(&freamData_, handleData.first)).first;

	//const bool is_selected = &data->second == nowSelect;

	//// �I��
	//if (is_selected)
	//	node_flags |= ImGuiTreeNodeFlags_Selected;

	//auto freamNum = MV1GetFrameNum(handleData.second);

	////// �t���[�����Ȃ��ꍇ
	//if (freamNum <= 0)
	//{
	//	node_flags = node_flags |= ImGuiTreeNodeFlags_Leaf;
	//}
	//else
	//{
	//	node_flags & ~ ImGuiTreeNodeFlags_Leaf;
	//}

	//if (Utility::IsMatch(Utility::WideStringToString(handleData.first), nowSelectFreamName_))
	//{
	//	// �c���[���J��
	//	ImGui::SetNextItemOpen(true);
	//}
	//// �m�[�h���J���Ă��邩�ǂ���
	////auto is_open = ImGui::TreeNodeEx((void*)&dataItr->second, node_flags, directoryName.c_str());
	//auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags,
	//	Utility::WideStringToString(handleData.first).c_str());

	//// �c���[��I��
	//if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	//{
	//	// ���ݑI��
	//	nowSelect = &data->second;
	//	nowSelectFreamName_ = handleData.first;
	//	freamNumber_ = -1;
	//}

	//// �m�[�h���J����Ă���
	//if (is_open)
	//{
	//	auto freamNum = MV1GetFrameNum(handleData.second);
	//	for (auto itr = 0; itr < freamNum; itr++)
	//	{
	//		auto freamName = MV1GetFrameName(handleData.second, itr);
	//		std::filesystem::path fileNameP = freamName;

	//		auto childNum = MV1GetFrameChildNum(handleData.second, itr);
	//		
	//		auto data = freamData_.freamMap_.try_emplace(fileNameP,
	//			FreamData(&freamData_, fileNameP)).first;

	//		const bool is_selected = &data->second == nowSelect;

	//		// �I��
	//		if (is_selected)
	//			node_flags |= ImGuiTreeNodeFlags_Selected;
	//		
	//		//// �t���[�����Ȃ��ꍇ
	//		if (childNum <= 0)
	//		{
	//			node_flags = node_flags |= ImGuiTreeNodeFlags_Leaf;
	//		}
	//		else
	//		{
	//			node_flags & ~ImGuiTreeNodeFlags_Leaf;
	//		}
	//		//std::pair<const std::wstring, int> pair(fileNameP, childNum);
	//		//Tree(pair);
	//		auto is = ImGui::TreeNodeEx((void*)&data->second, node_flags,fileNameP.u8string().c_str());

	//			// ���[�J���s����擾
	//		MATRIX matrix = MV1GetFrameLocalMatrix(handleData.second, itr);

	//		// �ʒu���擾
	//		Vector3 position;
	//		position.x_ = matrix.m[3][0];
	//		position.y_ = matrix.m[3][1];
	//		position.z_ = matrix.m[3][2];

	//		// ��]���擾
	//		Vector3 rotation;
	//		rotation.x_ = atan2f(matrix.m[2][1], matrix.m[2][2]);
	//		rotation.y_ = atan2f(-matrix.m[2][0], sqrtf(matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]));
	//		rotation.z_ = atan2f(matrix.m[1][0], matrix.m[0][0]);

	//		// �g�嗦���擾
	//		Vector3 scale;
	//		scale.x_ = sqrtf(matrix.m[0][0] * matrix.m[0][0] + matrix.m[0][1] * matrix.m[0][1] + matrix.m[0][2] * matrix.m[0][2]);
	//		scale.y_ = sqrtf(matrix.m[1][0] * matrix.m[1][0] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[1][2] * matrix.m[1][2]);
	//		scale.z_ = sqrtf(matrix.m[2][0] * matrix.m[2][0] + matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]);
	//	
	//		// �c���[��I��
	//		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	//		{
	//			// ���ݑI��
	//			//nowSelect = &data->second;
	//			nowSelectFreamName_ = fileNameP;
	//			freamNumber_ = itr;
	//		}

	//		if (is)
	//		{
	//			ImGui::TreePop();
	//		}
	//	}

	//	// �ċN����
	//	//Tree2(itr->path(), dataItr->second);
	//	//Tree(itr->path(), data->second, nowSelect);
	//	// �c���[�̏I��
	//	ImGui::TreePop();
	//}

	//