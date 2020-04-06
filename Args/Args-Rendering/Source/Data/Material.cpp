#include "Data/Material.h"

std::unordered_map<std::string, Args::Material> Args::Material::materials;
std::set<std::string> Args::Material::containedMaterials;

void Args::Material::Init(Shader* shader)
{
	this->shader = shader;

	for (std::string& samplerName : shader->GetSamplerNames())
		textures[samplerName] = new MaterialTexture(samplerName);

	for (auto uniformInfo : shader->GetUniformInfo())
		parameters[uniformInfo.first] = IMaterialParameter::CreateParam(uniformInfo);
}


Args::IMaterialParameter* Args::IMaterialParameter::CreateParam(std::pair<std::string, GLenum> info)
{
	switch (info.second)
	{
	case GL_FLOAT:
		return new MaterialParameter<float>(info.first);
		break;
	case GL_FLOAT_VEC2:
		return new MaterialParameter<Vector2>(info.first);
		break;
	case GL_FLOAT_VEC3:
		return new MaterialParameter<Vector3>(info.first);
		break;
	case GL_FLOAT_VEC4:
		return new MaterialParameter<Vector4>(info.first);
		break;
	case GL_INT:
		return new MaterialParameter<int>(info.first);
		break;
	case GL_INT_VEC2:
		return new MaterialParameter<IVector2>(info.first);
		break;
	case GL_INT_VEC3:
		return new MaterialParameter<IVector3>(info.first);
		break;
	case GL_INT_VEC4:
		return new MaterialParameter<IVector4>(info.first);
		break;
	case GL_BOOL:
		return new MaterialParameter<bool>(info.first);
		break;
	case GL_BOOL_VEC2:
		return new MaterialParameter<BVector2>(info.first);
		break;
	case GL_BOOL_VEC3:
		return new MaterialParameter<BVector3>(info.first);
		break;
	case GL_BOOL_VEC4:
		return new MaterialParameter<BVector4>(info.first);
		break;
	case GL_FLOAT_MAT2:
		return new MaterialParameter<Matrix2>(info.first);
		break;
	case GL_FLOAT_MAT3:
		return new MaterialParameter<Matrix3>(info.first);
		break;
	case GL_FLOAT_MAT4:
		return new MaterialParameter<Matrix4>(info.first);
		break;
	default:
		return nullptr;
	}
}


Args::Material* Args::Material::CreateMaterial(const std::string& name, Shader* shader)
{
	Debug::Log(DebugInfo, "Requested material creation with name: %s", name.c_str());

	if (containedMaterials.count(name))
		return &materials[name];

	if (shader == nullptr)
	{
		Debug::Error(DebugInfo, "Shader was null for material %s", name.c_str());
		return nullptr;
	}

	materials[name].Init(shader);
	materials[name].name = name;
	containedMaterials.insert(name);

	Debug::Log(DebugInfo, "Created material with name: %s", name.c_str());

	return &materials[name];
}

Args::Material* Args::Material::GetMaterial(const std::string& name)
{
	if (containedMaterials.count(name))
		return &materials[name];
	return nullptr;
}

void Args::Material::SetTexture(const std::string& name, const std::string& textureName)
{
	if (textures.count(name))
	{
		textures[name]->SetTexture(textureName);
		Debug::Log(DebugInfo, "Set texture %s for material %s to %s", name.c_str(), this->name.c_str(), textureName.c_str());
	}
	else
		Debug::Warning(DebugInfo, "Material %s does not have a texture called %s", this->name.c_str(), name.c_str());
}

void Args::Material::Bind(Mesh* mesh, const std::vector<LightData>& lights)
{
	shader->Use();

	SetParam<int>("lightCount", (int)lights.size());

	for (auto parameter : parameters)
		parameter.second->Apply(shader);

	for (auto texture : textures)
		texture.second->Apply(shader);

	shader->Bind(mesh, lights);
}

void Args::Material::Render(const std::vector<Matrix4>& instances, Mesh* mesh, Camera* camera) const
{
	shader->Render(instances, mesh, camera);
}

void Args::Material::Release(Mesh* mesh) const
{
	shader->Release(mesh);
}
