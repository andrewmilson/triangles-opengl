#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H
#include <GL/glew.h>

extern char* readFile(const char* filename);
extern void printLog(GLuint object);
extern GLuint createShader(const char* filename, GLenum type);

#endif
