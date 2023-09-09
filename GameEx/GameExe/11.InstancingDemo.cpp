#include "pch.h"
#include "11.InstancingDemo.h"
#include "SphereCollider.h"
int a = 1;
void InstancingDemo::Init()
{	
	/*if (a == 1) {
		CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		a = 0;
	}*/
	shared_ptr<Texture> CubeMapTexture = make_shared<Texture>();

	CubeMapTexture->Load(L"../Resources/Textures/grasscube1024.dds");
	shared_ptr<Shader> cubeShader = make_shared<Shader>(L"08.CubeMap.fx");
	

	shared_ptr<Material> cubeMaterial = make_shared<Material>();
	cubeMaterial->SetShader(cubeShader);
	cubeMaterial->SetCubeTexture(CubeMapTexture);



	modelShader = make_shared<Shader>(L"16.AnimDemo.fx");

	lightShader = make_shared<Shader>(L"07.Light.fx");
	tankShader = make_shared<Shader>(L"15.ModelDemo.fx");
	RESOURCES->Load<Texture>(L"Atrox", L"../Resources/Textures/Atrox_drx.jpg");


	shared_ptr<Material> atroxMaterial = make_shared<Material>();
	{
		MaterialDesc desc;
		desc.Ambient = Vec4(0.2f);
		desc.Diffuse = Vec4(1.f);
		desc.Specular = Vec4(0.f);

		atroxMaterial->SetMaterialDesc(desc);
	}
	atroxMaterial->SetShader(lightShader);
	atroxMaterial->SetDiffuseTexture(RESOURCES->Get<Texture>(L"Atrox"));

	for (int i = 0; i < 500; i++) {
	
		auto obj = make_shared<DefaultObject>(DEVICE,CONTEXT);
		obj->GetOrAddTransform()->SetPosition(Vec3(rand()%100,-30.f,rand()%110));
		obj->AddComponent(make_shared<MeshRenderer>());
		obj->GetMeshRenderer()->SetMaterial(atroxMaterial);
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		obj->GetMeshRenderer()->SetMesh(mesh);
		SCENE->GetCurrentScene()->Add(obj);
		//mObjs.push_back(obj);

	}

	shared_ptr<Model> tankModel = make_shared<Model>();
	tankModel->ReadModel(L"Tank/Tank");
	tankModel->ReadMaterial(L"Tank/Tank");

	for (int i = 0; i < 0; i++) 
	{
		auto obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		obj->GetOrAddTransform()->SetPosition(Vec3(rand() % 100, 0.f, rand() % 110));
		obj->AddComponent(make_shared<ModelRenderer>(tankShader));
		obj->GetModelRenderer()->SetModel(tankModel);
		//.push_back(obj);
		SCENE->GetCurrentScene()->Add(obj);
	}


	///오브젝트 만들고 부품 붙이기
	//오브젝트 만들 때에는 device랑 컨텍스트 둘다 주자
	//1.트랜스폼 2.카메라 3. 카메라 스크립트
	mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	mCamera->AddComponent(make_shared<CameraTransform>());
	mCamera->AddComponent(make_shared<Camera>());
	mCamera->AddComponent(make_shared<CameraScript>());

	mCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -20.f));

	SCENE->GetCurrentScene()->Add(mCamera);

	//object용 머티리얼 만들기 .(머티리얼데스크, 쉐이더, 텍스쳐)



	//atrox 만들기
	





	cubeObj = make_shared<DefaultObject>(DEVICE, CONTEXT);
	
	cubeObj->GetOrAddTransform()->SetScale(Vec3(30.f, 30.f, 30.f));
	cubeObj->AddComponent(make_shared<MeshRenderer>());
	{
		cubeObj->GetMeshRenderer()->SetMaterial(cubeMaterial);
	}

	{
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		cubeObj->GetMeshRenderer()->SetMesh(mesh);
	}

	SCENE->GetCurrentScene()->Add(cubeObj);
	



	//한 500 개 생성후 mObjs에 추가하자 
	CreateKachujin();




	///라이트 만들기 

	DirectionalLight lightDesc;
	{
		lightDesc.Ambient = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, -1.f);
	}

	shared_ptr<DefaultObject> light_obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
	light_obj->GetOrAddTransform();
	light_obj->AddComponent(make_shared<Light>());
	light_obj->GetLight()->SetLightType(eLightType::Direction);
	light_obj->GetLight()->SetLightDesc(lightDesc);
	SCENE->GetCurrentScene()->Add(light_obj);

	SCENE->GetCurrentScene()->Start();
	
}

void InstancingDemo::Update()
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
		lightDesc.Ambient = Vec4(0.6f, 0.6f, 0.4f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.2f, 0.1f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, -1.f);


		ManyDirectionalLightDesc manydesc;
		manydesc.num_light = 1;
		manydesc.dirlights[0] = lightDesc;


		//lightShader->PushManyDirLightBuffer(manydesc);


		modelShader->PushDirLightBuffer(lightDesc);
		tankShader->PushDirLightBuffer(lightDesc);
		lightShader->PushDirLightBuffer(lightDesc);
		
		
	}

	



	
	


	
}

void InstancingDemo::Render()
{
	
	
}

void InstancingDemo::CreateKachujin()
{
	shared_ptr<class Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Kachujin/Kachujin");
	m1->ReadMaterial(L"Kachujin/Kachujin");
	m1->ReadAnimation(L"Kachujin/Idle");
	m1->ReadAnimation(L"Kachujin/Run");
	m1->ReadAnimation(L"Kachujin/Slash");

	for (int i = 0; i < 1; i++) {
		
		auto _obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		_obj->GetOrAddTransform()->SetPosition(Vec3(rand()%100-50, 0, rand()%100-50));
		_obj->GetOrAddTransform()->SetPosition(Vec3(0.f));
		_obj->GetOrAddTransform()->SetScale(Vec3(0.01f));

		_obj->AddComponent(make_shared<ModelAnimator>(modelShader));
		{
			_obj->GetModelAnimator()->SetModel(m1);
			//_obj->GetModelAnimator()->SetPass(1);
		}
		//_obj->AddComponent(make_shared<SphereCollider>());

		SCENE->GetCurrentScene()->Add(_obj);
		
	}




	shared_ptr<class Model> m2 = make_shared<Model>();
	m2->ReadModel(L"Wolf/Wolf");
	m2->ReadMaterial(L"Wolf/Wolf");
	m2->ReadAnimation(L"Wolf/Anim_0");
	m2->ReadAnimation(L"Wolf/Anim_1");
	m2->ReadAnimation(L"Wolf/Anim_2");
	//m2->ReadAnimation(L"Wolf/Anim_3");
	

	for (int i = 0; i < 500; i++) {

		auto _obj = make_shared<DefaultObject>(DEVICE, CONTEXT);
		
		_obj->GetOrAddTransform()->SetPosition(Vec3(Vec3(rand() % 100 - 50, 0, rand() % 100 - 50)));
		_obj->GetOrAddTransform()->SetScale(Vec3(0.01f));

		_obj->AddComponent(make_shared<ModelAnimator>(modelShader));
		{
			_obj->GetModelAnimator()->SetModel(m2);
			//_obj->GetModelAnimator()->SetPass(1);
		}
		//_obj->AddComponent(make_shared<SphereCollider>());

		SCENE->GetCurrentScene()->Add(_obj);

	}
}
