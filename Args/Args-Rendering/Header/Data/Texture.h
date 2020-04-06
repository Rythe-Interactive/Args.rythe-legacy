#pragma once
#include <string>
#include <glad/glad.h>
#include <Args-Core.h>

namespace Args
{
	class Texture
	{
	private:
		/// Create a texture by loading from a file
		/// Uses stb_image (as SOIL does). Check for supported formats
		/// in the stb_image header
		void Load(const std::string& filename);

		///// Creates an empty texture with given size. You will need to provide
		///// the data.
		//Texture(int width, int height);

		static std::unordered_map<std::string, Texture> textures;
		static std::set<std::string> containedTextures;
	public:

		static Texture* CreateTexture(const std::string& name, const std::string& filename);
		static Texture* GetTexture(const std::string& name);

		/// Used for the label
		Texture() {}

		/// Releases OpenGL resources
		~Texture();

		/// Get the texture ID in OpenGL
		GLuint GetTexture() const { return texture; }

		/// Creates a texture from RGBA provided data
		void CreateGLTextureWithData(GLubyte* data, bool genMipMaps);

	protected:
		GLuint texture = 0;
		int width = 0;
		int height = 0;
		int channels = 0;
	};
}