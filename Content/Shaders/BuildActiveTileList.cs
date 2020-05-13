cbuffer localConstantBuffer : register(b1) { uint tileCount; }
Buffer<uint> volumeTileFlags : register(t0);
AppendStructuredBuffer<uint> activeTileList : register(u1);

[numthreads(32, 1, 1)]
void main(uint3 dispatchID : SV_DispatchThreadID)
{
	if(dispatchID.x < tileCount && volumeTileFlags[dispatchID.x])
	{
		activeTileList.Append(dispatchID.x);
	}
	
}