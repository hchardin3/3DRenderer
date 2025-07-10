#include "octree-test.hpp"

TEST_CASE("[Octree] testing octree insertion") {
    unsigned int max_depth = 5; // Maximum depth of the octree
    double initial_size = 2.0; // Initial size of the octree's root node (length of one side of the cube)
    unsigned int max_neighbors = 3; // Maximum number of neighbors in each octree leaf
    const Eigen::Vector3d& root_postion = Eigen::Vector3d::Zero(); // Position of the root node in 3D space

    Octree<MockTriangle> octree(max_depth, initial_size, max_neighbors, root_postion);

    SUBCASE("Insert a triangle into the octree") {
        MockTriangle triangle(Eigen::Vector3d(1.0, 1.0, 1.0));
        octree.insert(&triangle);
        const OctreeNode<MockTriangle>* root = octree.getRoot();
        CHECK(root != nullptr);
        CHECK(root->getBoundingBox().contains(triangle.getPosition()));
        CHECK(root->data.size() == 1);
        CHECK(root->data.front() == &triangle);
        CHECK(root->depth == 0);

        // std::cout << std::endl;
        // std::cout << "Octree root position: " << root->position.transpose() << std::endl;
        // std::cout << "Octree root size: " << root->size << std::endl;
        // std::cout << "Octree root depth: " << root->depth << std::endl;
        // std::cout << "Octree total children depth: " << root->total_children_depth << std::endl;
        // std::cout << std::endl;

        SUBCASE("Insert 3 triangles into the octree, within bounds") {
            MockTriangle triangle2(Eigen::Vector3d(1, 1, 1));
            MockTriangle triangle3(Eigen::Vector3d(0.5, 0.5, 0.5));
            octree.insert(&triangle2);
            octree.insert(&triangle3);

            CHECK(root->data.size() == 3);
            CHECK(root->data.front() == &triangle);
            CHECK(root->data.back() == &triangle3);
            
            // std::cout << std::endl;
            // std::cout << "After inserting multiple triangles:" << std::endl;
            // octree.print();

            SUBCASE("Insert one more triangle within bounds, exceeding max neighbors") {
                MockTriangle triangle4(Eigen::Vector3d(0.2, -0.8, -0.3));
                octree.insert(&triangle4);

                // Check that the root node is not a leaf anymore
                CHECK(root->total_children_depth > 0);
                CHECK(root->depth == 0);
                CHECK(root->data.size() == 0);

                // Check that the root node has children
                int total_data = 0;
                for (int i = 0; i < 8; ++i) {
                    CHECK(root->children[i] != nullptr);
                    CHECK(root->children[i]->depth == 1);
                    CHECK(root->children[i]->total_children_depth == 0);
                    CHECK(root->children[i]->data.size() <= max_neighbors);
                    total_data += root->children[i]->data.size();
                }
                CHECK(total_data == 4); // We inserted 4 triangles, so total data in children should be 4
                
                bool found[4] = {false, false, false, false};
                for (int i = 0; i < 8; ++i) {
                    for (const auto& data : root->children[i]->data) {
                        if (data == &triangle) found[0] = true;
                        else if (data == &triangle2) found[1] = true;
                        else if (data == &triangle3) found[2] = true;
                        else if (data == &triangle4) found[3] = true;
                    }
                }
                CHECK(found[0] == true);
                CHECK(found[1] == true);
                CHECK(found[2] == true);
                CHECK(found[3] == true);
            }

            SUBCASE("Insert one more triangle outside the bounding box") {
                MockTriangle triangle5(Eigen::Vector3d(2.5, 2.5, 2.5));
                octree.insert(&triangle5);

                root = octree.getRoot(); // Get the updated root after insertion

                // Check that the root node is not a leaf anymore
                CHECK(root->total_children_depth == 1);
                CHECK(root->depth == 0);
                CHECK(root->data.size() == 0);

                // Check that the root node has children
                int total_data = 0;
                for (int i = 0; i < 8; ++i) {
                    CHECK(root->children[i] != nullptr);
                    CHECK(root->children[i]->depth == 1);
                    CHECK(root->children[i]->total_children_depth == 0);
                    CHECK(root->children[i]->data.size() <= max_neighbors);
                    total_data += root->children[i]->data.size();
                }
                CHECK(total_data == 4); // We inserted 4 triangles, so total data in children should be 4

                bool found[4] = {false, false, false, false};
                for (int i = 0; i < 8; ++i) {
                    for (const auto& data : root->children[i]->data) {
                        if (data == &triangle) found[0] = true;
                        else if (data == &triangle2) found[1] = true;
                        else if (data == &triangle3) found[2] = true;
                        else if (data == &triangle5) found[3] = true;
                    }
                }
                CHECK(found[0] == true);
                CHECK(found[1] == true);
                CHECK(found[2] == true);
                CHECK(found[3] == true);
            }
        }
    }
}

