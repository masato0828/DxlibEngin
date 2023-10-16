#include "Fream_Model.h"
#include "../../imGui/imgui.h"
#include "../Common/ImGuiMyCustom.h"
#include "../Common/Utility.h"
#include "../Common/Shader/ShaderMng.h"
#include"../Common/FileDialog.h"
#include "../../strconv/strconv.h"

Fream_Model::Fream_Model():freamData_(nullptr,L"")
{
	Init();
}

Fream_Model::~Fream_Model()
{
	MV1DeleteModel(skyDomeHnadle_);
	for (auto &m:model_)
	{
		MV1RefreshReferenceMesh(m.second.handle, -1, TRUE);
		MV1DeleteModel(m.second.handle);

		for (auto& tex:m.second.textureHnadle)
		{
			if (tex.second.second == -1)
			{
				continue;
			}
			DeleteGraph(tex.second.second);
		}
	}
}

void Fream_Model::Init()
{
	// 選択している数
	nowSelect_ = nullptr;

	nowSelect_ = &freamData_;

	nowSelectFreamName_ = L"";

	freamNumber_ = -1;

	contextMenuFlg_ = false;

	handleCnt_ = 0;

	skyDomeHnadle_ = MV1LoadModel("data/modelData/SkyDome/Skydome.mv1");
	//SetModelPath("data/modelData/SkyDome/Skydome.mv1");
}

void Fream_Model::Update()
{
	// 要素選択用ウィンドウ
	if (ImGui::Begin("Items", NULL, ImGuiWindowFlags_NoCollapse))
	{
		// ファイルの名前分回す
		for (auto& handleM : model_)
		{
			ImGuiTreeNodeFlags node_flags =
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_OpenOnDoubleClick|
				ImGuiTreeNodeFlags_SpanAvailWidth;

			auto data = freamData_.freamMap_.try_emplace(handleM.first,
				FreamData(&freamData_, handleM.first)).first;

			const bool is_selected = &data->second == nowSelect_;

			// 選択中
			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

			// ノードを開いているかどうか
			auto is = ImGui::TreeNodeEx((void*)&data->second, node_flags, Utility::WStringToUTF8(handleM.first).c_str());

			// ツリーを選択
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				// 現在選択中
				nowSelect_ = &data->second;
				freamNumber_ = -1;
				nowSelectFreamName_ = handleM.first;
			}

			if (is)
			{
				Tree(handleM,data->second,nowSelect_);
				ImGui::TreePop();
			}

			// ボタンが右クリックされ、かつアイテムの上にカーソルがある場合
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
			{
				// 右クリックされたファイル名を保持し、コンテキストメニューのフラグを立てる
				//fileName_ = name.c_str();
				selectModelName_ = handleM.first;
				contextMenuFlg_ = true;
			}

			// 左クリックまたは右クリックされた場合
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				// 他のアイテムやウィンドウがホバーされておらず、カーソルがどこにもない場合、コンテキストメニューのフラグを解除する
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
	
	std::wstring duplicateName;

	std::filesystem::path filePath = path;
	auto ext = filePath.extension().wstring().substr(1);
	auto fileName = filePath.filename();

	if (ext == L"mv1")
	{
		//モデルが存在していない
		if(!model_.count(fileName))
		{
			handle = MV1LoadModel(filePath.string().c_str());
			freamData_.freamMap_.try_emplace(fileName,
				FreamData(&freamData_, fileName));

			Model model = CreateModelData(handle,fileName);

			model_.emplace(fileName, model);
			defoModelData_.emplace(fileName, model);
			LoadShaderProc(fileName);
			chackName_.push_back(fileName);
			return;
		}
	}

	for (auto name: chackName_)
	{
		handleCnt_++;
		//同じ名前のモデルがすでに存在しているとき
		if (model_.count(name))
		{
			auto fileHandle = model_.at(fileName.filename()).handle;
			auto name = fileName.wstring() + L"_" + std::to_wstring(handleCnt_);
			handle = MV1DuplicateModel(fileHandle);

			freamData_.freamMap_.try_emplace(name,
				FreamData(&freamData_, name));

			Model modelData = CreateModelData(handle,name);

			model_.emplace(name, modelData);

			defoModelData_.emplace(name, modelData);
			LoadShaderProc(name);

			chackName_.push_back(name);
			break;
		}
		
	}
}

void Fream_Model::Draw(int cubeTexture)
{
	if (model_.empty())
	{
		return;
	}

    for (auto& modelHandle : model_)
    {
		

        MV1_REF_POLYGONLIST RefPoly;

        // モデルの全フレームのポリゴンの情報を取得
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

		if (modelHandle.second.isShader)
		{
		
			lpShaderMng.DrawBegin(modelHandle.first);

			modelHandle.second.textureHnadle[SLOT_TYPE::CUBE_MAP].second = cubeTexture;

			// テクスチャの入力
			for (auto& tex : modelHandle.second.textureHnadle)
			{
				if (tex.second.second == -1)
				{
					continue;
				}
				lpShaderMng.SetModelTexture(tex.first, tex.second.second);
			}
		}

		MV1DrawModel(modelHandle.second.handle);
		//lpShaderMng.Draw(modelHandle.first,modelHandle.second.handle);

		if (modelHandle.second.isShader)
		{
			// テクスチャの入力
			for (auto& tex : modelHandle.second.textureHnadle)
			{
				if (tex.second.second == -1)
				{
					continue;
				}
				lpShaderMng.EndTextere(tex.first);
			}
			lpShaderMng.DrawEnd();


			MV1RefreshReferenceMesh(modelHandle.second.handle, -1, TRUE);

			int index = 0;
			for (auto material : modelHandle.second.material)
			{
				MV1SetMaterialDifColor(modelHandle.second.handle, index, material.color.at(COLOR_TYPE::DIF));
				MV1SetMaterialSpcColor(modelHandle.second.handle, index, material.color.at(COLOR_TYPE::SPC));
				MV1SetMaterialAmbColor(modelHandle.second.handle, index, material.color.at(COLOR_TYPE::AMB));
				MV1SetMaterialEmiColor(modelHandle.second.handle, index, material.color.at(COLOR_TYPE::EMI));
				MV1SetMaterialSpcPower(modelHandle.second.handle, index, material.spcPower);
				index++;
			}
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
	if (ImGui::TreeNode("spcPower"))
	{
		for (auto& m : model_.at(nowSelectFreamName_).material)
		{
			std::string materialName = Utility::WideStringToString(m.materialName);
			std::string spcPName = materialName + "spcPower";
			ImGui::SliderFloat(spcPName.c_str(), &m.spcPower, 0.0f, 256.0f);
		}
		ImGui::TreePop();
	}


	if (model_.at(nowSelectFreamName_).isShader)
	{
		lpShaderMng.Updater(model_.at(nowSelectFreamName_).name);

		if (ImGui::TreeNode("Texture"))
		{
			for (auto& m : model_.at(nowSelectFreamName_).textureHnadle)
			{
				LoadTexture(m.second.first, m.first);
			}
			ImGui::TreePop();
		}
	}

	ImGui::Checkbox("isShader",&model_.at(nowSelectFreamName_).isShader);

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

	// フレームの行列から拡大率行列の取得
	auto scl = MGetSize(mat);

	// フレームの行列から回転行列の取得
	auto rot = MGetRotElem(mat);

	// フレームの行列から平行移動行列の取得
	auto pos = MGetTranslateElem(mat);

	// 拡大率用単位行列の取得
	MATRIX mixMat = MGetIdent();

	//////////拡大率///////////////
	{
		//auto scale = freamSclMap_.at(nowSelectFreamName_).at(freamNumber_);
		auto scale = model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramscl;

		// 拡大率行列の乗算
		mixMat = MMult(mixMat, MGetScale(scale.toVECTOR()));
	}

	////////// 回転///////////////
	{
		// 回転用単位行列の取得
		MATRIX mixRot = MGetIdent();
		//// 回転行列三要素の乗算　
		mixRot = MMult(mixRot, MGetRotX(model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramrot.x_));
		mixRot = MMult(mixRot, MGetRotY(model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramrot.y_));
		mixRot = MMult(mixRot, MGetRotZ(model_.at(nowSelectFreamName_).fream.at(freamNumber_).feramrot.z_));
		// 回転行列との乗算(拡大率、回転)
		mixMat = MMult(mixMat, mixRot);
	}
	
	//////////位置///////////////
	{
		auto position = model_.at(nowSelectFreamName_).fream.at(freamNumber_).ferampos;
		// 平行移動行列との乗算(拡大率、回転、平行移動)
		mixMat = MMult(mixMat, MGetTranslate(position.toVECTOR()));
	}

	// フレームの行列とする
	MV1SetFrameUserLocalMatrix(selectHandle.handle, freamNumber_, mixMat);
}

int Fream_Model::FileCnt(const std::wstring& fileName)
{
    if (model_.count(fileName))
    {
		handleCnt_++;
        FileCnt(fileName + L"_" + std::to_wstring(handleCnt_));
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

void Fream_Model::DrawSkyDome()
{
	static float rotationPos = 0;
	MV1SetRotationXYZ(skyDomeHnadle_,VGet(0,rotationPos += Utility::Deg2Rad(0.01f), 0));
	MV1SetScale(skyDomeHnadle_,VGet(100,100,100));
	MV1DrawModel(skyDomeHnadle_);
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
	lpShaderMng.LoadShader(name, "", "data/ShaderBinary/Pixel/ModelPixelShader.ps", 0,2);
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
		float spcPower = MV1GetMaterialSpcPower(handle,m);
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

void Fream_Model::LoadTexture(std::string tagName,SLOT_TYPE type)
{
	if (ImGui::Button(tagName.c_str()))
	{
		std::string name = "テクスチャ画像の読み込み";
		std::string filter = ".png";
		std::filesystem::path tPath = (OpenFileDialog()(nullptr,&name,&filter));
		auto strSjis = wide_to_sjis(tPath);
		auto castStr = strSjis.c_str();
		int handle = LoadGraph(castStr);
		model_.at(nowSelectFreamName_).textureHnadle[type].second = handle;
	}
}

Fream_Model::Model Fream_Model::CreateModelData(const int& handle, const std::wstring& modelName)
{
	Vector3 pos;
	Vector3 rot;
	Vector3 scl;
	std::vector<ModelFream>fream;

	// フレーム初期値-----------------------------------------------
	int freamNum = MV1GetFrameNum(handle);

	for (auto itr = 0; itr < freamNum; itr++)
	{
		// ローカル行列を取得
		MATRIX matrix = MV1GetFrameBaseLocalMatrix(handle, itr);

		// 位置を取得
		Vector3 position;
		position.x_ = matrix.m[3][0];
		position.y_ = matrix.m[3][1];
		position.z_ = matrix.m[3][2];

		// 回転を取得
		Vector3 rotation;
		rotation.x_ = -atan2f(matrix.m[2][1], matrix.m[2][2]);
		rotation.y_ = atan2f(-matrix.m[2][0], sqrtf(matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]));
		rotation.z_ = atan2f(matrix.m[1][0], matrix.m[0][0]);

		// 拡大率を取得
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

	std::map < SLOT_TYPE, std::pair<std::string, int>>textureHnadle = {
		{SLOT_TYPE::DEFFUSE,std::make_pair("deffuse",-1)},
		{SLOT_TYPE::NORMAL_MAP,std::make_pair("normal",-1)},
		{SLOT_TYPE::SHADOW_MAP_0,std::make_pair("shadow_map0",-1)},
		{SLOT_TYPE::SHADOW_MAP_1,std::make_pair("shadow_map1",-1)},
		{SLOT_TYPE::SHADOW_MAP_2,std::make_pair("shadow_map2",-1)},
		{SLOT_TYPE::SPECULAR_MAP,std::make_pair("specular_map",-1)},
		{SLOT_TYPE::SUB,std::make_pair("sub",-1)},
		{SLOT_TYPE::TOON_DEFFUSE_GRAD,std::make_pair("toon_deffuse",-1)},
		{SLOT_TYPE::TOON_RGB_TO_MAXRGB_VOLUME,std::make_pair("toon_rgb",-1)},
		{SLOT_TYPE::TOON_SPECULAR_GRAD,std::make_pair("toon_specular",-1)},
		{SLOT_TYPE::TOON_SPHERE_MAP,std::make_pair("toon_sphere",-1)},
		{SLOT_TYPE::CUBE_MAP,std::make_pair("cube_map",-1)},
		{SLOT_TYPE::SUB_0,std::make_pair("sub_0",-1)},
		{SLOT_TYPE::SUB_1,std::make_pair("sub_1",-1)},
		{SLOT_TYPE::SUB_2,std::make_pair("sub_2",-1)},
	};

	Model model = { modelName,handle,pos,rot,scl,fream,false,materials,allChangeColor,textureHnadle,false };

	return model;
}

Fream_Model::Model Fream_Model::GetSingleData()
{
	return model_.at(nowSelect_->myName);
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

		// 選択中
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		auto childNum = MV1GetFrameChildNum(handleData.second.handle, itr);

		// フレームがない場合
		if (childNum <= 0)
		{
			node_flags = node_flags |= ImGuiTreeNodeFlags_Leaf;
		}

		auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags, fileNameP.u8string().c_str());

		// ツリーを選択
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// 現在選択中
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
		// ボタン上で右クリックされたときの処理
		ImGui::OpenPopup("ContextMenu");

		// Popup();
		if (ImGui::BeginPopup("ContextMenu"))
		{
			// コンテキストメニューの内容をここに記述
			if (ImGui::MenuItem(u8"削除"))
			{
				MV1TerminateReferenceMesh(model_.at(selectModelName_).handle, -1, TRUE);
				MV1DeleteModel(model_.at(selectModelName_).handle);
				model_.erase(selectModelName_);

				auto result = std::find(chackName_.begin(), chackName_.end(),selectModelName_);
				chackName_.erase(result);

				if (selectModelName_ == nowSelectFreamName_)
				{
					nowSelectFreamName_ = L"";
				}
				contextMenuFlg_ = false;
			}
			ImGui::EndPopup();
		}
	}
}
