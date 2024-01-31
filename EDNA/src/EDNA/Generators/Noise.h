#pragma once
namespace Noise {
	// make sure to change the shader equivalent if this gets modified
	inline float SmoothStep(float z)
	{
		return z * z * (3.0 - 2.0 * z);
	}

	inline uint32_t Squirrel1D( int x, uint32_t seed = 0)
	{
		constexpr unsigned int P1 = 3039394381;
		constexpr unsigned int P2 = 1759714724;
		constexpr unsigned int P3 = 458671337;
		unsigned int mangled = x;

		mangled *= P1;
		mangled += seed;
		mangled ^= (mangled >> 8);
		mangled += P2;
		mangled ^= (mangled << 8);
		mangled *= P3;
		mangled ^= (mangled >> 8);

		return mangled;
	}

	inline uint32_t Squirrel2D(int x, int y, uint32_t seed = 0)
	{
		constexpr unsigned int P4 = 198491317;
		return Squirrel1D(x + P4 * y, seed);
	}

	inline float ValueNoise(float x, float y, uint32_t seed = 0)
	{
		auto x_floor = floor(x);
		auto x_frac = x - x_floor;
		auto y_floor = floor(y);
		auto y_frac = y - y_floor;

		static const int scalar = 16777215; // 2^24 - 1
		static const float inv_scalar = 1.f/16777215.f; // 2^24 - 1

		float n0 = (Squirrel2D(x_floor, y_floor, seed) & scalar) * inv_scalar;
		float n1 = (Squirrel2D(x_floor + 1, y_floor, seed) & scalar) * inv_scalar;
		float n2 = (Squirrel2D(x_floor, y_floor + 1, seed) & scalar) * inv_scalar;
		float n3 = (Squirrel2D(x_floor +1, y_floor +1, seed)& scalar) * inv_scalar;

		auto f0 = SmoothStep(1.f - x_frac)*n0 + SmoothStep(x_frac) * n1;
		auto f1 = SmoothStep(1.f - x_frac)*n2 + SmoothStep(x_frac) * n3;

		float g0 = SmoothStep(1.f - y_frac)*f0 + SmoothStep(y_frac) * f1;

		return g0;
	}

	inline float ArctanType(float x)
	{
		// pi/2
		return 1.57079632679 * x / (1.0 + abs(x));
	}

	inline float CosType(float x)
	{
		// 4/pi^2
		return 1.0 - 0.405284734569 * x * x;
	}
}