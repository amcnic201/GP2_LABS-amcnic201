#include "Texture.h"

//This includes the texture header file and defines the function, at the moment
//the function initialises a Texture ID to zero and returns it back.

GLuint loadTextureFromFile(const std::string& filename)
{
	GLuint textureID = 0;
	
	SDL_Surface *imageSurface = IMG_Load(filename.c_str());

	if (!imageSurface){
		std::cout << "Can' Load Image" << filename << "" << IMG_GetError();
		return textureID;
	}

	GLint nOfColors = imageSurface->format->BytesPerPixel;
	GLenum textureFormat = GL_RGB;
	if (nOfColors == 4)  //cntains an alpha channel
	{
		if (imageSurface->format->Rmask == 0x000000ff)
			textureFormat == GL_RGBA;
		else
			textureFormat = GL_BGRA;
	}
	else if (nOfColors == 3)  //no alpha channel
	{
		if (imageSurface->format->Rmask == 0x000000ff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
	}
	else {
		std::cout << "warning: the imgae is not truecolor.. this will probably break";
		//this error sould not go unhandled
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, imageSurface->w, imageSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, imageSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D); 



	return textureID;

}

