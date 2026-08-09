#include "allpattern.h"

void AllPattern::generatePatternRecursive(int productIndex, int remainingLength, std::vector<double>& quantities, int stockId) {
    // Abbruchbedingung
    if (productIndex == products.size()) {
        // ignore empty Patterns
        bool hasProduct = false;
        for (double q : quantities) {
            if (q > 0) {
                hasProduct = true;
                break;
            }
        }

        if (hasProduct) {
            Pattern pattern{.stockType = stockId, .quantities = quantities};
            if(!(checkPatternSize(pattern) > stocks[stockId].length)) {
                patternSolver.addPattern({stockId, quantities});
            }
        }
        return;
    }

    const Product& product = products[productIndex];
    int maxQuantity = remainingLength / product.length;

    for (int quantity = 0; quantity <= maxQuantity; quantity++) {

        quantities[productIndex] = quantity;

        generatePatternRecursive(
            productIndex + 1,
            remainingLength - quantity * product.length,
            quantities,
            stockId
            );
    }

    quantities[productIndex] = 0;
}

void AllPattern::generateAllPattern() {

    for(auto stock : stocks) {
        std::vector<double> quantities(products.size(), 0);

        generatePatternRecursive(
            0,
            stock.length,
            quantities,
            stock.id
        );
    }
}

std::vector<PatternUsage> AllPattern::solve() {
    generateAllPattern();

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
