# Stockcutting

A cutting stock problem solver that computes optimal cutting patterns for cutting products from stock material of different lengths.

## Overview

The project solves the classic one-dimensional cutting stock problem: given a set of products with defined lengths and demands, find a set of cutting patterns (combinations of products that fit within a stock piece) that satisfies all demands at minimal cost.

Two solver strategies are implemented, both based on the Gilmore–Gomory LP formulation solved with [GLPK](https://www.gnu.org/software/glpk/):

- **HeuristicColumnGeneration** – starts with a trivial pattern set and iteratively generates improving patterns by solving an integer knapsack subproblem using the dual prices of the current LP relaxation.
- **AllPattern** – enumerates every feasible cutting pattern for each stock type and solves the resulting LP/IP directly. Only practical for small problem instances.

Both solvers derive from the `StockCuttingSolver` base class and return a list of `PatternUsage` (pattern plus usage quantity).

## Build

### Dependencies

- CMake >= 3.16
- C++17 compiler
- Qt 5 or Qt 6 (Widgets, LinguistTools)
- GLPK

### Ubuntu/Debian

```bash
sudo apt install cmake g++ qt6-base-dev libglpk-dev
```

### Configure and build

```bash
cmake -S . -B build
cmake --build build
```

Run the application:

```bash
./build/Stockcutting
```

The console output prints the patterns computed by both solver strategies for the example products/stocks defined in `main.cpp`.

## Project structure

| File | Purpose |
| --- | --- |
| `main.cpp` | Entry point with example data and demo output |
| `stockcuttingsolver.h` | Abstract base class for cutting stock solvers |
| `heuristiccolumngeneration.{h,cpp}` | Heuristic column generation solver |
| `allpattern.{h,cpp}` | Brute-force pattern enumeration solver |
| `patternsolver.{h,cpp}` | Builds and solves the pattern LP/MIP with GLPK |
| `knapsacksolver.{h,cpp}` | Integer knapsack solver for pattern generation |
| `Product.h` | Product definition (id, length, demand) |
| `StockType.h` | Stock definition (id, length, cost, availability) |
| `Pattern.h` / `PatternUsage.h` | Cutting pattern and usage data structures |
| `mainwindow.*` | Qt GUI (placeholder) |
| `Stockcutting_de_DE.ts` | German translation |

## Usage

Define products and stock types, instantiate a solver and call `solve()`:

```cpp
std::vector<Product> products = {{.id = 0, .length = 160, .demand = 6},
                                 {.id = 1, .length = 480, .demand = 7},
                                 {.id = 2, .length = 735, .demand = 4}};

std::vector<StockType> stocks = {{.id = 0, .length = 2400, .cost = 1, .availabilty = -1},
                                 {.id = 1, .length = 2880, .cost = 1, .availabilty = -1}};

HeuristicColumnGeneration solver(products, stocks);
std::vector<PatternUsage> result = solver.solve();
```

## License

[MIT](LICENSE)
