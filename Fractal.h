#pragma once
namespace Fractal {

	struct MainConstantBuffer {
		// Julia 4D constants
		unsigned int c_height;
		unsigned int c_width;      // view port size
		float epsilon;  // detail julia
		int selfShadow;  // selfshadowing on or off 
		float diffuse[4]; // diffuse shading color
		float mu[4];    // quaternion julia parameter
		float orientation[4 * 4]; // rotation matrix
		float zoom;
	};

	static float gSaturation = 1.0f;

	static float Epsilon = 0.003f;
	static float ColorT = 0.0f;
	static float ColorA[4] = { 0.25f, 0.45f, 1.0f, 1.0f };
	static float ColorB[4] = { 0.25f, 0.45f, 1.0f, 1.0f };
	static float ColorC[4] = { 0.25f, 0.45f, 1.0f, 1.0f };

	static float MuT = 0.0f;
	static float MuA[4] = { -.278f, -.479f, 0.0f, 0.0f };
	static float MuB[4] = { 0.278f, 0.479f, 0.0f, 0.0f };
	static float MuC[4] = { -.278f, -.479f, -.231f, .235f };

	static BOOL selfShadow = TRUE;
	static float zoom = 1.0f;

	//
	// Random number generator
	// see http://www.codeproject.com/KB/recipes/SimpleRNG.aspx

	// These values are not magical, just the default values Marsaglia used.
	// Any pair of unsigned integers should be fine.
	static unsigned int m_w = 521288629;
	//static unsigned int m_z = 362436069;
#define MZ ((36969 * (362436069 & 65535) + (362436069 >> 16)) << 16)

	static void SetSeed(unsigned int u)
	{
		m_w = u;
	}

	// This is the heart of the generator.
	// It uses George Marsaglia's MWC algorithm to produce an unsigned integer.
	// See http://www.bobwheeler.com/statistics/Password/MarsagliaPost.txt
	static unsigned int GetUint()
	{
		//	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
		m_w = 18000 * (m_w & 65535) + (m_w >> 16);
		return (MZ) +m_w;
	}

	// Produce a uniform random sample from the interval (-1, 1).
	// The method will not return either end point.
	static float GetUniform()
	{
		// 0 <= u < 2^32
		unsigned int u = GetUint();
		// The magic number below is 1/(2^32 + 2).
		// The result is strictly between 0 and 1.
		return (u) * (float) 2.328306435454494e-10 * 2.0f;
	}


	static void
		Interpolate(float m[4], float t, float a[4], float b[4])
	{
			int i;
			for (i = 0; i < 4; i++)
				m[i] = (1.0f - t) * a[i] + t * b[i];
		}


	float dt; // time increment depending on frame rendering time for same animation speed independent of rendering speed

	static void
		UpdateMu(float t[4], float a[4], float b[4])
	{
			*t += 0.01f *dt;

			if (*t >= 1.0f)
			{
				*t = 0.0f;

				a[0] = b[0];
				a[1] = b[1];
				a[2] = b[2];
				a[3] = b[3];

				b[0] = GetUniform();
				b[1] = GetUniform();
				b[2] = GetUniform();
				b[3] = GetUniform();
			}
		}

	static void
		RandomColor(float v[4])
	{
			do
			{
				v[0] = GetUniform();
				v[1] = GetUniform();
				v[2] = GetUniform();
			} while (v[0] < 0 && v[1] <0 && v[2]<0); // prevent black colors
			v[3] = 1.0f;
		}

	static void
		UpdateColor(float t[4], float a[4], float b[4])
	{
			*t += 0.01f *dt;

			if (*t >= 1.0f)
			{
				*t = 0.0f;

				a[0] = b[0];
				a[1] = b[1];
				a[2] = b[2];
				a[3] = b[3];

				RandomColor(b);
			}
		}

	// timer global variables
	DWORD StartTime;
	static DWORD CurrentTime;

	static void initialize() {
		// setup timer 
		StartTime = GetTickCount();

		// seed the random number generator
		SetSeed((unsigned int) GetCurrentTime());
	}

	static void update() {
		CurrentTime = GetTickCount() - StartTime;
		dt = CurrentTime / (20000.0f);
		UpdateMu(&MuT, MuA, MuB);
		Interpolate(MuC, MuT, MuA, MuB);

		UpdateColor(&ColorT, ColorA, ColorB);
		Interpolate(ColorC, ColorT, ColorA, ColorB);
	}

	static void fill(MainConstantBuffer* mc) {
		// this is a continous constant buffer
		// that means each value is aligned in the buffer one after each other without any spaces
		// the layout need to be in the same order as the constant buffer struct in the shader
		
		mc->epsilon = Epsilon;
		mc->selfShadow = selfShadow;
		mc->diffuse[0] = ColorC[0];
		mc->diffuse[1] = ColorC[1];
		mc->diffuse[2] = ColorC[2];
		mc->diffuse[3] = ColorC[3];
		mc->mu[0] = MuC[0];
		mc->mu[1] = MuC[1];
		mc->mu[2] = MuC[2];
		mc->mu[3] = MuC[3];
		mc->orientation[0] = 1.0;
		//		mc->orientation[1] = 0.0;
		//		mc->orientation[2] = 0.0;
		//		mc->orientation[3] = 0.0;
		//		mc->orientation[4] = 0.0;
		mc->orientation[5] = 1.0;
		//		mc->orientation[6] = 0.0;
		//		mc->orientation[7] = 0.0;
		//		mc->orientation[8] = 0.0;
		//		mc->orientation[9] = 0.0;
		mc->orientation[10] = 1.0;
		//		mc->orientation[11] = 0.0;
		//		mc->orientation[12] = 0.0;
		//		mc->orientation[13] = 0.0;
		//		mc->orientation[14] = 0.0;
		mc->orientation[15] = 1.0;
		mc->zoom = zoom;
	}
};


