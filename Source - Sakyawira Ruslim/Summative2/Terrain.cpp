#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Terrain.cpp"
* Description	: Terrain implementation file.
* Author		: Sakyawira
* Mail			: Sakyawira.Rus8080@mediadesign.school.nz
******************************************************/
#include <fstream>
#include <sstream>
#include "Terrain.h"
#include "Camera.h"
#include "ShaderLoader.h"

/***********************
* Terrain Constructor: Set up scene items
* @author: Sakyawira
***********************/
Terrain::Terrain(InitInfo _info, std::vector<Mesh*>& meshVector)
{
	m_info.HeightmapFilename	= _info.HeightmapFilename;
	m_info.HeightScale			= _info.HeightScale;
	m_info.HeightOffset			= _info.HeightOffset;
	m_info.NumRows				= _info.NumRows;
	m_info.NumCols				= _info.NumCols;
	m_info.CellSpacing			= _info.CellSpacing;

	m_uiNumVertices = m_info.NumRows * m_info.NumCols;
	m_uiNumFaces = (m_info.NumRows - 1) * (m_info.NumCols - 1) * 2;

	LoadHeightmap();
	Smooth();
	BuildVB();
	BuildIB();

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)(3 * 12));
	glEnableVertexAttribArray(1);

	meshVector.push_back(this);
}

/***********************
* GetHeight: Gets the height value from the terrain based on the position given.
* @author: Sakyawira
* @return: height position on terrain OR a very big negative value (-FLT_MAX) if not on the terrain.
***********************/
float Terrain::GetHeight(glm::vec3 _position) const
{
	float x = _position.x;
	float z = _position.z;

	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f * Width()) / m_info.CellSpacing;
	float d = (z - 0.5f * Depth()) / -m_info.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	if (row < 0 || col < 0 || (int)m_info.NumCols <= row || (int)m_info.NumRows <= col)
	{
		return -99999;
	}

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_vHeightmap[row * m_info.NumCols + col];
	float B = m_vHeightmap[row * m_info.NumCols + col + 1];
	float C = m_vHeightmap[(row + 1) * m_info.NumCols + col];
	float D = m_vHeightmap[(row + 1) * m_info.NumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s) * uy + (1.0f - t) * vy;
	}
}

/***********************
* Width: Gets the width of a cell
* @return: the width of a cell
***********************/
float Terrain::Width() const
{
	return (m_info.NumCols - 1) * m_info.CellSpacing;
}

/***********************
* Depth: Gets the depth of a cell
* @return: the depth of a cell
***********************/
float Terrain::Depth() const
{
	return (m_info.NumRows - 1) * m_info.CellSpacing;
}

/***********************
* LoadHeightmap: Loads heightmap from raw file
* @author: Sakyawira
***********************/
void Terrain::LoadHeightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in(m_info.NumRows * m_info.NumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(m_info.HeightmapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	m_vHeightmap.resize(m_info.NumRows * m_info.NumCols, 0);
	for (UINT i = 0; i < m_info.NumRows * m_info.NumCols; ++i)
	{
		m_vHeightmap[i] = static_cast<float>(in[i]) * m_info.HeightScale + m_info.HeightOffset;
	}
}

/***********************
* Smooth: Smooths the heightmap
***********************/
void Terrain::Smooth()
{
	std::vector<float> dest(m_vHeightmap.size());

	for (GLuint i = 0; i < m_info.NumRows; ++i)
	{
		for (GLuint j = 0; j < m_info.NumCols; ++j)
		{
			dest[i * m_info.NumCols + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_vHeightmap = dest;
}

/***********************
* InBounds: Check if ij are valid indices
* return: whether indices are valid
***********************/
bool Terrain::InBounds(GLuint i, GLuint j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < m_info.NumRows&&
		j >= 0 && j < m_info.NumCols;
}

/***********************
* Average: Computes the average height of ij
* return: the average height computed
***********************/
float Terrain::Average(GLuint i, GLuint j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	for (UINT m = i - 1; m <= i + 1; ++m)
	{
		for (UINT n = j - 1; n <= j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				avg += m_vHeightmap[m * m_info.NumCols + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

/***********************
* BuildVB: Creates vertex buffer for the terrain
* @author: Sakyawira
***********************/
void Terrain::BuildVB()
{
	vertices.resize(m_uiNumVertices);

	float halfWidth = (m_info.NumCols - 1) * m_info.CellSpacing * 0.5f;
	float halfDepth = (m_info.NumRows - 1) * m_info.CellSpacing * 0.5f;

	float du = 1.0f / (m_info.NumCols - 1);
	float dv = 1.0f / (m_info.NumRows - 1);
	for (UINT i = 0; i < m_info.NumRows; ++i)
	{
		float z = halfDepth - i * m_info.CellSpacing;
		for (UINT j = 0; j < m_info.NumCols; ++j)
		{
			float x = -halfWidth + j * m_info.CellSpacing;

			float y = m_vHeightmap[i * m_info.NumCols + j];
			vertices[i * m_info.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i * m_info.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

			//// Stretch texture over grid.
			//vertices[i*m_info.NumCols + j].texC.x = j*du;
			//vertices[i*m_info.NumCols + j].texC.y = i*dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f * m_info.CellSpacing);
	float invTwoDZ = 1.0f / (2.0f * m_info.CellSpacing);
	for (UINT i = 2; i < m_info.NumRows - 1; ++i)
	{
		for (UINT j = 2; j < m_info.NumCols - 1; ++j)
		{
			float t = m_vHeightmap[(i - 1) * m_info.NumCols + j];
			float b = m_vHeightmap[(i + 1) * m_info.NumCols + j];
			float l = m_vHeightmap[i * m_info.NumCols + j - 1];
			float r = m_vHeightmap[i * m_info.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b) * invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l) * invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			N = glm::normalize(N);

			vertices[i * m_info.NumCols + j].normal = N;
		}
	}

	//Generating vao and vbo
	glGenVertexArrays(1, &m_VAO); //Vert Array
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);	//Vert Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

}

/***********************
* BuildIB: Creates index buffer for the terrain
* @author: Sakyawira
***********************/
void Terrain::BuildIB()
{
	indices.resize(m_uiNumFaces * 3); // 3 indices per face
											   // Iterate over each quad and compute indices.
	int k = 0;
	for (UINT i = 0; i < m_info.NumRows - 1; ++i)
	{
		for (UINT j = 0; j < m_info.NumCols - 1; ++j)
		{
			indices[k] = i * m_info.NumCols + j;
			indices[k + 1] = i * m_info.NumCols + j + 1;
			indices[k + 2] = (i + 1) * m_info.NumCols + j;

			indices[k + 3] = (i + 1) * m_info.NumCols + j;
			indices[k + 4] = i * m_info.NumCols + j + 1;
			indices[k + 5] = (i + 1) * m_info.NumCols + j + 1;

			k += 6; // next quad
		}
	}

	glGenBuffers(1, &m_EBO); //Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	indicesSize = indices.size() * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &indices[0], GL_STATIC_DRAW);    //EBO Buffer
	m_indicesSize = indices.size();
}
