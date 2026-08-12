#ifndef PATTERN_H
#define PATTERN_H

#include "Product.h"
#include <StockType.h>
#include <vector>

struct Pattern {
    int stockIndex;
    std::vector<double> quantities;
    int getWaste(std::vector<StockType>& stocks, std::vector<Product>& products) {
        int totalLength = 0;
        int cuts = 0;
        for(int i = 0; i < quantities.size(); i++) {
            totalLength += products[i].length * quantities[i];
            cuts += quantities[i];
        }
        totalLength += stocks[stockIndex].sawWidth * (cuts > 0 ? cuts - 1 : 0);
        return stocks[stockIndex].length - totalLength;
    }
};

#endif // PATTERN_H
