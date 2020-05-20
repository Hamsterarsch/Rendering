cbuffer localConstantBuffer : register(b1) { uint tileCount; }
Buffer<uint> volumeTileFlags : register(t0);
globallycoherent RWBuffer<uint> activeTileList : register(u1);

[numthreads(32, 1, 1)]
void main(uint3 dispatchID : SV_DispatchThreadID)
{
	activeTileList[0] = 1;
	
	if(dispatchID.x >= tileCount)
	{
		return;
		
	}
	
	if(volumeTileFlags[dispatchID.x])
	{
		uint targetIndex = 1;
		InterlockedAdd(activeTileList[0], 1, targetIndex);
		activeTileList[targetIndex] = dispatchID.x;
	}
	
}
