#pragma once
#include "core/hittable.h"
#include <vector>
#include <memory>

// KD-Tree node structure
struct KDNode {
    BoundingBox bbox;                                    // Bounding box for this node
    std::vector<std::shared_ptr<Hittable>> objects;    // Objects stored in leaf nodes
    std::unique_ptr<KDNode> left;                       // Left child
    std::unique_ptr<KDNode> right;                      // Right child
    int axis;                                           // Split axis (0=x, 1=y, 2=z)
    float split_pos;                                    // Position of the split
    bool is_leaf;                                       // Whether this is a leaf node
    
    KDNode();
};

// KD-Tree acceleration structure
class KDTree : public Hittable {
public:
    KDTree();
    ~KDTree() = default;
    
    // Build the kd-tree from a list of objects
    void build(const std::vector<std::shared_ptr<Hittable>>& objects);
    
    // Add a single object (rebuilds the tree)
    void add(std::shared_ptr<Hittable> object);
    
    // Clear all objects
    void clear();
    
    // Hittable interface
    bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const override;
    BoundingBox bounding_box() const override;
    
    // Statistics
    int get_node_count() const;
    int get_max_depth() const;
    
private:
    std::unique_ptr<KDNode> root;
    std::vector<std::shared_ptr<Hittable>> all_objects;
    
    // Construction parameters
    static const int MAX_DEPTH = 20;
    static const int MIN_OBJECTS = 4;
    
    // Internal build method
    std::unique_ptr<KDNode> build_recursive(
        std::vector<std::shared_ptr<Hittable>>& objects, 
        const BoundingBox& bbox, 
        int depth
    );
    
    // Ray traversal
    bool hit_node(
        const KDNode* node, 
        const Ray& ray, 
        float t_min, 
        float t_max, 
        HitRecord& rec
    ) const;
    
    // Utility methods
    float find_split_position(const std::vector<std::shared_ptr<Hittable>>& objects, int axis) const;
    void partition_objects(
        std::vector<std::shared_ptr<Hittable>>& objects,
        int axis,
        float split_pos,
        std::vector<std::shared_ptr<Hittable>>& left_objects,
        std::vector<std::shared_ptr<Hittable>>& right_objects
    ) const;
    
    // Statistics helpers
    int count_nodes(const KDNode* node) const;
    int calculate_max_depth(const KDNode* node, int current_depth) const;
};