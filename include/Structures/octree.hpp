#pragma once

#include <list>
#include <Eigen/Dense>
#include <concepts>
#include <vector>
#include <iostream>
#include <string>

#include <Windows.h>
#include <cstdio>

#include "Structures/box.hpp"
#include "Structures/plane.hpp"

// Concept OctreeAcceptatble: type 'T' has 
//  `.getPosition` and its return is convertible to Vector3d.
//  `.intersect` and its return is convertible to bool.
template<typename T>
concept OctreeAcceptatble = requires(T a, Ray& ray, float& u, float& v, float& t) {
    { a.getPosition() } -> std::convertible_to<Eigen::Vector3d>;
    { a.intersect(ray, u, v, t) } -> std::convertible_to<bool>;
};

template <OctreeAcceptatble T>
class OctreeNode {
    public:
        Eigen::Vector3d position; // Coordinates of the node
        double size; // Size of the node (length of one side of the cube in the octree)
        unsigned int depth;  // Level in the octree hierarchy
        unsigned int total_children_depth; // Total depth of all children nodes

        OctreeNode* children[8]; // Pointers to the child nodes
        std::list<const T*> data; // List of pointers to the data associated with the node

        /// @brief Constructor for OctreeNode
        /// @param position The position of the node in 3D space
        /// @param size The size of the node (length of one side of the bounding cube)
        /// @param depth The depth of the node in the octree hierarchy
        /// @param total_children_depth The total depth of all children nodes
        /// @note The position is the center of the node, and the size is the length of one side of the cube.
        /// @note The node is initialized with a bounding box that is an Axis-Aligned cube centered at the position.
        OctreeNode(const Eigen::Vector3d& position, double size, unsigned int depth, unsigned int total_children_depth) : 
                position(position), 
                size(size), 
                depth(depth),
                total_children_depth(total_children_depth),
                m_half_size(size / 2),
                m_plane_xy(Eigen::Vector3d::UnitZ(), position), // Plane parallel to XY plane
                m_plane_xz(Eigen::Vector3d::UnitY(), position), // Plane parallel to XZ plane
                m_plane_yz(Eigen::Vector3d::UnitX(), position) // Plane parallel to YZ plane
        {
            assert(size > 0 && "Size of the octree node must be greater than zero.");

            // Initialize the bounding box based on position and size
            m_bounding_box = {position.array() - Eigen::Array3d(m_half_size, m_half_size, m_half_size), 
                            position.array() + Eigen::Array3d(m_half_size, m_half_size, m_half_size)};
            
            // Initialize child pointers to nullptr
            for (int i = 0; i < 8; ++i) {
                children[i] = nullptr;
            }

            m_plane_indices.reserve(3); // Reserve space for plane indices
        };

        /// @brief Destructor for OctreeNode
        ~OctreeNode() {
            // Clean up child nodes
            for (int i = 0; i < 8; ++i) {
                delete children[i];
            }
        };

        /// @brief Get the bounding box of the octree node.
        /// @return The bounding box of the node, which is an Axis-Aligned cube defined by its position and size.
        inline const Box& getBoundingBox() const {
            // Calculate the bounding box based on position and size
            return m_bounding_box;
        };

        /// @brief Get the half size of the octree node.
        /// @return The half size of the node, which is half the length of one side of the cube.
        inline double getHalfSize() const {
            return m_half_size;
        };

        /// @brief Trace a ray through the octree node and detect the first object hit by the ray.
        /// @note This method uses Sorted Sibling Traversal to efficiently traverse the octree.
        /// @param ray The ray to trace through the octree
        /// @param closest_collision_distance Reference to a float that will hold the distance to the first hit object
        /// @return A pointer to the first object hit by the ray, or nullptr if no object is hit
        const T* traceRay(const Ray& ray, double& closest_collision_distance);

        /// @brief Recursively print the structure of the octree node to the console.
        /// @param prefix The prefix string to print before the node's information
        /// @param isLast Indicates if this node is the last child in its parent's list of children
        /// @param postfix The postfix string to print after the node's information
        /// @note This method prints the node's position, size, and the number of triangles it contains.
        /// @note The method also prints the direction of each child node in its parent's bounding box.
        /// @note It is necessary to enable UTF-8 support in your console to see the tree structure correctly.
        void print(const std::string& prefix, bool isLast, const std::string& postfix) const {
            // Inspired by Adrian Schneider's answer at:
            //      https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c

            // Print the prefix and the links to the hierarchy
            std::cout << prefix;
            std::cout << (isLast ? "└──" : "├──");

            // Print the position and size of the node
            std::cout << postfix << ": [" << position.transpose() << "] +- " << m_half_size << " \t >> ";

            // print the value of the node
            std::cout << data.size() << " triangles" << std::endl;

            std::string direction[8] = {
                "Right-Bottom-Back", "Right-Bottom-Front", "Right-Top-Back", "Right-Top-Front",
                "Left-Bottom-Back", "Left-Bottom-Front", "Left-Top-Back", "Left-Top-Front"
            };
            if (total_children_depth > 0) {
                for (unsigned int i = 0; i < 8; ++i) {
                    children[i]->print(prefix + (isLast ? "    " : "│   "), i == 7, direction[i]);
                }
            }
        };

    private:
        Box m_bounding_box; // Bounding box of the node
        double m_half_size; // Half the size of the node, used for bounding box calculations

        // Planes for intersection tests
        // These planes are centered on the node's position and aligned with the axes
        Plane m_plane_xy, m_plane_xz, m_plane_yz; 

        // Indices of the planes hit by a ray
        std::vector<unsigned char> m_plane_indices;
};

template <OctreeAcceptatble T>
class Octree {
    typedef OctreeNode<T> Node;

    public:
        /// @brief Constructor for Octree
        /// @param max_depth Maximum depth of the octree
        /// @param initial_size Initial size of the octree's root node (length of one side of the cube)
        /// @param max_neighbors Maximum number of neighbors in each octree leaf
        /// @param root_postion Position of the root node in 3D space
        /// @note The root node's bounding box is initialized to a cube centered at `root_position` with a size of `min_size`
        Octree(unsigned int max_depth, double initial_size, unsigned int max_neighbors, const Eigen::Vector3d& root_postion);

        /// @brief Destructor for Octree
        ~Octree() {
            clear(); // Clear the octree to free memory
        };

        /// @brief Inserts a single object into the octree.
        /// @param data Pointer to the object to be inserted into the octree
        /// @param verbose If true, prints debug information during insertion
        /// @note The object must implement the `getPosition` method returning a 3D vector and the `intersect` method for ray intersection tests.
        void insert(const T* data, bool verbose = false);

        /// @brief Uses Sorted Sibling Traversal to trace a ray through the octree and detect the first object hit by the ray.
        /// @param ray The ray to trace through the octree
        /// @param hit_distance Reference to a double that will hold the distance to the first hit object
        /// @param max_distance Maximum distance to trace the ray (default is infinity)
        /// @return A pointer to the first object hit by the ray, or nullptr if no object is hit
        const T* traceRay(const Ray& ray, double& hit_distance, double max_distance = std::numeric_limits<double>::infinity()) const;

        /// @brief Clears the octree, deleting all nodes and freeing memory.
        void clear();

        /// @brief Prints the structure of the octree to the console.
        /// @note This method prints the octree in a tree-like format, showing the hierarchy of nodes and their positions, sizes and data.
        void print() const;

        /// @brief Returns the root node of the octree.
        /// @return A pointer to the root node of the octree
        inline const Node* getRoot() const {
            return m_root; // Return the root node of the octree
        };
        
    private:
        const unsigned int m_max_depth; // Maximum level of the octree
        const unsigned int m_max_neighbors; // Maximum number of neighbors in each octree leaf

        Node* m_root; // Root node of the octree

        /// @brief Helper method to add child nodes to a given node, skipping a specified index.
        /// @param node Pointer to the node to which child nodes will be added to
        /// @param ignore_index Index of the child node to skip (default is 8, which means no child is skipped)
        /// @note The child nodes are created based on the position and size of the parent node
        inline void addChildrenToNode(Node* node, const unsigned char ignore_index = 8);
};

// Include the implementation file to make the template class implementation accessible to the compiler
#include "Structures/octree.tpp"