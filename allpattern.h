#ifndef ALLPATTERN_H
#define ALLPATTERN_H

#include "stockcuttingsolver.h"

class AllPattern : public StockCuttingSolver
{
private:
    void generateAllPattern();
    void generatePatternRecursive(int productIndex, int remainingLength, std::vector<double>& quantities, int stockIndex);

    void solveInternal() override;
public:
    using StockCuttingSolver::StockCuttingSolver;
};

#endif // ALLPATTERN_H
