#pragma once

#define WIN32_LEAN_AND_MEAN

#include "Types.h"
#include "Define.h"

// STL
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;



// WIN
#include <windows.h>
#include <assert.h>
#include <optional>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <ddd/DirectXTex.h>
#include <ddd/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;

#include <FX11/d3dx11effect.h>





// Libs

//IMGUI 

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


// Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>



#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#pragma comment(lib, "FX11/Effects11d.lib")
#pragma comment(lib, "Assimp/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#pragma comment(lib, "FX11/Effects11.lib")
#pragma comment(lib, "Assimp/assimp-vc143-mt.lib")
#endif




//Utils
#include "tinyxml2.h"
using namespace tinyxml2;
#include "Utils.h"
#include "FileUtils.h"
#include "MathHelper.h"
#include "LightHelper.h"


// Managers
#include "Game.h"
#include "Graphics.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "InstancingManager.h"

#include "ImGuiManager.h"
#include "SceneManager.h"

#define OK(p)	assert(SUCCEEDED(p))
#define CHECK(p)	assert(SUCCEEDED(p))
#define GAME		GET_SINGLE(Game)		
#define GRAPHICS	GET_SINGLE(Graphics)
#define DEVICE		GRAPHICS->GetDevice()
#define CONTEXT			GRAPHICS->GetDeviceContext()
#define INPUT		GET_SINGLE(InputManager)
#define TIME		GET_SINGLE(TimeManager)
#define DT			TIME->GetDeltaTime()
#define RESOURCES	GET_SINGLE(ResourceManager)
#define INSTANCING GET_SINGLE(InstancingManager)
#define GUI  GET_SINGLE(ImGuiManager)
#define SCENE GET_SINGLE(SceneManager)
//#define RENDERMANAGER GET_SINGLE(RenderManager)

// Engine
#include "VertexData.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "IExecute.h"
#include "Geometry.h"
#include "GeometryHelper.h"



#include "DefaultObject.h"
#include "Component.h"
#include "MonoBehavior.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "CameraScript.h"
#include "Texture.h"


#include "Mesh.h"
#include "Material.h"
#include "MeshRenderer.h"

#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "BillboardRenderer.h"