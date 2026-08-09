#ifndef HEURISTICCOLUMNGENERATION_H
#define HEURISTICCOLUMNGENERATION_H

#include "knapsacksolver.h"
#include "stockcuttingsolver.h"


class HeuristicColumnGeneration : public StockCuttingSolver
{
private:
    KnapsackSolver knapsackSolver;

    void generateInitialPatterns();
    bool generateNewPattern();
public:
    using StockCuttingSolver::StockCuttingSolver;

    std::vector<PatternUsage> solve() override;
};

#endif // HEURISTICCOLUMNGENERATION_H
