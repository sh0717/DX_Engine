#include "pch.h"

#include "SphereCollider.h"
#include "Button.h"
#include  "Wave.h"
#include "RenderStates.h"
#include "b4.TessilDemo.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "CubeBehavior.h"
#include "CBehavior.h"

//float GetHillHeight(float x, float z)
//{
//	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z))*0.5;
//}

void TessilDemo2::Init()
{
	treeShader = make_shared<Shader>(L"21.tree.fx");
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


	RESOURCES->Load<Texture>(L"tree1", L"../Resources/Textures/tree1.dds");
	//cubemap make
	{
		shared_ptr<Texture> CubeMapTexture = make_shared<Texture>();
		CubeMapTexture->CreateCubeMap(L"../Resources/Textures/grasscube1024.dds");
		shared_ptr<Shader> cubeShader = make_shared<Shader>(L"08.CubeMap.fx");
		shared_ptr<Material> cubeMaterial = make_shared<Material>();
		cubeMaterial->SetShader(cubeShader);
		cubeMaterial->SetCubeTexture(CubeMapTexture);


		cubeObj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		cubeObj->GetOrAddTransform()->SetScale(Vec3(100.f, 100.f, 100.f));
		cubeObj->AddComponent(make_shared<MeshRenderer>());


		cubeObj->GetMeshRenderer()->SetMaterial(cubeMaterial);


		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		cubeObj->GetMeshRenderer()->SetMesh(mesh);
		cubeObj->AddComponent(make_shared<CubeBehavior>());

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
		mOverlapCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

		mOverlapCamera->AddComponent(make_shared<CameraTransform>());
		mOverlapCamera->AddComponent(make_shared<Camera>());
		mOverlapCamera->AddComponent(make_shared<CameraScript>());

		mOverlapCamera->GetTransform()->SetPosition(Vec3(0.f, 10.f, -5.f));
		mOverlapCamera->GetCamera()->SetCullingMaskAll();
		mOverlapCamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_Overlap, OFF);


		SCENE->GetCurrentScene()->Add(mOverlapCamera);

	}


	{
		ParticleCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

		ParticleCamera->AddComponent(make_shared<CameraTransform>());
		ParticleCamera->AddComponent(make_shared<Camera>());
		ParticleCamera->AddComponent(make_shared<CameraScript>());

		ParticleCamera->GetTransform()->SetPosition(Vec3(0.f, 10.f, -5.f));
		ParticleCamera->GetCamera()->SetCullingMaskAll();
		ParticleCamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_Particle, OFF);

		ParticleCamera->GetCamera()->cameraType = eCameraType::Particle;

		SCENE->GetCurrentScene()->Add(ParticleCamera);

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

	
	////terrain
	{
	
		 mTerrain = make_shared<DefaultObject>(DEVICE, CONTEXT);
		 mTerrain->GetOrAddTransform();
		 mTerrain->AddComponent(make_shared<Terrain>());

		 mTerrain->GetTerrain()->InitTerrain(10, 0.1, 2049, 2049, L"../Resources/Textures/terrain.raw");

		auto mat = make_shared<Material>();
		mat->SetShader(terrainShader);
		mat->SetHeightMapTexture(mTerrain->GetTerrain()->GetHieghtMapTextureFromTerrain());



		



		shared_ptr<Texture> blendTexture = make_shared<Texture>();
		blendTexture->Load(L"../Resources/Textures/blend.dds");

		shared_ptr<Texture> terrainTexture = make_shared<Texture>();
		std::vector<wstring>  filenames;
		filenames.push_back(L"../Resources/Textures/grass.dds");
		filenames.push_back(L"../Resources/Textures/darkdirt.dds");
		filenames.push_back(L"../Resources/Textures/lightdirt.dds");
		filenames.push_back(L"../Resources/Textures/snow.dds");
		filenames.push_back(L"../Resources/Textures/snow.dds");

		terrainTexture->LoadArray(filenames);

		mat->SetBlendMapTexture(blendTexture);
		mat->SetDiffuseTextureArray(terrainTexture);

		mTerrain->GetMeshRenderer()->SetMaterial(mat);
		mTerrain->GetMeshRenderer()->SetRasterizerState(RenderStates::WireframeRS);


		SCENE->GetCurrentScene()->Add(mTerrain);


	}


	///cylinder  
	{
		//device context 도 자동으로 생성됨
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetPosition(Vec3(0.f, mTerrain->GetTerrain()->GetHeight(0.f,0.f), 0.f));

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
		//



	}

	//clyinder2
	{
		//device context 도 자동으로 생성됨
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);

		obj->GetOrAddTransform()->SetPosition(Vec3(10.f, mTerrain->GetTerrain()->GetHeight(10.f,0.f)+2.f, 0.f));

		obj->AddComponent(make_shared<MeshRenderer>());

		MaterialDesc matdesc;
		matdesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);


		auto mat = make_shared<Material>();
		mat->SetShader(lightShader);
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
		//



	}

	//wolf 추가
	{
		shared_ptr<class Model> m1 = make_shared<Model>();
		m1->ReadModel(L"Wolf/Wolf");
		m1->ReadMaterial(L"Wolf/Wolf");
		m1->ReadAnimation(L"Wolf/Anim_1");
		m1->ReadAnimation(L"Wolf/Anim_2");
		

		for (int i = 0; i < 20; i++) {

			auto _obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
			float x = rand() % 100 - 50;
			float z = rand() % 100 - 50;
			float y = mTerrain->GetTerrain()->GetHeight(x, z);
			_obj->GetOrAddTransform()->SetPosition(Vec3(x,y,z));
			_obj->GetOrAddTransform()->SetRotation(Vec3(0.f,rand(),0.f));
			
			_obj->GetOrAddTransform()->SetScale(Vec3(0.05f));

			
			_obj->AddComponent(make_shared<ModelAnimator>(modelShader));
			{
				_obj->GetModelAnimator()->SetModel(m1);
				//_obj->GetModelAnimator()->SetPass(1);
			}
			//_obj->AddComponent(make_shared<SphereCollider>());

			SCENE->GetCurrentScene()->Add(_obj);

		}
	
	
	}


	//캐릭터 추가 
	{
		shared_ptr<class Model> m1 = make_shared<Model>();
		m1->ReadModel(L"Kachujin/Kachujin");
		m1->ReadMaterial(L"Kachujin/Kachujin");
		m1->ReadAnimation(L"Kachujin/Idle");
		m1->ReadAnimation(L"Kachujin/Run");
		m1->ReadAnimation(L"Kachujin/Slash");

		for (int i = 0; i < 1; i++) {

			auto _obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
			_obj->GetOrAddTransform()->SetPosition(Vec3(rand() % 100 - 50, 0, rand() % 100 - 50));
			_obj->GetOrAddTransform()->SetPosition(Vec3(0.f));
			_obj->GetOrAddTransform()->SetScale(Vec3(0.01f));
		
			_obj->AddComponent(make_shared<CBehavior>());
			_obj->AddComponent(make_shared<ModelAnimator>(modelAnimShader));
			{
				_obj->GetModelAnimator()->SetModel(m1);
				//_obj->GetModelAnimator()->SetPass(1);
			}
			

			SCENE->GetCurrentScene()->Add(_obj);

		}

	}

	//grass 추가
	{
		shared_ptr<class Model> m1 = make_shared<Model>();
		m1->ReadModel(L"grass/grass");
		m1->ReadMaterial(L"grass/grass");


		for (int i = 0; i < 3000; i++) {

			auto _obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
			float x = rand() % 100 - 50;
			float z = rand() % 100 - 50;
			float y = mTerrain->GetTerrain()->GetHeight(x, z);
			_obj->GetOrAddTransform()->SetPosition(Vec3(x, y, z));
		
			_obj->GetOrAddTransform()->SetScale(Vec3(0.07f));

			_obj->AddComponent(make_shared<SphereCollider>());
			
			_obj->AddComponent(make_shared<ModelRenderer>(modelShader));
			{
				_obj->GetModelRenderer()->SetModel(m1);
				//_obj->GetModelAnimator()->SetPass(1);
			}


			SCENE->GetCurrentScene()->Add(_obj);

		}

	}

	

	//raptor
	{
		shared_ptr<class Model> m1 = make_shared<Model>();
		m1->ReadModel(L"Raptor/Raptor");
		m1->ReadMaterial(L"Raptor/Raptor");


		for (int i = 0; i < 10; i++) {

			auto _obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
			float x = rand() % 100 - 50;
			float z = rand() % 100 - 50;
			float y = mTerrain->GetTerrain()->GetHeight(x, z);
			_obj->GetOrAddTransform()->SetPosition(Vec3(x, y, z));

			_obj->GetOrAddTransform()->SetScale(Vec3(0.03f));
			_obj->GetOrAddTransform()->SetRotation(Vec3(0.f, rand(), 0.f));
		

			_obj->AddComponent(make_shared<ModelRenderer>(modelShader));
			{
				_obj->GetModelRenderer()->SetModel(m1);
				//_obj->GetModelAnimator()->SetPass(1);
			}


			SCENE->GetCurrentScene()->Add(_obj);

		}

	}


	//Wave 만들기 
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform()->SetPosition(Vec3(0, 0.f, 0.f));
		obj->GetOrAddTransform()->SetScale(Vec3(3.f,1.f,3.f));
		obj->AddComponent(make_shared<MeshRenderer>());


		//material 
		shared_ptr<Material> waterMaterial = make_shared<Material>();

		MaterialDesc desc;
		desc.Ambient = Vec4(0.2f);
		desc.Diffuse = Vec4(0.7f);
		desc.Specular = Vec4(0.f, 0.f, 0.f, 1.f);
		waterMaterial->SetMaterialDesc(desc);
		waterMaterial->SetDiffuseTexture(RESOURCES->Get<Texture>(L"water"));
		waterMaterial->SetShader(lightShader);


		obj->GetMeshRenderer()->SetMaterial(waterMaterial);

		///wave 붙이기
		obj->AddComponent(make_shared<Wave>(60, 60, 1.f, 0.03f, 3.25f, 0.4f));
		auto wave = obj->GetWave();
		

		//mesh 붙이기 
		auto mesh = make_shared<Mesh>();
		shared_ptr<Geometry<VertexFull>> waveGeo = make_shared<Geometry<VertexFull>>();


		GeometryHelper::CreateWave(waveGeo, wave->GetRowCount(), wave->GetColumnCount());

		//CPUWRITE on
		mesh->SetGeometry(waveGeo, true, false);
		obj->GetMeshRenderer()->SetMesh(mesh);

		obj->SetLayerIndex(Layer_Overlap);

		obj->GetMeshRenderer()->SetBlendState(RenderStates::TransparentBS);

		SCENE->GetCurrentScene()->Add(obj);

	}



	//tree billboard 만들기 
	{

		std::vector<Vertex::VertexPosSize> vec;
		for (int i = 0; i < 10; i++) {
			VertexPosSize vertex;
			float x = MathHelper::RandF(-35.0f, 35.0f);
			float z = MathHelper::RandF(-35.0f, 35.0f);
			float y = mTerrain->GetTerrain()->GetHeight(x, z);
			vertex.position = Vec3(x, y+3.f, z);
			vertex.size = Vec2(5.f, 10.f);

			vec.push_back(vertex);
		}

		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->AddComponent(make_shared<BillboardRenderer>());
		obj->GetBillboardRenderer()->Create(vec);

		auto mat = make_shared<Material>();
		mat->SetShader(treeShader);
		mat->SetDiffuseTexture(RESOURCES->Get<Texture>(L"tree1"));


		obj->GetBillboardRenderer()->SetMaterial(mat);
		obj->GetBillboardRenderer()->SetBlendState(RenderStates::AlphaToCoverageBS);
		SCENE->GetCurrentScene()->Add(obj);

	}


	//fire 추가
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform();
		obj->AddComponent(make_shared<ParticleSystem>());

		auto mat = make_shared<Material>();          
		
		mat->SetShader(make_shared<Shader>(L"b4.Fire.fx"));
		shared_ptr<Texture> flareTexture = make_shared<Texture>();
		std::vector<wstring> filenames;
		filenames.push_back(L"../Resources/Textures/flare0.dds");
		flareTexture->LoadArray(filenames);

		mat->SetDiffuseTextureArray(flareTexture);


		obj->SetLayerIndex(eLayer::Layer_Particle);
		obj->GetParticleSystem()->SetMaterial(mat);
		obj->GetParticleSystem()->Init();
		SCENE->GetCurrentScene()->Add(obj);
	}



	DirectionalLight lightDesc;
	{
		lightDesc.Ambient = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.3f, 0.3f, 0.3f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, -0.8f);
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
		obj->GetMeshRenderer()->SetPass(0);
		obj->SetLayerIndex(eLayer::Layer_UI);
		SCENE->GetCurrentScene()->Add(obj);

	}

	
	 

	SCENE->GetCurrentScene()->Start();


	cubeObj->GetOrAddTransform()->SetPosition(SCENE->GetCurrentScene()->GetMainCamera()->GetCamera()->GetMyPos());
}

void TessilDemo2::Update()
{
	//큐브맵의 위치 바꾸기
	
	




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
		lightDesc.Ambient = Vec4(0.7f, 0.7f, 0.7f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, 1.f);


		ManyDirectionalLightDesc manydesc;
		manydesc.num_light = 1;
		manydesc.dirlights[0] = lightDesc;


		//lightShader->PushManyDirLightBuffer(manydesc);


		modelShader->PushDirLightBuffer(lightDesc);
		lightShader->PushDirLightBuffer(lightDesc);
		modelAnimShader->PushDirLightBuffer(lightDesc);
		treeShader->PushDirLightBuffer(lightDesc);
		
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

	if (INPUT->GetButtonDown(KEY_TYPE::KEY_1))
	{
		auto state = mTerrain->GetMeshRenderer()->GetRasterizerState();
		if (state.Get() == RenderStates::WireframeRS) {
			mTerrain->GetMeshRenderer()->SetRasterizerState(0);
		}
		else {
			mTerrain->GetMeshRenderer()->SetRasterizerState(RenderStates::WireframeRS);
		}
	}
	
}

void TessilDemo2::Render()
{
	
	
}
