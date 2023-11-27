#pragma once

#include <vector>
#include <functional>
#include <future>

#include <GrizzlyBear/Core/BaseDefines.h>
#include "AssetData.h"
#include "GrizzlyBear/API/GGL/GraphicsContext.h"

namespace GrizzlyBear {

	enum AssetManagerFlags
	{
		AssetManagerHint_None = 0,
		AssetManagerHint_NoEnvironment = 1 << 0
	};

	//An async asset loader
	class Worker
	{
	public:
		Worker(Asset type, const Gstring& filepath, std::function<void(Ref<AssetData>, int errorCode)> func, AssetManagerFlags flags = AssetManagerHint_None);
		~Worker();

		Ref<AssetData> get_result();

		Gstring get_filepath();
		int get_error_code();

		std::function<void(Ref<AssetData>, int errorCode)> get_notifier();

	private:
		Scope<std::future<Ref<AssetData>>> m_Future;

		std::function<void(Ref<AssetData>, int errorCode)> m_Func;

		Gstring m_Filepath;

		int m_ErrorCode = 0;
	};

	//Provides the assetmanager with requested assets
	class AssetProvider
	{
	public:

		void request_asset(Asset type, const Gstring& filepath, std::function<void(Ref<AssetData>, int errorCode)> func, AssetManagerFlags flags = AssetManagerHint_None);

		void update();

		size_t get_worker_size();

	private:
		std::vector<Ref<Worker>> m_Worker;
	};

	//TODO: Shutdown and thread interruption mechanism

	class AssetManager
	{
		using ErrorCode = int;

	public:

		AssetManager(Ref<GraphicsContext> context, AssetManagerFlags flags = AssetManagerHint_None);
		AssetManager(Ref<GraphicsContext> context, const Gstring& errorTexture, AssetManagerFlags flags = AssetManagerHint_None);

		void update();
		
		void request_asset(Asset type, const Gstring& filepath, std::function<void(ErrorCode)> func, bool streaming = false);
		void request_asset(Asset type, const Gstring& filepath, bool streaming = false);
		void request_delete(Asset type, const Gstring& filepath);
		
		Ref<Texture> get_texture(const Gstring& filepath, bool forceLoad = false);
		Ref<Texture> wait_for_texture(const Gstring& filepath);

		bool is_available(Asset type, const Gstring& filepath);
		bool is_loaded(Asset type, const Gstring& filepath);
		bool is_failed(Asset type, const Gstring& filepath);

		bool is_currently_loading(const Gstring& filepath);

	public:
		static Ref<Texture> wait_for_imm_texture(Ref<GraphicsContext> context, const Gstring& filepath, AssetManagerFlags flags = AssetManagerHint_None);

		static AssetProvider s_Provider;

	private:
		AssetProvider m_Provider;

		Ref<GraphicsContext> m_Context;

		std::unordered_map<Gstring, std::shared_ptr<Texture>> m_Textures;

		std::vector<Gstring> m_FailedTextures2D;

		std::unordered_map<Gstring, bool> m_StreamableAssets;
		std::unordered_map<Gstring, bool> m_CurrentlyLoading;

		AssetManagerFlags m_Flags;
	};

}
