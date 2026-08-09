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
    int checkPatternSize(Pattern pattern){
        int length = 0;
        int pieces = 0;

        for (int i = 0; i < products.size(); i++) {
            length += pattern.quantities[i] * products[i].length;
            pieces += pattern.quantities[i];
        }

        if (pieces > 0) {
            length += (pieces - 1) * stocks[pattern.stockType].sawWidth;
        }

        return length;
    }
public:
    StockCuttingSolver(std::vector<Product> products, std::vector<StockType> stocks) : patternSolver(products, stocks) {
        this->products = products;
        this->stocks = stocks;
    }
    virtual std::vector<PatternUsage> solve() = 0;
};

#endif // STOCKCUTTINGSOLVER_H
