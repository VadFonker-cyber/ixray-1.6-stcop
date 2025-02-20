//--------------------------------------------------------------------------------------
// Gather pattern
//--------------------------------------------------------------------------------------

//=================================================================================================================================
// The constant buffer
//=================================================================================================================================

#define g_f2RTSize (pos_decompression_params2.xy)
#pragma warning(disable : 4714)

//=================================================================================================================================
// Textures, Buffers & Samplers
//=================================================================================================================================

#ifdef SM_5_0
// CS Output buffers
RWTexture2D<float> g_ResultTexture : register(u0);
#endif

#define g_txDepth s_position
#define g_txNormal s_normal

// Samplers
#define g_SamplePoint smp_nofilter

//=================================================================================================================================
// Hard coded HDAO params
//=================================================================================================================================

static float g_fHDAORejectRadius = 0.43f; // Camera Z values must fall within the reject and accept radius to be
static float g_fHDAOAcceptRadius = 0.0001f; // considered as a valley
static float g_fHDAOIntensity = 0.5f; // Simple scaling factor to control the intensity of the occlusion
static float g_fHDAONormalScale = 0.10f; // Scaling factor to control the effect the normals have
static float g_fAcceptAngle = 0.98f; // Used by the ValleyAngle function to determine shallow valleys

//=================================================================================================================================
// Thread / Group Defines
//=================================================================================================================================

// Group Defines
#define GROUP_TEXEL_DIM (56)
#define GROUP_THREAD_DIM (32) // 32 * 32 = 1024 threads
#define GROUP_TEXEL_OVERLAP (12)

// Texture Op Defines
#define GATHER_THREADS (784)
#define GATHER_THREADS_PER_ROW (28)
#define GATHER_PER_THREAD (1)

// ALU Op Defines
#define ALU_DIM (32)

#ifdef SM_5_0

//=============================================================================================================================
// Group shared memory (LDS)
//=============================================================================================================================

groupshared struct
{
    float fCameraZ[GROUP_TEXEL_DIM][GROUP_TEXEL_DIM];
} g_LDS;

//=============================================================================================================================
// Helper function to load data from the LDS, given texel coord
// NOTE: X and Y are swapped around to ensure horizonatal reading across threads, this avoids
// LDS memory bank conflicts
//=============================================================================================================================
float LoadFromLDS(uint2 u2Texel)
{
    return g_LDS.fCameraZ[u2Texel.y][u2Texel.x];
}

//=============================================================================================================================
// Helper function to store data to the LDS, given texel coord
// NOTE: X and Y are swapped around to ensure horizonatal wrting across threads, this avoids
// LDS memory bank conflicts
//=============================================================================================================================
void StoreToLDS(float fValue, uint2 u2Texel)
{
    g_LDS.fCameraZ[u2Texel.y][u2Texel.x] = fValue;
}

#endif

//=================================================================================================================================
// HDAO sample pattern
//=================================================================================================================================

#if SSAO_QUALITY >= 3

    #define NUM_VALLEYS (48)

static const int2 g_i2HDAOSamplePattern[NUM_VALLEYS] =
{
    {0, -11},
    {2, -10},
    {0, -9},
    {5, -9},
    {2, -8},
    {7, -8},
    {0, -7},
    {5, -7},
    {2, -6},
    {7, -6},
    {8, -6},
    {0, -5},
    {5, -5},
    {10, -5},
    {2, -4},
    {7, -4},
    {0, -3},
    {5, -3},
    {10, -3},
    {2, -2},
    {7, -2},
    {0, -1},
    {5, -1},
    {10, -1},
    {2, 0},
    {7, 0},
    {5, 1},
    {10, 1},
    {2, 2},
    {7, 2},
    {5, 3},
    {10, 3},
    {2, 4},
    {7, 4},
    {5, 5},
    {10, 5},
    {2, 6},
    {7, 6},
    {5, 7},
    {6, 7},
    {10, 7},
    {2, 8},
    {7, 8},
    {5, 9},
    {2, 10},
    {7, 10},
    {5, 11},
    {2, 12},
};

static const float g_fHDAOSampleWeights[NUM_VALLEYS] =
{
    0.1538,
    0.2155,
    0.3077,
    0.2080,
    0.3657,
    0.1823,
    0.4615,
    0.3383,
    0.5135,
    0.2908,
    0.2308,
    0.6154,
    0.4561,
    0.1400,
    0.6560,
    0.3798,
    0.7692,
    0.5515,
    0.1969,
    0.7824,
    0.4400,
    0.9231,
    0.6078,
    0.2269,
    0.8462,
    0.4615,
    0.6078,
    0.2269,
    0.7824,
    0.4400,
    0.5515,
    0.1969,
    0.6560,
    0.3798,
    0.4561,
    0.1400,
    0.5135,
    0.2908,
    0.3383,
    0.2908,
    0.0610,
    0.3657,
    0.1823,
    0.2080,
    0.2155,
    0.0610,
    0.0705,
    0.0642,
};

static float g_fWeightTotal = 18.4198;
#endif

// Used by the valley angle function
#define NUM_NORMAL_LOADS (4)
static const int2 g_i2NormalLoadPattern[NUM_NORMAL_LOADS] =
{
    {0, -9},
    {6, -6},
    {10, 0},
    {8, 9},
};

#if SSAO_QUALITY >= 3
//=================================================================================================================================
// Computes the general valley angle
//=================================================================================================================================
float ValleyAngle(uint2 u2ScreenCoord)
{
    float3 f3N1;
    float3 f3N2;
    float fDot;
    float fSummedDot = 0.0f;
    int2 i2MirrorPattern;
    int2 i2OffsetScreenCoord;
    int2 i2MirrorOffsetScreenCoord;

    float3 N = 1.0f - 2.0f * g_txNormal.Load(int3(u2ScreenCoord, 0), 0).xyz;

    for (int iNormal = 0; iNormal < NUM_NORMAL_LOADS; iNormal++)
    {
        i2MirrorPattern = g_i2NormalLoadPattern[iNormal] * int2(-1, -1);
        i2OffsetScreenCoord = u2ScreenCoord + g_i2NormalLoadPattern[iNormal];
        i2MirrorOffsetScreenCoord = u2ScreenCoord + i2MirrorPattern;

        // Clamp our test to screen coordinates
        i2OffsetScreenCoord = (i2OffsetScreenCoord > (g_f2RTSize - float2(1.0f, 1.0f))) ? (g_f2RTSize - float2(1.0f, 1.0f)) : (i2OffsetScreenCoord);
        i2MirrorOffsetScreenCoord = (i2MirrorOffsetScreenCoord > (g_f2RTSize - float2(1.0f, 1.0f))) ? (g_f2RTSize - float2(1.0f, 1.0f)) : (i2MirrorOffsetScreenCoord);
        i2OffsetScreenCoord = (i2OffsetScreenCoord < 0) ? (0) : (i2OffsetScreenCoord);
        i2MirrorOffsetScreenCoord = (i2MirrorOffsetScreenCoord < 0) ? (0) : (i2MirrorOffsetScreenCoord);

        f3N1.xyz = 1.0f - 2.0f * g_txNormal.Load(int3(i2OffsetScreenCoord, 0), 0).xyz;
        f3N2.xyz = 1.0f - 2.0f * g_txNormal.Load(int3(i2MirrorOffsetScreenCoord, 0), 0).xyz;

        fDot = dot(f3N1, N);

        fSummedDot += (fDot > g_fAcceptAngle) ? (0.0f) : (1.0f - (abs(fDot) * 0.25f));

        fDot = dot(f3N2, N);

        fSummedDot += (fDot > g_fAcceptAngle) ? (0.0f) : (1.0f - (abs(fDot) * 0.25f));
    }

    fSummedDot /= 8.0f;
    fSummedDot += 0.5f;
    fSummedDot = (fSummedDot <= 0.5f) ? (fSummedDot / 10.0f) : (fSummedDot);

    return fSummedDot;
}
#endif

#ifdef SM_5_0

    #if SSAO_QUALITY >= 3
float ComputeHDAO(uint2 u2CenterTexel, uint2 u2ScreenPos)
{
    // Locals
    float fCenterZ;
    float2 f2SamplePos;
    float2 f2MirrorSamplePos;
    float fOcclusion = 0.0f;
    float2 f2SampledZ;
    float2 f2Diff;
    float2 f2Compare;
    float fDot;

    // Get the general valley angle, to scale the result by
    fDot = ValleyAngle(u2ScreenPos);

    // Sample center texel
    fCenterZ = LoadFromLDS(u2CenterTexel);

    // Loop through each valley
    [unroll]
    for (uint uValley = 0; uValley < NUM_VALLEYS; uValley++)
    {
        // Sample
        f2SampledZ.x = LoadFromLDS(u2CenterTexel + g_i2HDAOSamplePattern[uValley]);
        f2SampledZ.y = LoadFromLDS(u2CenterTexel - g_i2HDAOSamplePattern[uValley]);

        // Valley detect
        f2Diff = fCenterZ.xx - f2SampledZ;
        f2Compare = (f2Diff < g_fHDAORejectRadius.xx) ? (1.0f) : (0.0f);
        f2Compare *= (f2Diff > g_fHDAOAcceptRadius.xx) ? (1.0f) : (0.0f);

        // Weight occlusion
        fOcclusion += (f2Compare.x * f2Compare.y * g_fHDAOSampleWeights[uValley]);
    }

    // Finally calculate the HDAO occlusion value
    fOcclusion /= g_fWeightTotal;
    fOcclusion *= g_fHDAOIntensity * fDot;
    fOcclusion *= fCenterZ < 0.5f ? 0.0f : lerp(0.0f, 1.0f, saturate(fCenterZ - 0.5f));
    fOcclusion = 1.0f - saturate(fOcclusion);

    return fOcclusion;
}
    #endif

//=============================================================================================================================
// HDAO CS: Performs valley detection in Camera Z space, and offsets by the Z
// component of the camera space normal
//=============================================================================================================================
[numthreads(GROUP_THREAD_DIM, GROUP_THREAD_DIM, 1)]
void main(uint3 Gid : SV_GroupID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
    #ifndef SSAO_QUALITY
{
    // Calculate the screen pos
    uint2 u2ScreenPos = uint2(Gid.x * ALU_DIM + GTid.x, Gid.y * ALU_DIM + GTid.y);

    // Make sure we don't write outside the target buffer
    if ((u2ScreenPos.x < uint(g_f2RTSize.x)) && (u2ScreenPos.y < uint(g_f2RTSize.y)))
    {
        // Write the data directly to an AO texture:
        g_ResultTexture[u2ScreenPos.xy] = 1.0f;
    }
}
    #elif SSAO_QUALITY < 3
{
    // Calculate the screen pos
    uint2 u2ScreenPos = uint2(Gid.x * ALU_DIM + GTid.x, Gid.y * ALU_DIM + GTid.y);

    // Make sure we don't write outside the target buffer
    if ((u2ScreenPos.x < uint(g_f2RTSize.x)) && (u2ScreenPos.y < uint(g_f2RTSize.y)))
    {
        // Write the data directly to an AO texture:
        g_ResultTexture[u2ScreenPos.xy] = 1.0f;
    }
}
    #elif SSAO_QUALITY >= 3
{
    // Locals
    float2 f2ScreenCoord;
    float2 f2Coord;
    float2 f2InvTextureSize = 1.0f / g_f2RTSize;
    float4 f4Depth;
    float4 f4Normal;
    float4 f4LDSValue;
    uint uColumn, uRow;

    if (GI < GATHER_THREADS)
    {
        // Get the screen position for this threads TEX ops
        uColumn = (GI % GATHER_THREADS_PER_ROW) * GATHER_PER_THREAD * 2;
        uRow = (GI / GATHER_THREADS_PER_ROW) * 2;
        f2ScreenCoord = float2((float2(Gid.x, Gid.y) * float2(ALU_DIM, ALU_DIM)) - float2(GROUP_TEXEL_OVERLAP, GROUP_TEXEL_OVERLAP)) + float2(uColumn, uRow);

        // Offset for the use of gather4
        f2ScreenCoord += float2(1.0f, 1.0f);

        // Gather from input textures and lay down in the LDS
        [unroll]
        for (uint uGather = 0; uGather < GATHER_PER_THREAD; uGather++)
        {
            f2Coord = float2(f2ScreenCoord.x + float(uGather * 2), f2ScreenCoord.y) * f2InvTextureSize;
            f4Depth = depth_unpack.x / (g_txDepth.GatherRed(g_SamplePoint, f2Coord) - depth_unpack.y);
            f4Normal = 1.0f - 2.0f * g_txNormal.GatherBlue(g_SamplePoint, f2Coord);

            f4LDSValue = f4Depth + (f4Normal * g_fHDAONormalScale.xxxx);

            StoreToLDS(f4LDSValue.x, uint2(uColumn + (uGather * 2) + 0, uRow + 1));
            StoreToLDS(f4LDSValue.y, uint2(uColumn + (uGather * 2) + 1, uRow + 1));
            StoreToLDS(f4LDSValue.z, uint2(uColumn + (uGather * 2) + 1, uRow + 0));
            StoreToLDS(f4LDSValue.w, uint2(uColumn + (uGather * 2) + 0, uRow + 0));
        }
    }

    // Enforce a group barrier with sync
    GroupMemoryBarrierWithGroupSync();

    // Calculate the screen pos
    uint2 u2ScreenPos = uint2(Gid.x * ALU_DIM + GTid.x, Gid.y * ALU_DIM + GTid.y);

    // Make sure we don't write outside the target buffer
    if ((u2ScreenPos.x < uint(g_f2RTSize.x)) && (u2ScreenPos.y < uint(g_f2RTSize.y)))
    {
        // Write the data directly to an AO texture:
        g_ResultTexture[u2ScreenPos.xy] = ComputeHDAO(uint2(GTid.x + GROUP_TEXEL_OVERLAP, GTid.y + GROUP_TEXEL_OVERLAP), u2ScreenPos);
    }
}
    #endif

#else

//=================================================================================================================================
// HDAO PS: Performs valley detection in Camera Z space, and offsets by the Z
// component of the camera space normal
//=================================================================================================================================
float calc_new_hdao(float3 P, float3 N, float2 tc, float2 tcJ, float4 pos2d)
    #ifndef SSAO_QUALITY
{
    return 1.0f;
}
    #elif SSAO_QUALITY >= 3
{
    // Locals
    uint2 u2CenterScreenCoord;
    float2 f2ScreenCoord;
    float2 f2MirrorScreenCoord;
    float fCenterZ;
    float2 f2SampledZ;
    float2 f2Diff;
    float2 f2Compare;
    float fOcclusion = 0.0f;
    int iValley;
    float fDot;

    // Compute screen coord, and store off the inverse of the RT Size
    u2CenterScreenCoord = uint2(floor(tc * g_f2RTSize));

    // Get the general valley angle, to scale the result by
    fDot = ValleyAngle(u2CenterScreenCoord);

    // Sample center texel, convert to camera space and add normal
    float fDepth = depth_unpack.x / (g_txDepth.Load(int3(u2CenterScreenCoord, 0), 0).x - depth_unpack.y);
    fCenterZ = fDepth + (1.0f - 2.0f * g_txNormal.Load(int3(u2CenterScreenCoord, 0), 0).z) * g_fHDAONormalScale;

    // Loop through each valley
    for (iValley = 0; iValley < NUM_VALLEYS; iValley++)
    {
        // Sample depth & convert to camera space
        f2SampledZ.x = depth_unpack.x / (g_txDepth.Load(int3((u2CenterScreenCoord + g_i2HDAOSamplePattern[iValley]), 0), 0).x - depth_unpack.y);
        f2SampledZ.y = depth_unpack.x / (g_txDepth.Load(int3((u2CenterScreenCoord - g_i2HDAOSamplePattern[iValley]), 0), 0).x - depth_unpack.y);

        // Sample normal and do a scaled add
        f2SampledZ.x += (1.0f - 2.0f * g_txNormal.Load(int3((u2CenterScreenCoord + g_i2HDAOSamplePattern[iValley]), 0), 0).z) * g_fHDAONormalScale;
        f2SampledZ.y += (1.0f - 2.0f * g_txNormal.Load(int3((u2CenterScreenCoord - g_i2HDAOSamplePattern[iValley]), 0), 0).z) * g_fHDAONormalScale;

        // Detect valleys
        f2Diff = fCenterZ.xx - f2SampledZ;
        f2Compare = (f2Diff < g_fHDAORejectRadius.xx) ? (1.0f) : (0.0f);
        f2Compare *= (f2Diff > g_fHDAOAcceptRadius.xx) ? (1.0f) : (0.0f);

        // Accumulate weighted occlusion
        fOcclusion += f2Compare.x * f2Compare.y * g_fHDAOSampleWeights[iValley];
    }

    // Finally calculate the HDAO occlusion value
    fOcclusion /= g_fWeightTotal;
    fOcclusion *= g_fHDAOIntensity * fDot;
    fOcclusion *= fCenterZ < 0.5f ? 0.0f : lerp(0.0f, 1.0f, saturate(fCenterZ - 0.5f));
    fOcclusion = 1.0f - saturate(fOcclusion);

    return fOcclusion;
}
    #endif

#endif

//=================================================================================================================================
// EOF
//=================================================================================================================================
