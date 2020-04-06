#pragma once
#include "Shader.h"
#include "Data/LightData.h"
#include <unordered_map>
#include <string>

namespace Args
{
	class Material;

	class IMaterialParameter
	{
	protected:
		std::string name;
	public:
		IMaterialParameter(const std::string& name) : name(name) {}

		virtual void Apply(Shader* shader) = 0;

		static IMaterialParameter* CreateParam(std::pair<std::string, GLenum> info);
	};

	template<typename T>
	class MaterialParameter : public IMaterialParameter
	{
	private:
		T value;
	public:
		MaterialParameter(const std::string& name) : IMaterialParameter(name) {}

		void SetValue(const T& value) { this->value = value; }
		const T& GetValue() const { return value; }

		virtual void Apply(Shader* shader) override
		{
			shader->GetUniform<T>(name)->SetValue(value);
		}
	};

	class MaterialTexture : public IMaterialParameter
	{
	private:
		std::string texture;
	public:
		MaterialTexture(const std::string& name) : IMaterialParameter(name), texture() {}

		void SetTexture(const std::string& textureName) { this->texture = textureName; }
		std::string GetTexture() const { return texture; }

		// Inherited via IMaterialParameter
		virtual void Apply(Shader* shader) override
		{
			if (texture == "")
			{
				Debug::Warning(DebugInfo, "Texture %s was not assigned, default texture will be loaded instead", name.c_str());
				texture = "Default";
			}

			shader->GetSampler(name)->SetTexture(Texture::GetTexture(texture));
		}
	};

	class Material
	{
	private:
		Shader* shader;

		static std::set<std::string> containedMaterials;
		static std::unordered_map<std::string, Material> materials;

		void Init(Shader* shader);

		std::string name;

		std::unordered_map<std::string, IMaterialParameter*> parameters;
		std::unordered_map<std::string, MaterialTexture*> textures;
	public:
		static Material* CreateMaterial(const std::string& name, Shader* shader);
		static Material* GetMaterial(const std::string& name);

		template<typename T>
		void SetParam(const std::string& name, const T& value)
		{
			if (parameters.count(name))
				dynamic_cast<MaterialParameter<T>*>(parameters[name])->SetValue(value);
		}

		void SetTexture(const std::string& name, const std::string& textureName);

		void Bind(Mesh* mesh, const std::vector<LightData>& lights);
		void Render(const std::vector<Matrix4>& instances, Mesh* mesh, Camera* camera) const;
		void Release(Mesh* mesh) const;
	};
}