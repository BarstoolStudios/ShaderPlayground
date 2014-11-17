#ifndef TERRAIN_H
#define TERRAIN_H

class Terrain;

#include <map>
#include <functional>
#include <future>
#include "Main/Player.h"
#include "Main/Camera.h"
#include "Terrain/Sun.h"
#include "Util/GLMath.h"
#include "Main/Settings.h"

class Terrain {
	const int SQUARE_SIZE = 64;
	const int VERT_COUNT = 6;

	GLuint VAO;

	GLint sPositionHandle, sNormalHandle, sColorHandle, sSunHandle, sMVPHandle;

	GLuint shaderProgram;

	GLuint vertexVBO, normalVBO;

public:

	Terrain();

	void draw(Camera& camera, Sun& sun);

	float getHeightAt(float x, float y);
};

#endif // TERRAIN_H