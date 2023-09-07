#pragma once

enum class RESOURCE_TYPE :uint8 {

	NONE = -1,
	MESH,
	SHADER,
	TEXTURE,
	MATERIAL,
	ANIMATION,

	END
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<uint8>(RESOURCE_TYPE::END)
};


class ResourceBase:public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(RESOURCE_TYPE type);
	virtual ~ResourceBase();

	RESOURCE_TYPE GetType() { return mType; }


	void SetName(const wstring& name) { mName = name; }
	const wstring& GetName() { return mName; }
	uint32 GetID() { return id; }

public:
	virtual void Load(const wstring& path) {}
	virtual void Save(const wstring& path) {}
protected:
	

private:

	RESOURCE_TYPE mType=RESOURCE_TYPE::NONE;
	wstring mName;
	wstring mPath;

	uint32 id = 0;
};

