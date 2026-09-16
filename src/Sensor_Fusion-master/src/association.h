#pragma once

#include <cmath>
#include <limits>
#include <vector>

struct AssociationObject
{
    int id;
    double x;
    double y;
};

struct AssociationMatch
{
    int radar_id;
    int lidar_id;
    double distance;
};

inline std::vector<AssociationMatch> AssociateRadarToLidar(
    const std::vector<AssociationObject> & radar_objects,
    const std::vector<AssociationObject> & lidar_objects,
    double distance_threshold)
{
    std::vector<AssociationMatch> matches;
    matches.reserve(radar_objects.size());

    if (radar_objects.empty()) {
        return matches;
    }

    if (lidar_objects.empty()) {
        for (const auto & radar : radar_objects) {
            matches.push_back(
                {radar.id, -1, distance_threshold});
        }
        return matches;
    }

    const std::size_t radar_count =
        radar_objects.size();

    const std::size_t lidar_count =
        lidar_objects.size();

    /*
     * Add one dummy "unmatched" column per Radar.
     *
     * Therefore:
     *
     * rows    = Radar objects
     * columns = LiDAR objects + dummy unmatched slots
     *
     * This makes columns >= rows, which is convenient
     * for the rectangular Hungarian algorithm below.
     */
    const std::size_t column_count =
        lidar_count + radar_count;

    /*
     * We want lexicographic behaviour:
     *
     *   1. maximize number of valid matches
     *   2. among those solutions, minimize total distance
     *
     * Every valid edge has:
     *
     *   distance < distance_threshold
     *
     * So make one unmatched Radar more expensive than
     * any possible rearrangement of all valid distances.
     */
    const double unmatched_cost =
        (static_cast<double>(radar_count) + 1.0) *
        distance_threshold;

    const double forbidden_cost =
        (static_cast<double>(radar_count) + 1.0) *
        unmatched_cost;

    /*
     * Hungarian implementation below is 1-indexed.
     */
    std::vector<std::vector<double>> cost(
        radar_count + 1,
        std::vector<double>(
            column_count + 1,
            unmatched_cost));

    /*
     * Real Radar -> LiDAR costs.
     *
     * Outside the gate is forbidden.
     */
    for (std::size_t i = 1;
         i <= radar_count;
         ++i)
    {
        for (std::size_t j = 1;
             j <= lidar_count;
             ++j)
        {
            const double dx =
                lidar_objects[j - 1].x -
                radar_objects[i - 1].x;

            const double dy =
                lidar_objects[j - 1].y -
                radar_objects[i - 1].y;

            const double distance =
                std::hypot(dx, dy);

            if (distance < distance_threshold) {
                cost[i][j] = distance;
            } else {
                cost[i][j] = forbidden_cost;
            }
        }
    }

    /*
     * Rectangular Hungarian algorithm.
     *
     * p[j] = Radar row currently assigned
     *        to column j.
     */
    std::vector<double> u(
        radar_count + 1,
        0.0);

    std::vector<double> v(
        column_count + 1,
        0.0);

    std::vector<int> p(
        column_count + 1,
        0);

    std::vector<int> way(
        column_count + 1,
        0);

    for (std::size_t i = 1;
         i <= radar_count;
         ++i)
    {
        p[0] = static_cast<int>(i);

        std::size_t j0 = 0;

        std::vector<double> minv(
            column_count + 1,
            std::numeric_limits<double>::infinity());

        std::vector<bool> used(
            column_count + 1,
            false);

        do {
            used[j0] = true;

            const int i0 =
                p[j0];

            double delta =
                std::numeric_limits<double>::infinity();

            std::size_t j1 = 0;

            for (std::size_t j = 1;
                 j <= column_count;
                 ++j)
            {
                if (used[j]) {
                    continue;
                }

                const double cur =
                    cost[
                        static_cast<std::size_t>(i0)
                    ][j] -
                    u[
                        static_cast<std::size_t>(i0)
                    ] -
                    v[j];

                if (cur < minv[j]) {
                    minv[j] = cur;
                    way[j] =
                        static_cast<int>(j0);
                }

                if (minv[j] < delta) {
                    delta = minv[j];
                    j1 = j;
                }
            }

            for (std::size_t j = 0;
                 j <= column_count;
                 ++j)
            {
                if (used[j]) {
                    u[
                        static_cast<std::size_t>(
                            p[j])
                    ] += delta;

                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }

            j0 = j1;

        } while (p[j0] != 0);

        do {
            const std::size_t j1 =
                static_cast<std::size_t>(
                    way[j0]);

            p[j0] = p[j1];
            j0 = j1;

        } while (j0 != 0);
    }

    /*
     * Convert column -> row representation
     * into row -> column.
     */
    std::vector<int> assigned_column(
        radar_count + 1,
        -1);

    for (std::size_t j = 1;
         j <= column_count;
         ++j)
    {
        if (p[j] != 0) {
            assigned_column[
                static_cast<std::size_t>(
                    p[j])
            ] = static_cast<int>(j);
        }
    }

    /*
     * Convert Hungarian result to our public result.
     *
     * lidar_id == -1 means unmatched.
     */
    for (std::size_t i = 1;
         i <= radar_count;
         ++i)
    {
        const int column =
            assigned_column[i];

        if (column > 0 &&
            static_cast<std::size_t>(column) <=
                lidar_count)
        {
            const auto & radar =
                radar_objects[i - 1];

            const auto & lidar =
                lidar_objects[
                    static_cast<std::size_t>(
                        column) - 1];

            const double distance =
                std::hypot(
                    lidar.x - radar.x,
                    lidar.y - radar.y);

            if (distance < distance_threshold) {
                matches.push_back(
                    {
                        radar.id,
                        lidar.id,
                        distance
                    });

                continue;
            }
        }

        matches.push_back(
            {
                radar_objects[i - 1].id,
                -1,
                distance_threshold
            });
    }

    return matches;
}
