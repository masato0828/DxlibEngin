#pragma once
#include <filesystem>
class CodeReader
{
public:
	CodeReader();
	~CodeReader();

	/// <summary>
	/// �R�[�h�̓ǂݍ���
	/// </summary>
	/// <param name="filePath">�ǂݍ��݃t�@�C���̃p�X</param>
	void Read(std::filesystem::path filePath);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
private:

	/// <summary>
	/// ������
	/// </summary>
	void Init();

};

