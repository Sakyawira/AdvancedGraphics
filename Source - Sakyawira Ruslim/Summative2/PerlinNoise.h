#pragma once
#include <math.h>
#include <array>
#define PERLIN_HEIGHT 250
#define PERLIN_WIDTH 250

class Perlin
{
	public:
		Perlin() = default;
		~Perlin() = default;
		static float total_noise_perpoint(int x, int y, int _iOctaves, float _fZoom, float _fPersistance, int _seed);

	private:
		static float random(int x, int y, int _seed);
		static float smooth(float x, float y, int _seed);
		static float interpolate(float a, float b, float x);
		static float noise(float x, float y, int _seed);
};