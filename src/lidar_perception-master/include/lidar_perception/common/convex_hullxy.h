/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0.
 *****************************************************************************/

#ifndef MODULES_PERCEPTION_OBSTACLE_COMMON_CONVEX_HULLXY_H_
#define MODULES_PERCEPTION_OBSTACLE_COMMON_CONVEX_HULLXY_H_

#include <vector>

#include <pcl/surface/convex_hull.h>

namespace apollo {
namespace perception {

// PCL 1.12 uses the re-entrant Qhull API internally.  Keep the historical
// Apollo wrapper name while delegating to PCL's supported public interface.
template <typename PointInT>
class ConvexHull2DXY : public pcl::ConvexHull<PointInT> {
public:
  using PointCloud = pcl::PointCloud<PointInT>;
  using PointCloudPtr = typename PointCloud::Ptr;

  void Reconstruct2dxy(
      const PointCloudPtr & hull, std::vector<pcl::Vertices> * polygons) {
    if (!hull || !polygons) {
      return;
    }
    this->setDimension(2);
    this->reconstruct(*hull, *polygons);
  }
};

}  // namespace perception
}  // namespace apollo

#endif  // MODULES_PERCEPTION_OBSTACLE_COMMON_CONVEX_HULLXY_H_
