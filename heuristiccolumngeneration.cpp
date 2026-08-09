#include "heuristiccolumngeneration.h"
#include <iostream>

void HeuristicColumnGeneration::generateInitialPatterns() {
    for(StockType stock : stocks) {
        for(Product product : products) {
            int quantity = stock.length / product.length;

            if(quantity == 0) {
                continue;
            }

            Pattern pattern;
            pattern.stockType = stock.id;
            pattern.quantities.resize(products.size(), 0);
            pattern.quantities[product.id] = quantity;

            patternSolver.addPattern(pattern);
        }
    }
}

bool HeuristicColumnGeneration::generateNewPattern() {
    std::vector<double> duals = patternSolver.getDualPrices();

    for(StockType stock : stocks) {
        // TODO: exclude stocks without better patterns in the next round
        Pattern bestPattern = knapsackSolver.solve(products, stock, duals);
        double reducedCosts = knapsackSolver.reducedCost(bestPattern, stock, duals);


        if(reducedCosts < 0.0){
            patternSolver.addPattern(bestPattern);
            return true;
        }
    }

    return false;
}

std::vector<PatternUsage> HeuristicColumnGeneration::solve() {

    // add initial Patterns to Problem
    generateInitialPatterns();

    // pattern generation feedback loop
    int i = 0;
    while(i < 5) {
        // solve with current patterns
        patternSolver.solveRelaxed();

        // add best Pattern (calculated via dual prices)
        bool added = generateNewPattern();

        if(!added) { // no good patterns missing
            break;
        }
        i++;
    }

    patternSolver.solveLP();

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

    return erg;
}
