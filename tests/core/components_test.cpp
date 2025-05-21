#include <gtest/gtest.h>
#include "inc/core/components/vector2d.hpp"
#include "inc/core/components/transform.hpp"
#include "inc/core/components/player.hpp"

// Vector2D Tests
TEST(Vector2DTest, Constructor) {
    Vector2D v1;
    ASSERT_FLOAT_EQ(v1.x, 0.0f);
    ASSERT_FLOAT_EQ(v1.y, 0.0f);

    Vector2D v2(1.5f, -2.5f);
    ASSERT_FLOAT_EQ(v2.x, 1.5f);
    ASSERT_FLOAT_EQ(v2.y, -2.5f);
}

TEST(Vector2DTest, Addition) {
    Vector2D v1(1.0f, 2.0f);
    Vector2D v2(3.0f, 4.0f);
    Vector2D sum = v1 + v2;
    ASSERT_FLOAT_EQ(sum.x, 4.0f);
    ASSERT_FLOAT_EQ(sum.y, 6.0f);
}

TEST(Vector2DTest, Length) {
    Vector2D v(3.0f, 4.0f);
    ASSERT_FLOAT_EQ(v.Length(), 5.0f);
}

TEST(Vector2DTest, Normalized) {
    Vector2D v(3.0f, 4.0f);
    Vector2D norm = v.Normalized();
    ASSERT_FLOAT_EQ(norm.x, 0.6f);
    ASSERT_FLOAT_EQ(norm.y, 0.8f);
    // Check if it's unit length, considering potential float precision issues
    ASSERT_NEAR(norm.Length(), 1.0f, 1e-6); 
}


// TransformComponent Tests
TEST(TransformComponentTest, DefaultConstructor) {
    TransformComponent tc;
    ASSERT_FLOAT_EQ(tc.position.x, 0.0f);
    ASSERT_FLOAT_EQ(tc.position.y, 0.0f);
    ASSERT_FLOAT_EQ(tc.rotation, 0.0f);
    ASSERT_FLOAT_EQ(tc.scale.x, 1.0f);
    ASSERT_FLOAT_EQ(tc.scale.y, 1.0f);
}

TEST(TransformComponentTest, ParameterizedConstructor) {
    TransformComponent tc({10.0f, 20.0f}, 45.0f, {2.0f, 2.0f});
    ASSERT_FLOAT_EQ(tc.position.x, 10.0f);
    ASSERT_FLOAT_EQ(tc.position.y, 20.0f);
    ASSERT_FLOAT_EQ(tc.rotation, 45.0f);
    ASSERT_FLOAT_EQ(tc.scale.x, 2.0f);
    ASSERT_FLOAT_EQ(tc.scale.y, 2.0f);
}

// PlayerComponent Tests
TEST(PlayerComponentTest, DefaultConstructor) {
    PlayerComponent pc;
    ASSERT_TRUE(pc.is_active);
    ASSERT_EQ(pc.player_id, 0);
}

TEST(PlayerComponentTest, ParameterizedConstructor) {
    PlayerComponent pc(1, false);
    ASSERT_FALSE(pc.is_active);
    ASSERT_EQ(pc.player_id, 1);
}
