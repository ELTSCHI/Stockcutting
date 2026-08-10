#include "allpattern.h"
#include <algorithm>

void AllPattern::generatePatternRecursive(int productIndex, int remainingLength, std::vector<double>& quantities, int stockIndex) {
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
            Pattern pattern{.stockIndex = stockIndex, .quantities = quantities};
            if(!(checkPatternSize(pattern) > stocks[stockIndex].length)) {
                patternSolver.addPattern({stockIndex, quantities});
            }
        }
        return;
    }

    const Product& product = products[productIndex];
    int maxQuantity = std::min(remainingLength / product.length, product.demand);
    for (int quantity = 0; quantity <= maxQuantity; quantity++) {

        quantities[productIndex] = quantity;

        generatePatternRecursive(
            productIndex + 1,
            remainingLength - quantity * product.length,
            quantities,
            stockIndex
            );
    }

    quantities[productIndex] = 0;
}

void AllPattern::generateAllPattern() {

    for(int stockIndex = 0; stockIndex < stocks.size(); stockIndex++) {
        std::vector<double> quantities(products.size(), 0);

        generatePatternRecursive(
            0,
            stocks[stockIndex].length,
            quantities,
            stockIndex
        );
    }
}

void AllPattern::solveInternal() {
    generateAllPattern();

    patternSolver.solveLP();
}
