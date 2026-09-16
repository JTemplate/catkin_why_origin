#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include "association.h"

TEST(
    SensorFusionAssociation,
    DoesNotReuseSameLidarForTwoRadars)
{
    const std::vector<AssociationObject> radar_objects{
        {1, 0.0, 0.0},
        {2, 0.4, 0.0},
    };

    const std::vector<AssociationObject> lidar_objects{
        {10, 0.2, 0.0},
    };

    const auto matches =
        AssociateRadarToLidar(
            radar_objects,
            lidar_objects,
            1.0);

    ASSERT_EQ(matches.size(), 2U);

    const auto lidar_10_use_count =
        std::count_if(
            matches.begin(),
            matches.end(),
            [](const AssociationMatch & match) {
                return match.lidar_id == 10;
            });

    EXPECT_LE(lidar_10_use_count, 1);
}

TEST(
    SensorFusionAssociation,
    UsesGlobalAssignmentInsteadOfRadarOrderGreedy)
{
    /*
     * Geometry:
     *
     *            L20
     *             |
     *             | 0.7 m
     *             |
     * L10 ---- R1
     *  |
     *  | 0.1 m
     *  |
     * R2
     *
     * Coordinates:
     *
     * L10 =  0.0
     * R1  =  0.2
     * L20 =  0.9
     * R2  = -0.1
     *
     * gate = 1.0 m
     *
     * Distances:
     *
     * R1 -> L10 = 0.2
     * R1 -> L20 = 0.7
     *
     * R2 -> L10 = 0.1
     * R2 -> L20 = 1.0  (outside strict gate)
     *
     * Radar-order greedy would do:
     *
     * R1 -> L10
     * R2 -> unmatched
     *
     * Correct global assignment:
     *
     * R1 -> L20
     * R2 -> L10
     */
    const std::vector<AssociationObject>
        radar_objects{
            {1, 0.2, 0.0},
            {2, -0.1, 0.0},
        };

    const std::vector<AssociationObject>
        lidar_objects{
            {10, 0.0, 0.0},
            {20, 0.9, 0.0},
        };

    const auto matches =
        AssociateRadarToLidar(
            radar_objects,
            lidar_objects,
            1.0);

    ASSERT_EQ(matches.size(), 2U);

    const auto radar_1 =
        std::find_if(
            matches.begin(),
            matches.end(),
            [](const AssociationMatch & match) {
                return match.radar_id == 1;
            });

    const auto radar_2 =
        std::find_if(
            matches.begin(),
            matches.end(),
            [](const AssociationMatch & match) {
                return match.radar_id == 2;
            });

    ASSERT_NE(radar_1, matches.end());
    ASSERT_NE(radar_2, matches.end());

    EXPECT_EQ(radar_1->lidar_id, 20);
    EXPECT_EQ(radar_2->lidar_id, 10);

    EXPECT_NEAR(
        radar_1->distance,
        0.7,
        1e-12);

    EXPECT_NEAR(
        radar_2->distance,
        0.1,
        1e-12);
}
