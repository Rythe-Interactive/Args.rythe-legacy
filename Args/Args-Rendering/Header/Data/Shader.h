#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <Args-Core.h>
#include "Texture.h"
#include "Data/LightData.h"

namespace Args
{
	class Mesh;
	struct Camera;
	class Shader;

#pragma region Shader parameters
	class IShaderParameter
	{
	protected:
		Shader* shader;
		std::string		name;
		GLenum			type;
		GLint			location;

		IShaderParameter(Shader* shader, std::string name, GLenum type, GLint location) : shader(shader), name(name), type(type), location(location) {};

	public:
		virtual bool IsValid() const
		{
			if (this == nullptr)
				return false;

			return location != -1;
		}
		virtual GLenum GetType() const { return type; }
		virtual GLint GetLocation() const { return location; }
	};

	class Sampler : public IShaderParameter
	{
	protected:
		GLint sampler;
	public:
		Sampler(Shader* shader, std::string name, GLenum type, GLint location, GLint sampler) : IShaderParameter(shader, name, type, location), sampler(sampler) {}

		void SetTexture(const Texture* texture)
		{
			glActiveTexture(GL_TEXTURE0 + sampler);
			glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
			glUniform1i(location, sampler);
		}
	};

	template<typename T>
	class Uniform : public IShaderParameter
	{
	public:
		Uniform(Shader* shader, std::string name, GLenum type, GLint location) : IShaderParameter(shader, name, type, location) {}

		void SetValue(const T& value);
	};

	template<typename T>
	inline void Uniform<T>::SetValue(const T& value)
	{
	}

	template<>
	inline void Uniform<float>::SetValue(const float& value)
	{
		if (this != nullptr)
			glUniform1f(location, value);
	}

	template<>
	inline void Uniform<Vector2>::SetValue(const Vector2& value)
	{
		if (this != nullptr)
			glUniform2fv(location, 1, value_ptr(value));
	}

	template<>
	inline void Uniform<Vector3>::SetValue(const Vector3& value)
	{
		if (this != nullptr)
			glUniform3fv(location, 1, value_ptr(value));
	}

	template<>
	inline void Uniform<Vector4>::SetValue(const Vector4& value)
	{
		if (this != nullptr)
			glUniform4fv(location, 1, value_ptr(value));
	}

	template<>
	inline void Uniform<int>::SetValue(const int& value)
	{
		if (this != nullptr)
			glUniform1i(location, value);
	}

	template<>
	inline void Uniform<IVector2>::SetValue(const IVector2& value)
	{
		if (this != nullptr)
			glUniform2iv(location, 1, value_ptr(value));
	}

	template<>
	inline void Uniform<IVector3>::SetValue(const IVector3& value)
	{
		if (this != nullptr)
			glUniform3iv(location, 1, value_ptr(value));
	}

	template<>
	inline void Uniform<IVector4>::SetValue(const IVector4& value)
	{
		if (this != nullptr)
			glUniform4iv(location, 1, value_ptr(value));
	}

	template<>
	inline void Uniform<bool>::SetValue(const bool& value)
	{
		if (this != nullptr)
			glUniform1i(location, value);
	}

	template<>
	inline void Uniform<BVector2>::SetValue(const BVector2& value)
	{
		if (this != nullptr)
			glUniform2iv(location, 1, value_ptr(IVector2(value)));
	}

	template<>
	inline void Uniform<BVector3>::SetValue(const BVector3& value)
	{
		if (this != nullptr)
			glUniform3iv(location, 1, value_ptr(IVector3(value)));
	}

	template<>
	inline void Uniform<BVector4>::SetValue(const BVector4& value)
	{
		if (this != nullptr)
			glUniform4iv(location, 1, value_ptr(IVector4(value)));
	}

	template<>
	inline void Uniform<Matrix2>::SetValue(const Matrix2& value)
	{
		if (this != nullptr)
			glUniformMatrix2fv(location, 1, false, value_ptr(value));
	}

	template<>
	inline void Uniform<Matrix3>::SetValue(const Matrix3& value)
	{
		if (this != nullptr)
			glUniformMatrix3fv(location, 1, false, value_ptr(value));
	}

	template<>
	inline void Uniform<Matrix4>::SetValue(const Matrix4& value)
	{
		if (this != nullptr)
			glUniformMatrix4fv(location, 1, false, value_ptr(value));
	}



	class Attribute : public IShaderParameter
	{
	public:
		Attribute(Shader* shader, std::string name, GLenum type, GLint location) : IShaderParameter(shader, name, type, location) {}

		void SetAttributePointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
		{
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, size, type, normalized, stride, pointer);
		}

		void DisableAttributePointer()
		{
			glDisableVertexAttribArray(location);
		}
	};

#pragma endregion

	class Shader
	{
	public:
		static Shader* CreateShader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader);
		static Shader* GetShader(const std::string& name);

		void Use();
		void Bind(Mesh* mesh, const std::vector<LightData>& lights);
		void Render(const std::vector<Matrix4>& instances, Mesh* mesh, Camera* camera);
		void Release(Mesh* mesh);

		GLuint GetUniformBlockIndex(const std::string& pName) const;
		void BindUniformBlock(GLuint uniformBlockIndex, GLuint uniformBlockBinding) const;

		Sampler* GetSampler(const std::string& name);

		template<typename T>
		Uniform<T>* GetUniform(const std::string& name);

		Attribute* GetAttribute(const std::string& name);

		std::vector<std::string> GetSamplerNames();
		std::vector<std::pair<std::string, GLenum>> GetUniformInfo();

	private:
		std::string name;

		GLint programId;
		GLuint modelMatrixBufferId;
		GLint modelMatrixAttrib;
		static GLuint lightsBufferId;

		std::vector<GLuint> shaderIds;
		static std::unordered_map<std::string, Shader*> shaders;

		std::unordered_map<std::string, std::unique_ptr<Sampler>> samplers;
		std::unordered_map<std::string, std::unique_ptr<IShaderParameter>> uniforms;
		std::unordered_map<std::string, std::unique_ptr<Attribute>> attributes;

		Shader(const std::string& name);

		void Finalize();

		void AddShader(GLuint pShaderType, const std::string& pShaderPath);

		std::string ReadFile(const std::string& shaderPath);

		GLuint CompileShader(GLuint shaderType, const std::string& shaderSource);

		void ProcessIncludes(std::string& shaderSource);

		void ProcessParameters();

		GLuint GetUniformLocation(const std::string& pName) const;

		GLuint GetAttribLocation(const std::string& pName) const;
	};

	template<typename T>
	inline Uniform<T>* Shader::GetUniform(const std::string& name)
	{
		return dynamic_cast<Uniform<T>*>(uniforms[name].get());
	}
}