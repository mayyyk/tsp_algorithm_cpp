# GEMINI.md

## Project Overview

This project is a C++ implementation of a solver for the Traveling Salesperson Problem (TSP). It uses the branch and bound algorithm to find the optimal path(s) with the minimum cost.

The core logic is organized into several classes and files:
- **`TSP.hxx` / `TSP.cpp`**: Contains the main algorithm.
  - `CostMatrix`: A class to represent the cost matrix and perform operations like row/column reduction.
  - `StageState`: Represents a state in the branch and bound search tree, holding a partial solution, the current cost matrix, and the lower bound.
  - `solve_tsp`: The main function that orchestrates the search for the optimal solution.
- **`tsp_setup.hxx` / `tsp_setup.cpp`**: Defines fundamental types like `cost_t` (for costs), `path_t` (for paths), and the constant `INF` for forbidden transitions.
- **`main.cpp`**: The main entry point for the application. It initializes a cost matrix and calls `solve_tsp` to demonstrate its usage. It also contains a commented-out section to run unit tests.
- **`tests.cpp`**: Contains unit tests for the project using the Google Test framework.

**Note:** Many of the core functions in `TSP.cpp` are currently unimplemented (marked with `// TODO: Implement it!`). The primary task for a developer working on this project would be to complete this implementation.

## Building and Running

The project does not include a `Makefile` or `CMakeLists.txt`. It can be compiled directly using a C++ compiler like `g++`.

### Main Application

To build and run the main TSP solver:

```bash
# Compile the source files into an executable named 'tsp'
g++ main.cpp TSP.cpp tsp_setup.cpp -o tsp -std=c++17

# Run the solver
./tsp
```

### Unit Tests

The project uses the Google Test framework. You will need to have `gtest` installed and available to the linker.

To run the tests:
1.  In `main.cpp`, comment out the application logic.
2.  In `main.cpp`, uncomment the section that includes `gtest/gtest.h` and calls `RUN_ALL_TESTS()`.
3.  Compile and run the tests:

```bash
# Compile the test source files and link against gtest
g++ tests.cpp TSP.cpp tsp_setup.cpp -o tests -std=c++17 -lgtest -lgtest_main -pthread

# Run the tests
./tests
```

## Development Conventions

- **C++ Standard**: The code appears to be written using modern C++ features. Using `c++17` is recommended.
- **Headers**: Header files use `#ifndef` guards to prevent multiple inclusions.
- **Naming**:
  - Classes are named using `PascalCase` (e.g., `CostMatrix`).
  - Functions and variables use `snake_case` (e.g., `solve_tsp`, `lower_bound`).
  - Type aliases end with `_t` (e.g., `cost_t`, `path_t`).
- **Testing**: Unit tests are written using the Google Test framework and are located in `tests.cpp`. New functionality should be accompanied by corresponding tests.
