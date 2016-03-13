#include "shader_utils.h"
#include <cstdlib>
#include <iostream>
#include <SDL.h>

using namespace std;

char* readFile(const char* filename) {
  SDL_RWops* rw = SDL_RWFromFile(filename, "rb");
  if (rw == NULL) return NULL;

  Sint64 resSize = SDL_RWsize(rw);
  char* res = (char*) malloc(resSize + 1);

  Sint64 nbReadTotal = 0;
  Sint64 nbRead = 1;

  char* buf = res;

  while (nbReadTotal < resSize && nbRead != 0) {
    nbRead = SDL_RWread(rw, buf, 1, (resSize - nbReadTotal));
    nbReadTotal += nbRead;
    buf += nbRead;
  }

  SDL_RWclose(rw);

  if (nbReadTotal != resSize) {
    free(res);
    return NULL;
  }

  res[nbReadTotal] = '\0';
  return res;
}

void printLog(GLuint object) {
  GLint logLength = 0;

  if (glIsShader(object)) {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
  } else if (glIsProgram(object)) {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);
  } else {
    cerr << "printlog: Not a shader or a program" << endl;
    return;
  }

  char* log = (char*) malloc(logLength);

  if (glIsShader(object)) {
    glGetShaderInfoLog(object, logLength, NULL, log);
  } else if (glIsProgram(object)) {
    glGetProgramInfoLog(object, logLength, NULL, log);
  }

  cerr << log;
  free(log);
}

GLuint createShader(const char* filename, GLenum type) {
  const GLchar* source = readFile(filename);

  if (source == NULL) {
    cerr << "Error opening" << filename << ": " << SDL_GetError() << endl;
    return 0;
  }

  GLuint res = glCreateShader(type);

  const GLchar* sources[] = {
#ifdef GL_ES_VERSION_2_0
    "#version 100\n",
#else
    "#version 120\n",
#endif
    source
  };

  glShaderSource(res, 2, sources, NULL);
  free((void*) source);

  glCompileShader(res);
  GLint compileOk = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compileOk);

  if (compileOk == GL_FALSE) {
    cerr << filename << ":";
    printLog(res);
    glDeleteShader(res);
    return 0;
  }

  return res;
}
