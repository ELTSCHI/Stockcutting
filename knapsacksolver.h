#ifndef KNAPSACKSOLVER_H
#define KNAPSACKSOLVER_H

#include "Pattern.h"
#include "Product.h"
#include "StockType.h"
class KnapsackSolver
{
public:
    KnapsackSolver();

    Pattern solve(std::vector<Product>& products, StockType& stock, std::vector<double>& dualPrices);

    double reducedCost(Pattern& bestPattern, StockType& stock, std::vector<double>& dualPrices);
};

#endif // KNAPSACKSOLVER_H
