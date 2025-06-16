#include "core/kdtree.h"
#include "geometry/bounding_box.h"
#include "math/ray.h"
#include <algorithm>
#include <iostream>

// KDNode implementation
KDNode::KDNode() : axis(0), split_pos(0.0f), is_leaf(true) {}

// KDTree implementation
KDTree::KDTree() : root(nullptr) {}

void KDTree::build(const std::vector<std::shared_ptr<Hittable>>& objects) {
    if (objects.empty()) {
        root.reset();
        all_objects.clear();
        return;
    }
    
    // Store all objects for future reference
    all_objects = objects;
    
    // Calculate overall bounding box
    BoundingBox overall_bbox = objects[0]->bounding_box();
    for (size_t i = 1; i < objects.size(); ++i) {
        overall_bbox = surrounding_box(overall_bbox, objects[i]->bounding_box());
    }
    
    // Make a copy of objects for partitioning
    auto objects_copy = objects;
    
    // Build the tree recursively
    root = build_recursive(objects_copy, overall_bbox, 0);
    
    std::cerr << "KD-Tree built with " << get_node_count() << " nodes, max depth: " 
              << get_max_depth() << ", " << objects.size() << " objects\n";
}

std::unique_ptr<KDNode> KDTree::build_recursive(
    std::vector<std::shared_ptr<Hittable>>& objects, 
    const BoundingBox& bbox, 
    int depth) {
    
    auto node = std::make_unique<KDNode>();
    node->bbox = bbox;
    
    // Check for leaf node conditions
    if (objects.size() <= MIN_OBJECTS || depth >= MAX_DEPTH) {
        node->objects = objects;
        node->is_leaf = true;
        return node;
    }
    
    // Choose split axis and position
    int axis = depth % 3; 
    float split_pos = find_split_position(objects, axis);
    
    node->axis = axis;
    node->split_pos = split_pos;
    node->is_leaf = false;
    
    // Partition objects
    std::vector<std::shared_ptr<Hittable>> left_objects, right_objects;
    partition_objects(objects, axis, split_pos, left_objects, right_objects);
    
    // Handle edge cases where all objects end up on one side
    if (left_objects.empty() || right_objects.empty()) {
        node->objects = objects;
        node->is_leaf = true;
        return node;
    }
    
    // Create child bounding boxes
    BoundingBox left_bbox = bbox;
    BoundingBox right_bbox = bbox;
    
    if (axis == 0) {
        left_bbox.max.x = split_pos;
        right_bbox.min.x = split_pos;
    } else if (axis == 1) {
        left_bbox.max.y = split_pos;
        right_bbox.min.y = split_pos;
    } else {
        left_bbox.max.z = split_pos;
        right_bbox.min.z = split_pos;
    }
    
    // Recursively build children
    node->left = build_recursive(left_objects, left_bbox, depth + 1);
    node->right = build_recursive(right_objects, right_bbox, depth + 1);
    
    return node;
}

float KDTree::find_split_position(const std::vector<std::shared_ptr<Hittable>>& objects, int axis) const {
    // Find median centroid position along the chosen axis
    std::vector<float> positions;
    positions.reserve(objects.size());
    
    for (const auto& obj : objects) {
        BoundingBox bbox = obj->bounding_box();
        Point3 centroid = bbox.center();
        
        if (axis == 0) {
            positions.push_back(centroid.x);
        } else if (axis == 1) {
            positions.push_back(centroid.y);
        } else {
            positions.push_back(centroid.z);
        }
    }
    
    std::sort(positions.begin(), positions.end());
    return positions[positions.size() / 2];
}

void KDTree::partition_objects(
    std::vector<std::shared_ptr<Hittable>>& objects,
    int axis,
    float split_pos,
    std::vector<std::shared_ptr<Hittable>>& left_objects,
    std::vector<std::shared_ptr<Hittable>>& right_objects) const {
    
    for (const auto& obj : objects) {
        BoundingBox bbox = obj->bounding_box();
        Point3 centroid = bbox.center();
        
        float centroid_pos;
        if (axis == 0) {
            centroid_pos = centroid.x;
        } else if (axis == 1) {
            centroid_pos = centroid.y;
        } else {
            centroid_pos = centroid.z;
        }
        
        if (centroid_pos < split_pos) {
            left_objects.push_back(obj);
        } else {
            right_objects.push_back(obj);
        }
    }
}

bool KDTree::hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const {
    if (!root) {
        return false;
    }
    return hit_node(root.get(), ray, t_min, t_max, rec);
}

bool KDTree::hit_node(const KDNode* node, const Ray& ray, float t_min, float t_max, HitRecord& rec) const {
    // Quick bounding box test
    if (!node->bbox.hit(ray, t_min, t_max)) {
        return false;
    }
    
    if (node->is_leaf) {
        // Test all objects in this leaf
        bool hit_anything = false;
        float closest_so_far = t_max;
        
        for (const auto& object : node->objects) {
            HitRecord temp_rec;
            if (object->hit(ray, t_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
    
    // Interior node - test children
    bool hit_left = false, hit_right = false;
    HitRecord left_rec, right_rec;
    
    if (node->left) {
        hit_left = hit_node(node->left.get(), ray, t_min, t_max, left_rec);
    }
    if (node->right) {
        hit_right = hit_node(node->right.get(), ray, t_min, t_max, right_rec);
    }
    
    // Return the closest hit
    if (hit_left && hit_right) {
        rec = (left_rec.t < right_rec.t) ? left_rec : right_rec;
        return true;
    } else if (hit_left) {
        rec = left_rec;
        return true;
    } else if (hit_right) {
        rec = right_rec;
        return true;
    }
    
    return false;
}

BoundingBox KDTree::bounding_box() const {
    if (!root) {
        return BoundingBox();
    }
    return root->bbox;
}

void KDTree::add(std::shared_ptr<Hittable> object) {
    all_objects.push_back(object);
    build(all_objects);  // Rebuild the entire tree
}

void KDTree::clear() {
    root.reset();
    all_objects.clear();
}

int KDTree::get_node_count() const {
    return count_nodes(root.get());
}

int KDTree::get_max_depth() const {
    return calculate_max_depth(root.get(), 0);
}

int KDTree::count_nodes(const KDNode* node) const {
    if (!node) return 0;
    return 1 + count_nodes(node->left.get()) + count_nodes(node->right.get());
}

int KDTree::calculate_max_depth(const KDNode* node, int current_depth) const {
    if (!node) return current_depth;
    if (node->is_leaf) return current_depth + 1;
    
    int left_depth = calculate_max_depth(node->left.get(), current_depth + 1);
    int right_depth = calculate_max_depth(node->right.get(), current_depth + 1);
    
    return std::max(left_depth, right_depth);
}