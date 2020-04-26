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

class GeometryModel
{
	public:
		GeometryModel(GLuint program, Camera* camera);
		~GeometryModel();

		void render();

	private:
		GLuint program;
		Camera* camera;
		GLuint VAO, VBO, EBO;

};