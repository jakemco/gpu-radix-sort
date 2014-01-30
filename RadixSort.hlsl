StructuredBuffer<float4> Input : register(t0);
Texture2D<float4> Sums : register(t1);
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

#define THREADX 16
#define THREADY 16

#define GROUP_THREADS THREADX * THREADY
groupshared float iterateSum[GROUP_THREADS];

// SV_DispatchThreadID - index of the thread within the entire dispatch in each dimension: x - 0..x - 1; y - 0..y - 1; z - 0..z - 1
// SV_GroupID - index of a thread group in the dispatch — for example, calling Dispatch(2,1,1) results in possible values of 0,0,0 and 1,0,0, varying from 0 to (numthreadsX * numthreadsY * numThreadsZ) – 1
// SV_GroupThreadID - 3D version of SV_GroupIndex - if you specified numthreads(3,2,1), possible values for the SV_GroupThreadID input value have the range of values (0–2,0–1,0)
// SV_GroupIndex - index of a thread within a thread group

[numthreads(THREADX, THREADY, 1)]
void main(
	uint3 Gid  : SV_GroupID,
	uint3 DTid : SV_DispatchThreadID,
	uint3 GTid : SV_GroupThreadID,
	uint  GI   : SV_GroupIndex)
{
	
	Result[DTid.xy] = Sums[Gid.xy];
}