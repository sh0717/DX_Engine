#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"
void AssimpTool::Init()
{
	{
		shared_ptr<Converter> converter = make_shared<Converter>();
		converter->ReadAssetFile(L"Dino/irex.fbx");
		converter->ExportModelData(L"Irex/Irex");
		converter->ExportMaterialData(L"Irex/Irex");

	

	}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();
		converter->ReadAssetFile(L"Dino/raptor.fbx");
		converter->ExportModelData(L"Raptor/Raptor");
		converter->ExportMaterialData(L"Raptor/Raptor");

	}
	/*{
		shared_ptr<Converter> converter = make_shared<Converter>();


		converter->ReadAssetFile(L"Kachujin/Mesh.fbx");

		converter->ExportMaterialData(L"Kachujin/Kachujin");
		converter->ExportModelData(L"Kachujin/Kachujin");

		
		
	}*/
	/*{
		shared_ptr<Converter> converter = make_shared<Converter>();


		converter->ReadAssetFile(L"Kachujin/Run.fbx");

		converter->ExportAnimationData(L"Kachujin/Run");

	}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();


		converter->ReadAssetFile(L"Kachujin/Slash.fbx");

		converter->ExportAnimationData(L"Kachujin/Slash");

	}*/


	
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
