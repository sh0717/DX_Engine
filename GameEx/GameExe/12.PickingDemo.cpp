#include "pch.h"
#include "12.PickingDemo.h"
#include "SphereCollider.h"

void PickingDemo::Init()
{
	

	shared_ptr<Texture> CubeMapTexture = make_shared<Texture>();
	CubeMapTexture->CreateCubeMap(L"../Resources/Textures/grasscube1024.dds");
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

	for (int i = 0; i < 1; i++) {
	
		auto obj = make_shared<DefaultObject>(DEVICE,CONTEXT);
		obj->GetOrAddTransform()->SetPosition(Vec3(0,0,0));
		obj->GetOrAddTransform()->SetScale(Vec3(1.f));
		obj->AddComponent(make_shared<MeshRenderer>());
		obj->GetMeshRenderer()->SetMaterial(atroxMaterial);
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->AddComponent(make_shared<SphereCollider>());

		obj->SetLayerIndex(eLayer::Layer_Default);
		SCENE->GetCurrentScene()->Add(obj);
		//mObjs.push_back(obj);

	}


	///������Ʈ ����� ��ǰ ���̱�
	//������Ʈ ���� ������ device�� ���ؽ�Ʈ �Ѵ� ����
	//1.Ʈ������ 2.ī�޶� 3. ī�޶� ��ũ��Ʈ
	mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	mCamera->AddComponent(make_shared<CameraTransform>());
	mCamera->AddComponent(make_shared<Camera>());
	mCamera->AddComponent(make_shared<CameraScript>());

	mCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -5.f));
	mCamera->GetCamera()->SetCullingMaskAll();
	mCamera->GetCamera()->SetCullingMaskLayerOnOff(eLayer::Layer_Default, OFF);


	SCENE->GetCurrentScene()->Add(mCamera);

	//object�� ��Ƽ���� ����� .(��Ƽ���󵥽�ũ, ���̴�, �ؽ���)



	//atrox �����
	





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
	cubeObj->SetLayerIndex(eLayer::Layer_Default);
	SCENE->GetCurrentScene()->Add(cubeObj);
	



	//�� 500 �� ������ mObjs�� �߰����� 
	






	shared_ptr<DefaultObject> light_obj = make_shared<DefaultObject>(DEVICE, CONTEXT);


	SCENE->GetCurrentScene()->Start();
}

void PickingDemo::Update()
{
	//ť����� ��ġ �ٲٱ�
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


	//light ������ �־����
	{
		DirectionalLight lightDesc;
		lightDesc.Ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, -1.f);


		ManyDirectionalLightDesc manydesc;
		manydesc.num_light = 1;
		manydesc.dirlights[0] = lightDesc;


		lightShader->PushManyDirLightBuffer(manydesc);


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

void PickingDemo::Render()
{
	
	
}

void PickingDemo::CreateKachujin()
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
