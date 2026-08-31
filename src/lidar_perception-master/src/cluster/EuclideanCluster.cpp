#include "EuclideanCluster.h"
namespace cluster{
euclidean_cluster::euclidean_cluster(const rclcpp::Node::SharedPtr & node)
{
  clusterTolerance_ = node->declare_parameter("clusterTolerance", 0.005);
  minClusterSize_ = node->declare_parameter("minClusterSize", 10);
  maxClusterSize_ = node->declare_parameter("maxClusterSize", 300);
  // std::cout << "clusterTolerance_ = " << clusterTolerance_ << std::endl;
}
void euclidean_cluster::cluster_vector(const VPointCloud::ConstPtr cloud, std::vector<pcl::PointIndices> &clusters)
{
  //设置查找方式－kdtree
  pcl::search::Search<VPoint>::Ptr tree(new pcl::search::KdTree<VPoint>);

  pcl::EuclideanClusterExtraction<VPoint> ec;
  ec.setClusterTolerance (clusterTolerance_);// 2cm
  ec.setMinClusterSize (minClusterSize_); //100
  ec.setMaxClusterSize (maxClusterSize_);
  ec.setSearchMethod (tree);
  ec.setInputCloud (cloud);
  ec.extract (clusters);
}
}
