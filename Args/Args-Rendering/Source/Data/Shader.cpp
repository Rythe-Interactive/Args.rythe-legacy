#include "Data/Shader.h"
#include "Data/Mesh.h"
#include "Components/Camera.h"
#include <fstream>

const unsigned MAX_LIGHT_COUNT = 100; // <-- Move to config file
const size_t MAX_VBO_SIZE = 65536; // not 1MB <-- Move to config file
std::unordered_map<std::string, Args::Shader*> Args::Shader::shaders;
GLuint Args::Shader::lightsBufferId = -1;

Args::Shader::Shader(const std::string& name) : programId(0), shaderIds(), name(name), modelMatrixAttrib(-1), modelMatrixBufferId(-1)
{
	programId = glCreateProgram();
	Debug::Log(DebugInfo, "Program created with id: %i", programId);
}

void Args::Shader::AddShader(GLuint shaderType, const std::string& shaderPath)
{
	std::string shaderCode = ReadFile(shaderPath);
	if (shaderCode.length() <= 0)
	{
		Debug::Error(DebugInfo, "File \"%s\" was found empty", shaderPath.c_str());
		return;
	}

	GLuint shaderId = CompileShader(shaderType, shaderCode);

	if (shaderId != 0)
		shaderIds.push_back(shaderId);
}

std::string Args::Shader::ReadFile(const std::string& shaderPath)
{
	std::string contents;
	std::ifstream file(shaderPath, std::ios::in);
	if (file.is_open())
	{
		Debug::Log(DebugInfo, "Reading shader file... %s", shaderPath.c_str());
		std::string line = "";
		while (getline(file, line)) contents += "\n" + line;
		file.close();
	}
	else
	{
		Debug::Error(DebugInfo, "Error reading shader %s", shaderPath.c_str());
		contents = "";
	}
	return contents;
}

// compile the code, and detect errors.
GLuint Args::Shader::CompileShader(GLuint shaderType, const std::string& shaderSource)
{
	std::string shadertype = "unknown";

	switch (shaderType)
	{
	case GL_FRAGMENT_SHADER:
		shadertype = "fragment";
		break;
	case GL_VERTEX_SHADER:
		shadertype = "vertex";
		break;
	default:
		break;
	}

	Debug::Log(DebugInfo, "Compiling %s %s shader... ", name.c_str(), shadertype.c_str());

	std::string source = std::string(shaderSource);

	if (shaderType == GL_FRAGMENT_SHADER)
	{
		size_t shaderVersionIndex = source.find("#version");
		size_t shaderStartIndex = source.find("\n", shaderVersionIndex);
		std::string shaderVersion = source.substr(shaderVersionIndex, shaderStartIndex - shaderVersionIndex);
		source = shaderVersion + std::string("\n#define MAX_LIGHT_COUNT ") + std::to_string(MAX_LIGHT_COUNT) + std::string("\n#define PI 3.141592\n#define HALF_PI 1.570796\n") + source.substr(shaderStartIndex);
	}

	ProcessIncludes(source);

	const char* sourcePointer = source.c_str();
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &sourcePointer, nullptr);
	glCompileShader(shaderId);

	GLint compilerResult = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilerResult);

	if (compilerResult)
	{
		Debug::Success(DebugInfo, "Compiled %s %s shader successfully.", name.c_str(), shadertype.c_str());
		return shaderId;
	}
	else { // get error message
		int infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(shaderId, infoLogLength, nullptr, errorMessage);

		Debug::Error(DebugInfo, "Shader error in %s %s shader:\n\t%s", name.c_str(), shadertype.c_str(), errorMessage);
		delete[] errorMessage;
		return 0;
	}
}

void Args::Shader::ProcessIncludes(std::string& shaderSource)
{
	while (size_t includePosition = shaderSource.find("#include") != std::string::npos)
	{
		std::string fileStub = shaderSource.substr(includePosition + 8);
		std::string file = ReadFile(fileStub.substr(0, fileStub.find("\n")));

		if (!file.empty())
		{
			ProcessIncludes(file);
		}

		shaderSource = shaderSource.substr(0, includePosition) + file + fileStub.substr(fileStub.find("\n"));
	}
}

void Args::Shader::ProcessParameters()
{
	samplers.clear();
	uniforms.clear();
	attributes.clear();

	GLint numActiveUniforms = 0;
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

	GLint maxUniformNameLength = 0;
	glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
	std::vector<GLchar> uniformNameData(maxUniformNameLength);

	int samplerCount = 0;
	for (int uniform = 0; uniform < numActiveUniforms; uniform++)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(programId, uniform, maxUniformNameLength, &actualLength, &arraySize, &type, &uniformNameData[0]);

		std::string name(&uniformNameData[0], actualLength);

		if (name.find('[') != std::string::npos)
			continue;

		GLint location = glGetUniformLocation(programId, name.c_str());

		if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
		{
			samplers[name] = std::make_unique<Sampler>(this, name, type, location, samplerCount++);
		}
		else
		{
			IShaderParameter* uniform = nullptr;

			switch (type)
			{
			case GL_FLOAT:
				uniform = new Uniform<float>(this, name, type, location);
				break;
			case GL_FLOAT_VEC2:
				uniform = new Uniform<Vector2>(this, name, type, location);
				break;
			case GL_FLOAT_VEC3:
				uniform = new Uniform<Vector3>(this, name, type, location);
				break;
			case GL_FLOAT_VEC4:
				uniform = new Uniform<Vector4>(this, name, type, location);
				break;
			case GL_INT:
				uniform = new Uniform<int>(this, name, type, location);
				break;
			case GL_INT_VEC2:
				uniform = new Uniform<IVector2>(this, name, type, location);
				break;
			case GL_INT_VEC3:
				uniform = new Uniform<IVector3>(this, name, type, location);
				break;
			case GL_INT_VEC4:
				uniform = new Uniform<IVector4>(this, name, type, location);
				break;
			case GL_BOOL:
				uniform = new Uniform<bool>(this, name, type, location);
				break;
			case GL_BOOL_VEC2:
				uniform = new Uniform<BVector2>(this, name, type, location);
				break;
			case GL_BOOL_VEC3:
				uniform = new Uniform<BVector3>(this, name, type, location);
				break;
			case GL_BOOL_VEC4:
				uniform = new Uniform<BVector4>(this, name, type, location);
				break;
			case GL_FLOAT_MAT2:
				uniform = new Uniform<Matrix2>(this, name, type, location);
				break;
			case GL_FLOAT_MAT3:
				uniform = new Uniform<Matrix3>(this, name, type, location);
				break;
			case GL_FLOAT_MAT4:
				uniform = new Uniform<Matrix4>(this, name, type, location);
				break;
			default:
				continue;
			}

			uniforms[name] = std::unique_ptr<IShaderParameter>(uniform);
		}


		GLint numActiveAttribs = 0;
		glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);

		GLint maxAttribNameLength = 0;
		glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
		std::vector<GLchar> attribNameData(maxAttribNameLength);

		for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
		{
			GLint arraySize = 0;
			GLenum type = 0;
			GLsizei actualLength = 0;
			glGetActiveAttrib(programId, attrib, (GLsizei)attribNameData.size(), &actualLength, &arraySize, &type, &attribNameData[0]);

			std::string name(static_cast<char*>(&attribNameData[0]));
			GLint location = glGetAttribLocation(programId, name.c_str());

			Attribute* attribute = new Attribute(this, name, type, location);
			attributes[name] = std::unique_ptr<Attribute>(attribute);
		}

	}
}

void Args::Shader::Finalize()
{
	for (size_t i = 0; i < shaderIds.size(); ++i)
		glAttachShader(programId, shaderIds[i]);

	glLinkProgram(programId);

	// Check the program
	GLint linkResult = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkResult);

	if (linkResult)
		Debug::Success(DebugInfo, "Program %s linked successfully.", name.c_str());
	else
	{ // error, show message
		int infoLogLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* errorMessage = new char[infoLogLength + 1];
		glGetProgramInfoLog(programId, infoLogLength, nullptr, errorMessage);

		Debug::Error(DebugInfo, "Program error in %s:\n\t%s", name.c_str(), errorMessage);

		delete[] errorMessage;
	}

	for (size_t i = 0; i < shaderIds.size(); ++i)
		glDeleteShader(shaderIds[i]);

	ProcessParameters();

	modelMatrixAttrib = GetAttribLocation("modelMatrix");

	if (modelMatrixAttrib != -1)
	{
		glGenBuffers(1, &modelMatrixBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixBufferId);
		glBufferData(GL_ARRAY_BUFFER, MAX_VBO_SIZE, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
		Debug::Warning(DebugInfo, "Shader %s does not contain attribute \"modelMatrix\"\nNo instancing enabled on this shader", name.c_str());

	if (lightsBufferId == -1)
	{
		glGenBuffers(1, &lightsBufferId);
		glBindBuffer(GL_UNIFORM_BUFFER, lightsBufferId);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData) * MAX_LIGHT_COUNT, NULL, GL_DYNAMIC_DRAW);
	}
	else
		glBindBuffer(GL_UNIFORM_BUFFER, lightsBufferId);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsBufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

GLuint Args::Shader::GetUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(programId, name.c_str());
}

GLuint Args::Shader::GetAttribLocation(const std::string& name) const
{
	return glGetAttribLocation(programId, name.c_str());
}

Args::Shader* Args::Shader::CreateShader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader)
{
	if (shaders.count(name))
		return shaders[name];

	Shader* shader = new Shader(name);
	shader->AddShader(GL_VERTEX_SHADER, ShaderDir + vertexShader);
	shader->AddShader(GL_FRAGMENT_SHADER, ShaderDir + fragmentShader);
	shader->Finalize();
	shaders[name] = shader;
	return shader;
}

Args::Shader* Args::Shader::GetShader(const std::string& name)
{
	return shaders[name];
}

void Args::Shader::Use()
{
	glUseProgram(programId);
}

void Args::Shader::Bind(Mesh* mesh, const std::vector<LightData>& lights)
{
	mesh->Bind(GetAttribute("vertex"), GetAttribute("normal"), GetAttribute("uv"), GetAttribute("tangent"));

	glBindBuffer(GL_UNIFORM_BUFFER, lightsBufferId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightData) * lights.size(), lights.data());
}

void Args::Shader::Render(const std::vector<Matrix4>& instances, Mesh* mesh, Camera* camera)
{
	if (modelMatrixAttrib != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixBufferId);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Matrix4) * instances.size(), &(instances.data()[0]));

		glEnableVertexAttribArray(modelMatrixAttrib + 0);
		glEnableVertexAttribArray(modelMatrixAttrib + 1);
		glEnableVertexAttribArray(modelMatrixAttrib + 2);
		glEnableVertexAttribArray(modelMatrixAttrib + 3);

		glVertexAttribPointer(modelMatrixAttrib + 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(Vector4), (GLvoid*)0);
		glVertexAttribPointer(modelMatrixAttrib + 1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(Vector4), (GLvoid*)(sizeof(Vector4)));
		glVertexAttribPointer(modelMatrixAttrib + 2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(Vector4), (GLvoid*)(2 * sizeof(Vector4)));
		glVertexAttribPointer(modelMatrixAttrib + 3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(Vector4), (GLvoid*)(3 * sizeof(Vector4)));

		glVertexAttribDivisor(modelMatrixAttrib + 0, 1);
		glVertexAttribDivisor(modelMatrixAttrib + 1, 1);
		glVertexAttribDivisor(modelMatrixAttrib + 2, 1);
		glVertexAttribDivisor(modelMatrixAttrib + 3, 1);
	}

	GetUniform<Vector3>("cameraPosition")->SetValue(camera->GetPosition());
	GetUniform<Matrix4>("viewProjectionMatrix")->SetValue(camera->GetViewProjection());

	mesh->Draw((uint)instances.size());

	if (modelMatrixAttrib != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(modelMatrixAttrib + 0);
		glDisableVertexAttribArray(modelMatrixAttrib + 1);
		glDisableVertexAttribArray(modelMatrixAttrib + 2);
		glDisableVertexAttribArray(modelMatrixAttrib + 3);
	}
}

void Args::Shader::Release(Mesh* mesh)
{
	mesh->Unbind(GetAttribute("vertex"), GetAttribute("normal"), GetAttribute("uv"), GetAttribute("tangent"));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glUseProgram(0);
}

GLuint Args::Shader::GetUniformBlockIndex(const std::string& name) const
{
	return glGetUniformBlockIndex(programId, name.c_str());
}

void Args::Shader::BindUniformBlock(GLuint uniformBlockIndex, GLuint uniformBlockBinding) const
{
	glUniformBlockBinding(programId, uniformBlockIndex, uniformBlockBinding);
}

Args::Sampler* Args::Shader::GetSampler(const std::string& name)
{
	return samplers[name].get();
}

Args::Attribute* Args::Shader::GetAttribute(const std::string& name)
{
	return attributes[name].get();
}

std::vector<std::string> Args::Shader::GetSamplerNames()
{
	std::vector<std::string> names;
	for (auto& sampler : samplers)
		names.push_back(sampler.first);
	return names;
}

std::vector<std::pair<std::string, GLenum>> Args::Shader::GetUniformInfo()
{
	std::vector<std::pair<std::string, GLenum>> info;
	for (auto& uniform : uniforms)
		info.push_back(std::make_pair(uniform.first, uniform.second->GetType()));
	return info;
}
