#ifndef PATTERNUSAGE_H
#define PATTERNUSAGE_H

#include "Pattern.h"


struct PatternUsage {
    Pattern pattern;
    double quantity;
    int getWaste(std::vector<StockType>& stocks, std::vector<Product>& products) {
        return quantity * pattern.getWaste(stocks, products);
    }
    int getCost(std::vector<StockType>& stocks) {
        return stocks[pattern.stockIndex].cost * quantity;
    }
};

#endif // PATTERNUSAGE_H
