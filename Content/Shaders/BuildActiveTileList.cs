ConstantBuffer<uint> tileCount : register(b1);
RWBuffer<uint> volumeTileFlags : register(u1);
AppendStructuredBuffer<uint> activeTileList : register(u2);

[numthreads(32, 1, 1)]
void main(uint3 dispatchID : SV_DispatchThreadID)
{
	if(dispatch.ID < tileCount && activeTileList[dispatchID.x])
	{
		activeTileList.Append(dispatchID.x);
	}
	
}