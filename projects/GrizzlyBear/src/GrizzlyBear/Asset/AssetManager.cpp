#include <gbpch.h>

#include "AssetManager.h"

#include "AssetLoader.h"

namespace GrizzlyBear {
		
	//Worker -------------------------------------------------------------------------

	Ref<AssetData> WorkImpl(Asset type, const Gstring& filepath, int* errorCode, AssetManagerFlags flags = AssetManagerHint_None)
	{
		Ref<AssetData> assetData;

		if (type == Asset::Texture2D)
		{
			assetData = AssetLoader::load_texture(filepath, errorCode, !(flags & AssetManagerHint_NoEnvironment));
		}
		else
		{
			GRIZZLY_CORE_ASSERT(false, "Invalid asset type");
		}

		return assetData;
	}

	Worker::Worker(Asset type, const Gstring& filepath, std::function<void(Ref<AssetData>, int errorCode)> func, AssetManagerFlags flags)
	{
		m_Func = func;
		m_Filepath = filepath;

		m_Future = std::make_unique<std::future<Ref<AssetData>>>(std::async(std::launch::async, WorkImpl, type, filepath, &m_ErrorCode, flags));
	}

	Worker::~Worker()
	{
		m_Future.reset();
	}

	Ref<AssetData> Worker::get_result()
	{
		if (m_Future->valid())
		{
			return m_Future->get();
		}

		return nullptr;
	}

	Gstring Worker::get_filepath()
	{
		return m_Filepath;
	}

	int Worker::get_error_code()
	{
		return m_ErrorCode;
	}

	std::function<void(Ref<AssetData>, int errorCode)> Worker::get_notifier()
	{
		return m_Func;
	}

	//AssetProvider -------------------------------------------------------------------

	void AssetProvider::request_asset(Asset type, const Gstring& filepath, std::function<void(Ref<AssetData>, int errorCode)> func, AssetManagerFlags flags)
	{
		for (auto it = m_Worker.begin(); it != m_Worker.end(); it++)
		{
			if ((*it)->get_filepath() == filepath)
			{
				return;
			}
		}

		m_Worker.push_back(CreateRef<Worker>(type, filepath, func, flags));
	}

	void AssetProvider::update()
	{
		for (auto it = m_Worker.begin(); it != m_Worker.end(); it++)
		{
			Ref<AssetData> data = (*it)->get_result();

			if (data != nullptr)
			{
				(*it)->get_notifier()(data, (*it)->get_error_code());

				it = m_Worker.erase(it);

				if (it == m_Worker.begin())
				{
					update();

					break;
				}
			}
		}
	}

	size_t AssetProvider::get_worker_size()
	{
		return m_Worker.size();
	}

	//AssetManager --------------------------------------------------------------------

	AssetProvider AssetManager::s_Provider;

	AssetManager::AssetManager(Ref<GraphicsContext> context, AssetManagerFlags flags)
	{
		m_Context = context;

		m_Flags = flags;

		int errorCode = 0;

		m_Textures.insert({ AssetLoader::ERROR_TEXTURE_FILEPATH, context->create_texture_2d(AssetLoader::load_texture(AssetLoader::ERROR_TEXTURE_FILEPATH, &errorCode, !(m_Flags & AssetManagerHint_NoEnvironment))) });
	}

	AssetManager::AssetManager(Ref<GraphicsContext> context, const Gstring& errorTexture, AssetManagerFlags flags)
	{
		m_Context = context;

		m_Flags = flags;

		AssetLoader::ERROR_TEXTURE_FILEPATH = errorTexture;

		int errorCode = 0;

		m_Textures.insert({ errorTexture, context->create_texture_2d(AssetLoader::load_texture(errorTexture, &errorCode, !(m_Flags & AssetManagerHint_NoEnvironment))) });
	}

	void AssetManager::update()
	{
		m_Provider.update();
	}

	void AssetManager::request_asset(Asset type, const Gstring& filepath, std::function<void(ErrorCode)> func, bool streaming)
	{
		if (filepath == "" || filepath == AssetLoader::ERROR_TEXTURE_FILEPATH || m_CurrentlyLoading[filepath]) return;

		if (is_loaded(type, filepath))
		{
			GRIZZLY_CORE_LOG(LogLevel::Warn, "Skipping asset {0} already loaded", filepath.c_str());
			return;
		}

		m_StreamableAssets[filepath] = streaming;

		m_Provider.request_asset(type, filepath, [=](Ref<AssetData> data, int errorCode)
		{
			if (type == Asset::Texture2D)
			{
				if (errorCode != GRIZZLY_ASSET_ERRORCODE_NOT_FOUND)
				{
					auto it = std::find(m_FailedTextures2D.begin(), m_FailedTextures2D.end(), filepath);

					if (it != m_FailedTextures2D.end())
					{
						m_FailedTextures2D.erase(it);
					}

					m_Textures.insert({ filepath, m_Context->create_texture_2d(std::static_pointer_cast<TextureData>(data)) });
				}
			}

			m_CurrentlyLoading[filepath] = false;
			func(errorCode);
		}, m_Flags);

		m_CurrentlyLoading[filepath] = true;
	}

	void AssetManager::request_asset(Asset type, const Gstring& filepath, bool streaming)
	{
		if (filepath == "" || filepath == AssetLoader::ERROR_TEXTURE_FILEPATH || m_CurrentlyLoading[filepath]) return;

		if (is_loaded(type, filepath))
		{
			GRIZZLY_CORE_LOG(LogLevel::Warn, "Skipping asset {0} already loaded", filepath.c_str());
			return;
		}

		m_StreamableAssets[filepath] = streaming;

		m_Provider.request_asset(type, filepath, [=](Ref<AssetData> data, int errorCode)
		{
			if (type == Asset::Texture2D)
			{
				auto it = std::find(m_FailedTextures2D.begin(), m_FailedTextures2D.end(), filepath);

				if (it != m_FailedTextures2D.end())
				{
					m_FailedTextures2D.erase(it);
				}

				m_Textures.insert({ filepath, m_Context->create_texture_2d(std::static_pointer_cast<TextureData>(data)) });
			}

			m_CurrentlyLoading[filepath] = false;

		}, m_Flags);

		m_CurrentlyLoading[filepath] = true;
	}

	void AssetManager::request_delete(Asset type, const Gstring& filepath)
	{
		if (type == Asset::Texture2D)
		{
			m_Textures.erase(filepath);
		}

		m_StreamableAssets[filepath] = false;
	}

	Ref<Texture> AssetManager::get_texture(const Gstring& filepath, bool forceLoad)
	{
		if (filepath == "")
		{
			return nullptr;
		}

		if (is_loaded(Asset::Texture2D, filepath))
		{
			return m_Textures.at(filepath);
		}
		else
		{
			if (!forceLoad && std::find(m_FailedTextures2D.begin(), m_FailedTextures2D.end(), filepath) != m_FailedTextures2D.end())
			{
				return m_Textures.at(AssetLoader::ERROR_TEXTURE_FILEPATH);
			}

			request_asset(Asset::Texture2D, filepath, [=](ErrorCode code)
			{
				if (code == GRIZZLY_ASSET_ERRORCODE_NOT_FOUND && !forceLoad)
				{
					m_FailedTextures2D.push_back(filepath);
				}
			});
		}

		return m_Textures.at(AssetLoader::ERROR_TEXTURE_FILEPATH);
	}

	Ref<Texture> AssetManager::wait_for_texture(const Gstring& filepath)
	{
		if (is_loaded(Asset::Texture2D, filepath))
		{
			return m_Textures.at(filepath);
		}
		else
		{
			if (std::find(m_FailedTextures2D.begin(), m_FailedTextures2D.end(), filepath) != m_FailedTextures2D.end())
			{
				return m_Textures.at(AssetLoader::ERROR_TEXTURE_FILEPATH);
			}

			request_asset(Asset::Texture2D, filepath, [=](ErrorCode code)
			{
				if (code == GRIZZLY_ASSET_ERRORCODE_NOT_FOUND)
				{
					m_FailedTextures2D.push_back(filepath);
				}
			});

			//Wait till our asset is loaded. Other assets will not be blocked!
			while (!is_failed(Asset::Texture2D, filepath) && !is_loaded(Asset::Texture2D, filepath))
			{
				update();
			}

			if (is_loaded(Asset::Texture2D, filepath))
			{
				return m_Textures.at(filepath);
			}

			return m_Textures.at(AssetLoader::ERROR_TEXTURE_FILEPATH);
		}
	}

	bool AssetManager::is_available(Asset type, const Gstring& filepath)
	{
		if (is_loaded(type, filepath)) return true;
		return m_StreamableAssets[filepath];
	}

	bool AssetManager::is_loaded(Asset type, const Gstring& filepath)
	{
		if (filepath.empty()) return true;

		if (type == Asset::Texture2D)
		{
			auto it = m_Textures.find(filepath);

			if (it != m_Textures.end())
			{
				return true;
			}

			return false;
		}

		return false;
	}

	bool AssetManager::is_failed(Asset type, const Gstring& filepath)
	{
		if (type == Asset::Texture2D)
		{
			auto it = std::find(m_FailedTextures2D.begin(), m_FailedTextures2D.end(), filepath);

			if (it != m_FailedTextures2D.end())
			{
				return true;
			}

			return false;
		}

		return false;
	}

	bool AssetManager::is_currently_loading(const Gstring& filepath)
	{
		return m_CurrentlyLoading[filepath];
	}

	Ref<Texture> AssetManager::wait_for_imm_texture(Ref<GraphicsContext> context, const Gstring& filepath, AssetManagerFlags flags)
	{
		bool failed = false;
		Ref<Texture2D> tex = nullptr;

		s_Provider.request_asset(Asset::Texture2D, filepath, [&](Ref<AssetData> data, int errorCode)
		{
			if (errorCode != GRIZZLY_ASSET_ERRORCODE_NOT_FOUND)
			{
				tex = context->create_texture_2d(std::static_pointer_cast<TextureData>(data));
			}
			else
			{
				failed = true;
			}

		}, flags);

		while (tex == nullptr && !failed)
		{
			s_Provider.update();
		}

		return tex;
	}
}
