StructuredBuffer<float4> Input : register(t0);
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
groupshared float sharedMem[GROUP_THREADS];

[numthreads(THREADX, THREADY, 1)]
void main(
	uint3 Gid  : SV_GroupID,
	uint3 DTid : SV_DispatchThreadID,
	uint3 GTid : SV_GroupThreadID,
	uint  GI   : SV_GroupIndex)
{
	Result[DTid.xy] = Input[DTid.x + DTid.y * c_width];
}