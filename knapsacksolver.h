#ifndef KNAPSACKSOLVER_H
#define KNAPSACKSOLVER_H

#include "Pattern.h"
#include "Product.h"
#include "StockType.h"
class KnapsackSolver
{
public:
    KnapsackSolver();

    Pattern solve(const std::vector<Product>& products, const StockType& stock, int stockIndex, const std::vector<double>& dualPrices);

    double reducedCost(const Pattern& bestPattern, const StockType& stock, const std::vector<double>& dualPrices);
};

#endif // KNAPSACKSOLVER_H
