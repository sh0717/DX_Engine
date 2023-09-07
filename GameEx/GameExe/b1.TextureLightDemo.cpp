#include "pch.h"
#include "b1.TextureLightDemo.h"
#include "SphereCollider.h"
#include  "Wave.h"
#include "RenderStates.h"

float GetHillHeight(float x, float z)
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z))*0.5;
}

void BookTextureDemo::Init()
{

	modelAnimShader = make_shared<Shader>(L"16.AnimDemo.fx");
	lightShader = make_shared<Shader>(L"07.Light.fx");
	modelShader = make_shared<Shader>(L"15.ModelDemo.fx");
	treeShader = make_shared<Shader>(L"21.tree.fx");

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

	{
		mOverlapCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

		mOverlapCamera->AddComponent(make_shared<CameraTransform>());
		mOverlapCamera->AddComponent(make_shared<Camera>());
		mOverlapCamera->AddComponent(make_shared<CameraScript>());

		mOverlapCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -5.f));
		mOverlapCamera->GetCamera()->SetCullingMaskAll();
		mOverlapCamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_Overlap, OFF);


		SCENE->GetCurrentScene()->Add(mOverlapCamera);

	}


	RESOURCES->Load<Texture>(L"Atrox", L"../Resources/Textures/Atrox_drx.jpg");
	RESOURCES->Load<Texture>(L"grass", L"../Resources/Textures/grass.dds");
	RESOURCES->Load<Texture>(L"water", L"../Resources/Textures/water1.dds");
	RESOURCES->Load<Texture>(L"tree1", L"../Resources/Textures/tree1.dds");

	//지형 만들기 
	{
		shared_ptr<Material> atroxMaterial = make_shared<Material>();
		
		MaterialDesc desc;
		desc.Ambient = Vec4(0.5f);
		desc.Diffuse = Vec4(1.f);
		desc.Specular = Vec4(0.f,0.f,0.f,1.f);

		atroxMaterial->SetMaterialDesc(desc);
		atroxMaterial->SetShader(lightShader);
		atroxMaterial->SetDiffuseTexture(RESOURCES->Get<Texture>(L"grass"));


		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform()->SetPosition(Vec3(0,-3.f, 0.f));
		obj->GetOrAddTransform()->SetScale(Vec3(1.f));

		obj->AddComponent(make_shared<MeshRenderer>());
		obj->GetMeshRenderer()->SetMaterial(atroxMaterial);



		//mesh 생성하기 

		shared_ptr<Mesh> mountainMesh = make_shared<Mesh>();
		shared_ptr<Geometry<VertexFull>> mountainGeo = make_shared<Geometry<VertexFull>>();
		GeometryHelper::CreateGrid(mountainGeo, 100,100,50,50,10,10);


		std::vector<VertexFull>& vertices = mountainGeo->GetVertices();
		for (auto& v : vertices) {
			float posX = v.Position.x;
			float posY = v.Position.y;
			float posZ = v.Position.z;

			v.Position.y = GetHillHeight(posX, posZ);
		}

		mountainMesh->SetGeometry(mountainGeo,false);

		obj->GetMeshRenderer()->SetMesh(mountainMesh);



		obj->SetLayerIndex(eLayer::Layer_Default);
		SCENE->GetCurrentScene()->Add(obj);


	}


	//Wave 만들기 
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform()->SetPosition(Vec3(0, -3.f, 0.f));
		obj->GetOrAddTransform()->SetScale(Vec3(1.f));
		obj->AddComponent(make_shared<MeshRenderer>());


		//material 
		shared_ptr<Material> waterMaterial = make_shared<Material>();

		MaterialDesc desc;
		desc.Ambient = Vec4(0.2f);
		desc.Diffuse = Vec4(0.7f);
		desc.Specular = Vec4(0.f,0.f,0.f,1.f);
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
		mesh->SetGeometry(waveGeo, true,false);
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
			float y = GetHillHeight(x, z);
			vertex.position = Vec3(x, y, z);
			vertex.size = Vec2(5.f, 10.f);
			 
			vec.push_back(vertex);
		}

		auto obj = make_shared<DefaultObject>(DEVICE,CONTEXT);
		obj->AddComponent(make_shared<BillboardRenderer>());
		obj->GetBillboardRenderer()->Create(vec);

		auto mat = make_shared<Material>();
		mat->SetShader(treeShader);
		mat->SetDiffuseTexture(RESOURCES->Get<Texture>(L"tree1"));


		obj->GetBillboardRenderer()->SetMaterial(mat);
		
		SCENE->GetCurrentScene()->Add(obj);
		
	}



	DirectionalLight lightDesc;
	{
		lightDesc.Ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.1f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(0.f, -1.f, 0.f);
	}

	shared_ptr<DefaultObject> light_obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
	light_obj->GetOrAddTransform();
	light_obj->AddComponent(make_shared<Light>());
	light_obj->GetLight()->SetLightType(eLightType::Direction);
	light_obj->GetLight()->SetLightDesc(lightDesc);
	SCENE->GetCurrentScene()->Add(light_obj);




	


	SCENE->GetCurrentScene()->Start();
}

void BookTextureDemo::Update()
{
	//큐브맵의 위치 바꾸기
	cubeObj->GetOrAddTransform()->SetPosition(Camera::GetCameraPosition());
	




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
		lightDesc.Specular = Vec4(0.1f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, -1.f);


		ManyDirectionalLightDesc manydesc;
		manydesc.num_light = 1;
		manydesc.dirlights[0] = lightDesc;


		//lightShader->PushManyDirLightBuffer(manydesc);

		treeShader->PushDirLightBuffer(lightDesc);
		modelShader->PushDirLightBuffer(lightDesc);
		//lightShader->PushDirLightBuffer(lightDesc);
		
		
	}

	



	//if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	//{
	//	int32 mouseX = INPUT->GetMousePos().x;
	//	int32 mouseY = INPUT->GetMousePos().y;

	//	// Picking
	//	auto pickObj = SCENE->GetCurrentScene()->Pick(mouseX, mouseY);
	//	if (pickObj)
	//	{
	//		SCENE->GetCurrentScene()->Remove(pickObj);
	//	}
	//}


	
}

void BookTextureDemo::Render()
{
	
	
}

void BookTextureDemo::CreateKachujin()
{
	shared_ptr<class Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Kachujin/Kachujin");
	m1->ReadMaterial(L"Kachujin/Kachujin");
	m1->ReadAnimation(L"Kachujin/Idle");
	m1->ReadAnimation(L"Kachujin/Run");
	m1->ReadAnimation(L"Kachujin/Slash");

	for (int i = 0; i < 500; i++) {
		
		auto _obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		_obj->GetOrAddTransform()->SetPosition(Vec3(rand()%100-50, 0, rand()%100-50));
		_obj->GetOrAddTransform()->SetScale(Vec3(0.01f));

		_obj->AddComponent(make_shared<ModelAnimator>(modelShader));
		{
			_obj->GetModelAnimator()->SetModel(m1);
			//_obj->GetModelAnimator()->SetPass(1);
		}

		SCENE->GetCurrentScene()->Add(_obj);
		
	}
}
