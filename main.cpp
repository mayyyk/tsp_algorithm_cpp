
#include "TSP.hxx"

#include <iostream>


int main() {
    // cost_matrix_t cm = {{INF, 10, 8,   19, 12}, // INF cost when trying to travel to current city, matrix is symetrical since cost of travel is the same both ways
    //                   {10, INF, 20,  6,  3}, // travel from city 1
    //                   {8,   20, INF, 4,  2}, // travel from city 2
    //                   {19,  6,  4, INF,  7}, // etc...
    //                   {12,  3,  2,   7, INF}};
 

   cost_matrix_t cm = {
           {INF, 12,   3,  45,   6},
           {78, INF,  90,  21,   3},
           { 5,  56, INF,  23,  98},
           {12,   6,   8, INF,  34},
           { 3,  98,   3,   2, INF}
   };


//    cost_matrix_t cm {
//            {INF,  3,  4,  2,  7},
//            {3,  INF,  4,  6,  3},
//            {4,  4,  INF,  5,  8},
//            {2,  6,  5,  INF,  6},
//            {7,  3,  8,  6,  INF},
//    }; 

    tsp_solutions_t solutions = solve_tsp(cm);


    for (auto elem : solutions) {
        std::cout << elem.lower_bound << " : ";
        for (auto pelem : elem.path) {
            std::cout << pelem << " ";
        }
        std::cout << "\n";
    }

    return EXIT_SUCCESS;
}

/*
// Uncomment to run unit tests and comment the code above 

#include "gtest/gtest.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

*/
