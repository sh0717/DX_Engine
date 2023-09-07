#include "pch.h"
#include "09.ModelInitDemo.h"

void ModelInitDemo::Init()
{
	lightShader = make_shared<Shader>(L"07.Light.fx");
	
	

	RESOURCES->Load<Texture>(L"Atrox", L"../Resources/Textures/Atrox_drx.jpg");
	//RESOURCES->Load<Texture>(L"CubeMap", L"../Resources/Textures/grasscube1024.dds");

	shared_ptr<Texture> CubeMapTexture = make_shared<Texture>();
	CubeMapTexture->CreateCubeMap(L"../Resources/Textures/grasscube1024.dds");
	shared_ptr<Shader> cubeShader = make_shared<Shader>(L"08.CubeMap.fx");
	

	shared_ptr<Material> cubeMaterial = make_shared<Material>();
	cubeMaterial->SetShader(cubeShader);
	cubeMaterial->SetCubeTexture(CubeMapTexture);




	///오브젝트 만들고 부품 붙이기
	//오브젝트 만들 때에는 device랑 컨텍스트 둘다 주자
	//1.트랜스폼 2.카메라 3. 카메라 스크립트
	mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	mCamera->AddComponent(make_shared<CameraTransform>());
	mCamera->AddComponent(make_shared<Camera>());
	mCamera->AddComponent(make_shared<CameraScript>());

	mCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -5.f));



	//object용 머티리얼 만들기 .(머티리얼데스크, 쉐이더, 텍스쳐)
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





	// Object
	_obj = make_shared<DefaultObject>(DEVICE,CONTEXT);
	_obj->GetOrAddTransform()->SetPosition(Vec3(0.0,0.0,-2.0));

	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		//메쉬렌더러에는 메쉬와 머티리얼이 들어간다.

		_obj->GetMeshRenderer()->SetMaterial(atroxMaterial);
	}
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
	
	}


	// Object2
	_obj2 = make_shared<DefaultObject>(DEVICE, CONTEXT);
	_obj2->GetOrAddTransform()->SetPosition(Vec3{ 0.5f, 0.f, 2.f });
	//_obj2->GetOrAddTransform()->SetScale(Vec3(5.f, 5.f, 5.f));

	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		_obj2->GetMeshRenderer()->SetMaterial(atroxMaterial);
	}
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		
	}



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

	{
		
	}

	mCamera->Init();
	mCamera->Start();


	CreateTank();
	CreateTurret();
}

void ModelInitDemo::Update()
{
	mCamera->Update();
	cubeObj->GetOrAddTransform()->SetPosition(Camera::GetCameraPosition());
	

	



	{
		DirectionalLight lightDesc;
		lightDesc.Ambient = Vec4(0.2f,0.2f,0.2f,1.0f);
		lightDesc.Diffuse = Vec4(1.f,1.f,1.f,1.f);
		lightDesc.Specular = Vec4(0.2f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, -1.f);
		lightShader->PushDirLightBuffer(lightDesc);
		modelShader->PushDirLightBuffer(lightDesc);
	}

	{
		
		_obj->Update();
	}

	{

		_obj2->Update();
	}

	static float f = 0.0f;
	static int counter = 0;


	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();



	
	cubeObj->Update();



	
	modelObj->Update();
	turretObj->Update();
}

void ModelInitDemo::Render()
{
	
}

void ModelInitDemo::CreateTank()
{



	modelShader = make_shared<Shader>(L"15.ModelDemo.fx");
	
	


	shared_ptr<Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Tank/Tank");
	m1->ReadMaterial(L"Tank/Tank");

	modelObj = make_shared<DefaultObject>(DEVICE,CONTEXT);
	modelObj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 50));
	modelObj->GetOrAddTransform()->SetScale(Vec3(1.f));


	modelObj->AddComponent(make_shared<ModelRenderer>(modelShader));
	modelObj->GetModelRenderer()->SetModel(m1);


}

void ModelInitDemo::CreateTurret()
{

	shared_ptr<Model> m2 = make_shared<Model>();
	m2->ReadModel(L"Tower/Tower");
	m2->ReadMaterial(L"Tower/Tower");

	turretObj = make_shared<DefaultObject>(DEVICE, CONTEXT);
	turretObj->GetOrAddTransform()->SetPosition(Vec3(10.f, 0.f, 0.f));

	turretObj->GetOrAddTransform()->SetScale(Vec3(0.01f));

	turretObj->AddComponent(make_shared<ModelRenderer>(modelShader));
	turretObj->GetModelRenderer()->SetModel(m2);
	
}

