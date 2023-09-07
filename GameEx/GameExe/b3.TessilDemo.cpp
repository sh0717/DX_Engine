#include "pch.h"
#include "b3.TessilDemo.h"
#include "SphereCollider.h"
#include "Button.h"
#include  "Wave.h"
#include "RenderStates.h"

//float GetHillHeight(float x, float z)
//{
//	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z))*0.5;
//}

void TessilDemo::Init()
{

	modelAnimShader = make_shared<Shader>(L"16.AnimDemo.fx");
	lightShader = make_shared<Shader>(L"07.Light.fx");
	modelShader = make_shared<Shader>(L"15.ModelDemo.fx");
	tessilShader = make_shared<Shader>(L"b1.Tritessilation.fx");
	tessilShader->PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;

	RESOURCES->Load<Texture>(L"mirror", L"../Resources/Textures/ice.dds");
	MaterialDesc mirrordesc;
	mirrordesc.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	mirrordesc.Diffuse = XMFLOAT4(0.7f, 0.8f, 1.0f, 0.3f);
	mirrordesc.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);
	shared_ptr<Material> mirrorMaterial = make_shared<Material>();
	mirrorMaterial->SetMaterialDesc(mirrordesc);
	mirrorMaterial->SetDiffuseTexture(RESOURCES->Get<Texture>(L"mirror"));
	mirrorMaterial->SetShader(lightShader);

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

		mCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -5.f));
		mCamera->GetCamera()->SetCullingMaskAll();
		mCamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_Default, OFF);


		SCENE->GetCurrentScene()->Add(mCamera);
	
	}


	/*{
		mCameraOverlap = make_shared<DefaultObject>(DEVICE, CONTEXT);

		mCameraOverlap->AddComponent(make_shared<CameraTransform>());
		mCameraOverlap->AddComponent(make_shared<Camera>());
		mCameraOverlap->AddComponent(make_shared<CameraScript>());

		mCameraOverlap->GetTransform()->SetPosition(Vec3(0.f, 0.f, -5.f));
		mCameraOverlap->GetCamera()->SetCullingMaskAll();
		mCameraOverlap->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_Overlap, OFF);


		SCENE->GetCurrentScene()->Add(mCameraOverlap);

	}*/


	//{
	//	UICamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	//	//UICamera->AddComponent(make_shared<CameraTransform>());
	//	UICamera->AddComponent(make_shared<Camera>());
	//	//UICamera->AddComponent(make_shared<CameraScript>());

	//	UICamera->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, -50.f));
	//	UICamera->GetCamera()->SetCullingMaskAll();
	//	UICamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_UI, OFF);
	//	UICamera->GetCamera()->SetMode(eProjectionType::ORTHO);
	//	UICamera->GetCamera()->SetNear(1.f);
	//	UICamera->GetCamera()->SetFar(100.f);


	//	SCENE->GetCurrentScene()->Add(UICamera);
	//}

	RESOURCES->Load<Texture>(L"Atrox", L"../Resources/Textures/Atrox_drx.jpg");
	RESOURCES->Load<Texture>(L"grass", L"../Resources/Textures/grass.dds");
	RESOURCES->Load<Texture>(L"water", L"../Resources/Textures/water1.dds");

	

	///삼각형추가해주기 
	{
	
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform();

		
		shared_ptr<Geometry<VertexFull>> mGeO = make_shared<Geometry<VertexFull>>();
		

		std::vector<VertexFull> vertex_vector(3);
		vertex_vector[0].Position = Vec3(-10.f, 10.f, 0.f);
		vertex_vector[1].Position = Vec3(0.f, 10.f, 0.f);
		vertex_vector[2].Position = Vec3(10.f, -10.f, 0.f);

		std::vector<uint32> index_vector(3);
		index_vector[0] = 0;
		index_vector[1] = 1;
		index_vector[2] = 2;
		
		mGeO->SetIndices(index_vector);
		mGeO->SetVertices(vertex_vector);


		auto material = make_shared<Material>();
		material->SetShader(tessilShader);

		MaterialDesc desc;
		material->SetMaterialDesc(desc); 

		auto mesh = make_shared<Mesh>();
		mesh->SetGeometry(mGeO);


		obj->AddComponent(make_shared<MeshRenderer>());
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetMaterial(material);
		obj->GetMeshRenderer()->SetRasterizerState(RenderStates::WireframeRS);

		SCENE->GetCurrentScene()->Add(obj);




	}
	

	

	

	



	





	DirectionalLight lightDesc;
	{
		lightDesc.Ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, 1.f);
	}






	shared_ptr<DefaultObject> light_obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
	light_obj->GetOrAddTransform();
	light_obj->AddComponent(make_shared<Light>());
	light_obj->GetLight()->SetLightType(eLightType::Direction);
	light_obj->GetLight()->SetLightDesc(lightDesc);
	SCENE->GetCurrentScene()->Add(light_obj);




	
	 

	SCENE->GetCurrentScene()->Start();
}

void TessilDemo::Update()
{
	//큐브맵의 위치 바꾸기
	cubeObj->GetOrAddTransform()->SetPosition(SCENE->GetCurrentScene()->GetMainCamera()->GetCamera()->GetMyPos());
	




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

void TessilDemo::Render()
{
	
	
}
