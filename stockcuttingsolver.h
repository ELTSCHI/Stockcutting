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
            length += (pieces - 1) * stocks[pattern.stockIndex].sawWidth;
        }

        return length;
    }

    virtual void solveInternal() = 0;
    void removeOverproduction(std::vector<PatternUsage>& erg);

public:
    StockCuttingSolver(std::vector<Product> products, std::vector<StockType> stocks) : patternSolver(products, stocks) {
        this->products = products;
        this->stocks = stocks;
    }

    std::vector<PatternUsage> solve() {
        solveInternal();
        std::vector<double> patternQuantities = patternSolver.getPrimalValues();
        std::vector<Pattern> patterns = patternSolver.getPatterns();

        std::vector<PatternUsage> erg;
        erg.resize(patterns.size());
        for(int i = 0; i < patterns.size(); i++) {
            PatternUsage patternUsage;
            patternUsage.pattern = patterns[i];
            patternUsage.quantity = patternQuantities[i];
            erg[i] = patternUsage;
        }
        removeOverproduction(erg);
        return erg;
    }
};

#endif // STOCKCUTTINGSOLVER_H
