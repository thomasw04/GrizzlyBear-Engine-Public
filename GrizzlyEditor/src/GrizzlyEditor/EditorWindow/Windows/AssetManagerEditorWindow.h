#pragma once

#include <GrizzlyEditor/EditorWindow/WindowManager.h>
#include <GrizzlyBear/Core/FileIO.h>
#include <GrizzlyBear/Asset/AssetManager.h>

#include <imgui/misc/cpp/imgui_stdlib.h>

#include <GrizzlyBear/Template/SceneManager.h>

namespace GrizzlyEditor {

	class AssetManagerEditorWindow : public EditorWindow
	{
	public:
		AssetManagerEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager);
		AssetManagerEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager);
		virtual ~AssetManagerEditorWindow();

		virtual void render_window() override;

		virtual void reset_window() override;

		void open_level_create_dialog();

	private:

		std::string m_InputBuf = "";

		GrizzlyBear::FilePath m_CurrentPath{""};

		GrizzlyBear::Ref<GrizzlyBear::Texture> m_FolderIconEmpty;
		GrizzlyBear::Ref<GrizzlyBear::Texture> m_FolderIconFilled;

		GrizzlyBear::Ref<GrizzlyBear::Texture> m_FileIconNormal;
		GrizzlyBear::Ref<GrizzlyBear::Texture> m_FileIconCode;

		GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> m_SceneManager;
	};

}
