#include "Fream_Model.h"
#include "../../imGui/imgui.h"
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
	// 選択している数
	nowSelect = nullptr;

	nowSelect = &freamData_;

	nowSelectFreamName_ = L"";

	freamNumber_ = -1;

	contextMenuFlg_ = false;

	handleCnt_ = 0;

	nullPos_ = {0,0,0};

	SetModelPath("data/modelData/cube.mv1");
}

void Fream_Model::Update()
{
	// 要素選択用ウィンドウ
	if (ImGui::Begin("Items", NULL, ImGuiWindowFlags_NoCollapse))
	{
		// ファイルの名前分回す
		for (auto& handleM : handleMap_)
		{
			ImGuiTreeNodeFlags node_flags =
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_OpenOnDoubleClick|
				ImGuiTreeNodeFlags_SpanAvailWidth;

			auto data = freamData_.freamMap_.try_emplace(handleM.first,
				FreamData(&freamData_, handleM.first)).first;

			const bool is_selected = &data->second == nowSelect;

			// 選択中
			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

			// ノードを開いているかどうか
			auto is = ImGui::TreeNodeEx((void*)&data->second, node_flags, Utility::WStringToUTF8(handleM.first).c_str());

			// ツリーを選択
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				// 現在選択中
				nowSelect = &data->second;
				freamNumber_ = -1;
				nowSelectFreamName_ = handleM.first;
			}

			if (is)
			{
				Tree(handleM,data->second,nowSelect);
				ImGui::TreePop();
			}

			// ボタンが右クリックされ、かつアイテムの上にカーソルがある場合
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
			{
				// 右クリックされたファイル名を保持し、コンテキストメニューのフラグを立てる
				//fileName_ = name.c_str();
				deleteModelName_ = handleM.first;
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
		}
	}

	ContextMenu();

	ImGui::End();



	if (nowSelectFreamName_ == L"")
	{
		return;
	}

	if (is_rotation_.at(nowSelectFreamName_))
	{
		rotMap_.at(nowSelectFreamName_).y_ += Utility::Deg2Rad(1.0f);
	}
}

void Fream_Model::SetModelPath(const std::filesystem::path& path)
{
	if (path == "")
	{
		return;
	}

	std::filesystem::path filePath = path;
	auto ext = filePath.extension().wstring().substr(1);
	auto fileName = filePath.filename();

	if (ext == L"mv1")
	{
		//同じ名前のモデルが存在しているとき
		if (handleMap_.count(fileName))
		{
			FileCnt(fileName);
			auto fileHandle = handleMap_.at(fileName.filename());
			std::wstring name = fileName.wstring() + std::to_wstring(handleCnt_);
			handleMap_.emplace(name, MV1DuplicateModel(fileHandle));
		}
		else
		{
			handleMap_.emplace(fileName, MV1LoadModel(filePath.string().c_str()));
		}


	}

	for (auto& handleM : handleMap_)
	{
		if (sclMap_.count(handleM.first))
		{
			continue;
		}
		sclMap_.emplace(handleM.first, Vector3(1, 1, 1));
		posMap_.emplace(handleM.first, Vector3(0, 0, 0));
		rotMap_.emplace(handleM.first, Vector3(0, 0, 0));

		freamData_.freamMap_.try_emplace(handleM.first,
			FreamData(&freamData_, handleM.first));


		// フレーム初期値-----------------------------------------------
		int freamNum = MV1GetFrameNum(handleM.second);

		std::vector<Vector3> vpos;
		std::vector<Vector3> vrot;
		std::vector<Vector3> vscl;

		for (auto itr = 0; itr < freamNum; itr++)
		{
			// ローカル行列を取得
			MATRIX matrix = MV1GetFrameBaseLocalMatrix(handleM.second, itr);

			// 位置を取得
			Vector3 position;
			position.x_ = matrix.m[3][0];
			position.y_ = matrix.m[3][1];
			position.z_ = matrix.m[3][2];
			vpos.push_back(position);

			// 回転を取得
			Vector3 rotation;
			rotation.x_ = -atan2f(matrix.m[2][1], matrix.m[2][2]);
			rotation.y_ = atan2f(-matrix.m[2][0], sqrtf(matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]));
			rotation.z_ = atan2f(matrix.m[1][0], matrix.m[0][0]);
			vrot.push_back(rotation);

			// 拡大率を取得
			Vector3 scale;
			scale.x_ = sqrtf(matrix.m[0][0] * matrix.m[0][0] + matrix.m[0][1] * matrix.m[0][1] + matrix.m[0][2] * matrix.m[0][2]);
			scale.y_ = sqrtf(matrix.m[1][0] * matrix.m[1][0] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[1][2] * matrix.m[1][2]);
			scale.z_ = sqrtf(matrix.m[2][0] * matrix.m[2][0] + matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]);
			vscl.push_back(scale);

		}
		freamPosMap_.emplace(handleM.first, vpos);
		freamRotMap_.emplace(handleM.first, vrot);
		freamSclMap_.emplace(handleM.first, vscl);

		is_rotation_.emplace(handleM.first,false);


		//LoadShaderProc(Utility::WideStringToString(handleM.first));
	}

	
}

void Fream_Model::Draw()
{

	if (handleMap_.empty())
	{
		return;
	}

    for (auto& modelHandle : handleMap_)
    {
        MV1_REF_POLYGONLIST RefPoly;

        // モデルの全フレームのポリゴンの情報を取得
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

	if (nowSelectFreamName_ == L"")
	{
		return;
	}

	if (freamNumber_ > MV1GetFrameNum(handleMap_.at(nowSelectFreamName_)))
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
		ImGui::DragFloat3("##scl", &sclMap_.at(nowSelectFreamName_));

		ImGui::SetCursorPos(ImVec2(70, 80));
		ImGui::DragFloat3("##pos", &posMap_.at(nowSelectFreamName_));

		ImGui::SetCursorPos(ImVec2(70, 120));
		ImGui::DragFloat3("##rot", &rotMap_.at(nowSelectFreamName_),Utility::Deg2Rad(1.0f));
	}
	else
	{
		ImGui::SetCursorPos(ImVec2(70, 40));
		ImGui::DragFloat3("##scl", &freamSclMap_.at(nowSelectFreamName_).at(freamNumber_));

		ImGui::SetCursorPos(ImVec2(70, 80));
		ImGui::DragFloat3("##pos", &freamPosMap_.at(nowSelectFreamName_).at(freamNumber_));

		ImGui::SetCursorPos(ImVec2(70, 120));
		ImGui::DragFloat3("##rot", &freamRotMap_.at(nowSelectFreamName_).at(freamNumber_), Utility::Deg2Rad(1.0f));

		Fream();
	}

	ImGui::Checkbox("rotation", &is_rotation_.at(nowSelectFreamName_));
}

void Fream_Model::Fream()
{
	auto selectHandle = handleMap_.at(nowSelectFreamName_);
	MATRIX mat = MV1GetFrameLocalMatrix(selectHandle,freamNumber_);

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
		auto scale = freamSclMap_.at(nowSelectFreamName_).at(freamNumber_);

		// 拡大率行列の乗算
		mixMat = MMult(mixMat, MGetScale(scale.toVECTOR()));
	}

	////////// 回転///////////////
	{
		// 回転用単位行列の取得
		MATRIX mixRot = MGetIdent();
		//// 回転行列三要素の乗算　
		mixRot = MMult(mixRot, MGetRotX(freamRotMap_.at(nowSelectFreamName_).at(freamNumber_).x_));
		mixRot = MMult(mixRot, MGetRotY(freamRotMap_.at(nowSelectFreamName_).at(freamNumber_).y_));
		mixRot = MMult(mixRot, MGetRotZ(freamRotMap_.at(nowSelectFreamName_).at(freamNumber_).z_));
		// 回転行列との乗算(拡大率、回転)
		mixMat = MMult(mixMat, mixRot);
	}
	
	//////////位置///////////////
	{
		auto position = freamPosMap_.at(nowSelectFreamName_).at(freamNumber_);
		// 平行移動行列との乗算(拡大率、回転、平行移動)
		mixMat = MMult(mixMat, MGetTranslate(position.toVECTOR()));
	}

	// フレームの行列とする
	MV1SetFrameUserLocalMatrix(selectHandle, freamNumber_, mixMat);
}

int Fream_Model::FileCnt(const std::wstring& fileName)
{
    if (handleMap_.count(fileName))
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
		return freamPosMap_.at(nowSelectFreamName_).at(freamNumber_);
	}

	// model
	return posMap_.at(nowSelectFreamName_);
}

bool Fream_Model::IsModelSelect()
{
	if (handleMap_.empty())
	{
		return false;
	}

	if (nowSelect->myName == L"")
	{
		return false;
	}

	if (nowSelectFreamName_ == L"")
	{
		return false;
	}

	return true;
}

void Fream_Model::LoadShaderProc(const std::string& name)
{
	lpShaderMng.LoadShader(name, "", "", 0);

	auto it = handleMap_;
	auto rit = it.rbegin();

	lpShaderMng.SetSkiningVertex(name, rit->second,"");
}

void Fream_Model::Tree(std::pair<const std::wstring, int>& handleData, FreamData& freamData, FreamData*& nowSelect)
{
	int freamNum = MV1GetFrameNum(handleData.second);

	nowSelectFreamName_ = handleData.first;

	for (auto itr = 0; itr < freamNum; itr++)
	{
		ImGuiTreeNodeFlags node_flags = 0;

		auto freamName = MV1GetFrameName(handleData.second, itr);
		std::filesystem::path fileNameP = freamName;

		auto data = freamData.freamMap_.try_emplace(fileNameP, FreamData(&freamData, fileNameP)).first;

		const bool is_selected = &data->second == nowSelect;

		// 選択中
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		auto childNum = MV1GetFrameChildNum(handleData.second, itr);

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

			if (ImGui::MenuItem(u8"削除")) // ユニークなIDを指定する
			{
				sclMap_.erase(deleteModelName_);
				posMap_.erase(deleteModelName_);
				rotMap_.erase(deleteModelName_);
				freamSclMap_.erase(deleteModelName_);
				freamPosMap_.erase(deleteModelName_);
				freamRotMap_.erase(deleteModelName_);
				handleMap_.erase(deleteModelName_);

				if (deleteModelName_ == nowSelectFreamName_)
				{
					nowSelectFreamName_ = L"";
				}

				// オプション1が選択されたときの処理
				contextMenuFlg_ = false;
			}
			ImGui::EndPopup();
		}
	}
}
