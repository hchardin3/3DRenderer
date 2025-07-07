#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <iostream>
#include <Eigen/Dense>

#include "octree-test.hpp"

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);

    context.setOption("no-breaks", true); // Disable breaks to avoid stopping on first failure

    // overrides
    int res = context.run(); // run

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    context.clearFilters(); // removes all filters added up to this point

    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code
    
    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}