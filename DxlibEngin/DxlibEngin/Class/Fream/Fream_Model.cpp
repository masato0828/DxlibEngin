#include "Fream_Model.h"
#include "../../imGui/imgui.h"
#include "../Common/ImGuiMyCustom.h"
#include "../Common/Utility.h"
#include "../Common/Shader/ShaderMng.h"

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
	nowSelect_ = nullptr;

	nowSelect_ = &freamData_;

	nowSelectFreamName_ = L"";

	freamNumber_ = -1;

	contextMenuFlg_ = false;

	handleCnt_ = 0;

	nullPos_ = {0,0,0};

	//SetModelPath("data/modelData/cube.mv1");
}

void Fream_Model::Update()
{
	// �v�f�I��p�E�B���h�E
	if (ImGui::Begin("Items", NULL, ImGuiWindowFlags_NoCollapse))
	{
		// �t�@�C���̖��O����
		for (auto& handleM : model_)
		{
			ImGuiTreeNodeFlags node_flags =
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_OpenOnDoubleClick|
				ImGuiTreeNodeFlags_SpanAvailWidth;

			auto data = freamData_.freamMap_.try_emplace(handleM.first,
				FreamData(&freamData_, handleM.first)).first;

			const bool is_selected = &data->second == nowSelect_;

			// �I��
			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

			// �m�[�h���J���Ă��邩�ǂ���
			auto is = ImGui::TreeNodeEx((void*)&data->second, node_flags, Utility::WStringToUTF8(handleM.first).c_str());

			// �c���[��I��
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				// ���ݑI��
				nowSelect_ = &data->second;
				freamNumber_ = -1;
				nowSelectFreamName_ = handleM.first;
			}

			if (is)
			{
				Tree(handleM,data->second,nowSelect_);
				ImGui::TreePop();
			}

			// �{�^�����E�N���b�N����A���A�C�e���̏�ɃJ�[�\��������ꍇ
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
			{
				// �E�N���b�N���ꂽ�t�@�C������ێ����A�R���e�L�X�g���j���[�̃t���O�𗧂Ă�
				//fileName_ = name.c_str();
				deleteModelName_ = handleM.first;
				contextMenuFlg_ = true;
			}

			// ���N���b�N�܂��͉E�N���b�N���ꂽ�ꍇ
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				// ���̃A�C�e����E�B���h�E���z�o�[����Ă��炸�A�J�[�\�����ǂ��ɂ��Ȃ��ꍇ�A�R���e�L�X�g���j���[�̃t���O����������
				if (!ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
				{
					contextMenuFlg_ = false;
				}
			}

			if (handleM.second.isRotation)
			{
				handleM.second.rot.y_ += Utility::Deg2Rad(1.0f);
				//rotMap_.at(nowSelectFreamName_).y_ += Utility::Deg2Rad(1.0f);
			}
		}
	}

	ContextMenu();

	ImGui::End();



	if (nowSelectFreamName_ == L"")
	{
		return;
	}

	
}

void Fream_Model::SetModelPath(const std::filesystem::path& path)
{
	if (path == "")
	{
		return;
	}

	int handle = 0;
	Vector3 pos;
	Vector3 rot;
	Vector3 scl;
	std::vector<ModelFream>fream;

	std::filesystem::path filePath = path;
	auto ext = filePath.extension().wstring().substr(1);
	auto fileName = filePath.filename();

	if (ext == L"mv1")
	{
		//�������O�̃��f�������݂��Ă���Ƃ�
		if (model_.count(fileName))
		{
			FileCnt(fileName);
			auto fileHandle = model_.at(fileName.filename()).handle;
			std::wstring name = fileName.wstring() + std::to_wstring(handleCnt_);
			handle = MV1DuplicateModel(fileHandle);
		}
		else
		{
			handle = MV1LoadModel(filePath.string().c_str());
		}

		handleCnt_ = 0;
	}
	freamData_.freamMap_.try_emplace(fileName,
		FreamData(&freamData_, fileName));

	std::wstring name = fileName;


	// �t���[�������l-----------------------------------------------
	int freamNum = MV1GetFrameNum(handle);

	for (auto itr = 0; itr < freamNum; itr++)
	{
		// ���[�J���s����擾
		MATRIX matrix = MV1GetFrameBaseLocalMatrix(handle, itr);

		// �ʒu���擾
		Vector3 position;
		position.x_ = matrix.m[3][0];
		position.y_ = matrix.m[3][1];
		position.z_ = matrix.m[3][2];

		// ��]���擾
		Vector3 rotation;
		rotation.x_ = -atan2f(matrix.m[2][1], matrix.m[2][2]);
		rotation.y_ = atan2f(-matrix.m[2][0], sqrtf(matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]));
		rotation.z_ = atan2f(matrix.m[1][0], matrix.m[0][0]);

		// �g�嗦���擾
		Vector3 scale;
		scale.x_ = sqrtf(matrix.m[0][0] * matrix.m[0][0] + matrix.m[0][1] * matrix.m[0][1] + matrix.m[0][2] * matrix.m[0][2]);
		scale.y_ = sqrtf(matrix.m[1][0] * matrix.m[1][0] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[1][2] * matrix.m[1][2]);
		scale.z_ = sqrtf(matrix.m[2][0] * matrix.m[2][0] + matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]);

		fream.push_back({ position, rotation, scale });
	}

	scl = Vector3(1, 1, 1);
	pos = Vector3(0, 0, 0);
	rot = Vector3(0, 0, 0);
	std::vector<Material> materials = CreateMaterialData(handle);

	std::map<COLOR_TYPE, bool>allChangeColor = { 
		{COLOR_TYPE::DIF,false}, 
		{COLOR_TYPE::SPC,false},
		{COLOR_TYPE::AMB,false},
		{COLOR_TYPE::EMI,false} };

	Model model = { name,handle,pos,rot,scl,fream,false,materials,allChangeColor};

	materials.clear();
	
	model_.emplace(fileName, model);
	defoModelData_.emplace(fileName, model);
	LoadShaderProc(fileName);

}

void Fream_Model::Draw()
{
	if (model_.empty())
	{
		return;
	}

    for (auto& modelHandle : model_)
    {
        MV1_REF_POLYGONLIST RefPoly;

        // ���f���̑S�t���[���̃|���S���̏����擾
        RefPoly = MV1GetReferenceMesh(modelHandle.second.handle, -1, TRUE);

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
		MV1SetScale(modelHandle.second.handle, modelHandle.second.scl.toVECTOR());
		MV1SetPosition(modelHandle.second.handle, modelHandle.second.pos.toVECTOR());
		MV1SetRotationXYZ(modelHandle.second.handle, modelHandle.second.rot.toVECTOR());

		

		lpShaderMng.DrawBegin(modelHandle.first);

		//MV1DrawModel(modelHandle.second);
		MV1DrawModel(modelHandle.second.handle);
		lpShaderMng.DrawEnd();
        //MV1RefreshReferenceMesh(modelHandle.second, -1, TRUE);
        MV1RefreshReferenceMesh(modelHandle.second.handle, -1, TRUE);

		int index = 0;
		for (auto material : modelHandle.second.material)
		{
			MV1SetMaterialDifColor(modelHandle.second.handle,index,material.color.at(COLOR_TYPE::DIF));
			MV1SetMaterialSpcColor(modelHandle.second.handle, index, material.color.at(COLOR_TYPE::SPC));
			MV1SetMaterialAmbColor(modelHandle.second.handle, index, material.color.at(COLOR_TYPE::AMB));
			MV1SetMaterialEmiColor(modelHandle.second.handle, index, material.color.at(COLOR_TYPE::EMI));
			index++;
		}
    }
}

void Fream_Model::CustomStatus()
{
	if (model_.empty())
	{
		return;
	}

	if (nowSelect_->myName == L"")
	{
		return;
	}

	if (nowSelectFreamName_ == L"")
	{
		return;
	}

	if (freamNumber_ > MV1GetFrameNum(model_.at(nowSelectFreamName_).handle))
	{
		freamNumber_ = -1;
	}

	ImGui::SetCursorPos(ImVec2(30, 50));
	ImGui::Text("scl");

	ImGui::SetCursorPos(ImVec2(30, 90));
	ImGui::Text("pos");

	ImGui::SetCursorPos(ImVec2(30, 130));
	ImGui::Text("rot");

	if (freamNumber_ == -1)
	{
		ImGui::SetCursorPos(ImVec2(70, 40));
		ImGui::DragFloat3("##scl", &model_.at(nowSelectFreamName_).scl);
		ImGui::SameLine();
		if (ImGui::Button("##sscl")) {model_.at(nowSelectFreamName_).scl = defoModelData_.at(nowSelectFreamName_).scl; };

		ImGui::SetCursorPos(ImVec2(70, 80));
		ImGui::DragFloat3("##pos", &model_.at(nowSelectFreamName_).pos);
		ImGui::SameLine();
		if (ImGui::Button("##spos")) {model_.at(nowSelectFreamName_).pos = defoModelData_.at(nowSelectFreamName_).pos; };

		ImGui::SetCursorPos(ImVec2(70, 120));
		ImGui::DragFloat3("##rot", &model_.at(nowSelectFreamName_).rot, Utility::Deg2Rad(1.0f));
		ImGui::SameLine();
		if (ImGui::Button("##srot")) {model_.at(nowSelectFreamName_).rot = defoModelData_.at(nowSelectFreamName_).rot; };
	}
	else
	{
		ImGui::SetCursorPos(ImVec2(70, 40));
		ImGui::DragFloat3("##scl", &model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramscl);

		ImGui::SetCursorPos(ImVec2(70, 80));
		ImGui::DragFloat3("##pos", &model_.at(nowSelectFreamName_).fream.at(freamNumber_).ferampos);

		ImGui::SetCursorPos(ImVec2(70, 120));
		ImGui::DragFloat3("##rot", &model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramrot, Utility::Deg2Rad(1.0f));

		Fream();
	}

	ImGui::Checkbox("rotation", &model_.at(nowSelectFreamName_).isRotation);

	ColorEdit("[dif]", COLOR_TYPE::DIF);
	ColorEdit("[spc]", COLOR_TYPE::SPC);
	ColorEdit("[amb]", COLOR_TYPE::AMB);
	ColorEdit("[emi]", COLOR_TYPE::EMI);

	lpShaderMng.Updater(model_.at(nowSelectFreamName_).name);
}

void Fream_Model::ColorEdit(const std::string& tagName,COLOR_TYPE type)
{
	
	if (ImGui::TreeNode(tagName.c_str()))
	{
		ImGui::Checkbox(tagName.c_str(), &model_.at(nowSelectFreamName_).allChangeColor.at(type));
		for (auto& m : model_.at(nowSelectFreamName_).material)
		{
			std::string materialName = Utility::WideStringToString(m.materialName);
			std::string difName = materialName + tagName.c_str();
			if (!model_.at(nowSelectFreamName_).allChangeColor.at(type))
			{	
				ImGuiCustom::ColorEdit4(difName.c_str(), &m.color[type]);
			}
		}

		if (model_.at(nowSelectFreamName_).allChangeColor.at(type))
		{
			auto chengeColor = model_.at(nowSelectFreamName_).material.at(0).color.at(type);
			ImGuiCustom::ColorEdit4(tagName.c_str(), &chengeColor);
			for (auto& m : model_.at(nowSelectFreamName_).material)
			{
				m.color[type] = chengeColor;
			}
		}
		ImGui::TreePop();
	}
}

void Fream_Model::AllColorEdit(const std::string& tagName)
{
	if (ImGui::TreeNode(tagName.c_str()))
	{
		for (auto& m : model_.at(nowSelectFreamName_).material)
		{
			std::string materialName = Utility::WideStringToString(m.materialName);

			//std::string difName = materialName + tagName.c_str();
			ImGuiCustom::ColorEdit4(materialName.c_str(), &m.color[COLOR_TYPE::DIF]);
			ImGuiCustom::ColorEdit4(materialName.c_str(), &m.color[COLOR_TYPE::SPC]);
			ImGuiCustom::ColorEdit4(materialName.c_str(), &m.color[COLOR_TYPE::AMB]);
			ImGuiCustom::ColorEdit4(materialName.c_str(), &m.color[COLOR_TYPE::EMI]);
		}
		ImGui::TreePop();
	}
}

void Fream_Model::Fream()
{
	auto selectHandle = model_.at(nowSelectFreamName_);
	MATRIX mat = MV1GetFrameLocalMatrix(selectHandle.handle,freamNumber_);

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
		//auto scale = freamSclMap_.at(nowSelectFreamName_).at(freamNumber_);
		auto scale = model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramscl;

		// �g�嗦�s��̏�Z
		mixMat = MMult(mixMat, MGetScale(scale.toVECTOR()));
	}

	////////// ��]///////////////
	{
		// ��]�p�P�ʍs��̎擾
		MATRIX mixRot = MGetIdent();
		//// ��]�s��O�v�f�̏�Z�@
		mixRot = MMult(mixRot, MGetRotX(model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramrot.x_));
		mixRot = MMult(mixRot, MGetRotY(model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramrot.y_));
		mixRot = MMult(mixRot, MGetRotZ(model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramrot.z_));
		// ��]�s��Ƃ̏�Z(�g�嗦�A��])
		mixMat = MMult(mixMat, mixRot);
	}
	
	//////////�ʒu///////////////
	{
		auto position = model_.at(nowSelectFreamName_).fream.at(freamNumber_).ferampos;
		// ���s�ړ��s��Ƃ̏�Z(�g�嗦�A��]�A���s�ړ�)
		mixMat = MMult(mixMat, MGetTranslate(position.toVECTOR()));
	}

	// �t���[���̍s��Ƃ���
	MV1SetFrameUserLocalMatrix(selectHandle.handle, freamNumber_, mixMat);
}

int Fream_Model::FileCnt(const std::wstring& fileName)
{
    if (model_.count(fileName))
    {
		handleCnt_++;
        FileCnt(fileName+std::to_wstring(handleCnt_));
    }
    return handleCnt_;
}
Vector3& Fream_Model::GetModelPos()
{
	Vector3 null = Vector3(0,0,0);

	if (freamNumber_ != -1)
	{
		// model fream
		return model_.at(nowSelectFreamName_).fream.at(freamNumber_).ferampos;
	}

	// model
	return model_.at(nowSelectFreamName_).pos;
}

bool Fream_Model::IsModelSelect()
{
	if (model_.empty())
	{
		return false;
	}

	if (nowSelect_->myName == L"")
	{
		return false;
	}

	if (nowSelectFreamName_ == L"")
	{
		return false;
	}

	return true;
}

void Fream_Model::LoadShaderProc(const std::wstring& name)
{
	lpShaderMng.LoadShader(name, "data/ShaderBinary/Vertex/ModelPixelShader.vs", "data/ShaderBinary/Pixel/ModelPixelShader.ps", 0);


	lpShaderMng.SetSkiningVertex(name, model_.at(name).handle);
}

std::vector<Fream_Model::Material> Fream_Model::CreateMaterialData(const int& handle)
{
	std::vector<Material> materials;
	std::map<COLOR_TYPE, COLOR_F> color;

	int totalNum = MV1GetMaterialNum(handle);
	for (int m = 0; m < totalNum; m++)
	{
		std::wstring name = Utility::MultiByteToUnicode(MV1GetMaterialName(handle,m));
		COLOR_F difColor = MV1GetMaterialDifColor(handle, m);
		COLOR_F spcColor = MV1GetMaterialSpcColor(handle, m);
		COLOR_F ambColor = MV1GetMaterialAmbColor(handle, m);
		COLOR_F emiColor = MV1GetMaterialEmiColor(handle, m);
		int spcPower = MV1GetMaterialSpcPower(handle,m);
		int difMapTex = MV1GetMaterialDifMapTexture(handle,m);
		int spcMapTex = MV1GetMaterialSpcMapTexture(handle, m);
		int normalMapTex = MV1GetMaterialNormalMapTexture(handle, m);

		//Material material = { name,difColor,spcColor,ambColor,emiColor,spcPower,difMapTex,spcMapTex,normalMapTex };
		
		color = {
			{COLOR_TYPE::DIF,difColor},
			{COLOR_TYPE::SPC,spcColor},
			{COLOR_TYPE::AMB,ambColor},
			{COLOR_TYPE::EMI,emiColor} };


		Material material = {name,color,spcPower,difMapTex,spcMapTex,normalMapTex };

		materials.push_back(material);
	}

	return materials;
}

void Fream_Model::Tree(std::pair<const std::wstring, Model>& handleData, FreamData& freamData, FreamData*& nowSelect)
{
	int freamNum = MV1GetFrameNum(handleData.second.handle);

	nowSelectFreamName_ = handleData.first;

	for (auto itr = 0; itr < freamNum; itr++)
	{
		ImGuiTreeNodeFlags node_flags = 0;

		auto freamName = MV1GetFrameName(handleData.second.handle, itr);
		std::filesystem::path fileNameP = freamName;

		auto data = freamData.freamMap_.try_emplace(fileNameP, FreamData(&freamData, fileNameP)).first;

		const bool is_selected = &data->second == nowSelect;

		// �I��
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		auto childNum = MV1GetFrameChildNum(handleData.second.handle, itr);

		// �t���[�����Ȃ��ꍇ
		if (childNum <= 0)
		{
			node_flags = node_flags |= ImGuiTreeNodeFlags_Leaf;
		}

		auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags, fileNameP.u8string().c_str());

		// �c���[��I��
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// ���ݑI��
			//nowSelect = &dataItr->second;
			nowSelect = &data->second;
			freamNumber_ = itr;
		}

		if (is_open)
		{
			ImGui::TreePop();
		}
	}

}

void Fream_Model::ContextMenu()
{
	if (contextMenuFlg_)
	{
		// �{�^����ŉE�N���b�N���ꂽ�Ƃ��̏���
		ImGui::OpenPopup("ContextMenu");

		// Popup();
		if (ImGui::BeginPopup("ContextMenu"))
		{
			// �R���e�L�X�g���j���[�̓��e�������ɋL�q

			if (ImGui::MenuItem(u8"�폜")) // ���j�[�N��ID���w�肷��
			{
				model_.erase(deleteModelName_);

				if (deleteModelName_ == nowSelectFreamName_)
				{
					nowSelectFreamName_ = L"";
				}

				// �I�v�V����1���I�����ꂽ�Ƃ��̏���
				contextMenuFlg_ = false;
			}
			ImGui::EndPopup();
		}
	}
}
