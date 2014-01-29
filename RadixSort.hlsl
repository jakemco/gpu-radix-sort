StructuredBuffer<float> Input : register(u0);

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
	//TODO
}