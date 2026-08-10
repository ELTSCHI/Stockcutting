#include "stockcuttingsolver.h"
#include <algorithm>

namespace {

int findUsedPatternWithProduct(const std::vector<PatternUsage>& erg, int productIndex) {
    for(int i = 0; i < erg.size(); i++) {
        if(erg[i].quantity <= 0) {
            continue;
        }
        if(erg[i].pattern.quantities[productIndex] > 0) {
            return i;
        }
    }
    return -1;
}

int findOrCreatePattern(std::vector<PatternUsage>& erg, const Pattern& newPattern) {
    for(int i = 0; i < erg.size(); i++) {
        if(erg[i].pattern.stockIndex == newPattern.stockIndex && erg[i].pattern.quantities == newPattern.quantities) {
            return i;
        }
    }
    erg.push_back({newPattern, 0.0});
    return erg.size() - 1;
}

}

void StockCuttingSolver::removeOverproduction(std::vector<PatternUsage>& erg) {
    std::vector<int> produced;
    produced.resize(products.size(), 0);
    for(const PatternUsage& usage : erg) {
        if(usage.quantity > 0) {
            for(int j = 0; j < usage.pattern.quantities.size(); j++) {
                produced[j] += usage.quantity * usage.pattern.quantities[j];
            }
        }
    }

    for(int i = 0; i < produced.size(); i++) {
        while(produced[i] > products[i].demand) {
            int patternIndex = findUsedPatternWithProduct(erg, i);
            if(patternIndex < 0) {
                break;
            }
            int producedByPattern = erg[patternIndex].pattern.quantities[i];
            int maxRemoval = std::min(producedByPattern, produced[i] - products[i].demand);

            Pattern newPattern = erg[patternIndex].pattern;
            newPattern.quantities[i] -= maxRemoval;

            int newPatternIndex = findOrCreatePattern(erg, newPattern);
            erg[newPatternIndex].quantity++;
            erg[patternIndex].quantity--;
            produced[i] -= maxRemoval;
        }
    }
}
