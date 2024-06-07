#include <fstream>
#include <json/json.hpp>

#include "UIEditorMain.h"
#include "../xrScripts/stdafx.h"
#include "../xrScripts/script_engine.h"
#include "../xrScripts/script_process.h"
#include "../xrScripts/lua_ext.h"

using json = nlohmann::json;
string_path jsonSnippetsPath;
#define SNIPPET_JSON_NAME(buf) { FS.update_path(buf,"$app_data_root$","lua_snippets.json"); }

static xr_string CodeText;
static json jsonArray({
    {
        {
            "name", "No check weapons"
        },
        {
            "code", "bind_stalker.check_for_weapon_hide_by_zones = function() return false end"
        }
    }
});


namespace Platform
{
	XRCORE_API xr_string TCHAR_TO_ANSI_U8(const wchar_t* C);
}
void EditorLuaCodespace()
{
	if (!Engine.External.EditorStates[static_cast<std::uint8_t>(EditorUI::LuaCodespace)])
		return;

	if (!ImGui::Begin("Lua Coder", &Engine.External.EditorStates[static_cast<u8>(EditorUI::LuaCodespace)]))
	{
		ImGui::End();
		return;
	}

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Name snippet:");
    ImGui::SameLine();

    static char name[100] = {};
    ImGui::InputText("##Name", name, IM_ARRAYSIZE(name));
    ImGui::SameLine();

    if (ImGui::Button("Save", ImVec2(70.f, 25.f)))
    {
        jsonArray.push_back({
            { "name", name },
            { "code", CodeText.data() }
        });

        name[0] = 0;
        //CodeText[0] = 0;

        std::ofstream o(jsonSnippetsPath);
        o << jsonArray;
        o.close();
    }

    float rightPaneWidth = 250.0f;

	float WndSizeX = ImGui::GetWindowSize().x;
	float WndSizeY = ImGui::GetWindowSize().y;

    ImGui::BeginChild("LeftPane", ImVec2(WndSizeX - rightPaneWidth - ImGui::GetStyle().ItemSpacing.x, 0), true);

    ImGui::InputTextMultiline("##CodeText", CodeText.data(), 4096, ImVec2(-1, -1), ImGuiInputTextFlags_AllowTabInput);

    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::BeginChild("RightPane", ImVec2(rightPaneWidth, 0), true);

    if (ImGui::Button("Run", ImVec2(-1.0f, 50.0f)))
	{
		xr_string AnsiStr = Platform::UTF8_to_CP1251(CodeText.data());
		g_pScriptEngine->script_process(ScriptEngine::eScriptProcessorHelper)->add_script(AnsiStr.data(), true, true);
		g_pScriptEngine->script_process(ScriptEngine::eScriptProcessorHelper)->update();
	}

    ImGui::Spacing();

    ImGui::BeginChild("ListBox", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    for (int i = 0; i < jsonArray.size(); i++)
    {
        if (ImGui::Button(jsonArray[i]["name"].get<std::string>().c_str(), ImVec2(-1, 0)))
        {
            CodeText = jsonArray[i]["code"];
        }
    }

    ImGui::EndChild();
    ImGui::Spacing();
    ImGui::EndChild();

	ImGui::End();
}


void EditorLuaInit()
{
    SNIPPET_JSON_NAME(jsonSnippetsPath);

    if (FS.exist(jsonSnippetsPath))
    {
        std::ifstream jsonSnippets(jsonSnippetsPath);
        jsonArray = json::parse(jsonSnippets);
        jsonSnippets.close();
    }


	CodeText.resize(4096);
	CImGuiManager::Instance().Subscribe("LuaCoder", CImGuiManager::ERenderPriority::eMedium, EditorLuaCodespace);

	CImGuiManager::Instance().Subscribe("LuaDebug", CImGuiManager::ERenderPriority::eLow, []()
	{
		static bool Attach = false;

		if (!Engine.External.EditorStates[static_cast<std::uint8_t>(EditorUI::LuaDebug)])
			return;

		if (!Attach)
		{
			DebbugerAttach();
			Attach = true;
		}
	});

}
