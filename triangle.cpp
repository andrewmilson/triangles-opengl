#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include "lib/shader_utils.h"

using namespace std;

GLuint program;
GLuint vboTriangle, vboTriangleColors;
GLint attributeCoord2d, attributeVColor;

bool initResources() {
  GLfloat triangleColors[] = {
    1.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    1.0, 0.0, 0.0,
  };

  glGenBuffers(1, &vboTriangleColors);
  glBindBuffer(GL_ARRAY_BUFFER, vboTriangleColors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColors),
    triangleColors, GL_STATIC_DRAW);

  GLfloat triangleVertices[] = {
    0.0,  3,
    -3, -3,
    3, -3,
  };

  GLint linkOk = GL_FALSE;

  GLuint vs = createShader("glsl/triangle.v.glsl", GL_VERTEX_SHADER);
  if (!vs) return false;

  GLuint fs = createShader("glsl/triangle.f.glsl", GL_FRAGMENT_SHADER);
  if (!fs) return false;

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &linkOk);

  if (!linkOk) {
    cerr << "glLinkProgram:";
    printLog(program);
    return false;
  }

  const char* attributeName = "coord2d";
  attributeCoord2d = glGetAttribLocation(program, attributeName);

  if (attributeCoord2d == -1) {
    cerr << "Could not bind attribute " << attributeName << endl;
    return false;
  }

  glGenBuffers(1, &vboTriangle);
  glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices),
    triangleVertices, GL_STATIC_DRAW);

  const char* attributeColorName = "v_color";
  attributeVColor = glGetAttribLocation(program, attributeColorName);

  if (attributeVColor == -1) {
    cerr << "Could not bind attribute " << attributeColorName << endl;
    return false;
  }

  return true;
}

void render(SDL_Window* window) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);

  glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
  glEnableVertexAttribArray(attributeCoord2d);

  glVertexAttribPointer(
    attributeCoord2d,
    2,
    GL_FLOAT,
    GL_FALSE,
    0,
    0);

  glEnableVertexAttribArray(vboTriangleColors);
  glBindBuffer(GL_ARRAY_BUFFER, attributeVColor);

  glVertexAttribPointer(
    attributeVColor,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    0);

  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(attributeCoord2d);
  glDisableVertexAttribArray(attributeVColor);
  SDL_GL_SwapWindow(window);
}

void freeResources() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &vboTriangle);
}

void mainLoop(SDL_Window* window) {
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        return;
      }
    }

    render(window);
  }
}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow(
    "My Second Triangle",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    800, 800,
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if (window == NULL) {
		cerr << "Error: can't create window: " << SDL_GetError() << endl;
		return EXIT_FAILURE;
	}

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);

  if (SDL_GL_CreateContext(window) == NULL) {
    cerr << "Error: SDL_GL_CreateContext: " << SDL_GetError() << endl;
    return EXIT_FAILURE;
  }

  SDL_GL_CreateContext(window);
  GLenum glew_status = glewInit();

  if (!GLEW_VERSION_2_0) {
		cerr << "Error: your graphic card does not support OpenGL 2.0" << endl;
		return EXIT_FAILURE;
	}

  if (glew_status != GLEW_OK) {
    cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
    return EXIT_FAILURE;
  }

  if (!initResources()) {
    return EXIT_FAILURE;
  }

  mainLoop(window);

  freeResources();
  return EXIT_SUCCESS;
}
