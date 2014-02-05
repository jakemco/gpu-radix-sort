StructuredBuffer<float4> Input : register(t0);
//StructuredBuffer<float> Sums : register(t0);
RWTexture2D<float4> Result : register (u0);



cbuffer cbCS : register(b0)
{
	int c_height : packoffset(c0.x);
	int c_width : packoffset(c0.y);		// size view port
	/*
	This is in the constant buffer as well but not used in this shader, so I just keep it in here as a comment

	float c_epsilon : packoffset(c0.z);	// julia detail
	int c_selfShadow : packoffset(c0.w);  // selfshadowing on or off
	float4 c_diffuse : packoffset(c1);	// diffuse shading color
	float4 c_mu : packoffset(c2);		// julia quaternion parameter
	float4x4 rotation : packoffset(c3);
	float zoom : packoffset(c7.x);
	*/
};

bool getBit(uint i, uint n) {
	return ((n >> i) & 1) == 1;
}

#define THREADX 16
#define THREADY 16

#define GROUP_THREADS THREADX * THREADY
groupshared uint o[GROUP_THREADS];
groupshared uint e[GROUP_THREADS];
groupshared uint f[GROUP_THREADS];
//groupshared uint t[GROUP_THREADS];
groupshared uint d[GROUP_THREADS];
groupshared uint totalFalses;

// SV_DispatchThreadID - index of the thread within the entire dispatch in each dimension: x - 0..x - 1; y - 0..y - 1; z - 0..z - 1
// SV_GroupID - index of a thread group in the dispatch — for example, calling Dispatch(2,1,1) results in possible values of 0,0,0 and 1,0,0, varying from 0 to (numthreadsX * numthreadsY * numThreadsZ) – 1
// SV_GroupThreadID - 3D version of SV_GroupIndex - if you specified numthreads(3,2,1), possible values for the SV_GroupThreadID input value have the range of values (0–2,0–1,0)
// SV_GroupIndex - index of a thread within a thread group

[numthreads(THREADX, THREADY, 1)]
void main(
	uint3 Gid  : SV_GroupID,
	uint3 DTid : SV_DispatchThreadID,
	uint3 GTid : SV_GroupThreadID,
	uint  GI : SV_GroupIndex)
{
	const float4 LumVector = float4(0.2125f, 0.7154f, 0.0721f, 0.0f);

	// index out of structured buffer input
	uint idx = DTid.x + DTid.y * c_width;

	// populate the o array with the uint versions of this block
	o[GI] = uint(dot(Input[idx], LumVector) * 0xFFFFFFFF);


	// loop through each bit
	[unroll(32)]
	for (int n = 0; n < 32; n++) {

		// e is 1 where the nth bit is 0.
		e[GI] = getBit(n, o[GI]) == 0;

		GroupMemoryBarrierWithGroupSync(); // wait for e to be populated so we can random access it

		if (GI != 0) {
			f[GI] = e[GI - 1];
		}
		else {
			f[GI] = 0;
		}

		GroupMemoryBarrierWithGroupSync(); // wait for f to be populated before we loop on it

		// Scan Operation (AKA Prefix Sum)
		[unroll(int(log2(GROUP_THREADS)))]
		for (uint i = 1; i < GROUP_THREADS; i <<= 1) { //for n = 0 .. log2(N), i =  2^n
			uint temp;
			if (GI > i) {
				temp = f[GI] + f[GI-i];
			}
			else {
				temp = f[GI];
			}
			GroupMemoryBarrierWithGroupSync();
			f[GI] = temp;
			GroupMemoryBarrierWithGroupSync();

		}

		// Sum up the falses
		if (GI == 0) {
			totalFalses = e[GROUP_THREADS - 1] + f[GROUP_THREADS - 1];
		}

		GroupMemoryBarrierWithGroupSync(); // wait for thread 0 to finish

		// t contains the indexes for the 1 bits
		//t[GI] = GI - f[GI] + totalFalses;

		// we now construct t on the fly

		// d contains the destination indexes for all the bits
		d[GI] = e[GI] ? f[GI] : GI - f[GI] + totalFalses;

		// get the variable
		uint temp = o[GI];

		GroupMemoryBarrierWithGroupSync(); // read-before-write

		// rewrite o
		o[d[GI]] = temp;

		GroupMemoryBarrierWithGroupSync(); // wait for o to be fully populated

	}

	Result[DTid.xy] = float(o[GI]) / 0xFFFFFFFF;

	/*
	uint mod = GI % 16;
	
	if (mod == 0 || GI < 16) {
		Result[DTid.xy] = float4(1, 0, 0, 1);
	}
	*/
}

