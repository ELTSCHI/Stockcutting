#ifndef ALLPATTERN_H
#define ALLPATTERN_H

#include "stockcuttingsolver.h"

class AllPattern : public StockCuttingSolver
{
private:
    void generateAllPattern();
    void generatePatternRecursive(int productIndex, int remainingLength, std::vector<double>& quantities, int stockIndex);
public:
    using StockCuttingSolver::StockCuttingSolver;

    std::vector<PatternUsage> solve() override;
};

#endif // ALLPATTERN_H
