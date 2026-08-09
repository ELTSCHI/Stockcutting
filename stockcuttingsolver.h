#ifndef STOCKCUTTINGSOLVER_H
#define STOCKCUTTINGSOLVER_H

#include "PatternUsage.h"
#include "Product.h"
#include "StockType.h"
#include "patternsolver.h"


class StockCuttingSolver
{
protected:
    std::vector<Product> products;
    std::vector<StockType> stocks;
    PatternSolver patternSolver;
public:
    StockCuttingSolver(std::vector<Product> products, std::vector<StockType> stocks) : patternSolver(products, stocks) {
        this->products = products;
        this->stocks = stocks;
    }
    virtual std::vector<PatternUsage> solve() = 0;
};

#endif // STOCKCUTTINGSOLVER_H
