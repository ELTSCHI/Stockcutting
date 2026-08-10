#include "heuristiccolumngeneration.h"

void HeuristicColumnGeneration::generateInitialPatterns() {
    for(int stockIndex = 0; stockIndex < stocks.size(); stockIndex++) {
        const StockType& stock = stocks[stockIndex];

        for(int productIndex = 0; productIndex < products.size(); productIndex++) {
            const Product& product = products[productIndex];
            int quantity = stock.length / product.length;

            if(quantity == 0) {
                continue;
            }

            Pattern pattern;
            pattern.stockIndex = stockIndex;
            pattern.quantities.resize(products.size(), 0);
            pattern.quantities[productIndex] = quantity;

            while(checkPatternSize(pattern) > stock.length) {
                pattern.quantities[productIndex]--;
            }

            patternSolver.addPattern(pattern);
        }
    }
}

bool HeuristicColumnGeneration::generateNewPattern() {
    std::vector<double> duals = patternSolver.getDualPrices();

    Pattern bestPattern = {};
    double bestReducedCost = 0.0;
    for(int stockIndex = 0; stockIndex < stocks.size(); stockIndex++) {
        const StockType& stock = stocks[stockIndex];
        Pattern pattern = knapsackSolver.solve(products, stock, stockIndex, duals);
        double reducedCosts = knapsackSolver.reducedCost(pattern, stock, duals);

        if(reducedCosts < bestReducedCost) {
            bestReducedCost = reducedCosts;
            bestPattern = pattern;
        }
    }

    if(bestReducedCost < 0.0){
        patternSolver.addPattern(bestPattern);
        return true;
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
