//
// Created by will on 6/21/16.
//

#ifndef PROJECT_DETECTSURFACES_HPP
#define PROJECT_DETECTSURFACES_HPP

#include <pcl/search/search.h>

// Forward declarations
namespace pcl {
struct PointXYZ;
struct PointNormal;
struct PointXYZRGB;
class PointIndices;
class ModelCoefficients;
namespace search {
template <typename PointT>
class Search;
}
}

class SurfaceVisualizationController;

class Surface;

class DetectSurfaces {
    typedef pcl::PointXYZ Point;
    typedef pcl::PointCloud<Point> PointCloud;

    typedef pcl::PointXYZRGB ColoredPoint;
    typedef pcl::PointCloud<ColoredPoint> ColoredPointCloud;

    typedef pcl::PointNormal Normal;
    typedef pcl::PointCloud<Normal> NormalCloud;
    typedef pcl::search::Search<Normal> NormalSearch;

public:
    DetectSurfaces(double perpendicular_dist, double parallel_dist, double mls_radius,
                   unsigned int min_points_per_surface, double min_plane_width);

    void detect_surfaces(const PointCloud &input, const std::vector<int> &indices,
                             std::vector<int> &new_labels, const SurfaceVisualizationController &v,
                             std::function<void(const Surface &)> callback);

private:
    std::vector<int> radius_filter(const PointCloud &cloud, const std::vector<int> &indices);

    NormalCloud::Ptr get_normals(const PointCloud &cloud, const std::vector<int> &indices);

    unsigned long region_segmentation(NormalCloud::Ptr &normals, NormalSearch::Ptr &search,
                                      std::function<void(pcl::PointIndices)> callback);

    pcl::PointIndices sac_segmentation_and_fit(NormalCloud::Ptr &normals, NormalSearch::Ptr &search,
                                               pcl::PointIndices &region,
                                               std::function<void(pcl::PointIndices, pcl::ModelCoefficients)> callback);

    void euclidean_segmentation(NormalCloud::Ptr &normals, NormalSearch::Ptr &search, pcl::PointIndices &inliers,
                                std::function<void(pcl::PointIndices)> callback);

    void find_transform_and_filter(NormalCloud::Ptr &normals, pcl::PointIndices &inliers,
                                   std::function<void(Eigen::Affine3f)> callback);

    ColoredPointCloud::Ptr make_segment_colored_cloud(NormalCloud::Ptr &normals,
                                                      std::vector<pcl::PointIndices> &segments);

protected:
    double perpendicular_distance_;
    double parallel_distance_;
    double mls_radius_;
    unsigned int min_points_per_surface_;
    double min_plane_width_;
};

#endif // PROJECT_DETECTSURFACES_HPP
