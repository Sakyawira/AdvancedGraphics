#pragma once
#include <math.h>
#include <array>

// A typedef for 3D array
// A neater interface for arrays of arrays of arrays
template <typename T, size_t I, size_t J, size_t K>
using Array3D = std::array<std::array<std::array<T, K>, J>, I>;
#define PERLIN_HEIGHT 250
#define PERLIN_WIDTH 250
//typedef std::array<std::array<std::array<float, 250>, 250>, 3> Array3D;

class Perlin
{
	public:
		Perlin() = default;
		~Perlin() = default;

		static void perlin_noise(Array3D<float, PERLIN_HEIGHT, PERLIN_WIDTH, 3>& _r3DImage, int _iOctaves, float _fZoom, float _fPersistance, int _iXoffset, int _iYoffset);
		static float total_noise_perpoint(int x, int y, int _iOctaves, float _fZoom, float _fPersistance);

	private:
		static float random(int x, int y);
		static float smooth(float x, float y);
		static float interpolate(float a, float b, float x);
		static float noise(float x, float y);
		
};