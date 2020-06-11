#include "PerlinNoise.h"

//  A noise function is essentially a seeded number generator.
float Perlin::random(int x, int y, int _seed)
{
	int n = x + y * 57 + _seed;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - double(t) * 0.931322574615478515625e-9;
}

// Smoothens the value by averaging the corners, sidesand center
float Perlin::smooth(float x, float y, int _seed)
{
	float corners = (random(x - 1.0f, y - 1.0f, _seed) + random(x + 1.0f, y - 1.0f, _seed) + random(x - 1.0f, y + 1.0f, _seed) + random(x + 1.0f, y + 1.0f, _seed)) / 16.0f;
	float sides = (random(x - 1.0f, y, _seed) + random(x + 1.0f, y, _seed) + random(x, y - 1.0f, _seed) + random(x, y + 1.0f, _seed)) / 8.0f;
	float center = random(x, y, _seed) / 4.0f;
	return corners + sides + center;
}

// Interpolation – Can be linear, cubic, cosine
float Perlin::interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * 0.5f;
	return a * (1 - f) + b * f;
}

// Averaged and Interpolated Noise
float Perlin::noise(float x, float y, int _seed)
{
	float	fractional_X = x - int(x);
	float 	fractional_Y = y - int(y);
	//float 	fractional_Z = z - int(z);

	//smooths
	float v1 = smooth(int(x), int(y), _seed);
	float v2 = smooth(int(x) + 1, int(y), _seed);
	float v3 = smooth(int(x), int(y) + 1, _seed);
	float v4 = smooth(int(x) + 1, int(y) + 1, _seed);

	// interpolates
	float i1 = interpolate(v1, v2, fractional_X);
	float i2 = interpolate(v3, v4, fractional_X);

	float final = interpolate(i1, i2, fractional_Y);

	return final;
}

float Perlin::total_noise_perpoint(int x, int y, int _iOctaves, float _fZoom, float _fPersistance, int _seed)
{
	float total = 0.0f;

	for (int i = 0; i < _iOctaves - 1; i++) {

		float frequency = pow(2, i) / _fZoom;
		float amplitude = pow(_fPersistance, i);

		total += noise(x * frequency, y * frequency,_seed) * amplitude;
	}

	return total;
}
