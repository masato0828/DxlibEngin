#include "TextEditerSetUp.h"

void TextEditerSetUp::SetUp(TextEditor& editor)
{
	TextEditor::LanguageDefinition customLanguage;

	SetLanguageDefinition(editor,customLanguage);

	SetCustomPalette(editor);
	
	// ���K�\���ƐF�̃C���f�b�N�X�̃y�A�̃��X�g���쐬
	TextEditor::LanguageDefinition::TokenRegexStrings customRegexStrings;
	customRegexStrings.push_back(std::make_pair("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b", TextEditor::PaletteIndex::Identifier)); // ���ʎq�̃��[��
	customRegexStrings.push_back(std::make_pair("\\b(if|else|while|for)\\b", TextEditor::PaletteIndex::Keyword)); // �L�[���[�h�̃��[��
	// ...

	// �J�X�^���̃V���^�b�N�X�n�C���C�g���[�����e�L�X�g�G�f�B�^�ɐݒ�
	customLanguage.mTokenRegexStrings = customRegexStrings;
	editor.SetLanguageDefinition(customLanguage);
}

void TextEditerSetUp::SetLanguageDefinition(TextEditor& editor,TextEditor::LanguageDefinition& customLanguage)
{

	
	customLanguage.mName = "CustomLanguage";
	customLanguage.mCaseSensitive = true; // �啶���Ə���������ʂ��邩�ǂ���

	// �L�[���[�h�̒ǉ�
	customLanguage.mKeywords.insert("if");
	customLanguage.mKeywords.insert("else");
	customLanguage.mKeywords.insert("while");
	customLanguage.mKeywords.insert("for");
	// ...

	// ���ʎq�̒ǉ�
	TextEditor::Identifier customIdentifier;
	// int �̐ݒ�
	customIdentifier.mDeclaration = "int";
	customIdentifier.mLocation = TextEditor::Coordinates(0, 0);
	customLanguage.mIdentifiers["int"] = customIdentifier;

	// for �̐ݒ�
	customIdentifier.mDeclaration = "for";
	customIdentifier.mLocation = TextEditor::Coordinates(0, 0);
	customLanguage.mIdentifiers["for"] = customIdentifier;

	// while �̐ݒ�
	customIdentifier.mDeclaration = "while";
	customIdentifier.mLocation = TextEditor::Coordinates(0, 0);
	customLanguage.mIdentifiers["while"] = customIdentifier;

	// �R�����g�̐ݒ�
	customLanguage.mCommentStart = "//";
	customLanguage.mCommentEnd = "\n";
	customLanguage.mSingleLineComment = "//";
	customLanguage.mCommentStart = "/*";
	customLanguage.mCommentEnd = "*/";
	// ...

	// �����`���e�L�X�g�G�f�B�^�ɐݒ�
	editor.SetLanguageDefinition(customLanguage);
}

void TextEditerSetUp::SetCustomPalette(TextEditor& editor)
{
	TextEditor::Palette customPalette;
	customPalette[(unsigned)TextEditor::PaletteIndex::Default] = 0xFFD4D4D4;		// �f�t�H���g	(���D�F)
	customPalette[(unsigned)TextEditor::PaletteIndex::Keyword] = 0xFF569CD6;		// �L�[���[�h	(��)
	customPalette[(unsigned)TextEditor::PaletteIndex::Number] = 0xFFB5CEA8;			// ����			(����)
	customPalette[(unsigned)TextEditor::PaletteIndex::String] = 0xFFCE9178;			// ������		(�����F)
	customPalette[(unsigned)TextEditor::PaletteIndex::CharLiteral] = 0xFFCE9178;	// �������e���� (�����F)
	customPalette[(unsigned)TextEditor::PaletteIndex::Punctuation] = 0xFFD4D4D4;	// ��Ǔ_		(���D�F)
	customPalette[(unsigned)TextEditor::PaletteIndex::Preprocessor] = 0xFF808080;	// �v���v���Z�b�T (�D�F)
	customPalette[(unsigned)TextEditor::PaletteIndex::Identifier] = 0xFF569CD6;		// ���ʎq		(��)
	customPalette[(unsigned)TextEditor::PaletteIndex::KnownIdentifier] = 0xFF569CD6;	// ���m�̎��ʎq (��)
	customPalette[(unsigned)TextEditor::PaletteIndex::PreprocIdentifier] = 0xFF569CD6;	// �v���v���Z�b�T���ʎq (��)
	customPalette[(unsigned)TextEditor::PaletteIndex::Comment] = 0xFF57A64A;		// �R�����g		(��)
	customPalette[(unsigned)TextEditor::PaletteIndex::MultiLineComment] = 0xFF57A64A;	// �����s�R�����g (��)
	customPalette[(unsigned)TextEditor::PaletteIndex::Background] = 0xFF1E1E1E;		// �w�i			(�_�[�N�J���[)
	customPalette[(unsigned)TextEditor::PaletteIndex::Cursor] = 0xFFFFFFFF;			// �J�[�\��		(��)
	customPalette[(unsigned)TextEditor::PaletteIndex::Selection] = 0xFF264F78;		// �I��͈�		(�Z����)
	customPalette[(unsigned)TextEditor::PaletteIndex::ErrorMarker] = 0xFFD16969;	// �G���[�}�[�J�[ (����)
	customPalette[(unsigned)TextEditor::PaletteIndex::Breakpoint] = 0xFFD16969;		// �u���[�N�|�C���g (����)
	customPalette[(unsigned)TextEditor::PaletteIndex::LineNumber] = 0xFF008000;		// �s�ԍ�		(�×�)
	customPalette[(unsigned)TextEditor::PaletteIndex::CurrentLineFill] = 0xFF323232;	// ���݂̍s�̔w�i (�ÊD)
		
	// �p���b�g���e�L�X�g�G�f�B�^�ɐݒ�
	editor.SetPalette(customPalette);

}
