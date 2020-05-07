#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes

#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Camera.h"
class TessModel
{
	public:
		TessModel(GLuint program, Camera* camera);
		~TessModel() {};

		void render(glm::vec3 position);

	private:
		GLuint program;
		Camera* camera;
		GLuint VAO, VBO, EBO;
};