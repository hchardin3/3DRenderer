#include "Structures/octree.hpp"

#include <iostream>

template <PositionType T>
Octree<T>::Octree(unsigned int max_depth, double initial_size, unsigned int max_neighbors, const Eigen::Vector3d& root_postion) : 
        m_max_depth(max_depth),
        m_max_neighbors(max_neighbors)
{
    assert(initial_size > 0 && "Initial size of the octree must be greater than zero.");

    // Initialize the root node of the octree
    m_root = new Node(root_postion, initial_size, 0, 0);
}

/// @brief Get the branch index based on the position relative to the node's position
/// @param position The position to check
/// @param node_position The position of the node
/// @return A value between 0 and 7 (111) representing the octant in which the position lies
///         0: (-,-,-), 1: (-,-,+), 2: (-,+,-), 3: (-,+,+),
///         4: (+,-,-), 5: (+,-,+), 6: (+,+,-), 7: (+,+,+)
inline unsigned char getBranchIndex(const Eigen::Vector3d& position, const Eigen::Vector3d& node_position) {
    unsigned char branch_index = 0; // Value between 0 and 7 (111) representing the octant in which the position lies
    if (position.x() >= node_position.x()) branch_index |= 4;
    if (position.y() >= node_position.y()) branch_index |= 2;
    if (position.z() >= node_position.z()) branch_index |= 1;
    return branch_index;
}

template <PositionType T>
inline void Octree<T>::addChildrenToNode(Node* node, const unsigned char ignore_index) {
    // Create child nodes for the current node
    double new_half_size = node->getHalfSize() / 2;
    for (int i = 0; i < 8; ++i) {
        if (i == ignore_index) continue; // Skip the ignored index

        Eigen::Vector3d child_position = node->position + 
            (Eigen::Array3d((i & 4) ? 1 : -1, (i & 2) ? 1 : -1, (i & 1) ? 1 : -1) * 
            Eigen::Array3d(new_half_size, new_half_size, new_half_size)).matrix();
        
        node->children[i] = new Node(child_position, node->getHalfSize(), node->depth + 1, 0);
    }
}

template <PositionType T>
void Octree<T>::insert(const T* data, bool verbose) {
    Eigen::Vector3d position = data->getPosition(); // Get the position of the data to be inserted
    if (verbose) std::cout << "Inserting data at position: " << position.transpose() << std::endl;

    // If the position is outside the bounding box, expand the octree
    while (!m_root->getBoundingBox().contains(position) && m_root->total_children_depth < m_max_depth) {
        // Value between 0 and 7 (111) representing the octant in which the current root lies in the new root node
        unsigned char current_root_index = getBranchIndex(m_root->position, position);

        // Position of the new root node is the center of the current root node's bounding box
        Eigen::Vector3d new_root_position = m_root->position +
            (Eigen::Array3d((current_root_index & 4) ? -1 : 1, (current_root_index & 2) ? -1 : 1, (current_root_index & 1) ? -1 : 1) *
            Eigen::Array3d(m_root->getHalfSize(), m_root->getHalfSize(), m_root->getHalfSize())).matrix();

        // Create a new root node with double the size of the current root node
        Node* new_root = new Node(new_root_position, m_root->size * 2, 0, m_root->total_children_depth + 1);

        // Set the current root as a child of the new root
        new_root->children[current_root_index] = m_root;
        m_root->depth = 1; // Update the depth of the current root node to 1

        // Create the other 7 children of the new root node
        addChildrenToNode(new_root, current_root_index);

        // Update the root to the new root
        m_root = new_root;

        if (verbose) std::cout << "Expanded octree to new root at position: " << new_root_position.transpose() << " with size: " << m_root->size << " and bounding box: " << m_root->getBoundingBox().min.transpose() << ", " << m_root->getBoundingBox().max.transpose() << std::endl;
    }

    // Check if the position is within the bounding box of the root node
    if (!m_root->getBoundingBox().contains(position)) {
        // If the position is still outside the bounding box, we cannot insert it
        throw std::length_error("Cannot insert data: Position still outside the bounding box of the octree root after maximum depth (" + std::to_string(m_max_depth) + ") reached.");
        return;
    }

    if (verbose) std::cout << "Position is within the bounding box of the root node." << std::endl;

    // If the position is within the bounding box, insert the data into the octree
    unsigned char branch_index = 0; // Value between 0 and 7 (111) representing the octant in which the position lies
    Node* current_node = m_root;
    while (current_node->depth <= m_max_depth) {
        if (verbose) std::cout << "Current node position: " << current_node->position.transpose() << ", depth: " << current_node->depth << ", total children depth: " << current_node->total_children_depth << std::endl;
    // for (int current_depth = 0; current_depth < m_max_depth; ++current_depth) {
        // Check if the current node is a leaf node
        if (current_node->total_children_depth == 0) {
            if (verbose) std::cout << "Current node is a leaf node." << std::endl;
            // If the current node is a leaf, check if it can accommodate the new data
            if (current_node->data.size() < m_max_neighbors) {
                if (verbose) std::cout << "Current node has space for new data." << std::endl;
                // If there is space, add the data to the current node
                current_node->data.push_back(data);
                break; // Data inserted successfully
            } 
            
            // If the leaf is full, check if we can subdivide it
            else if (current_node->depth < m_max_depth) {
                if (verbose) std::cout << "Current node is full, subdividing..." << std::endl;
                // If the current node is full, we need to subdivide it
                // We will create child nodes and redistribute the existing data to the new children
                Node* current_subdivision = current_node;
                while(current_subdivision->data.size() >= m_max_neighbors && current_subdivision->depth < m_max_depth) {
                    if (verbose) std::cout << "Subdividing node at position: " << current_subdivision->position.transpose() << ", depth: " << current_subdivision->depth << " and size: " << current_subdivision->size << std::endl;
                    // Create child nodes
                    addChildrenToNode(current_subdivision);

                    // Redistribute existing data to the new children
                    for (const T* existing_data : current_subdivision->data) {
                        unsigned char index = getBranchIndex(existing_data->getPosition(), current_subdivision->position);
                        current_subdivision->children[index]->data.push_back(existing_data);
                    }

                    // Set the current node to not be a leaf anymore
                    current_subdivision->total_children_depth = 1; // Set the total children depth to 1 after subdivision
                    current_subdivision->data.clear(); // Clear the data in the leaf after redistribution

                    // Move to the child node where we will insert the new data
                    branch_index = getBranchIndex(position, current_subdivision->position);
                    current_subdivision = current_subdivision->children[branch_index];
                }

                // Now we can insert the new data into the appropriate child node if it is not full
                if (current_subdivision->data.size() < m_max_neighbors) {
                    if (verbose) std::cout << "Inserting data into subdivided node at position: " << current_subdivision->position.transpose() << ", depth: " << current_subdivision->depth << " and size: " << current_subdivision->size << std::endl;
                    current_subdivision->data.push_back(data);
                    break; // Data inserted successfully
                }
                else {
                    // If the child node is also full, we cannot insert the data
                    throw std::length_error("Cannot insert data: Child node is full after subdivision until maximum depth (" + std::to_string(m_max_depth) + ") reached.");
                    return;
                }
            }

            // If we cannot subdivide further, we cannot insert the data
            else {
                throw std::length_error("Cannot insert data: Maximum octree depth (" + std::to_string(m_max_depth) + ") reached during subdivision.");
                return;
            }
        }
        // If the current node is not a leaf, move to the appropriate child node
        else {
            if (verbose) std::cout << "Current node is not a leaf, moving to child node." << std::endl;
            // Determine the branch index based on the position relative to the current node's position
            branch_index = getBranchIndex(position, current_node->position);

            // If the child node does not exist, error: The child nodes should always exist on a non-leaf node
            if (current_node->children[branch_index] == nullptr) {
                throw std::runtime_error("Child node of a non-leaf node at depth " + std::to_string(current_node->depth) + "has not been created. This should not happen.");
                return;
            }

            // Move to the child node
            current_node = current_node->children[branch_index];
        }
    }
}

template <PositionType T>
void Octree<T>::remove(const Node& node) {
    // Remove logic for the octree node
}

template <PositionType T>
const std::list<const T*> Octree<T>::getNeighbors(const Eigen::Vector3d& position, const Box& bounding_box) const {
    std::list<const T*> neighbors;
    // Logic to find neighbors within the bounding box
    return neighbors;
}

template <PositionType T>
void Octree<T>::clear() {
    // Clear the octree
    if (m_root) {
        delete m_root; // Delete the root node, which will recursively delete all child nodes
        m_root = nullptr; // Set the root to nullptr after deletion
    }
}

template <PositionType T>
void Octree<T>::print() const {
    // Print the octree structure
}
