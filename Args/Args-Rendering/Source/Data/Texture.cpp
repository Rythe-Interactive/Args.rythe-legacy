#include "Data/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::unordered_map<std::string, Args::Texture> Args::Texture::textures;
std::set<std::string> Args::Texture::containedTextures;

void Args::Texture::Load(const std::string& filename)
{
	stbi_set_flip_vertically_on_load(true);
	GLubyte* data = stbi_load(filename.c_str(), &width, &height, &channels, 4);

	if (data == nullptr)
	{
		Debug::Log(DebugInfo, "Image %s could not be loaded!", filename.c_str());
	}
	else
	{
		CreateGLTextureWithData(data, true);
		stbi_image_free(data);
		Debug::Success(DebugInfo, "Loaded Texture %s", filename.c_str());
	}
}

//Args::Texture::Texture(int width, int height) : texture(0), width(width), height(height) {}

Args::Texture* Args::Texture::CreateTexture(const std::string& name, const std::string& filename)
{
	if (containedTextures.count(name))
		return &(textures[name]);

	textures[name].Load(TextureDir + filename);
	containedTextures.insert(name);
	return &(textures[name]);
}

Args::Texture* Args::Texture::GetTexture(const std::string& name)
{
	if(containedTextures.count(name))
		return &(textures[name]);

	return nullptr;
}

Args::Texture::~Texture()
{
	if (texture)
		glDeleteTextures(1, &texture);
}

void Args::Texture::CreateGLTextureWithData(GLubyte* data, bool genMipMaps)
{
	if (texture)
		glDeleteTextures(1, &texture);

	glGenTextures(1, &texture);											// Gen    

	glBindTexture(GL_TEXTURE_2D, texture);                                 // Bind

	if (genMipMaps)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);    // Minmization
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);                   // Minmization


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                       // Magnification


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(
		GL_TEXTURE_2D,						// What (target)
		0,									// Mip-map level
		GL_RGBA,		                    // Internal format
		width,								// Width
		height,							// Height
		0,									// Border
		GL_RGBA,							// Format (how to use)
		GL_UNSIGNED_BYTE,					// Type   (how to intepret)
		data);								// Data


	if (genMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);

}