#include <fstream>
#include <sstream>
#include "Terrain.h"

Terrain::Terrain(const InitInfo& initInfo, std::vector<Mesh*>& meshVector)
{
	//md3dDevice = device;

	//mTech = fx::TerrainFX->GetTechniqueByName("TerrainTech");
	//mfxWVPVar = fx::TerrainFX->GetVariableByName("gWVP")->AsMatrix();
	//mfxWorldVar = fx::TerrainFX->GetVariableByName("gWorld")->AsMatrix();
	//mfxDirToSunVar = fx::TerrainFX->GetVariableByName("gDirToSunW")->AsVector();
	//mfxLayer0Var = fx::TerrainFX->GetVariableByName("gLayer0")->AsShaderResource();
	//mfxLayer1Var = fx::TerrainFX->GetVariableByName("gLayer1")->AsShaderResource();
	//mfxLayer2Var = fx::TerrainFX->GetVariableByName("gLayer2")->AsShaderResource();
	//mfxLayer3Var = fx::TerrainFX->GetVariableByName("gLayer3")->AsShaderResource();
	//mfxLayer4Var = fx::TerrainFX->GetVariableByName("gLayer4")->AsShaderResource();
	//mfxBlendMapVar = fx::TerrainFX->GetVariableByName("gBlendMap")->AsShaderResource();

	mInfo = initInfo;

	mNumVertices = mInfo.NumRows * mInfo.NumCols;
	mNumFaces = (mInfo.NumRows - 1) * (mInfo.NumCols - 1) * 2;

	loadHeightmap();
	smooth();

	std::vector<GLfloat> vertices = buildVB();
	std::vector<GLuint> indices =  buildIB();
	m_indicesSize = indices.size();
	m_vertices = vertices;

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// 3 is the size of each vertex
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// 3 is the size of each vertex
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

	/// Position
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),				// Stride of the single vertex (pos)
		(GLvoid*)0);						// Offset from beginning of Vertex

	glEnableVertexAttribArray(0);

	/// Color
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),				// Stride of the single vertex (pos)
		(GLvoid*)(3 * sizeof(GLfloat)));	// Offset from beginning of Vertex

	glEnableVertexAttribArray(1);


	/// Texture
	glVertexAttribPointer(
		2,
		2,									// 2 float components for coordinates 
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),				// Stride the single vertex (pos + color + tex)
		(GLvoid*)(6 * sizeof(GLfloat)));	// offset from beginning of Vertex
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	meshVector.push_back(this);
	//mLayer0 = GetTextureMgr().createTex(initInfo.LayerMapFilename0);
	//mLayer1 = GetTextureMgr().createTex(initInfo.LayerMapFilename1);
	//mLayer2 = GetTextureMgr().createTex(initInfo.LayerMapFilename2);
	//mLayer3 = GetTextureMgr().createTex(initInfo.LayerMapFilename3);
	//mLayer4 = GetTextureMgr().createTex(initInfo.LayerMapFilename4);
	//mBlendMap = GetTextureMgr().createTex(initInfo.BlendMapFilename);
}

float Terrain::width() const
{
	return (mInfo.NumCols - 1) * mInfo.CellSpacing;
}

float Terrain::depth() const
{
	return (mInfo.NumRows - 1) * mInfo.CellSpacing;
}

float Terrain::getHeight(float x, float z) const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f * width()) / mInfo.CellSpacing;
	float d = (z - 0.5f * depth()) / -mInfo.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = mHeightmap[row * mInfo.NumCols + col];
	float B = mHeightmap[row * mInfo.NumCols + col + 1];
	float C = mHeightmap[(row + 1) * mInfo.NumCols + col];
	float D = mHeightmap[(row + 1) * mInfo.NumCols + col + 1];

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

void Terrain::loadHeightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in(mInfo.NumRows * mInfo.NumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(mInfo.HeightmapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	mHeightmap.resize(mInfo.NumRows * mInfo.NumCols, 0);
	for (UINT i = 0; i < mInfo.NumRows * mInfo.NumCols; ++i)
	{
		mHeightmap[i] = (float)in[i] * mInfo.HeightScale + mInfo.HeightOffset;
	}
}

void Terrain::smooth()
{
	std::vector<float> dest(mHeightmap.size());

	for (UINT i = 0; i < mInfo.NumRows; ++i)
	{
		for (UINT j = 0; j < mInfo.NumCols; ++j)
		{
			dest[i * mInfo.NumCols + j] = average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one. 
	mHeightmap = dest;
}

bool Terrain::inBounds(UINT i, UINT j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < mInfo.NumRows&&
		j >= 0 && j < mInfo.NumCols;
}

float Terrain::average(UINT i, UINT j)
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
			if (inBounds(m, n))
			{
				avg += mHeightmap[m * mInfo.NumCols + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

std::vector<GLfloat> Terrain::buildVB()
{
	std::vector<GLfloat> vertices(mNumVertices);

	float halfWidth = (mInfo.NumCols - 1) * mInfo.CellSpacing * 0.5f;
	float halfDepth = (mInfo.NumRows - 1) * mInfo.CellSpacing * 0.5f;

	float du = 1.0f / (mInfo.NumCols - 1);
	float dv = 1.0f / (mInfo.NumRows - 1);
	for (UINT i = 0; i < mInfo.NumRows; ++i)
	{
		float z = halfDepth - i * mInfo.CellSpacing;
		for (UINT j = 0; j < mInfo.NumCols; ++j)
		{
			float x = -halfWidth + j * mInfo.CellSpacing;

			float y = mHeightmap[i * mInfo.NumCols + j];
			
			// Position
			vertices[i * mInfo.NumCols + j + 0] = x;
			vertices[i * mInfo.NumCols + j + 1] = y;
			vertices[i * mInfo.NumCols + j + 2] = z;

			// Normal
			vertices[i * mInfo.NumCols + j + 3] = 0.0f;
			vertices[i * mInfo.NumCols + j + 4] = 1.0f;
			vertices[i * mInfo.NumCols + j + 5] = 0.0f;

			// Stretch texture over grid.
			vertices[i * mInfo.NumCols + j + 6] = j * du;
			vertices[i * mInfo.NumCols + j + 7] = i * dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f * mInfo.CellSpacing);
	float invTwoDZ = 1.0f / (2.0f * mInfo.CellSpacing);
	for (UINT i = 2; i < mInfo.NumRows - 1; ++i)
	{
		for (UINT j = 2; j < mInfo.NumCols - 1; ++j)
		{
			float t = mHeightmap[(i - 1) * mInfo.NumCols + j];
			float b = mHeightmap[(i + 1) * mInfo.NumCols + j];
			float l = mHeightmap[i * mInfo.NumCols + j - 1];
			float r = mHeightmap[i * mInfo.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b) * invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l) * invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			N = glm::normalize(N);

			// Normal
			vertices[i * mInfo.NumCols + j + 3] = N.x;
			vertices[i * mInfo.NumCols + j + 4] = N.y;
			vertices[i * mInfo.NumCols + j + 5] = N.z;
			//vertices[i * mInfo.NumCols + j].normal = N;
		}
	}

	return vertices;

	//D3D10_BUFFER_DESC vbd;
	//vbd.Usage = D3D10_USAGE_IMMUTABLE;
	//vbd.ByteWidth = sizeof(TerrainVertex) * mNumVertices;
	//vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	//vbd.CPUAccessFlags = 0;
	//vbd.MiscFlags = 0;
	//D3D10_SUBRESOURCE_DATA vinitData;
	//vinitData.pSysMem = &vertices[0];
	//HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}

std::vector<GLuint> Terrain::buildIB()
{
	std::vector<GLuint> indices(mNumFaces * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	int k = 0;
	for (UINT i = 0; i < mInfo.NumRows - 1; ++i)
	{
		for (UINT j = 0; j < mInfo.NumCols - 1; ++j)
		{
			indices[k] = i * mInfo.NumCols + j;
			indices[k + 1] = i * mInfo.NumCols + j + 1;
			indices[k + 2] = (i + 1) * mInfo.NumCols + j;

			indices[k + 3] = (i + 1) * mInfo.NumCols + j;
			indices[k + 4] = i * mInfo.NumCols + j + 1;
			indices[k + 5] = (i + 1) * mInfo.NumCols + j + 1;

			k += 6; // next quad
		}
	}

	return indices;
	//D3D10_BUFFER_DESC ibd;
	//ibd.Usage = D3D10_USAGE_IMMUTABLE;
	//ibd.ByteWidth = sizeof(DWORD) * mNumFaces * 3;
	//ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	//ibd.CPUAccessFlags = 0;
	//ibd.MiscFlags = 0;
	//D3D10_SUBRESOURCE_DATA iinitData;
	//iinitData.pSysMem = &indices[0];
	//HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}
