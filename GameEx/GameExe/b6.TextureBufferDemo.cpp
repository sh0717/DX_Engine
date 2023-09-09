#include "pch.h"

#include "SphereCollider.h"
#include "Button.h"
#include  "Wave.h"
#include "RenderStates.h"
#include "b6.TextureBufferDemo.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "CubeBehavior.h"
#include "TextureBuffer.h"
//float GetHillHeight(float x, float z)
//{
//	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z))*0.5;
//}

void TexutreBufferDemo::Init()
{
	auto changed_SRV = MakeComputeShaderTexture(0,0);
	auto changed_SRV_1 = MakeComputeShaderTexture(0,1);
	auto changed_SRV_2 = MakeComputeShaderTexture(0,2);
	auto changed_SRV_3 = MakeComputeShaderTexture(0,3);


	DebugShader = make_shared<Shader>(L"TextureDebug.fx");
	modelAnimShader = make_shared<Shader>(L"16.AnimDemo.fx");
	lightShader = make_shared<Shader>(L"07.Light.fx");
	
	modelShader = make_shared<Shader>(L"15.ModelDemo.fx");
	tessilShader = make_shared<Shader>(L"b1.Tritessilation.fx");
	tessilShader->PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;

	DisplacementShaer = make_shared<Shader>(L"b2.Displacement.fx");
	DisplacementShaer->PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	

	terrainShader = make_shared<Shader>(L"b3.terrain.fx");
	terrainShader->PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;

	//cubemap make
	{
		shared_ptr<Texture> CubeMapTexture = make_shared<Texture>();
		CubeMapTexture->CreateCubeMap(L"../Resources/Textures/grasscube1024.dds");
		shared_ptr<Shader> cubeShader = make_shared<Shader>(L"08.CubeMap.fx");
		shared_ptr<Material> cubeMaterial = make_shared<Material>();
		cubeMaterial->SetShader(cubeShader);
		cubeMaterial->SetCubeTexture(CubeMapTexture);


		cubeObj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		cubeObj->GetOrAddTransform()->SetScale(Vec3(30.f, 30.f, 30.f));
		cubeObj->AddComponent(make_shared<MeshRenderer>());


		cubeObj->GetMeshRenderer()->SetMaterial(cubeMaterial);
		cubeObj->AddComponent(make_shared<CubeBehavior>());

		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		cubeObj->GetMeshRenderer()->SetMesh(mesh);


		cubeObj->SetLayerIndex(eLayer::Layer_Default);
		SCENE->GetCurrentScene()->Add(cubeObj);
	}
	
	
	//Camera making(기본 카메라)

	{
		mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

		mCamera->AddComponent(make_shared<CameraTransform>());
		mCamera->AddComponent(make_shared<Camera>());
		mCamera->AddComponent(make_shared<CameraScript>());

		mCamera->GetTransform()->SetPosition(Vec3(0.f, 10.f, -5.f));
		mCamera->GetCamera()->SetCullingMaskAll();
		mCamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_Default, OFF);

		mCamera->GetCamera()->cameraType = eCameraType::Default;

		SCENE->GetCurrentScene()->Add(mCamera);
	
	}


	{
		UICamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

		
		UICamera->AddComponent(make_shared<Camera>());
		UICamera->AddComponent(make_shared<CameraScript>());

		UICamera->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, -1.f));
		UICamera->GetCamera()->SetMode(eProjectionType::ORTHO);
		UICamera->GetCamera()->SetCullingMaskAll();
		UICamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_UI, OFF);
		UICamera->GetCamera()->cameraType = eCameraType::UI;

		SCENE->GetCurrentScene()->Add(UICamera);

	}


	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetScale(Vec3(200.f, 200.f, 1.f));
		obj->GetOrAddTransform()->SetPosition(Vec3(-250,250, 1.f));

		obj->AddComponent(make_shared<MeshRenderer>());

		auto mesh = make_shared<Mesh>();
		mesh->CreateQuad();

		auto mat = make_shared<Material>();
		mat->SetShader(DebugShader);
		shared_ptr<Texture> tex = make_shared<Texture>();
		tex->SetSRV(changed_SRV);
		mat->SetDiffuseTexture(tex);

		obj->GetMeshRenderer()->SetMaterial(mat);
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetPass(1);

		obj->SetLayerIndex(eLayer::Layer_UI);
		SCENE->GetCurrentScene()->Add(obj);
		
	}

	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetScale(Vec3(200.f, 200.f, 1.f));
		obj->GetOrAddTransform()->SetPosition(Vec3(-250, -250, 1.f));

		obj->AddComponent(make_shared<MeshRenderer>());

		auto mesh = make_shared<Mesh>();
		mesh->CreateQuad();

		auto mat = make_shared<Material>();
		mat->SetShader(DebugShader);
		shared_ptr<Texture> tex = make_shared<Texture>();
		tex->SetSRV(changed_SRV_1);
		mat->SetDiffuseTexture(tex);

		obj->GetMeshRenderer()->SetMaterial(mat);
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetPass(1);
		obj->SetLayerIndex(eLayer::Layer_UI);
		SCENE->GetCurrentScene()->Add(obj);

	}
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetScale(Vec3(200.f, 200.f, 1.f));
		obj->GetOrAddTransform()->SetPosition(Vec3(250, -250, 1.f));

		obj->AddComponent(make_shared<MeshRenderer>());

		auto mesh = make_shared<Mesh>();
		mesh->CreateQuad();

		auto mat = make_shared<Material>();
		mat->SetShader(DebugShader);
		shared_ptr<Texture> tex = make_shared<Texture>();
		tex->SetSRV(changed_SRV_2);
		mat->SetDiffuseTexture(tex);

		obj->GetMeshRenderer()->SetMaterial(mat);
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetPass(1);
		obj->SetLayerIndex(eLayer::Layer_UI);
		SCENE->GetCurrentScene()->Add(obj);

	}
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetScale(Vec3(200.f, 200.f, 1.f));
		obj->GetOrAddTransform()->SetPosition(Vec3(250, 250, 1.f));

		obj->AddComponent(make_shared<MeshRenderer>());

		auto mesh = make_shared<Mesh>();
		mesh->CreateQuad();

		auto mat = make_shared<Material>();
		mat->SetShader(DebugShader);
		shared_ptr<Texture> tex = make_shared<Texture>();
		tex->SetSRV(changed_SRV_3);
		mat->SetDiffuseTexture(tex);

		obj->GetMeshRenderer()->SetMaterial(mat);
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetPass(1);
		obj->SetLayerIndex(eLayer::Layer_UI);
		SCENE->GetCurrentScene()->Add(obj);

	}
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetScale(Vec3(200.f, 200.f, 1.f));
		obj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 1.f));

		obj->AddComponent(make_shared<MeshRenderer>());

		auto mesh = make_shared<Mesh>();
		mesh->CreateQuad();

		auto mat = make_shared<Material>();
		mat->SetShader(DebugShader);

		mat->SetDiffuseTexture(RESOURCES->Get<Texture>(L"Atrox"));

		obj->GetMeshRenderer()->SetMaterial(mat);
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetPass(1);
		obj->SetLayerIndex(eLayer::Layer_UI);
		SCENE->GetCurrentScene()->Add(obj);

	}
	 

	SCENE->GetCurrentScene()->Start();
}

void TexutreBufferDemo::Update()
{

	




	//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//ImGui::Checkbox("Another Window", &show_another_window);

	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//	counter++;
	//ImGui::SameLine();
	//ImGui::Text("counter = %d", counter);

	////ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	//ImGui::End();


	//light 데이터 넣어놓기
	{
		DirectionalLight lightDesc;
		lightDesc.Ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, 1.f);


		ManyDirectionalLightDesc manydesc;
		manydesc.num_light = 1;
		manydesc.dirlights[0] = lightDesc;


		//lightShader->PushManyDirLightBuffer(manydesc);


		modelShader->PushDirLightBuffer(lightDesc);
		lightShader->PushDirLightBuffer(lightDesc);
		
		
	}

	



	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		int32 mouseX = INPUT->GetMousePos().x;
		int32 mouseY = INPUT->GetMousePos().y;

		// Picking
		auto pickObj = SCENE->GetCurrentScene()->Pick(mouseX, mouseY);
		if (pickObj)
		{
			SCENE->GetCurrentScene()->Remove(pickObj);
		}
	}


	
}

void TexutreBufferDemo::Render()
{
	
	
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TexutreBufferDemo::MakeComputeShaderTexture(UINT tech , UINT pass)
{
	auto shader = make_shared<Shader>(L"10.TextureBufferDemo.fx");

	auto texture = RESOURCES->Load<Texture>(L"Atrox", L"..\\Resources\\Textures\\Atrox_drx.jpg");
	shared_ptr<TextureBuffer> textureBuffer = make_shared<TextureBuffer>(texture->GetTexture2D());

	shader->GetSRV("Input")->SetResource(textureBuffer->GetSRV().Get());
	shader->GetUAV("Output")->SetUnorderedAccessView(textureBuffer->GetUAV().Get());

	uint32 width = textureBuffer->GetWidth();
	uint32 height = textureBuffer->GetHeight();
	uint32 arraySize = textureBuffer->GetArraySize();

	uint32 x = max(1, (width + 31) / 32);
	uint32 y = max(1, (height + 31) / 32);
	shader->Dispatch(tech, pass, x, y, arraySize);

	return textureBuffer->GetOutputSRV();
}
