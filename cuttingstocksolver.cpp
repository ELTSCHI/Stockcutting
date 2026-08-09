#include "cuttingstocksolver.h"


CuttingStockSolver::CuttingStockSolver(std::vector<Product> products, std::vector<StockType> stocks) : patternSolver(products, stocks) {
    this->products = products;
    this->stocks = stocks;
}

void CuttingStockSolver::generateInitialPatterns() {
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

bool CuttingStockSolver::generateNewPattern() {
    std::vector<double> duals = patternSolver.getDualPrices();
    for(StockType stock : stocks) {
        // TODO: exclude stocks without better patterns in the next round
        Pattern bestPattern = knapsackSolver.solve(products, stock, duals);
        double reducedCosts = knapsackSolver.reducedCost(bestPattern, stock, duals);

        if(reducedCosts > 1){
            patternSolver.addPattern(bestPattern);
            return true;
        }
    }

    return false;
}

std::vector<PatternUsage> CuttingStockSolver::solve() {

    // add initial Patterns to Problem
    generateInitialPatterns();

    // pattern generation feedback loop
    while(true) {
        // solve with current patterns
        patternSolver.solveRelaxed();

        // add best Pattern (calculated via dual prices)
        bool added = generateNewPattern();

        if(!added) { // no good patterns missing
            break;
        }
    }

    std::vector<double> patternQuantities = patternSolver.getPrimalValues();

    // TODO: Ganzzahligkeitsbedingung enforcen

}
