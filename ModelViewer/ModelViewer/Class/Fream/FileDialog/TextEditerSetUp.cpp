#include "TextEditerSetUp.h"

void TextEditerSetUp::SetUp(TextEditor& editor)
{
	TextEditor::LanguageDefinition customLanguage;

	SetLanguageDefinition(editor,customLanguage);

	SetCustomPalette(editor);
	
	// 正規表現と色のインデックスのペアのリストを作成
	TextEditor::LanguageDefinition::TokenRegexStrings customRegexStrings;
	customRegexStrings.push_back(std::make_pair("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b", TextEditor::PaletteIndex::Identifier)); // 識別子のルール
	customRegexStrings.push_back(std::make_pair("\\b(if|else|while|for)\\b", TextEditor::PaletteIndex::Keyword)); // キーワードのルール
	// ...

	// カスタムのシンタックスハイライトルールをテキストエディタに設定
	customLanguage.mTokenRegexStrings = customRegexStrings;
	editor.SetLanguageDefinition(customLanguage);
}

void TextEditerSetUp::SetLanguageDefinition(TextEditor& editor,TextEditor::LanguageDefinition& customLanguage)
{

	
	customLanguage.mName = "CustomLanguage";
	customLanguage.mCaseSensitive = true; // 大文字と小文字を区別するかどうか

	// キーワードの追加
	customLanguage.mKeywords.insert("if");
	customLanguage.mKeywords.insert("else");
	customLanguage.mKeywords.insert("while");
	customLanguage.mKeywords.insert("for");
	// ...

	// 識別子の追加
	TextEditor::Identifier customIdentifier;
	// int の設定
	customIdentifier.mDeclaration = "int";
	customIdentifier.mLocation = TextEditor::Coordinates(0, 0);
	customLanguage.mIdentifiers["int"] = customIdentifier;

	// for の設定
	customIdentifier.mDeclaration = "for";
	customIdentifier.mLocation = TextEditor::Coordinates(0, 0);
	customLanguage.mIdentifiers["for"] = customIdentifier;

	// while の設定
	customIdentifier.mDeclaration = "while";
	customIdentifier.mLocation = TextEditor::Coordinates(0, 0);
	customLanguage.mIdentifiers["while"] = customIdentifier;

	// コメントの設定
	customLanguage.mCommentStart = "//";
	customLanguage.mCommentEnd = "\n";
	customLanguage.mSingleLineComment = "//";
	customLanguage.mCommentStart = "/*";
	customLanguage.mCommentEnd = "*/";
	// ...

	// 言語定義をテキストエディタに設定
	editor.SetLanguageDefinition(customLanguage);
}

void TextEditerSetUp::SetCustomPalette(TextEditor& editor)
{
	TextEditor::Palette customPalette;
	customPalette[(unsigned)TextEditor::PaletteIndex::Default] = 0xFFD4D4D4;		// デフォルト	(明灰色)
	customPalette[(unsigned)TextEditor::PaletteIndex::Keyword] = 0xFF569CD6;		// キーワード	(青)
	customPalette[(unsigned)TextEditor::PaletteIndex::Number] = 0xFFB5CEA8;			// 数字			(明緑)
	customPalette[(unsigned)TextEditor::PaletteIndex::String] = 0xFFCE9178;			// 文字列		(明茶色)
	customPalette[(unsigned)TextEditor::PaletteIndex::CharLiteral] = 0xFFCE9178;	// 文字リテラル (明茶色)
	customPalette[(unsigned)TextEditor::PaletteIndex::Punctuation] = 0xFFD4D4D4;	// 句読点		(明灰色)
	customPalette[(unsigned)TextEditor::PaletteIndex::Preprocessor] = 0xFF808080;	// プリプロセッサ (灰色)
	customPalette[(unsigned)TextEditor::PaletteIndex::Identifier] = 0xFF569CD6;		// 識別子		(青)
	customPalette[(unsigned)TextEditor::PaletteIndex::KnownIdentifier] = 0xFF569CD6;	// 既知の識別子 (青)
	customPalette[(unsigned)TextEditor::PaletteIndex::PreprocIdentifier] = 0xFF569CD6;	// プリプロセッサ識別子 (青)
	customPalette[(unsigned)TextEditor::PaletteIndex::Comment] = 0xFF57A64A;		// コメント		(緑)
	customPalette[(unsigned)TextEditor::PaletteIndex::MultiLineComment] = 0xFF57A64A;	// 複数行コメント (緑)
	customPalette[(unsigned)TextEditor::PaletteIndex::Background] = 0xFF1E1E1E;		// 背景			(ダークカラー)
	customPalette[(unsigned)TextEditor::PaletteIndex::Cursor] = 0xFFFFFFFF;			// カーソル		(白)
	customPalette[(unsigned)TextEditor::PaletteIndex::Selection] = 0xFF264F78;		// 選択範囲		(濃い青)
	customPalette[(unsigned)TextEditor::PaletteIndex::ErrorMarker] = 0xFFD16969;	// エラーマーカー (明赤)
	customPalette[(unsigned)TextEditor::PaletteIndex::Breakpoint] = 0xFFD16969;		// ブレークポイント (明赤)
	customPalette[(unsigned)TextEditor::PaletteIndex::LineNumber] = 0xFF008000;		// 行番号		(暗緑)
	customPalette[(unsigned)TextEditor::PaletteIndex::CurrentLineFill] = 0xFF323232;	// 現在の行の背景 (暗灰)
		
	// パレットをテキストエディタに設定
	editor.SetPalette(customPalette);

}
