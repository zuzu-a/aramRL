#include <gtest/gtest.h>
#include "inc/core/procgen/PointGenerator.hpp"
#include "inc/core/procgen/MeshPrimitives.hpp"
#include "inc/core/procgen/DualMeshGenerator.hpp" // Placeholder, may not be fully used if Delaunay fails
#include <vector>

// PointGenerator Tests
TEST(PointGeneratorTest, GeneratePoissonPointsInBounds) {
    PointGenerator pg;
    AABB bounds(0.0f, 0.0f, 100.0f, 100.0f);
    float spacing = 10.0f;
    unsigned int seed = 12345; // Fixed seed for reproducibility

    std::vector<Vector2D> points = pg.GeneratePoissonPoints(bounds, spacing, 30, seed);

    ASSERT_FALSE(points.empty()); // Should generate some points
    for (const auto& p : points) {
        ASSERT_GE(p.x, bounds.min.x);
        ASSERT_LE(p.x, bounds.max.x);
        ASSERT_GE(p.y, bounds.min.y);
        ASSERT_LE(p.y, bounds.max.y);
    }
    // A more robust test might check average point density or min distance,
    // but this requires more complex analysis.
}

TEST(PointGeneratorTest, GeneratePoissonPointsZeroArea) {
    PointGenerator pg;
    AABB bounds(0.0f, 0.0f, 0.0f, 0.0f); // Zero area
    float spacing = 10.0f;
    std::vector<Vector2D> points = pg.GeneratePoissonPoints(bounds, spacing);
    ASSERT_TRUE(points.empty());
}


TEST(PointGeneratorTest, GenerateBoundaryPoints) {
    PointGenerator pg;
    AABB bounds(0.0f, 0.0f, 20.0f, 20.0f);
    float spacing = 10.0f;
    std::vector<Vector2D> points = pg.GenerateBoundaryPoints(bounds, spacing);

    // Expected points:
    // Top: (0,0), (10,0)
    // Right: (20,0), (20,10)
    // Bottom: (20,20), (10,20)
    // Left: (0,20), (0,10)
    // Total 8 points
    ASSERT_EQ(points.size(), 8); 
    
    // Check a few points (exact order depends on implementation)
    bool found_0_0 = false;
    bool found_20_0 = false;
    bool found_20_20 = false;
    bool found_0_20 = false;
    for(const auto& p : points) {
        if (p.x == 0.0f && p.y == 0.0f) found_0_0 = true;
        if (p.x == 20.0f && p.y == 0.0f) found_20_0 = true;
        if (p.x == 20.0f && p.y == 20.0f) found_20_20 = true;
        if (p.x == 0.0f && p.y == 20.0f) found_0_20 = true;
    }
    ASSERT_TRUE(found_0_0);
    ASSERT_TRUE(found_20_0);
    ASSERT_TRUE(found_20_20);
    ASSERT_TRUE(found_0_20);
}

TEST(PointGeneratorTest, GeneratePointsMainMethod) {
    PointGenerator pg;
    AABB bounds(0.0f, 0.0f, 50.0f, 50.0f);
    float spacing = 15.0f;
    std::vector<Vector2D> points = pg.GeneratePoints(bounds, spacing, 25.0f, 123);
    ASSERT_FALSE(points.empty());
     for (const auto& p : points) {
        ASSERT_GE(p.x, bounds.min.x);
        ASSERT_LE(p.x, bounds.max.x);
        ASSERT_GE(p.y, bounds.min.y);
        ASSERT_LE(p.y, bounds.max.y);
    }
}

// DualMeshGenerator Tests (basic, using placeholder delaunator)
TEST(DualMeshGeneratorTest, DelaunayWithFewPoints) {
    ProcGen::MeshData meshData;
    meshData.bounds = AABB(0.0f, 0.0f, 100.0f, 100.0f);
    
    // Add 3 points to form one triangle
    meshData.sites.push_back({0, {10.0f, 10.0f}});
    meshData.sites.push_back({1, {90.0f, 10.0f}});
    meshData.sites.push_back({2, {50.0f, 90.0f}});

    ProcGen::DualMeshGenerator dmg;
    bool success = dmg.GenerateDelaunay(meshData);

    ASSERT_TRUE(success);
    // The placeholder delaunator.hpp creates one dummy triangle (0,1,2) if n >= 3
    ASSERT_EQ(meshData.triangles.size(), 1); 
    if (!meshData.triangles.empty()) {
        const auto& tri = meshData.triangles[0];
        ASSERT_EQ(tri.siteIds[0], 0);
        ASSERT_EQ(tri.siteIds[1], 1);
        ASSERT_EQ(tri.siteIds[2], 2);
    }
}

TEST(DualMeshGeneratorTest, DelaunayLessThan3Points) {
    ProcGen::MeshData meshData;
    meshData.bounds = AABB(0.0f, 0.0f, 100.0f, 100.0f);
    meshData.sites.push_back({0, {10.0f, 10.0f}});
    meshData.sites.push_back({1, {90.0f, 10.0f}});

    ProcGen::DualMeshGenerator dmg;
    bool success = dmg.GenerateDelaunay(meshData);
    ASSERT_TRUE(success); // Should succeed but generate no triangles
    ASSERT_TRUE(meshData.triangles.empty());
}

TEST(DualMeshGeneratorTest, DelaunayNoPoints) {
    ProcGen::MeshData meshData; // No sites
    meshData.bounds = AABB(0.0f, 0.0f, 100.0f, 100.0f);
    
    ProcGen::DualMeshGenerator dmg;
    bool success = dmg.GenerateDelaunay(meshData);
    ASSERT_FALSE(success); // No sites, should return false as per current implementation
    ASSERT_TRUE(meshData.triangles.empty());
}

// ElevationGenerator Tests
#include "inc/core/procgen/ElevationGenerator.hpp"

TEST(ElevationGeneratorTest, GetIslandShapeModifier) {
    // Note: ElevationGenerator constructor takes int64_t for seed, not used by GetIslandShapeModifier directly.
    // We can test GetIslandShapeModifier by calling it directly if we make it static or create an instance.
    // For simplicity, let's assume we can call it or test its effects via AssignElevation.
    // The current ElevationGenerator.cpp makes GetIslandShapeModifier private.
    // To test it directly, it would need to be public or tested via AssignElevation.
    // Let's test via AssignElevation for now.

    ProcGen::MeshData meshData;
    meshData.bounds = AABB(-100.0f, -100.0f, 100.0f, 100.0f); // 200x200 area
    
    // Site at center (normalized centered coords: 0,0)
    meshData.sites.push_back({0, {0.0f, 0.0f}}); 
    // Site at edge (normalized centered coords: 0.5, 0.0 if bounds.min.x=0, bounds.max.x=200)
    // (normalized centered coords: 0.5, 0.0 for point (100,0) if bounds are [0,200]x[0,200])
    // For bounds [-100,100]x[-100,100]:
    // Point (0,0) -> nx=0.5, ny=0.5. nx_c=0, ny_c=0.
    // Point (100,0) -> nx=1.0, ny=0.5. nx_c=0.5, ny_c=0.
    meshData.sites.push_back({1, {100.0f, 0.0f}}); 
    meshData.sites.push_back({2, {0.0f, 100.0f}}); 
    meshData.sites.push_back({3, {100.0f, 100.0f}}); // Corner

    ProcGen::ElevationGenerator elevGen(12345); // Seed
    float islandExponent = 2.0f; // Example exponent for island shape modifier in GetIslandShapeModifier
                                 // This is the `islandShapeFactor` param in AssignElevation

    elevGen.AssignElevation(meshData, islandExponent);

    // Center site (id 0) should have highest island modifier influence, closer to 1.0 * (weight in AssignElevation)
    // Edge sites (id 1, 2) should have lower influence.
    // Corner site (id 3) should have lowest influence from island shape, closer to 0.
    // Noise makes exact values hard to predict, but trends can be checked.
    // Elevations should be in [-1, 1] roughly. isWater based on elevation < 0.

    ASSERT_GE(meshData.sites[0].elevation, meshData.sites[1].elevation); // Center higher than edge
    ASSERT_GE(meshData.sites[0].elevation, meshData.sites[2].elevation); // Center higher than edge
    ASSERT_GE(meshData.sites[1].elevation, meshData.sites[3].elevation); // Edge higher than corner (roughly)
    
    for(const auto& site : meshData.sites) {
        ASSERT_GE(site.elevation, -1.1f); // Allow some margin due to noise
        ASSERT_LE(site.elevation, 1.1f);  // Allow some margin
        if (site.elevation < 0.0f) {
            ASSERT_TRUE(site.isWater);
        } else {
            ASSERT_FALSE(site.isWater);
        }
    }
    // Example: check if center is land and corner is water for a strong island shape
    // This depends heavily on noise values and weights in AssignElevation
    // For this test, we primarily check that values are set and isWater is consistent.
    // std::cout << "Site 0 (center): elev=" << meshData.sites[0].elevation << ", water=" << meshData.sites[0].isWater << std::endl;
    // std::cout << "Site 1 (edgeX): elev=" << meshData.sites[1].elevation << ", water=" << meshData.sites[1].isWater << std::endl;
    // std::cout << "Site 3 (corner): elev=" << meshData.sites[3].elevation << ", water=" << meshData.sites[3].isWater << std::endl;

}

TEST(ElevationGeneratorTest, AssignElevationNoSites) {
    ProcGen::MeshData meshData; // No sites
    ProcGen::ElevationGenerator elevGen(123);
    ASSERT_NO_THROW(elevGen.AssignElevation(meshData)); // Should not crash
    ASSERT_TRUE(meshData.sites.empty());
}

// RiverGenerator Tests
#include "inc/core/procgen/RiverGenerator.hpp"

TEST(RiverGeneratorTest, GenerateRiversSimpleFlow) {
    ProcGen::MeshData meshData;
    meshData.bounds = AABB(0.0f, 0.0f, 100.0f, 100.0f); // Define bounds for neighbor search

    // Site A (high, moist) -> Site B (mid) -> Site C (low, water)
    // Site IDs are their indices for this test (0, 1, 2)
    meshData.sites.push_back({0, {10.0f, 10.0f}, false, 0.8f, false, 0.7f, 0.0f, ProcGen::Site::BiomeType::GRASSLAND, 0.0f, -1});
    meshData.sites.push_back({1, {20.0f, 10.0f}, false, 0.5f, false, 0.3f, 0.0f, ProcGen::Site::BiomeType::GRASSLAND, 0.0f, -1});
    meshData.sites.push_back({2, {30.0f, 10.0f}, false, 0.1f, true,  0.8f, 0.0f, ProcGen::Site::BiomeType::SHALLOW_WATER, 0.0f, -1}); // Water site

    ProcGen::RiverGenerator riverGen(123);
    riverGen.GenerateRivers(meshData);

    // Check initial flow for site A (land, moist)
    ASSERT_GT(meshData.sites[0].riverFlow, 0.0f); 
    float initialFlowA = meshData.sites[0].riverFlow;

    // Check Site A flows to Site B
    ASSERT_EQ(meshData.sites[0].downslopeSiteId, 1);

    // Check Site B's flow (should be its initial flow + A's flow)
    // Site B initial flow: 0.3 * 0.1 = 0.03. Site A initial flow: 0.7 * 0.1 = 0.07
    // Expected on B: 0.03 + 0.07 = 0.1
    // Note: RiverGenerator.cpp uses site.moisture * 0.1f for initial flow.
    float expectedInitialFlowB = 0.3f * 0.1f;
    ASSERT_FLOAT_EQ(meshData.sites[1].riverFlow, initialFlowA + expectedInitialFlowB);
    
    // Check Site B flows to Site C
    ASSERT_EQ(meshData.sites[1].downslopeSiteId, 2);

    // Check Site C's flow (water site, should remain its initial flow, which is 0 as it's water)
    ASSERT_FLOAT_EQ(meshData.sites[2].riverFlow, 0.0f); 
    // Check Site C's downslope (water site, should be -1 or flow to another water body if lower)
    // Current FindDownslopeNeighbor might not set it if it's water. Or it might point to itself or -1.
    // The current logic in GenerateRivers skips processing water sites, so downslopeSiteId remains -1.
    ASSERT_EQ(meshData.sites[2].downslopeSiteId, -1);

    // Check river segments (optional, based on current minFlowForRiver = 0.01f)
    // Segment A->B should exist if flow from A (0.07) >= 0.01
    // Segment B->C should exist if flow from B (0.1) >= 0.01
    ASSERT_GE(meshData.sites[0].riverFlow, 0.01f);
    ASSERT_GE(meshData.sites[1].riverFlow, 0.01f);
    
    bool foundAB = false;
    bool foundBC = false;
    for(const auto& seg : meshData.riverSegments) {
        if (seg.first == 0 && seg.second == 1) foundAB = true;
        if (seg.first == 1 && seg.second == 2) foundBC = true;
    }
    ASSERT_TRUE(foundAB);
    ASSERT_TRUE(foundBC);
    ASSERT_EQ(meshData.riverSegments.size(), 2);
}

TEST(RiverGeneratorTest, GenerateRiversNoFlowToHigherElevation) {
    ProcGen::MeshData meshData;
    meshData.bounds = AABB(0.0f, 0.0f, 100.0f, 100.0f);
    // Site A (low) -> Site B (high, should not receive flow from A)
    meshData.sites.push_back({0, {10.0f, 10.0f}, false, 0.2f, false, 0.7f}); // Low, moist
    meshData.sites.push_back({1, {20.0f, 10.0f}, false, 0.8f, false, 0.3f}); // High

    ProcGen::RiverGenerator riverGen(123);
    riverGen.GenerateRivers(meshData);

    ASSERT_EQ(meshData.sites[0].downslopeSiteId, -1); // Should not find a downslope to B
    ASSERT_FLOAT_EQ(meshData.sites[1].riverFlow, 0.3f * 0.1f); // B's flow should just be its initial
}

TEST(RiverGeneratorTest, GenerateRiversNoSites) {
    ProcGen::MeshData meshData; // No sites
    ProcGen::RiverGenerator riverGen(123);
    ASSERT_NO_THROW(riverGen.GenerateRivers(meshData));
    ASSERT_TRUE(meshData.sites.empty());
    ASSERT_TRUE(meshData.riverSegments.empty());
}

// BiomeGenerator Tests
#include "inc/core/procgen/BiomeGenerator.hpp"

TEST(BiomeGeneratorTest, DetermineBiomeLogic) {
    // BiomeGenerator constructor takes int64_t for seed, not used by DetermineBiome directly.
    // To test DetermineBiome, we'd need to make it public or static, or test via AssignBiomes.
    // Assuming we can call it (e.g., if made public for testing or via a helper).
    // For this test, let's assume it's callable or we test its effects via AssignBiomes.
    // Since the provided BiomeGenerator.cpp makes it private, we'll test via AssignBiomes.
    // This test will be more of an integration test for AssignBiomes's biome determination part.

    ProcGen::MeshData meshData;
    // Site 0: Deep Water
    meshData.sites.push_back({0, {0,0}, false, -0.8f, true}); 
    // Site 1: Shallow Water
    meshData.sites.push_back({1, {1,0}, false, -0.2f, true}); 
    // Site 2: Beach (low, dry land)
    meshData.sites.push_back({2, {2,0}, false, 0.05f, false, 0.1f}); // rainfall=0.1 (becomes moisture)
    // Site 3: Grassland (mid, moderate moisture land)
    meshData.sites.push_back({3, {3,0}, false, 0.3f, false, 0.4f}); 
    // Site 4: Forest (mid, wet land)
    meshData.sites.push_back({4, {4,0}, false, 0.4f, false, 0.7f}); 
    // Site 5: Rocky (high, dry land)
    meshData.sites.push_back({5, {5,0}, false, 0.8f, false, 0.1f}); 
    // Site 6: Snow (high, wet land)
    meshData.sites.push_back({6, {6,0}, false, 0.9f, false, 0.5f}); 
    
    ProcGen::BiomeGenerator biomeGen(123);
    // AssignBiomes will set rainfall/moisture based on its internal logic,
    // then DetermineBiome. For direct test of DetermineBiome logic, we'd need to
    // set moisture directly on sites *then* call a method that only calls DetermineBiome.
    // For now, we rely on the fact that for land, moisture is primarily rainfall + noise.
    // We've set rainfall in the site constructor above to guide moisture for this test.
    // The actual AssignBiomes will overwrite this rainfall value.
    // So, this test structure is a bit flawed for *isolating* DetermineBiome.
    // Let's restructure to test DetermineBiome more directly.

    // For a direct test of DetermineBiome (if it were public/static):
    // ASSERT_EQ(biomeGen.DetermineBiome(-0.8f, 0.0f, true), ProcGen::Site::BiomeType::DEEP_WATER);
    // ASSERT_EQ(biomeGen.DetermineBiome(-0.2f, 0.0f, true), ProcGen::Site::BiomeType::SHALLOW_WATER);
    // ASSERT_EQ(biomeGen.DetermineBiome(0.05f, 0.1f, false), ProcGen::Site::BiomeType::BEACH);
    // ASSERT_EQ(biomeGen.DetermineBiome(0.3f, 0.4f, false), ProcGen::Site::BiomeType::GRASSLAND);
    // ASSERT_EQ(biomeGen.DetermineBiome(0.4f, 0.7f, false), ProcGen::Site::BiomeType::FOREST);
    // ASSERT_EQ(biomeGen.DetermineBiome(0.8f, 0.1f, false), ProcGen::Site::BiomeType::ROCKY);
    // ASSERT_EQ(biomeGen.DetermineBiome(0.9f, 0.5f, false), ProcGen::Site::BiomeType::SNOW);

    // Test AssignBiomes' overall effect
    biomeGen.AssignBiomes(meshData); // This will calculate rainfall and moisture internally.
                                     // The pre-set rainfall values above will be overwritten.

    ASSERT_EQ(meshData.sites[0].biome, ProcGen::Site::BiomeType::DEEP_WATER);
    ASSERT_EQ(meshData.sites[1].biome, ProcGen::Site::BiomeType::SHALLOW_WATER);
    // The biomes for land sites will now depend on the noise-generated rainfall/moisture
    // and the specific thresholds in DetermineBiome.
    // This makes it harder to assert exact biomes without knowing the noise output.
    // We can at least check they are not UNKNOWN.
    for(size_t i = 2; i < meshData.sites.size(); ++i) {
        ASSERT_NE(meshData.sites[i].biome, ProcGen::Site::BiomeType::UNKNOWN);
        ASSERT_GE(meshData.sites[i].rainfall, 0.0f);
        ASSERT_LE(meshData.sites[i].rainfall, 1.0f);
        ASSERT_GE(meshData.sites[i].moisture, 0.0f);
        ASSERT_LE(meshData.sites[i].moisture, 1.0f);
    }
}


TEST(BiomeGeneratorTest, AssignBiomesNoSites) {
    ProcGen::MeshData meshData; // No sites
    ProcGen::BiomeGenerator biomeGen(123);
    ASSERT_NO_THROW(biomeGen.AssignBiomes(meshData)); // Should not crash
    ASSERT_TRUE(meshData.sites.empty());
}
