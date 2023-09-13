#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include "../Common/ImGuiMyCustom.h"

// �F��ȏ��Ŏg�p�����
namespace Utility
{
	/// <summary>
	/// �x���烉�W�A���p�x�� 
	/// </summary>
	/// <param name="deg">�f�O���[�p�x</param>
	/// <returns>���W�A���p�x</returns>
	float Deg2Rad(float deg);

	/// <summary>
	/// �ő���񐔂����߂�
	/// </summary>
	/// <param name="x">���l�P</param>
	/// <param name="y">���l�Q</param>
	/// <returns>�ő����</returns>
	int Gcd(int x, int y);

	/// <summary>
	/// string�^����wstring�^�ւ̕ϊ�
	/// </summary>
	/// <param name="str">string</param>
	/// <returns>�ϊ���</returns>
	std::wstring StringToWideString(const std::string& str);

	/// <summary>
	/// wstring�^����string�^�ւ̕ϊ�
	/// </summary>
	/// <param name="wideString">wstring</param>
	/// <returns>�ϊ���</returns>
	std::string WideStringToString(const std::wstring& wideString);

	/// <summary>
	/// wstring�^����u8string�^�ւ̕ϊ�
	/// </summary>
	/// <param name="wstr">wstring</param>
	/// <returns>�ϊ���</returns>
	std::string WStringToUTF8(const std::wstring& wstr);

	/// <summary>
	/// �}���`�o�C�g�������wstring�^�ɕϊ�
	/// </summary>
	/// <param name="mbStr">string</param>
	/// <returns>�ϊ���</returns>
	std::wstring MultiByteToUnicode(const std::string& mbStr);

	/// <summary>
	/// �K��A�v���̃p�X�擾
	/// </summary>
	/// <param name="extension">���삳�������t�@�C���̊g���q</param>
	/// <returns>�K��A�v���̃p�X</returns>
	std::wstring GetDefaultApplication(const std::wstring& extension);

	/// <summary>
	/// ����̃A�v�����J���A����̃A�v����������ΑI��
	/// </summary>
	/// <param name="filePath">�A�v���̃p�X</param>
	/// <returns>�A�v�����J�����Ftrue�@�@�G���[�Ffalse</returns>
	bool OpenWithDefaultApplication(const std::wstring& filePath);

	/// <summary>
	/// �A�v�����J�� 
	/// </summary>
	/// <param name="applicationPath">�A�v���̃p�X</param>
	/// <param name="filePath">�J�������t�@�C���̃p�X</param>
	/// <returns>�����Ftrue�@���s�Ffalse</returns>
	bool LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath);

	/// <summary>
	/// �g���q�̔�r
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	/// <param name="ext">��r�������g���q</param>
	/// <returns>�t�@�C�����Ɗg���q����v���Ă�Ftrue  �t�@�C�����Ɗg���q����v���Ă�Ffalse</returns>
	bool ComparisonExtensionFile(const std::wstring& filename, const std::wstring& ext);

	/// <summary>
	/// �����̊g���q���w�肳�ꂽ�g���q�ł���΃e�L�X�g�̐F��ς���
	/// </summary>
	/// <param name="showName">�\�����閼�O</param>
	/// <param name="searchFileName">�g���q</param>
	/// <param name="color">�ύX�F</param>
	/// <param name="defaultTarget">�\�������������̃t���p�X</param>
	/// <returns></returns>
	bool CharacterSearch(std::string showName, std::wstring searchFileName, ImGuiCustom::IM_COLOR color, std::wstring defaultTarget);

	/// <summary>
	/// maltbyte�̃G���R�[�f�B���O�𔻕ʂ���֐�
	/// </summary>
	/// <param name="maltbyte">����</param>
	/// <returns>�}���`�o�C�g�̃G���R�[�f�B���O</returns>
	std::string DetectMaltbyteEncoding(const std::string& maltbyte);

	/// <summary>
	/// �t�@�C�����̕ύX����
	/// </summary>
	/// <param name="filePath">�t�@�C����</param>
	/// <param name="newFileName">�V�������O</param>
	/// <returns>�����Ftrue�@���s�Ffalse</returns>
	bool RenameFile(const std::string& filePath, const std::string& newFileName);

	/// <summary>
	/// �t�@�C�������w�蕶���ƈ�v���Ă��邩�ǂ���
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X</param>
	/// <param name="target">�w�蕶��</param>
	/// <returns>��v�Ftrue�@�s��v�Ffalse</returns>
	bool IsMatch(const std::string& filepath, const std::wstring& target);

}
// vector����index�Ԗڂ̗v�f���폜����
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)
{
	vector.erase(vector.begin() + index);
}

// �w�肳�ꂽ�ʒu�i�C���f�b�N�X�j�̗v�f���擾����֐�
template<typename T,typename N>
std::pair<T, N> GetElementNum(const std::map<T, N>& myMap, size_t index) {
	if (index >= myMap.size()) {
		throw std::out_of_range("�C���f�b�N�X���͈͊O�ł��B");
	}

	auto it = myMap.begin();
	std::advance(it, index); // �C���f�b�N�X�܂ŃC�e���[�^��i�߂�

	return *it;
}


