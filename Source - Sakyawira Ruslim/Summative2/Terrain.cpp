/****************************************************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand
 
  (c) 2018 Media Design School
 
  File Name		: "Terrain.cpp"
  Description	: definitions of Terrain's constructor and member functions
  Author		: Sakyawira
  Mail			: Sakyawira.Rus8080@mediadesign.school.nz
******************************************************/
#include <fstream>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Terrain.h"
Array3D<float, PERLIN_HEIGHT, PERLIN_WIDTH, 3> Terrain::m_imagePerlin = { {{}} };
/***********************
* Constructor	: Setup the Terrain Mesh by loading the info and then push it to a meshVector
* @parameter	: _info -> Terrain settings that needs to be adjusted based on the Heightmap, meshVector -> vector of meshes used to handle in GameManager
* @return		: Terrain
***********************/
Terrain::Terrain(InitInfo _info, std::vector<Mesh*>& meshVector, GLuint program, Camera* camera, Texture* _texture)
{
	
	this->program = program;
	this->camera = camera;

	m_info.HeightmapFilename	= _info.HeightmapFilename;
	m_info.HeightScale			= _info.HeightScale;
	m_info.HeightOffset			= _info.HeightOffset;
	m_info.NumRows				= _info.NumRows;
	m_info.NumCols				= _info.NumCols;
	m_info.CellSpacing			= _info.CellSpacing;

	m_vertices_number_ = m_info.NumRows * m_info.NumCols;
	m_faces_number_ = (m_info.NumRows - 1) * (m_info.NumCols - 1) * 2;

	load_heightmap();
	smooth();

	srand(time(NULL));
	m_seed = rand() % 100 + 1;
	//Perlin::perlin_noise(m_imagePerlin, 4, 10.0f, 0.5f, 0, 0);

	build_vb();
	build_ib();

	// Position
	glVertexAttribPointer(
		0, 
		3, 									// 3 float components for coordinates 
		GL_FLOAT,
		GL_FALSE,
		sizeof(TerrainAttribute), 			// Stride the single vertex (pos + normal + tex)
		(GLvoid*)0);						// offset from beginning of Vertex
	glEnableVertexAttribArray(0);


	// Normal
	glVertexAttribPointer(
		1, 
		3, 									// 3 float components for coordinates 
		GL_FLOAT,
		GL_FALSE,
		sizeof(TerrainAttribute), 			// Stride the single vertex (pos + normal + tex)
		(GLvoid*)(3 * sizeof(GLfloat)));	// offset from beginning of Vertex
	glEnableVertexAttribArray(1);

	// Texture
	glVertexAttribPointer(
		2,
		2,									// 2 float components for coordinates 
		GL_FLOAT,
		GL_FALSE,
		sizeof(TerrainAttribute),			// Stride the single vertex (pos + normal + tex)
		(GLvoid*)(6 * sizeof(GLfloat)));	// offset from beginning of Vertex
	glEnableVertexAttribArray(2);

	m_texture = _texture;
	// m_texture->Load(&m_imagePerlin[0][0][0], 250, 250);
	//meshVector.push_back(this);
}

Terrain::~Terrain()
{

}

void Terrain::render(glm::vec3 _position)
{
	glUseProgram(this->program);

	//this->camera->use_camera(this->program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->GetID());
	const GLchar* name = "tex";
	glUniform1i(glGetUniformLocation(program, name), 0);

	float camDistance = glm::distance(_position, camera->get_position());
	GLint camDistanceLoc = glGetUniformLocation(program, "camDistance");
	glUniform1f(camDistanceLoc, camDistance);

	GLuint camLoc = glGetUniformLocation(program, "camPos");
	glUniform3fv(camLoc, 1, glm::value_ptr(camera->get_position() + camera->get_look_dir() * 15.0f));

	glm::mat4 model;
	model = glm::translate(model, _position);

	GLuint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 mvp = camera->get_projection() * camera->get_view() * model;
	GLint mvLoc = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(VAO);

	glPatchParameteri(GL_PATCH_VERTICES, /*m_indicesSize*/3);
	//glDrawArrays(GL_PATCHES, 0, m_indicesSize);
	glDrawElements(GL_PATCHES, m_indicesSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/***********************
* get_height	: Gets the height of the Terrain in a certain position
* @parameter	: _position -> position in the game world
* @return		: height of terrain / 99999
***********************/
float Terrain::get_height(glm::vec3 _position) const
{
	//float x = _position.x;
	//float z = _position.z;

	//// Transform from terrain local space to "cell" space.
	//float c = (x + 0.5f * width()) / m_info.CellSpacing;
	//float d = (z - 0.5f * depth()) / -m_info.CellSpacing;

	//// Get the row and column we are in.
	//int row = (int)floorf(d);
	//int col = (int)floorf(c);

	//if (row < 0 || col < 0 || (int)m_info.NumCols - 2 < row || (int)m_info.NumRows - 2 < col)
	//{
		return -99999;
	//}

	//// Grab the heights of the cell we are in.
	//// A*--*B
	////  | /|
	////  |/ |
	//// C*--*D
	//float A = m_v_heightmap[row * m_info.NumCols + col];
	//float B = m_v_heightmap[row * m_info.NumCols + col + 1];
	//float C = m_v_heightmap[(row + 1) * m_info.NumCols + col];
	//float D = m_v_heightmap[(row + 1) * m_info.NumCols + col + 1];

	//// Where we are relative to the cell.
	//float s = c - (float)col;
	//float t = d - (float)row;

	//// If upper triangle ABC.
	//if (s + t <= 1.0f)
	//{
	//	float uy = B - A;
	//	float vy = C - A;
	//	return A + s * uy + t * vy;
	//}
	//else // lower triangle DCB.
	//{
	//	float uy = C - D;
	//	float vy = B - D;
	//	return D + (1.0f - s) * uy + (1.0f - t) * vy;
	//}
}

/***********************
* get_vertices	: Getter for the vertices vector.
* @parameter	: -
* @return		: vertices vector.
***********************/
std::vector<TerrainAttribute>* Terrain::get_vertices()
{
	return &vertices;
}

/***********************
* width			: calculate a cell's width
* @parameter	: -
* @return		: cell's width
***********************/
float Terrain::width() const
{
	return (m_info.NumCols - 1) * m_info.CellSpacing;
}

/***********************
* depth			: calculate a cell's depth
* @parameter	: -
* @return		: cell's depth
***********************/
float Terrain::depth() const
{
	return (m_info.NumRows - 1) * m_info.CellSpacing;
}

/***********************
* load_heightmap	: read a raw file and populate the heightmap vector with its data
* @parameter		: -
* @return			: -
***********************/
void Terrain::load_heightmap()
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
	m_v_heightmap.resize(m_info.NumRows * m_info.NumCols, 0);
	for (UINT i = 0; i < m_info.NumRows * m_info.NumCols; ++i)
	{
		m_v_heightmap[i] = static_cast<float>(in[i]) * m_info.HeightScale + m_info.HeightOffset;
	}
}

/***********************
* smooth		: apply a filter to the floating-point heightmap to smooth it
* @parameter	: -
* @return		: -
***********************/
void Terrain::smooth()
{
	std::vector<float> dest(m_v_heightmap.size());

	for (GLuint i = 0; i < m_info.NumRows; ++i)
	{
		for (GLuint j = 0; j < m_info.NumCols; ++j)
		{
			dest[i * m_info.NumCols + j] = average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_v_heightmap = dest;
}

/***********************
* in_bounds		: check whether i and j exists within the bounds of the heightmap 
* @parameter	: index of heightmap based on info's col and row
* @return		: do i and j exist?
***********************/
bool Terrain::in_bounds(GLuint i, GLuint j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < m_info.NumRows&&
		j >= 0 && j < m_info.NumCols;
}

/***********************
* average		: calculate the average height of elements between [i][j]
* @parameter	: index of heightmap based on info's col and row
* @return		: average height
***********************/
float Terrain::average(GLuint i, GLuint j)
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
			if (in_bounds(m, n))
			{
				avg += m_v_heightmap[m * m_info.NumCols + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

void Terrain::create_texture()
{

}

/***********************
* build_vb		: generates vertex array and buffer (VAO, VBO)
* @parameter	: -
* @return		: -
***********************/
void Terrain::build_vb()
{
	vertices.resize(m_vertices_number_);

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

			float y = (Perlin::total_noise_perpoint(i + 0, j + 0, 4, 10.0f, 0.5f,m_seed))*255;
			m_v_heightmap[i * m_info.NumCols + j] = y; // Comment and swap with above to use heightmap
			vertices[i * m_info.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i * m_info.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			 vertices[i*m_info.NumCols + j].texCoord.x = j*du;
			 vertices[i*m_info.NumCols + j].texCoord.y = i*dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f * m_info.CellSpacing);
	float invTwoDZ = 1.0f / (2.0f * m_info.CellSpacing);
	for (UINT i = 2; i < m_info.NumRows - 1; ++i)
	{
		for (UINT j = 2; j < m_info.NumCols - 1; ++j)
		{
			float t = m_v_heightmap[(i - 1) * m_info.NumCols + j];
			float b = m_v_heightmap[(i + 1) * m_info.NumCols + j];
			float l = m_v_heightmap[i * m_info.NumCols + j - 1];
			float r = m_v_heightmap[i * m_info.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b) * invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l) * invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			N = glm::normalize(N);

			vertices[i * m_info.NumCols + j].normal = N;
		}
	}
	//std::vector<VertexFormat> vecVertices{};
	//float fHalfSize = m_fSize / 2;

	//for (int i = 0; i < m_info.NumRows; i++)
	//{
	//	float fZPercentage = static_cast<float>(i) / (m_info.NumRows - 1);
	//	for (int j = 0; j < m_info.NumCols; j++)
	//	{
	//		float fXPercentage = static_cast<float>(j) / (m_info.NumCols - 1);
	//		//int iPerlinXvalue = static_cast<int>(fXPercentage * static_cast<float>(PERLIN_WIDTH - 1));
	//		//int iPerlinYvalue = static_cast<int>(fZPercentage * static_cast<float>(PERLIN_HEIGHT - 1));

	//		//vecVertices.push_back({ {
	//		//		(fXPercentage  * m_fSize) - fHalfSize,
	//		//		static_cast<float>(tenImage[iPerlinXvalue][iPerlinYvalue][0]) * m_fMaxHeight,
	//		//		(fZPercentage  * m_fSize) - fHalfSize },													// Position
	//		//		{1.0f,1.0f,1.0f},																			// Color
	//		//		{ fXPercentage, fZPercentage } });															// UV

	//		vertices.push_back({ {
	//				(fXPercentage * m_fSize) - fHalfSize,
	//				0.0f,
	//				(fZPercentage * m_fSize) - fHalfSize },													// Position
	//				{ 1.0f,1.0f,1.0f },																			// Color
	//				{ fXPercentage, fZPercentage } });															// UV
	//	}
	//}

	glPatchParameteri(GL_PATCH_VERTICES, /*m_indicesSize*/3); //comment for tri patch
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainAttribute), vertices.data(), GL_STATIC_DRAW);

	// Creating Vertex Array
	//glGenVertexArrays(1, &m_VAO); 
	//glBindVertexArray(m_VAO);

	// Creating Vertex Buffer
	//glGenBuffers(1, &m_VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainAttribute), &vertices.front(), GL_STATIC_DRAW);
}

/***********************
* build_ib		: generates index buffer (EBO)
* @parameter	: -
* @return		: -
***********************/
void Terrain::build_ib()
{
	indices.resize(m_faces_number_ * 3); // 3 indices per face
	
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

	/*for (int i = 0; i < m_info.NumRows - 1; i++)
	{
		for (int j = 0; j < m_info.NumCols - 1; j++)
		{
			int iColValue = j + (i * m_info.NumRows);
			indices.push_back(iColValue);
			indices.push_back(iColValue + 1);
			indices.push_back(j + ((i + 1) * m_info.NumRows));

			indices.push_back(iColValue + 1);
			indices.push_back(j + ((i + 1) * m_info.NumRows) + 1);
			indices.push_back(j + ((i + 1) * m_info.NumRows));
		}
	}*/

	// Create Index Buffer
	glGenBuffers(1, &EBO); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
	m_indicesSize = indices.size();

}

