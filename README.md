# TSP Algorithm C++

**Optimal route-finding for the Traveling Salesperson Problem using Branch & Bound in modern C++.**

---

## About

This is one of my univeristy projects for Advanced Object Oriendted Programming course.

It is a high-performance implementation of the **Traveling Salesperson Problem (TSP)** solver using the **Branch and Bound** algorithm.  The project tackles the classic combinatorial optimization problem of finding the shortest possible route that visits each city exactly once and returns to the origin city.  

Built with efficiency and clarity in mind, this solver constructs a search tree, progressively reducing cost matrices and pruning suboptimal branches to converge on all optimal solutions.  The implementation features modular architecture with separate classes for cost matrix operations (`CostMatrix`) and state management (`StageState`), making it suitable for both educational purposes and practical route optimization scenarios.

The solver guarantees finding **all** optimal solutions when multiple paths share the minimum cost, utilizing LIFO stack-based tree traversal for memory-efficient exploration of the solution space.

---

## Tech Stack

- **C++17** — Modern C++ standard with STL containers and algorithms
- **Google Test (GTest)** — Unit testing framework for comprehensive test coverage
- **G++/GCC** — Compiler with C++17 support
- **POSIX Threads** — Threading support for test execution

---

## Key Features

- **Branch & Bound Optimization** — Intelligent pruning of the search tree using lower bound calculations to eliminate suboptimal paths early
- **Complete Matrix Reduction** — Row and column reduction techniques to transform cost matrices and compute tight lower bounds
- **Multiple Solution Detection** — Identifies and returns all equally optimal routes when they exist, not just a single solution
- **Comprehensive Test Suite** — Full unit test coverage for matrix operations, vertex selection, path reconstruction, and end-to-end TSP solving

---

## Installation/Usage

### Building the Solver

```bash
# Compile the TSP solver
g++ main.cpp TSP.cpp tsp_setup.cpp -o tsp -std=c++17

# Run the solver
./tsp
```

### Running Tests

```bash
# Compile tests (requires gtest installation)
g++ tests. cpp TSP.cpp tsp_setup.cpp -o tests -std=c++17 -lgtest -lgtest_main -pthread

# Execute test suite
./tests
```

**Note:** To switch between running the solver and tests, toggle the commented sections in `main.cpp`.

---

## Project Structure

```
tsp_algorithm_cpp/
├── TSP.cpp              # Core algorithm implementation
├── TSP.hxx              # Header with class definitions
├── tsp_setup.cpp        # Setup utilities and type definitions
├── tsp_setup.hxx        # Setup header
├── main.cpp             # Entry point for solver
├── tests.cpp            # Unit tests
└── GEMINI. md            # Additional documentation
```

---

## Algorithm Overview

The solver implements a **Branch and Bound** strategy:

1. **Matrix Reduction** — Reduce rows and columns to establish initial lower bound
2. **Vertex Selection** — Choose the most promising vertex transition based on cost analysis
3. **Branch Creation** — Generate left (include edge) and right (exclude edge) branches
4. **Bound Checking** — Prune branches where lower bound exceeds current best solution
5. **Solution Extraction** — Reconstruct complete paths from 2×2 terminal matrices

---

## License

This project is available for educational and research purposes. 

---

**License:** MIT
**Author:** [@mayyyk](https://github.com/mayyyk)
