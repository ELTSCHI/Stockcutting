#ifndef STOCKCUTTINGSOLVER_H
#define STOCKCUTTINGSOLVER_H

#include "PatternUsage.h"
#include "Product.h"
#include "StockType.h"
#include "patternsolver.h"
#include <Result.h>
#include <iostream>


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
            length += (pieces - 1) * stocks[pattern.stockIndex].sawWidth;
        }

        return length;
    }

    virtual void solveInternal() = 0;
    void removeOverproduction(std::vector<PatternUsage>& erg);
    int calculateWaste(std::vector<PatternUsage>& erg);
    int calculateCost(std::vector<PatternUsage>& erg);

public:
    StockCuttingSolver(std::vector<Product>& products, std::vector<StockType>& stocks);
    Result solve();
};

#endif // STOCKCUTTINGSOLVER_H
