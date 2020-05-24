#pragma once
#include <math.h>
#include <array>

// A typedef for 3D array
typedef std::array<std::array<std::array<float, 250>, 250>, 3> Array3D;

class Perlin
{
	public:
		Perlin() = default;
		~Perlin() = default;

		static void perlin_noise(Array3D & _r3DImage, int _iOctaves, float _fZoom, float _fPersistance, int _iXoffset, int _iYoffset);

	private:
		static float random(int x, int y);
		static float smooth(float x, float y);
		static float interpolate(float a, float b, float x);
		static float noise(float x, float y);
		static float total_noise_perpoint(int x, int y, int _iOctaves, float _fZoom, float _fPersistance);
};