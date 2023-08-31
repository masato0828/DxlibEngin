#pragma once
#include "../../../imGui/ImGuiColorTextEdit/TextEditor.h"

namespace TextEditerSetUp
{
	void SetUp(TextEditor& editor);
	void SetLanguageDefinition(TextEditor& editor, TextEditor::LanguageDefinition& customLanguage);
	void SetCustomPalette(TextEditor& editor);
}

