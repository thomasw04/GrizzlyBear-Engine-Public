#pragma once

//Only for Applications

#define GRIZZLY_AUTHOR "thomasw04"
#define GRIZZLY_VERSION "1.0.0"

//Core
#include "GrizzlyBear/Core/Application.h"
#include "GrizzlyBear/Core/Log.h"
#include "GrizzlyBear/Core/FileIO.h"

//Platform
#include "GrizzlyBear/Core/Platform/Platform.h"

//Event
#include "GrizzlyBear/Core/Event.h"

//Templates
#include "GrizzlyBear/Template/OrthographicCamera.h"
#include "GrizzlyBear/Template/PerspectiveCamera.h"
#include "GrizzlyBear/Template/GuiCamera.h"
#include "GrizzlyBear/Template/SceneManager.h"

//Manager
#include "GrizzlyBear/Asset/AssetLoader.h"
#include "GrizzlyBear/Asset/AssetManager.h"
#include "GrizzlyBear/Script/ScriptManager.h"

//ImGui
#include "GrizzlyBear/Addon/ImGuiAddon.h"

//Misc
#include "GrizzlyBear/Misc/Timer.h"
#include "GrizzlyBear/Misc/Random.h"
#include "GrizzlyBear/Misc/InputConverter.h"
#include "GrizzlyBear/Misc/Utils.h"

//GameObjects
#include "GrizzlyBear/Scene/Scene.h"

//Physics
#include "GrizzlyBear/Phys75/Phys75.h"

//Render
#include "GrizzlyBear/Renderer/Renderer.h"
#include "GrizzlyBear/Renderer/Renderer2D.h"