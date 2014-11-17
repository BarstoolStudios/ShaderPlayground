#include "Terrain/Terrain.h"
#include "Util/ShaderUtil.h"
#include "Util/Utility.h"
#include <climits>
#include <future>


Terrain::Terrain() {

	//------------------------------------------------------------------------------
	// Create and Bind VAO
	//------------------------------------------------------------------------------
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//------------------------------------------------------------------------------
	// Create Shader Program
	//------------------------------------------------------------------------------
	std::vector<GLenum> shadersUsed = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	shaderProgram = ShaderUtil::createProgram("Terrain", shadersUsed, true);

	//------------------------------------------------------------------------------
	// Get Shader Variable Locations
	//------------------------------------------------------------------------------
	sPositionHandle = glGetAttribLocation(shaderProgram, "iPosition");
	sNormalHandle = glGetAttribLocation(shaderProgram, "iNormal");
	sColorHandle = glGetUniformLocation(shaderProgram, "uColor");
	sSunHandle = glGetUniformLocation(shaderProgram, "uSunDirection");
	sMVPHandle = glGetUniformLocation(shaderProgram, "uMVP");

	//------------------------------------------------------------------------------
	// Generate VBOs
	//------------------------------------------------------------------------------
	GLuint buffs[2];
	glGenBuffers(2, buffs);
	vertexVBO = buffs[0];
	normalVBO = buffs[1];

	std::vector<float> verts = {
		-SQUARE_SIZE / 2.0f, 0,  SQUARE_SIZE / 2.0f,
		-SQUARE_SIZE / 2.0f, 0, -SQUARE_SIZE / 2.0f,
		 SQUARE_SIZE / 2.0f, 0,  SQUARE_SIZE / 2.0f,
		-SQUARE_SIZE / 2.0f, 0, -SQUARE_SIZE / 2.0f,
		 SQUARE_SIZE / 2.0f, 0, -SQUARE_SIZE / 2.0f,
		 SQUARE_SIZE / 2.0f, 0,  SQUARE_SIZE / 2.0f,
	};

	std::vector<float> norms = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};



	//------------------------------------------------------------------------------
	// Allocate Memory for Position Data
	//------------------------------------------------------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, VERT_COUNT * 3 * sizeof(GLfloat), &verts[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(sPositionHandle);
	glVertexAttribPointer(sPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//------------------------------------------------------------------------------
	// Allocate Memory for Normal Data
	//------------------------------------------------------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, VERT_COUNT * 3 * sizeof(GLfloat), &norms[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(sNormalHandle);
	glVertexAttribPointer(sNormalHandle, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Terrain::draw(Camera& camera, Sun& sun) {
	//------------------------------------------------------------------------------
	// Create MVP Matrix
	//------------------------------------------------------------------------------
	Matrix4f MVP = camera.getProjection() * camera.getView();

	glBindVertexArray(VAO);

	//------------------------------------------------------------------------------
	// Tell OGL to use Shader Program
	//------------------------------------------------------------------------------
	glUseProgram(shaderProgram);

	//------------------------------------------------------------------------------
	// Load Color
	//------------------------------------------------------------------------------
	glUniform3f(sColorHandle, 0.36f, 0.25f, 0.2f);

	//------------------------------------------------------------------------------
	// Load Sun Position
	//------------------------------------------------------------------------------
	glUniform3f(sSunHandle, sun.getPosition().x, sun.getPosition().y, 0);

	//------------------------------------------------------------------------------
	// Load MVP Matrix 
	//------------------------------------------------------------------------------
	GLfloat arr[16];
	MVP.asArray(arr);
	glUniformMatrix4fv(sMVPHandle, 1, GL_TRUE, arr);

	//------------------------------------------------------------------------------
	// Draw
	//------------------------------------------------------------------------------
	glDrawArrays(GL_TRIANGLES, 0, VERT_COUNT);

	//------------------------------------------------------------------------------
	// Unbind / Disable Code
	//------------------------------------------------------------------------------
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float Terrain::getHeightAt(float x, float y) {
	if(x > SQUARE_SIZE / 2.0f || x < -SQUARE_SIZE / 2.0f || y > SQUARE_SIZE / 2.0f || y < -SQUARE_SIZE / 2.0f)
		return INT_MIN;
	return 0;
}