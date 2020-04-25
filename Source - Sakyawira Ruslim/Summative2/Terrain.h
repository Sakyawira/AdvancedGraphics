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

	float GetHeight(glm::vec3 _position)const;

	float Width()const;
	float Depth()const;

	void LoadHeightmap();
	void Smooth();
	bool InBounds(GLuint i, GLuint j);
	float Average(GLuint i, GLuint j);
	void BuildVB();
	void BuildIB();

private:
	InitInfo m_info;

	GLuint m_uiNumVertices;
	GLuint m_uiNumFaces;

	std::vector<float> m_vHeightmap;

	GLsizei indicesSize;
	std::vector<TerrainVertex> vertices;
	std::vector<GLuint> indices;
};

