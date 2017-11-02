Texture2D<float4> reference : register(t0);
Texture2D<float4> actual : register(t1);

RWBuffer<bool> same : register(u0);

[numthreads(16, 16, 1)]
void cs(uint3 threadID : SV_DispatchThreadID)
{
	const float4 diff = reference[threadID.xy] - actual[threadID.xy];
	if (length(diff) > 0.0f) {
		same[0] = 0;
	}
}
