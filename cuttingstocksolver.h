#ifndef CUTTINGSTOCKSOLVER_H
#define CUTTINGSTOCKSOLVER_H

#include "PatternUsage.h"
#include "Product.h"
#include "StockType.h"
#include "knapsacksolver.h"
#include "patternsolver.h"
#include <vector>


class CuttingStockSolver
{
private:
    std::vector<Product> products;
    std::vector<StockType> stocks;

    PatternSolver patternSolver;
    KnapsackSolver knapsackSolver;

    void generateInitialPatterns();
    bool generateNewPattern();

public:
    CuttingStockSolver(std::vector<Product> products, std::vector<StockType> stocks);
    std::vector<PatternUsage> solve();
};

#endif // CUTTINGSTOCKSOLVER_H
