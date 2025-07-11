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

    SUBCASE("Insert too many triangles at the same position") {
        MockTriangle triangle(Eigen::Vector3d(1.0, 1.0, 1.0));
        octree.insert(&triangle);
        const OctreeNode<MockTriangle>* root = octree.getRoot();
        CHECK(root != nullptr);
        CHECK(root->getBoundingBox().contains(triangle.getPosition()));
        CHECK(root->data.size() == 1);
        CHECK(root->data.front() == &triangle);

        // Insert more triangles at the same position
        MockTriangle triangle2(Eigen::Vector3d(1.0, 1.0, 1.0));
        MockTriangle triangle3(Eigen::Vector3d(1.0, 1.0, 1.0));
        octree.insert(&triangle2);
        octree.insert(&triangle3);

        CHECK(root->data.size() == 3);
        CHECK(root->data.front() == &triangle);
        CHECK(root->data.back() == &triangle3);

        // Check that an error is thrown when trying to insert more than max_neighbors triangles at the same position
        MockTriangle triangle4(Eigen::Vector3d(1.0, 1.0, 1.0));
        CHECK_THROWS_AS(octree.insert(&triangle4), std::length_error);
    }
}

TEST_CASE("[Octree] testing ray tracing collision") {
    unsigned int max_depth = 5; // Maximum depth of the octree
    double initial_size = 2.0; // Initial size of the octree's root node (length of one side of the cube)
    unsigned int max_neighbors = 3; // Maximum number of neighbors in each octree leaf
    const Eigen::Vector3d& root_postion = Eigen::Vector3d::Zero(); // Position of the root node in 3D space

    Octree<Triangle> octree(max_depth, initial_size, max_neighbors, root_postion);

    SUBCASE("Trace ray through empty octree") {
        Eigen::Vector3d origin(0.0, 0.0, 0.0);
        Eigen::Vector3d direction(1.0, 1.0, 1.0);
        Ray ray(origin, direction);
        double max_distance = 10.0;

        Triangle* hit_triangle = octree.traceRay(ray, max_distance);
        CHECK(hit_triangle == nullptr);
    }

    // Create some triangles to insert into the octree
    Eigen::Vector3d p1(1.0, 0.0, 0.0);
    Eigen::Vector3d p2(0.0, 1.0, 0.0);
    Eigen::Vector3d p3(0.0, 0.0, 1.0);
    Eigen::Vector3d p4(1.0, 1.0, 1.0);

    Triangle triangle1(p1, p2, p3);
    Triangle triangle2(p1, p2, p4);
    Triangle triangle3(p1, p3, p4);
    Triangle triangle4(p2, p3, p4);

    octree.insert(&triangle1);
    octree.insert(&triangle2);
    octree.insert(&triangle3);

    SUBCASE("Trace ray from inside octree") {
        Eigen::Vector3d origin(0.5, 0.5, 0.5);
        Eigen::Vector3d direction(1.0, 1.0, 1.0);
        Ray ray(origin, direction);
        double max_distance = 10.0;

        Triangle* hit_triangle = octree.traceRay(ray, max_distance);
        CHECK(hit_triangle != nullptr);
        CHECK(hit_triangle->getPosition().isApprox(triangle1.getPosition(), 1e-6));
    }

    SUBCASE("Trace ray that does not hit any triangles because it is outside the bounding box") {
        Eigen::Vector3d origin(10.0, 10.0, 10.0);
        Eigen::Vector3d direction(-1.0, -1.0, -1.0);
        Ray ray(origin, direction);
        double max_distance = 10.0;

        Triangle* hit_triangle = octree.traceRay(ray, max_distance);
        CHECK(hit_triangle == nullptr);
    }
    
    Eigen::Vector3d origin(-3, -3, -3); // Origin outside the bounding box of the octree

    SUBCASE("Trace ray that does not hit any triangles because of its direction") {
        Eigen::Vector3d direction(1.0, -1.0, 1.0);
        Ray ray(origin, direction);
        double max_distance = 10.0;

        Triangle* hit_triangle = octree.traceRay(ray, max_distance);
        CHECK(hit_triangle == nullptr);
    }

    SUBCASE("Trace ray through existing triangle") {
        Eigen::Vector3d direction(1.0, 1.0, 1.0);
        Ray ray(origin, direction);
        double max_distance = 10.0;

        Triangle* hit_triangle = octree.traceRay(ray, max_distance);
        CHECK(hit_triangle != nullptr);
        CHECK(hit_triangle->getPosition().isApprox(triangle1.getPosition(), 1e-6));
    }

    octree.insert(&triangle4);

    SUBCASE("Trace ray that hits multiple triangles") {
        Eigen::Vector3d direction(1.0, 1.0, 1.0);
        Ray ray(origin, direction);
        double max_distance = 10.0;

        Triangle* hit_triangle = octree.traceRay(ray, max_distance);
        CHECK(hit_triangle != nullptr);
        CHECK(hit_triangle->getPosition().isApprox(triangle1.getPosition(), 1e-6));
    }

    SUBCASE("Trace ray too short to hit any triangles") {
        Eigen::Vector3d direction(1.0, 1.0, 1.0);
        Ray ray(origin, direction);
        double max_distance = 0.5; // Too short to hit any triangle

        Triangle* hit_triangle = octree.traceRay(ray, max_distance);
        CHECK(hit_triangle == nullptr);
    }

    Eigen::Vector3d pp1(1.0, 0.0, 0.0);
    Eigen::Vector3d pp2(-1.0, 0.0, 0.0);
    Eigen::Vector3d pp3(0.0, 0.0, 1.0);

    Triangle triangle11(pp1, pp2, pp3);
    triangle11.translate(Eigen::Vector3d(0.0, 30, -0.5)); // Move triangle1 outside the bounding box

    octree.insert(&triangle11);

    SUBCASE("Trace ray that hits a triangle multiple nodes away") {
        Eigen::Vector3d origin(0.0, 3.0, 0.0); // Start outside the bounding box
        Eigen::Vector3d direction(0.0, 1.0, 0.0); // Direction towards the triangles
        Ray ray(origin, direction);
        double max_distance = 30.0;

        Triangle* hit_triangle = octree.traceRay(ray, max_distance);
        CHECK(hit_triangle != nullptr);
        CHECK(hit_triangle == &triangle11);
        CHECK(hit_triangle->getPosition().isApprox(triangle11.getPosition()));
    }
}