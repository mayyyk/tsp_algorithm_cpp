#include <gtest/gtest.h>

#include "TSP.hxx"

class CostMatrixTest : public ::testing::Test {
protected:
    cost_matrix_t cm_raw = {
            {INF, 10,  8,   19,  12},
            {10,  INF, 20,  6,   3},
            {8,   20,  INF, 4,   2},
            {19,  6,   4,   INF, 7},
            {12,  3,   2,   7,   INF}
    };
    CostMatrix cm{cm_raw};
};

TEST_F(CostMatrixTest, get_min_values_in_rows) {
    std::vector<cost_t> expected_mins = {8, 3, 2, 4, 2};
    EXPECT_EQ(cm.get_min_values_in_rows(), expected_mins);
}

TEST_F(CostMatrixTest, reduce_rows) {
    EXPECT_EQ(cm.reduce_rows(), 19);
}

TEST_F(CostMatrixTest, get_min_values_in_cols) {
    std::vector<cost_t> expected_mins = {8, 3, 2, 4, 2};
    EXPECT_EQ(cm.get_min_values_in_cols(), expected_mins);
}

TEST_F(CostMatrixTest, reduce_cols) {
    cm.reduce_rows();
    EXPECT_EQ(cm.reduce_cols(), 9);
}

TEST_F(CostMatrixTest, get_vertex_cost) {
    cm.reduce_rows();
    cm.reduce_cols();
    // After row and column reduction, the cost of vertex (0, 2) which is 0 should be calculated.
    // min in row 0 (excluding col 2) is 1.
    // min in col 2 (excluding row 0) is 0.
    // Total cost is 1 + 0 = 1.
    EXPECT_EQ(cm.get_vertex_cost(0, 2), 1);
}

class TSPTest : public ::testing::Test {
protected:
    cost_matrix_t cm_raw = {
            {INF, 10,  8,   19,  12},
            {10,  INF, 20,  6,   3},
            {8,   20,  INF, 4,   2},
            {19,  6,   4,   INF, 7},
            {12,  3,   2,   7,   INF}
    };
};

TEST_F(TSPTest, choose_new_vertex) {
    StageState ss(cm_raw);
    ss.reduce_cost_matrix();
    NewVertex chosen = ss.choose_new_vertex();
    EXPECT_EQ(chosen.coordinates.row, 0);
    EXPECT_EQ(chosen.coordinates.col, 2);
}

TEST_F(TSPTest, update_cost_matrix) {
    StageState ss(cm_raw);
    vertex_t v(1, 2);
    ss.update_cost_matrix(v);
    const auto& matrix = ss.get_matrix();
    EXPECT_TRUE(is_inf(matrix[1][0]));
    EXPECT_TRUE(is_inf(matrix[1][1]));
    EXPECT_TRUE(is_inf(matrix[1][2]));
    EXPECT_TRUE(is_inf(matrix[0][2]));
    EXPECT_TRUE(is_inf(matrix[1][2]));
    EXPECT_TRUE(is_inf(matrix[2][2]));
    EXPECT_TRUE(is_inf(matrix[2][1]));
}

TEST_F(TSPTest, reduce_cost_matrix) {
    StageState ss(cm_raw);
    EXPECT_EQ(ss.reduce_cost_matrix(), 28); // 19 from rows, 9 from columns
}

TEST_F(TSPTest, get_path) {
    cost_matrix_t m = {
            {INF, INF, INF, INF},
            {INF, INF, 1, INF},
            {INF, INF, INF, INF},
            {INF, 1, INF, INF}
    };
    unsorted_path_t unsorted_path = {{0, 2}, {2, 3}};
    StageState ss(m, unsorted_path);
    path_t expected_path = {0, 2, 3, 1};
    EXPECT_EQ(ss.get_path(), expected_path);
}

TEST_F(TSPTest, solve_tsp) {
    tsp_solutions_t solutions = solve_tsp(cm_raw);
    ASSERT_FALSE(solutions.empty());
    EXPECT_EQ(solutions[0].lower_bound, 32);

    // The problem has two optimal solutions
    path_t p1 = {0, 2, 3, 4, 1};
    path_t p2 = {0, 1, 4, 3, 2};
    
    bool found_p1 = false;
    bool found_p2 = false;

    for(const auto& s : solutions) {
        if (s.path == p1) found_p1 = true;
        if (s.path == p2) found_p2 = true;
    }
    EXPECT_TRUE(found_p1 || found_p2);
}