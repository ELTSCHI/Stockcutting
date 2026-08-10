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

    void solveInternal() override;
public:
    using StockCuttingSolver::StockCuttingSolver;
};

#endif // HEURISTICCOLUMNGENERATION_H
