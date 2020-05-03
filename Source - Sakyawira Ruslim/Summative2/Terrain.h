#pragma once

#include <glew.h>
#include <freeglut.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <string>
#include <vector>
#include "Camera.h"
#include "Mesh.h"
#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2018 Media Design School
*
* File Name		: "Terrain.h"
* Description	: Terrain Declaration file
* Author		: Sakyawira
* Mail			: Sakyawira.Rus8080@mediadesign.school.nz
******************************************************/

namespace
{
	struct TerrainVertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
	};
}

class Terrain : public Mesh
{
public:
	struct InitInfo
	{
		std::string HeightmapFilename;

		float HeightScale;
		float HeightOffset;
		UINT NumRows;
		UINT NumCols;
		float CellSpacing;
	};


	Terrain(InitInfo _info, std::vector<Mesh*>& meshVector);
	~Terrain() {};

	float get_height(glm::vec3 _position)const;

	float width()const;
	float depth()const;

	void load_heightmap();
	void smooth();
	bool in_bounds(GLuint i, GLuint j);
	float average(GLuint i, GLuint j);
	void build_vb();
	void build_ib();

private:
	InitInfo m_info;

	GLuint m_uiNumVertices;
	GLuint m_uiNumFaces;

	std::vector<float> m_vHeightmap;

	GLsizei indicesSize;
	std::vector<TerrainVertex> vertices;
	std::vector<GLuint> indices;
};

