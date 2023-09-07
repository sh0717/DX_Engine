#include "pch.h"

#include "SphereCollider.h"
#include "Button.h"
#include  "Wave.h"
#include "RenderStates.h"
#include "b5.ShadowDemo.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "CubeBehavior.h"
//float GetHillHeight(float x, float z)
//{
//	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z))*0.5;
//}

void ShadowDemo::Init()
{
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
	RESOURCES->Load<Texture>(L"stone", L"../Resources/Textures/stone.dds");
	
	RESOURCES->Load	<Texture>(L"brick_nmap", L"../Resources/Textures/bricks_nmap.dds");


	Vec3 cylinderPosition[10];

	for (int32 i = 0; i < 5; ++i)
	{
		cylinderPosition[i * 2 + 0] = Vec3(-5.f, 1.5f, -10.f + i * 5.f);
		cylinderPosition[i * 2 + 1] = Vec3(+5.f, 1.5f, -10.f + i * 5.f);
	}
	Vec3 SpherePosition[10];

	for (int32 i = 0; i < 5; ++i)
	{
		SpherePosition[i * 2 + 0] = Vec3(-5.f, 3.5f, -10.f + i * 5.f);
		SpherePosition[i * 2 + 1] = Vec3(+5.f, 3.5f, -10.f + i * 5.f);
	}


	///cylinder  
	{
		for (int i = 0; i < 10; ++i) {
			auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

			obj->GetOrAddTransform()->SetPosition(cylinderPosition[i]);
			obj->GetOrAddTransform()->SetScale(Vec3(1.f, 2.f, 1.f));
			obj->AddComponent(make_shared<MeshRenderer>());

			MaterialDesc matdesc;
			matdesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);


			auto mat = make_shared<Material>();
			mat->SetShader(DisplacementShaer);
			mat->SetMaterialDesc(matdesc);
			mat->SetDiffuseTexture(RESOURCES->Get<Texture>(L"stone"));
			mat->SetNormalTexture(RESOURCES->Get<Texture>(L"brick_nmap"));
			shared_ptr<Mesh> mesh = make_shared<Mesh>();
			//

			shared_ptr<Geometry<VertexFull>> geo_cylinder = make_shared<Geometry<VertexFull>>();

			GeometryHelper::CreateFullCylinder(geo_cylinder, 1, 1, 1, 50, 50);
			mesh->SetGeometry(geo_cylinder);

			obj->GetMeshRenderer()->SetMesh(mesh);
			obj->GetMeshRenderer()->SetMaterial(mat);
			//obj->GetMeshRenderer()->SetRasterizerState(RenderStates::WireframeRS);

			SCENE->GetCurrentScene()->Add(obj);
		}
		//device context 도 자동으로 생성됨
		
		//
	}


	//Sphere
	{
		for (int i = 0; i < 10; ++i) {
			auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

			obj->GetOrAddTransform()->SetPosition(SpherePosition[i]);
			obj->GetOrAddTransform()->SetScale(Vec3(0.6f, .6f, .6f));
			obj->AddComponent(make_shared<MeshRenderer>());

			MaterialDesc matdesc;
			matdesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);


			auto mat = make_shared<Material>();
			mat->SetShader(lightShader);
			mat->SetMaterialDesc(matdesc);
			mat->SetDiffuseTexture(RESOURCES->Get<Texture>(L"Atrox"));
			
			shared_ptr<Mesh> mesh = make_shared<Mesh>();
			//

			shared_ptr<Geometry<VertexFull>> geo_cylinder = make_shared<Geometry<VertexFull>>();

			GeometryHelper::CreateSphere(geo_cylinder, 2,50,50);
			mesh->SetGeometry(geo_cylinder);

			obj->GetMeshRenderer()->SetMesh(mesh);
			obj->GetMeshRenderer()->SetMaterial(mat);
			//obj->GetMeshRenderer()->SetRasterizerState(RenderStates::WireframeRS);

			SCENE->GetCurrentScene()->Add(obj);
		}
	}


	//Grid
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetPosition(Vec3(0.f,0.f,0.f));
		//obj->GetOrAddTransform()->SetScale(Vec3(0.6f, .6f, .6f));
		obj->AddComponent(make_shared<MeshRenderer>());

		MaterialDesc matdesc;
		matdesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);


		auto mat = make_shared<Material>();
		mat->SetShader(lightShader);
		mat->SetMaterialDesc(matdesc);
		mat->SetDiffuseTexture(RESOURCES->Get<Texture>(L"grass"));

		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		//

		shared_ptr<Geometry<VertexFull>> geo_cylinder = make_shared<Geometry<VertexFull>>();

		GeometryHelper::CreateGrid(geo_cylinder, 30,30,100,100,10,10);
		mesh->SetGeometry(geo_cylinder);

		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetMaterial(mat);
		//obj->GetMeshRenderer()->SetRasterizerState(RenderStates::WireframeRS);

		SCENE->GetCurrentScene()->Add(obj);
	
	}

	shared_ptr<Model> tankModel = make_shared<Model>();
	tankModel->ReadModel(L"Tank/Tank");
	tankModel->ReadMaterial(L"Tank/Tank");
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform()->SetPosition(Vec3(0.f,2.f,0.f));
		obj->AddComponent(make_shared<ModelRenderer>(modelShader));
		obj->GetModelRenderer()->SetModel(tankModel);
		//.push_back(obj);
		SCENE->GetCurrentScene()->Add(obj);
	}


	//clyinder2
	//{
	//	//device context 도 자동으로 생성됨
	//	auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

	//	obj->GetOrAddTransform()->SetPosition(Vec3(10.f,10.f,0.f));

	//	obj->AddComponent(make_shared<MeshRenderer>());

	//	MaterialDesc matdesc;
	//	matdesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);


	//	auto mat = make_shared<Material>();
	//	mat->SetShader(lightShader);
	//	mat->SetMaterialDesc(matdesc);
	//	mat->SetDiffuseTexture(RESOURCES->Get<Texture>(L"stone"));
	//	mat->SetNormalTexture(RESOURCES->Get<Texture>(L"brick_nmap"));
	//	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	//	//

	//	shared_ptr<Geometry<VertexFull>> geo_cylinder = make_shared<Geometry<VertexFull>>();

	//	GeometryHelper::CreateCylinder(geo_cylinder, 1, 1, 1, 50, 50);
	//	mesh->SetGeometry(geo_cylinder);

	//	obj->GetMeshRenderer()->SetMesh(mesh);
	//	obj->GetMeshRenderer()->SetMaterial(mat);
	//	//obj->GetMeshRenderer()->SetRasterizerState(RenderStates::WireframeRS);

	//	SCENE->GetCurrentScene()->Add(obj);
	//	//



	//}
	


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



	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetScale(Vec3(200.f, 200.f, 1.f));
		obj->GetOrAddTransform()->SetPosition(Vec3(-300, -300.f, 1.f));

		obj->AddComponent(make_shared<MeshRenderer>());

		auto mesh = make_shared<Mesh>();
		mesh->CreateQuad();

		auto mat = make_shared<Material>();
		mat->SetShader(DebugShader);
		shared_ptr<Texture> tex = make_shared<Texture>();
		tex->SetSRV(light_obj->GetLight()->GetShadowMap()->GetDepthMapSRV());
		mat->SetDiffuseTexture(tex);

		obj->GetMeshRenderer()->SetMaterial(mat);
		obj->GetMeshRenderer()->SetMesh(mesh);

		obj->SetLayerIndex(eLayer::Layer_UI);
		SCENE->GetCurrentScene()->Add(obj);

	}

	
	 

	SCENE->GetCurrentScene()->Start();
}

void ShadowDemo::Update()
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

void ShadowDemo::Render()
{
	
	
}
