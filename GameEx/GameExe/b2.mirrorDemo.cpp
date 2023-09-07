#include "pch.h"
#include "b2.mirrorDemo.h"
#include "SphereCollider.h"
#include "Button.h"
#include  "Wave.h"
#include "RenderStates.h"

//float GetHillHeight(float x, float z)
//{
//	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z))*0.5;
//}

void MirrorDemo::Init()
{
	DebugShader = make_shared<Shader>(L"TextureDebug.fx");
	modelAnimShader = make_shared<Shader>(L"16.AnimDemo.fx");
	lightShader = make_shared<Shader>(L"07.Light.fx");
	modelShader = make_shared<Shader>(L"15.ModelDemo.fx");

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
	{
		mCameraOverlap = make_shared<DefaultObject>(DEVICE, CONTEXT);

		mCameraOverlap->AddComponent(make_shared<CameraTransform>());
		mCameraOverlap->AddComponent(make_shared<Camera>());
		mCameraOverlap->AddComponent(make_shared<CameraScript>());

		mCameraOverlap->GetTransform()->SetPosition(Vec3(0.f, 0.f, -5.f));
		mCameraOverlap->GetCamera()->SetCullingMaskAll();
		mCameraOverlap->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_Overlap, OFF);


		SCENE->GetCurrentScene()->Add(mCameraOverlap);

	}


	{
		UICamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

		//UICamera->AddComponent(make_shared<CameraTransform>());
		UICamera->AddComponent(make_shared<Camera>());
		//UICamera->AddComponent(make_shared<CameraScript>());

		UICamera->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, -50.f));
		UICamera->GetCamera()->SetCullingMaskAll();
		UICamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_UI, OFF);
		UICamera->GetCamera()->SetMode(eProjectionType::ORTHO);
		UICamera->GetCamera()->SetNear(1.f);
		UICamera->GetCamera()->SetFar(100.f);


		SCENE->GetCurrentScene()->Add(UICamera);
	}

	RESOURCES->Load<Texture>(L"Atrox", L"../Resources/Textures/Atrox_drx.jpg");
	RESOURCES->Load<Texture>(L"grass", L"../Resources/Textures/grass.dds");
	RESOURCES->Load<Texture>(L"water", L"../Resources/Textures/water1.dds");

	//지형 만들기 
	{
		shared_ptr<Material> atroxMaterial = make_shared<Material>();
		
		MaterialDesc desc;
		desc.Ambient = Vec4(0.2f);
		desc.Diffuse = Vec4(0.5f,0.5f,0.5f,1.0f);
		desc.Specular = Vec4(0.1f);

		atroxMaterial->SetMaterialDesc(desc);
		atroxMaterial->SetShader(lightShader);
		atroxMaterial->SetDiffuseTexture(RESOURCES->Get<Texture>(L"grass"));


		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform()->SetPosition(Vec3(0,0.f, 15.f));
		obj->GetOrAddTransform()->SetScale(Vec3(10.f));

		obj->AddComponent(make_shared<MeshRenderer>());
		obj->GetMeshRenderer()->SetMaterial(atroxMaterial);



		//mesh 생성하기 

		
		obj->GetMeshRenderer()->SetMesh(RESOURCES->Get<Mesh>(L"Sphere"));

		//obj->GetMeshRenderer()->SetRasterizerState(RenderStates::NoCullRS);

		obj->SetLayerIndex(eLayer::Layer_Default);





		auto obj2 = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj2->GetOrAddTransform()->SetPosition(Vec3(0, -5.f, 10.f));
		obj2->GetOrAddTransform()->SetScale(Vec3(10.f));
		//radian
		obj2->GetOrAddTransform()->SetRotation(Vec3(XM_PIDIV2, 0.f, 0.f));

		obj2->AddComponent(make_shared<MeshRenderer>());
		obj2->GetMeshRenderer()->SetMaterial(atroxMaterial);



		//mesh 생성하기 


		obj2->GetMeshRenderer()->SetMesh(RESOURCES->Get<Mesh>(L"Quad"));

		//obj2->GetMeshRenderer()->SetRasterizerState(RenderStates::NoCullRS);

		obj2->SetLayerIndex(eLayer::Layer_Default);

		
		//SCENE->GetCurrentScene()->Add(obj);
		SCENE->GetCurrentScene()->Add(obj2);

	}

	
	//Tank 추가하기
	{
	
		shared_ptr<DefaultObject> obj = make_shared<DefaultObject>(DEVICE,CONTEXT);
		obj->GetOrAddTransform()->SetScale(Vec3(0.3f));
		obj->GetOrAddTransform()->SetPosition(Vec3(0.f,0.f,10.f));
		obj->AddComponent(make_shared<ModelRenderer>(modelShader));
		

		shared_ptr<Model> tankModel = make_shared<Model>();
		tankModel->ReadMaterial(L"Tank/Tank");
		tankModel->ReadModel(L"Tank/Tank");

		obj->GetModelRenderer()->SetModel(tankModel);

		SCENE->GetCurrentScene()->Add(obj);
	}
	
	//mirror stencil marking 
	{
		
		auto obj2 = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj2->GetOrAddTransform()->SetPosition(Vec3(0, 0, 15.f));
		obj2->GetOrAddTransform()->SetScale(Vec3(7.f,7.f,1.f));
		
		

		obj2->AddComponent(make_shared<MeshRenderer>());
		obj2->GetMeshRenderer()->SetMaterial(mirrorMaterial);


		obj2->GetMeshRenderer()->SetBlendState(RenderStates::NoRenderWriteBS);
		obj2->GetMeshRenderer()->SetDepthStencilState(RenderStates::MarkMirrorDSS);
		obj2->GetMeshRenderer()->StencilRef = 1;

		//mesh 생성하기 

		obj2->GetMeshRenderer()->SetID(0);
		obj2->GetMeshRenderer()->SetMesh(RESOURCES->Get<Mesh>(L"Quad"));

		obj2->SetLayerIndex(eLayer::Layer_Default);
		SCENE->GetCurrentScene()->Add(obj2);
	}


	//반사된 탱크 
	{
	
		shared_ptr<DefaultObject> obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform()->SetScale(Vec3(0.3f));
		obj->AddComponent(make_shared<ModelRenderer>(modelShader));


		XMVECTOR mirrorPlane = ::XMVectorSet(0.0f, 0.0f, 1.0f, -10.0f); // xy plane
		XMMATRIX R = ::XMMatrixReflect(mirrorPlane);

		Matrix world = obj->GetOrAddTransform()->GetWorldMatrix();
		obj->GetOrAddTransform()->SetWorldMatrix(world* R);

		shared_ptr<Model> tankModel = make_shared<Model>();
		tankModel->ReadMaterial(L"Tank/Tank");
		tankModel->ReadModel(L"Tank/Tank");

		obj->GetModelRenderer()->SetModel(tankModel);
		obj->GetModelRenderer()->SetID(2);
		obj->GetModelRenderer()->SetRasterizerState(RenderStates::CullClockwiseRS);
		obj->GetModelRenderer()->SetDepthStencilState(RenderStates::DrawReflectionDSS);
		obj->GetModelRenderer()->StencilRef = 1;

		SCENE->GetCurrentScene()->Add(obj);
	
	
	}






	{
		auto obj2 = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj2->GetOrAddTransform()->SetPosition(Vec3(0, 0, 15.f));
		obj2->GetOrAddTransform()->SetScale(Vec3(7.f, 7.f, 1.f));

		obj2->AddComponent(make_shared<MeshRenderer>());
		obj2->GetMeshRenderer()->SetMaterial(mirrorMaterial);

		obj2->GetMeshRenderer()->SetRasterizerState(RenderStates::NoCullRS);
		obj2->GetMeshRenderer()->SetBlendState(RenderStates::TransparentBS);
		
		

		//mesh 생성하기 
		obj2->GetMeshRenderer()->SetID(3);

		obj2->GetMeshRenderer()->SetMesh(RESOURCES->Get<Mesh>(L"Quad"));

		obj2->SetLayerIndex(eLayer::Layer_Overlap);
		SCENE->GetCurrentScene()->Add(obj2);

	}


	DirectionalLight lightDesc;
	{
		lightDesc.Ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, 1.f);
	}




	//UI 추가하기
	{

		MaterialDesc desc;
		desc.Ambient = Vec4(1.f);
		desc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.0f);
		desc.Specular = Vec4(0.f);
		auto mater = make_shared<Material>();
		mater->SetDiffuseTexture(RESOURCES->Get<Texture>(L"grass"));
		mater->SetShader(DebugShader);
		mater->SetMaterialDesc(desc);


		auto ojb = make_shared<DefaultObject>(DEVICE,CONTEXT);
		ojb->AddComponent(make_shared<Button>());
		ojb->GetButton()->Create(Vec2(200, 200), Vec2(100, 100), mater);
		ojb->SetLayerIndex(Layer_UI);

		SCENE->GetCurrentScene()->Add(ojb);
	}


	shared_ptr<DefaultObject> light_obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
	light_obj->GetOrAddTransform();
	light_obj->AddComponent(make_shared<Light>());
	light_obj->GetLight()->SetLightType(eLightType::Direction);
	light_obj->GetLight()->SetLightDesc(lightDesc);
	SCENE->GetCurrentScene()->Add(light_obj);




	


	SCENE->GetCurrentScene()->Start();
}

void MirrorDemo::Update()
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

void MirrorDemo::Render()
{
	
	
}

void MirrorDemo::CreateKachujin()
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
