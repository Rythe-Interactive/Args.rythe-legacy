#include "Components/Renderable.h"
#include "Components/RenderData.h"

void Args::Renderable::SetMaterial(const std::string& materialName)
{
	Material* newMaterial = Material::GetMaterial(materialName);
	if (newMaterial == nullptr)
	{
		Debug::Warning(DebugInfo, "Material %s doesn't exist", materialName.c_str());
		return;
	}

	if (mesh != "")
	{
		RenderData* renderData = owner->manager->GetGlobalComponent<RenderData>();

		if (renderData->batches.count(mesh))
			if (renderData->batches[mesh].count(material))
				if (renderData->batches[mesh][material].count(owner))
					renderData->batches[mesh][material].erase(owner);

		renderData->batches[mesh][materialName].insert(owner);
	}

	material = materialName;
}

void Args::Renderable::SetMesh(const std::string& meshName)
{
	Mesh* newMesh = Mesh::GetMesh(meshName);
	if (newMesh == nullptr)
	{
		Debug::Warning(DebugInfo, "Mesh %s doesn't exist", meshName.c_str());
		return;
	}

	if (material != "")
	{
		RenderData* renderData = owner->manager->GetGlobalComponent<RenderData>();

		if (renderData->batches.count(mesh))
			if (renderData->batches[mesh].count(material))
				if (renderData->batches[mesh][material].count(owner))
					renderData->batches[mesh][material].erase(owner);

		renderData->batches[meshName][material].insert(owner);
	}

	mesh = meshName;
}

void Args::Renderable::CleanUp()
{
	RenderData* renderData = owner->manager->GetGlobalComponent<RenderData>();

	if (renderData->batches.count(mesh))
		if (renderData->batches[mesh].count(material))
			if (renderData->batches[mesh][material].count(owner))
				renderData->batches[mesh][material].erase(owner);
}

std::string Args::Renderable::ObjectType()
{
	return Args::GetTypeName<Renderable>();
}

bool Args::Renderable::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::Renderable::GetData(const std::string& name, std::string& value)
{
	return false;
}
