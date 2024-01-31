float SmoothStep(float z)
	{
		return z * z * (3.0 - 2.0 * z);
	}
	 
uint Squirrel1D( int x, uint seed)
	{
		uint P1 = 3039394381u;
		uint P2 = 1759714724u;
		uint P3 = 458671337u;
		uint mangled = x;

		mangled *= P1;
		mangled += seed;
		mangled ^= (mangled >> 8);
		mangled += P2;
		mangled ^= (mangled << 8);
		mangled *= P3;
		mangled ^= (mangled >> 8);

		return mangled;
	}

uint Squirrel2D(int x, int y, uint seed)
	{
		uint P4 = 198491317u;
		return Squirrel1D(int(x + P4 * y), seed);
	}

float ValueNoise(float x, float y, uint seed)
	{
		int x_floor = int(floor(x));
		float x_frac = x - float(x_floor);
		int y_floor = int(floor(y));
		float y_frac = y - float(y_floor);

		uint scalar = 16777215u; // 2^24 - 1
		float inv_scalar = 1.0/16777215.0; // 2^24 - 1

		float n0 = float(Squirrel2D(x_floor,     y_floor,     seed) & scalar) * inv_scalar;
		float n1 = float(Squirrel2D(x_floor + 1, y_floor,     seed) & scalar) * inv_scalar;
		float n2 = float(Squirrel2D(x_floor,     y_floor + 1, seed) & scalar) * inv_scalar;
		float n3 = float(Squirrel2D(x_floor + 1, y_floor + 1, seed) & scalar) * inv_scalar;

		float f0 = SmoothStep(1.0 - x_frac) * n0 + SmoothStep(x_frac) * n1;
		float f1 = SmoothStep(1.0 - x_frac) * n2 + SmoothStep(x_frac) * n3;

		float g0 = SmoothStep(1.0 - y_frac) * f0 + SmoothStep(y_frac) * f1;

		return g0;
	}

float Read64Bit(int x, int y, uint lower, uint upper)
{
	uint N = x + 8*y;
	uint M = (N<32) ? N : N - 32;
	uint C = (N<32) ? lower : upper;

	return float(C>>M&1);
}

float ArctanType(float x)
{
	// pi/2
	return 1.57079632679*x/(1.0 + abs(x));
}

float Arctan2Type(float y, float x)
{
	float pi2 =  1.57079632679;
	if (x==0) return sign(y)*pi2;
	return ArctanType(y/x) + pi2*sign(y)*sign(x)*(sign(x)-1);

}

float CosType(float x)
{
	// 4/pi^2
	return 1.0 - 0.405284734569*x*x;
}