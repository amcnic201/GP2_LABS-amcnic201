#ifndef  TEXTURE_H
#define  TEXTURE_H

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

//This will take in a string that represents the path to the filename to  load
//and return a GLuint which represents the ID of the load OpenGL Texture
GLuint loadTextureFromFile(const std::string& filename);



#endif