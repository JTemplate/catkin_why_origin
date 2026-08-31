#ifndef EUCLIDEAN_CLUSTER_H_
#define EUCLIDEAN_CLUSTER_H_
#include <iostream>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/search/search.h>
#include <pcl/search/kdtree.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/segmentation/extract_clusters.h>
#include <rclcpp/rclcpp.hpp>
#include "types.h"

namespace cluster{
class euclidean_cluster{
    public:
    explicit euclidean_cluster(const rclcpp::Node::SharedPtr & node);
    ~euclidean_cluster() {};
    void cluster_vector(const VPointCloud::ConstPtr cloud, std::vector<pcl::PointIndices> &clusters);

    private:
    double clusterTolerance_;
    int minClusterSize_;
    int maxClusterSize_;
};
}


#endif
