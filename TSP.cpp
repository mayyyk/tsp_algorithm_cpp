#include "TSP.hxx"

#include <algorithm>
#include <stack>
#include <optional>
#include <numeric> // Added for std::accumulate

std::ostream& operator<<(std::ostream& os, const CostMatrix& cm) {
    for (std::size_t r = 0; r < cm.size(); ++r) { // size_t is standard CPP language type, able to represent the size/index of the biggest object my system can handle, it's uint
        for (std::size_t c = 0; c < cm.size(); ++c) {
            const auto& elem = cm[r][c];
            os << (is_inf(elem) ? "INF" : std::to_string(elem)) << " ";
        }
        os << "\n";
    }
    os << std::endl;

    return os;
}

/* PART 1 */

/**
 * Create path from unsorted path and last 2x2 cost matrix.
 * @return The vector of consecutive vertex.
 */
path_t StageState::get_path() {
    throw;  // TODO: Implement it!
}

/**
 * Get minimum values from each row and returns them.
 * @return Vector of minimum values in row.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_rows() const { // rows of cost matrix represent starting cities
    std::vector<cost_t> min_values;
    min_values.reserve(matrix_.size());

    for (const auto& row : matrix_) {
        // Corrected comparator lambda
        auto comparator = [](const cost_t& a, const cost_t& b){
            if (is_inf(a)) {
                return false;
            }
            if (is_inf(b)) {
                return true;
            }
            return a < b; // Correctly returns a boolean
        };
        auto min_element = *std::min_element(row.begin(), row.end(), comparator);

        min_values.push_back(min_element);
    }

    return min_values;
}

/**
 * Reduce rows so that in each row at least one zero value is present. If we dubstract min value from get_min_values_in_rows then it must happen
 * @return Sum of values reduced in rows. - that's what will be added to the cost
 */
cost_t CostMatrix::reduce_rows() {
    std::vector<cost_t> min_values = get_min_values_in_rows();
    
    for (std::size_t idx = 0; idx < matrix_.size(); ++idx) {
        if (is_inf(min_values[idx])) { // No reduction if min is INF
             continue;
        }
        for (std::size_t jdx = 0; jdx < matrix_.size(); ++jdx) {
            if (!is_inf(matrix_[idx][jdx])) {
                matrix_[idx][jdx] -= min_values[idx];
            }
        }
    }

    // Use std::accumulate to sum the minimums, ignoring INF values
    return std::accumulate(min_values.begin(), min_values.end(), 0,
                           [](cost_t acc, cost_t val) {
                               return is_inf(val) ? acc : acc + val;
                           });
}

/**
 * Get minimum values from each column and returns them.
 * @return Vector of minimum values in columns.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_cols() const { // cols in cost matrix represent destination cities
    if (matrix_.empty()) {
        return {};
    }

    std::vector<cost_t> min_values(matrix_.size(), INF);
    for (std::size_t c = 0; c < matrix_.size(); ++c) {
        for (std::size_t r = 0; r < matrix_.size(); ++r) {
            if (!is_inf(matrix_[r][c]) && matrix_[r][c] < min_values[c]) {
                min_values[c] = matrix_[r][c];
            }
        }
    }
    return min_values;
}

/**
 * Reduces rows so that in each column at least one zero value is present.
 * @return Sum of values reduced in columns.
 */
cost_t CostMatrix::reduce_cols() {
    std::vector<cost_t> min_values = get_min_values_in_cols();

    for (std::size_t c = 0; c < matrix_.size(); ++c) {
        if (is_inf(min_values[c])) {
            continue;
        }
        for (std::size_t r = 0; r < matrix_.size(); ++r) {
            if (!is_inf(matrix_[r][c])) {
                matrix_[r][c] -= min_values[c];
            }
        }
    }

    return std::accumulate(min_values.begin(), min_values.end(), 0,
                           [](cost_t acc, cost_t val) {
                               return is_inf(val) ? acc : acc + val;
                           });
}

/**
 * Get the cost of not visiting the vertex_t (@see: get_new_vertex())
 * @param row
 * @param col
 * @return The sum of minimal values in row and col, excluding the intersection value.
 */
cost_t CostMatrix::get_vertex_cost(std::size_t row, std::size_t col) const {
    cost_t min_in_row = INF;
    // Find the minimum in the given row, excluding the element at 'col'
    for (std::size_t c = 0; c < size(); ++c) {
        if (c == col) {
            continue;
        }
        if (matrix_[row][c] < min_in_row) {
            min_in_row = matrix_[row][c];
        }
    }

    cost_t min_in_col = INF;
    // Find the minimum in the given column, excluding the element at 'row'
    for (std::size_t r = 0; r < size(); ++r) {
        if (r == row) {
            continue;
        }
        if (matrix_[r][col] < min_in_col) {
            min_in_col = matrix_[r][col];
        }
    }

    return min_in_row + min_in_col;
}

/* PART 2 */

/**
 * Choose next vertex to visit:
 * - Look for vertex_t (pair row and column) with value 0 in the current cost matrix.
 * - Get the vertex_t cost (calls get_vertex_cost()).
 * - Choose the vertex_t with maximum cost and returns it.
 * @param cm
 * @return The coordinates of the next vertex.
 */
NewVertex StageState::choose_new_vertex() {
    NewVertex best_choice({}, -1); // Initialize with an invalid cost so that the first one we go to becomes a better choice
    // best choice is a variable of type NewVertex representing the best choice of this particular StageState, with now coordinates and cost equal to -1
    const auto& cm = get_matrix(); // we access the cost matrix (matrix_) by a getter function which returns a constant reference to the internal matrix of StageStage instance
    for (std::size_t r = 0; r < cm.size(); ++r) {
        for (std::size_t c = 0; c < cm.size(); ++c) {
            if (cm[r][c] == 0) {
                cost_t cost = cm.get_vertex_cost(r, c); // we get the costs of avoiding the zero cost vectors
                if (cost > best_choice.cost) {
                    best_choice.cost = cost;
                    best_choice.coordinates = vertex_t(r, c);
                }
            }
        }
    }
    return best_choice;
}

/**
 * Update the cost matrix with the new vertex.
 * @param new_vertex
 */
void StageState::update_cost_matrix(vertex_t new_vertex) {// discarding the rest of current row and column to block further travel options

    // Set the chosen row and column to INF
    for (std::size_t i = 0; i < matrix_.size(); ++i) {
        matrix_[new_vertex.row][i] = INF;
        matrix_[i][new_vertex.col] = INF;
    }

    // Forbid the reverse path
    matrix_[new_vertex.col][new_vertex.row] = INF;
}

/**
 * Reduce the cost matrix.
 * @return The sum of reduced values.
 */
cost_t StageState::reduce_cost_matrix() { // StageStage bundles all the data needed to represent one step of the solution process
    return matrix_.reduce_rows() + matrix_.reduce_cols(); // matrix_ is the cost matrix at current stage. StageState contains it, which represents the problem from its (current) perspective.
}

/**
 * Given the optimal path, return the optimal cost.
 * @param optimal_path
 * @param m
 * @return Cost of the path.
 */
cost_t get_optimal_cost(const path_t& optimal_path, const cost_matrix_t& m) {
    cost_t cost = 0;

    for (std::size_t idx = 1; idx < optimal_path.size(); ++idx) {
        cost += m[optimal_path[idx - 1]][optimal_path[idx]];
    }

    // Add the cost of returning from the last city to the initial one.
    cost += m[optimal_path[optimal_path.size() - 1]][optimal_path[0]];

    return cost;
}

/**
 * Create the right branch matrix with the chosen vertex forbidden and the new lower bound.
 * @param m
 * @param v
 * @param lb
 * @return New branch.
 */
StageState create_right_branch_matrix(cost_matrix_t m, vertex_t v, cost_t lb) {
    CostMatrix cm(m);
    cm[v.row][v.col] = INF;
    return StageState(cm, {}, lb);
}

/**
 * Retain only optimal ones (from all possible ones).
 * @param solutions
 * @return Vector of optimal solutions.
 */
tsp_solutions_t filter_solutions(tsp_solutions_t solutions) {
    cost_t optimal_cost = INF;
    for (const auto& s : solutions) {
        optimal_cost = (s.lower_bound < optimal_cost) ? s.lower_bound : optimal_cost;
    }

    tsp_solutions_t optimal_solutions;
    std::copy_if(solutions.begin(), solutions.end(),
                 std::back_inserter(optimal_solutions),
                 [&optimal_cost](const tsp_solution_t& s) { return s.lower_bound == optimal_cost; }
    );

    return optimal_solutions;
}

/**
 * Solve the TSP.
 * @param cm The cost matrix.
 * @return A list of optimal solutions.
 */
tsp_solutions_t solve_tsp(const cost_matrix_t& cm) {

    StageState left_branch(cm);

    // The branch & bound tree.
    std::stack<StageState> tree_lifo;

    // The number of levels determines the number of steps before obtaining
    // a 2x2 matrix.
    std::size_t n_levels = cm.size() - 2;

    tree_lifo.push(left_branch);   // Use the first cost matrix as the root.

    cost_t best_lb = INF;
    tsp_solutions_t solutions;

    while (!tree_lifo.empty()) {

        left_branch = tree_lifo.top();
        tree_lifo.pop();

        while (left_branch.get_level() != n_levels && left_branch.get_lower_bound() <= best_lb) {
            // Repeat until a 2x2 matrix is obtained or the lower bound is too high...

            if (left_branch.get_level() == 0) {
                left_branch.reset_lower_bound();
            }

            // 1. Reduce the matrix in rows and columns.
            cost_t new_cost = left_branch.reduce_cost_matrix();

            // 2. Update the lower bound and check the break condition.
            left_branch.update_lower_bound(new_cost);
            if (left_branch.get_lower_bound() > best_lb) {
                break;
            }

            // 3. Get new vertex and the cost of not choosing it.
            NewVertex new_vertex = left_branch.choose_new_vertex();

            // 4. Update the path - use append_to_path method.
            left_branch.append_to_path(new_vertex.coordinates);

            // 5. Update the cost matrix of the left branch.
            left_branch.update_cost_matrix(new_vertex.coordinates);

            // 6. Update the right branch and push it to the LIFO.
            cost_t new_lower_bound = left_branch.get_lower_bound() + new_vertex.cost;
            tree_lifo.push(create_right_branch_matrix(cm, new_vertex.coordinates,
                                                      new_lower_bound));
        }

        if (left_branch.get_lower_bound() <= best_lb) {
            // If the new solution is at least as good as the previous one,
            // save its lower bound and its path.
            best_lb = left_branch.get_lower_bound();
            path_t new_path = left_branch.get_path();
            solutions.push_back({get_optimal_cost(new_path, cm), new_path});
        }
    }

    return filter_solutions(solutions); // Filter solutions to find only optimal ones.
}
