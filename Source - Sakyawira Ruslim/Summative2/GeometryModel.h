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

#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"

class GeometryModel : public Mesh
{
	public:
		GeometryModel(GLuint program, Camera* camera);
		virtual ~GeometryModel() = default;

		void render(glm::vec3 position, Texture* _texture);

	private:
		GLuint program;
		Camera* camera;
		//GLuint VAO, VBO, EBO;

};