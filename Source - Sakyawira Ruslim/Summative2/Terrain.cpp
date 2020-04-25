#include <fstream>
#include <sstream>
#include "Terrain.h"

Terrain::Terrain()
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

	//mInfo = initInfo;

	mNumVertices = mInfo.NumRows * mInfo.NumCols;
	mNumFaces = (mInfo.NumRows - 1) * (mInfo.NumCols - 1) * 2;

	loadHeightmap();
	smooth();

	buildVB();
	buildIB();

	//mLayer0 = GetTextureMgr().createTex(initInfo.LayerMapFilename0);
	//mLayer1 = GetTextureMgr().createTex(initInfo.LayerMapFilename1);
	//mLayer2 = GetTextureMgr().createTex(initInfo.LayerMapFilename2);
	//mLayer3 = GetTextureMgr().createTex(initInfo.LayerMapFilename3);
	//mLayer4 = GetTextureMgr().createTex(initInfo.LayerMapFilename4);
	//mBlendMap = GetTextureMgr().createTex(initInfo.BlendMapFilename);
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

void Terrain::buildVB()
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
			vertices[i * mInfo.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i * mInfo.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			vertices[i * mInfo.NumCols + j].texC.x = j * du;
			vertices[i * mInfo.NumCols + j].texC.y = i * dv;
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

			vertices[i * mInfo.NumCols + j].normal = N;
		}
	}

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
