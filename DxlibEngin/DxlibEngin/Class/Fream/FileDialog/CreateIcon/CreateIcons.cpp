#include "CreateIcons.h"
#include "../../../../imGui/imgui.h"
#include "../../../Common/ImGuiMyCustom.h"
#include "../../../Common/Utility.h"

CreateIcons::CreateIcons()
{
	Init();
}

CreateIcons::~CreateIcons()
{
}

void CreateIcons::Init()
{
	fileImageShaderDatas_ = {
		{L"non",nullptr},
		{L"cpp",nullptr},
		{L"h",nullptr},
		{L"hlsl",nullptr},
		{L"hlsli",nullptr},
		{L"txt",nullptr},
		{L"hpp",nullptr},
		{L"vs",nullptr},
		{L"ps",nullptr},
		{L"file",nullptr},
		{L"test",nullptr},
		{L"unknown",nullptr},
	};

	int textureSizeX;
	int textureSizeY;
	ImguiSup::LoadBackGroundTextureFromFile(&fileImageShaderDatas_.at(L"non"), &textureSizeX, &textureSizeY);

	ImguiSup::LoadTextureFromFile("data/iconData/CPP.png", &fileImageShaderDatas_.at(L"cpp"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/h.png", &fileImageShaderDatas_.at(L"h"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/hlsl.png", &fileImageShaderDatas_.at(L"hlsl"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/hlsli.png", &fileImageShaderDatas_.at(L"hlsli"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/txt.png", &fileImageShaderDatas_.at(L"txt"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/file.png", &fileImageShaderDatas_.at(L"file"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/HPP.png", &fileImageShaderDatas_.at(L"hpp"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/vs.png", &fileImageShaderDatas_.at(L"vs"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/ps.png", &fileImageShaderDatas_.at(L"ps"), &textureSizeX, &textureSizeY);
	ImguiSup::LoadTextureFromFile("data/iconData/unknown.png", &fileImageShaderDatas_.at(L"unknown"), &textureSizeX, &textureSizeY);



	stage_ = std::make_unique<Fream_Stage>();

	CreateModelIcon("data/Bomber_2.mv1", L"test");

}

void CreateIcons::Update()
{
}

void CreateIcons::MakeFileImage(std::filesystem::path name, std::filesystem::path fileFullPath)
{
	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(80, 80);
	float buttonSpacing = 10.0f; // �{�^���Ԃ̃X�y�[�X

	

	// �{�^���̎��ʎq���擾
	ImGuiID buttonID = ImGui::GetID(name.c_str());

	// �{�^���̎��ʎq��Push���āA���̒��ŏ������s��
	//ImGui::PushID(buttonID);

	// �{�^���������ꂽ���������ϐ�
	bool buttonPressed = false;

	

	// FileAssignments�֐����Ăяo���A�t�@�C���̊��蓖�Ăƃ{�^���̕\�����s��
	FileAssignments(name, buttonPressed, { buttonSize.x,buttonSize.y }, fileFullPath);

	
}

void CreateIcons::FileAssignments(std::filesystem::path& name, bool& buttonPressed, Vector2Flt buttonSize, std::filesystem::path fileFullPath)
{
	ImGuiCustom::SetCustomStyle(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGuiCustom::SetCustomStyle(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.7f, 1.f));


	//auto u8name = name.u8string();
	auto u8fileName = name.filename().wstring();
	auto wfileName = Utility::WideStringToString(name.filename());


	if (std::filesystem::is_directory(name))
	{
		buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(L"file"), ImVec2(buttonSize.x_, buttonSize.y_));
	}
	else
	{
		// �g���q���Ȃ��ꍇ
		if (name.extension() == "")
		{
			ImGuiCustom::SetCustomStyle(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.4f, 1.f));
			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(L"unknown"), ImVec2(buttonSize.x_, buttonSize.y_));
			// �J�X�^���X�^�C�������ɖ߂�
			//ImGui::StyleColorsDark();
			ImGui::StyleColorsClassic();
			return;
		}

		auto ext = name.extension().wstring().substr(1);
		if (fileImageShaderDatas_.count(ext))
		{
			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(ext), ImVec2(buttonSize.x_, buttonSize.y_));
		}
		else if (fileImageShaderDatas_.count(u8fileName))
		{
			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(u8fileName), ImVec2(buttonSize.x_, buttonSize.y_));
		
			enum Mode
			{
				Mode_Copy,
				Mode_Move,
				Mode_Swap
			};
			static int mode = Mode_Copy;

			// Our buttons are both drag sources and drag targets here!
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				// Set payload to carry the index of our item (could be anything)
				const wchar_t* itemPath = name.c_str();
				ImGui::SetDragDropPayload("TEST", itemPath, (wcslen(itemPath)+1)*sizeof(wchar_t));

				// Display preview (could be anything, e.g. when dragging an image we could decide to display
				// the filename and a small preview of the image, etc.)
				if (mode == Mode_Copy) { ImGui::ImageButton((void*)fileImageShaderDatas_.at(u8fileName), ImVec2(buttonSize.x_, buttonSize.y_)); }
				if (mode == Mode_Move) { ImGui::ImageButton((void*)fileImageShaderDatas_.at(u8fileName), ImVec2(buttonSize.x_, buttonSize.y_)); }
				if (mode == Mode_Swap) { ImGui::ImageButton((void*)fileImageShaderDatas_.at(u8fileName), ImVec2(buttonSize.x_, buttonSize.y_)); }
				ImGui::EndDragDropSource();
			}
			
		}
		else if (ext == L"mv1")
		{
			fileImageShaderDatas_.emplace(u8fileName, nullptr);
			auto IconDataPath = fileFullPath / u8fileName;
			CreateModelIcon(IconDataPath, u8fileName);

			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(u8fileName), ImVec2(buttonSize.x_, buttonSize.y_));

			
		}
		else if (ext == L"png" || ext == L"tif")
		{
			fileImageShaderDatas_.emplace(u8fileName, nullptr);
			auto IconDataPath = fileFullPath / u8fileName;
			CreateImageIcon(IconDataPath, u8fileName);

			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(u8fileName), ImVec2(buttonSize.x_, buttonSize.y_));
		}
		else
		{
			buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(L"unknown"), ImVec2(buttonSize.x_, buttonSize.y_));
			//buttonPressed = ImGui::Button("unknown", ImVec2(buttonSize.x_, buttonSize.y_));
		}
	}


	// �J�X�^���X�^�C�������ɖ߂�
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();
}

bool CreateIcons::SettingIcon(std::wstring& name, bool& buttonPressed, Vector2Flt buttonSize, std::wstring ext)
{
	if (Utility::ComparisonExtensionFile(name, ext))
	{
		buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at(ext), ImVec2(buttonSize.x_, buttonSize.y_));
		return true;
	}

	return false;
}

bool CreateIcons::CreateModelIcon(std::filesystem::path path, std::wstring key)
{
	// �摜�̃T�C�Y
	Vector2 ImageSize = { 256 ,256};
	// �摜�̔w�i�F
	auto backImageColor = 0xfafafa;
	// �摜�ɕ\�����镶���̐F
	auto extensionColor = 0xff0000;
	//Samples:�}���`�T���v�������Ɏg�p����h�b�g��(�����قǏd���Ȃ�܂�)  Quality : �}���`�T���v�������̕i��
	auto samples = 4;
	auto quality = 4;
	// ���f���̃T�C�Y
	Vector3 scl = { 1.f,1.f,1.f };

	auto filePath = path.string();
	auto model = MV1LoadModel(filePath.c_str());
	if (model == -1)
	{
		return false;
	}
	MV1_REF_POLYGONLIST RefPoly;
	//// ���f���̑S�t���[���̃|���S���̏����擾
	RefPoly = MV1GetReferenceMesh(model, -1, TRUE);
	auto minRP = RefPoly.MinPosition;
	auto maxRP = RefPoly.MaxPosition;

	// �A���`�G�C���A�V���O����
	SetCreateDrawValidGraphMultiSample(samples, quality);
	// ImageSize�̃A���t�@�`�����l���Ȃ��̕`��\�摜���쐬����
	auto handle = MakeScreen(ImageSize.x_, ImageSize.y_, true);

	// �쐬�����摜��`��Ώۂɂ���
	SetDrawScreen(handle);

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
		// �J�����̃N���b�s���O������ݒ�
	SetCameraNearFar(10.0f, 300000.0f);
	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)(�J�����̈ʒu�A�J�����̉�])
	SetCameraPositionAndAngle(VGet(-293.486f, 185.f, -279.488f), 0.264f, 0.808f, 0.0f);

	while (true)
	{
		// ���f���̑S�t���[���̃|���S���̏����擾
		RefPoly = MV1GetReferenceMesh(model, -1, TRUE);
		// ���_���X�V
		maxRP = RefPoly.MaxPosition;
		minRP = RefPoly.MinPosition;

		// �J�������ɓ��肫���Ă��邩
		if (!CheckCameraViewClip(maxRP) &&
			!CheckCameraViewClip(minRP))
		{
			scl *= 1.9f;
			MV1SetScale(model, scl.toVECTOR());
			MV1RefreshReferenceMesh(model, -1, TRUE);
		}
		else
		{
			break;
		}	
	}

	while (true)
	{
		// ���f���̑S�t���[���̃|���S���̏����擾
		RefPoly = MV1GetReferenceMesh(model, -1, TRUE);
		// ���_���X�V
		maxRP = RefPoly.MaxPosition;
		minRP = RefPoly.MinPosition;

		// �J�������ɓ��肫���Ă��邩
		if (!CheckCameraViewClip(maxRP) &&
			!CheckCameraViewClip(minRP))
		{
			break;
		}
		else
		{
			scl *= 0.9f;
			MV1SetScale(model, scl.toVECTOR());
			MV1RefreshReferenceMesh(model, -1, TRUE);
		}

		if (RefPoly.VertexNum == 0&& RefPoly.PolygonNum == 0)
		{
			break;
		}
	}

	// �w�i
	DrawBox(0, 0, ImageSize.x_, ImageSize.y_, backImageColor, true);

	// �X�e�[�W�̕`��
	stage_->Update();
	stage_->Draw();

	// ���f���̕`��
	MV1DrawModel(model);

	SetFontSize(64);
	DrawString(0,0, path.extension().string().c_str(), extensionColor);
	SetFontSize(8);

	int textureSizeX;
	int textureSizeY;
	// �e�N�X�`���`��
	ImguiSup::LoadTextureFromFile(handle, &fileImageShaderDatas_.at(key), &textureSizeX, &textureSizeY);

	MV1DeleteModel(model);
	DeleteGraph(handle);
	MV1TerminateReferenceMesh(model, -1, TRUE);
	
	return true;
}

bool CreateIcons::CreateImageIcon(std::filesystem::path path, std::wstring key)
{
	auto filePath = path.string();

	auto image = LoadGraph(filePath.c_str());

	if (image == -1)
	{
		return false;
	}

	int x, y;
	GetGraphSize(image, &x, &y);

	int thickness = 2;
	Vector2 thicknessXY = { 0,0 };

	if (x > 64)
	{
		thicknessXY.x_ += 2;
	}
	if (y > 64)
	{
		thicknessXY.y_ += 2;
	}

	// 20x20�T�C�Y�̃A���t�@�`�����l���Ȃ��̕`��\�摜���쐬����
	auto handle = MakeScreen(x, y, true);

	// �쐬�����摜��`��Ώۂɂ���
	SetDrawScreen(handle);

	//�摜�̕`��
	DrawGraph(0, 0, image, true);

	// �摜���͂ޘg�̕`��
	DrawLine(thicknessXY.x_, 0, thicknessXY.x_, y, 0xffffff, thickness);
	DrawLine(0, thicknessXY.y_, x, thicknessXY.y_, 0xffffff, thickness);
	DrawLine(x - thicknessXY.x_, 0, x - thicknessXY.x_, y, 0xffffff, thickness);
	DrawLine(x, y - thicknessXY.y_, 0, y - thicknessXY.y_, 0xffffff, thickness);


	int textureSizeX;
	int textureSizeY;
	// �e�N�X�`���`��
	ImguiSup::LoadTextureFromFile(handle, &fileImageShaderDatas_.at(key), &textureSizeX, &textureSizeY);

	DeleteGraph(image);
	DeleteGraph(handle);
	return true;
}
