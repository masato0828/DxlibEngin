#include <iomanip>
#include <sstream>
#include "../../imGui/imgui.h"
#include "Fream_Console.h"

std::string consoleTextBuffer_;

int consoleTextBufferCnt_;

bool windowFlg_ = false;

Fream_Console::Fream_Console()
{
    Init();
}

Fream_Console::~Fream_Console()
{
}

void Fream_Console::Init()
{
    consoleTextBufferCnt_ = 0;
    consoleTextBuffer_.clear();
}

void Fream_Console::Update()
{
    ConsoleWindow();
}

void Fream_Console::ConsoleWindow()
{
    {
        ImGui::SetNextWindowSize(ImVec2{ 1000, 500 }, ImGuiCond_Once);
        windowFlg_ = ImGui::Begin("Console");

        if (ImGui::SmallButton("clear"))
        {
            consoleTextBufferCnt_ = 0;
            consoleTextBuffer_.clear();
        }

        // セパレーター1つ＋入力テキスト1つ分の高さを確保する
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        const float footer_with_to_reserve = ImGui::GetStyle().ItemSpacing.x;
        ImGui::BeginChild("debuglog", ImVec2(-footer_with_to_reserve, -footer_height_to_reserve), true, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

        ImGui::Separator();

        // コンソールテキストを表示
        ImGui::TextUnformatted(consoleTextBuffer_.c_str());

        // 操作していない時スクロールバーを固定
        if ((ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();

        static char buffer[256] = "";

        ImGui::Text(u8"テキスト入力"); ImGui::SameLine();
        ImGui::InputText("##", buffer, 256); ImGui::SameLine();

        if (ImGui::SmallButton(u8"出力"))
        {
            AddConsoleText(buffer);
        }

        ImGui::End();
    }
}

void AddConsoleText(const std::string& text)
{
    if (windowFlg_)
    {
        std::string formattedText;
        std::stringstream ss;
        // n桁のゼロ埋め形式で文字列を生成
        ss << std::setw(4) << std::setfill('0') << consoleTextBufferCnt_;

        consoleTextBufferCnt_++;

        // n桁のゼロ埋め形式で文字列を生成
        ss.str("");
        ss << std::setw(4) << std::setfill('0') << consoleTextBufferCnt_;
        formattedText = ss.str();

        // テキストをバッファに追加
        consoleTextBuffer_ += "[" + formattedText + "]:" + text + "\n";
    }
}
