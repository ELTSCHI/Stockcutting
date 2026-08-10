#ifndef PATTERN_H
#define PATTERN_H

#include "Product.h"

#include <vector>
struct Pattern {
    int stockIndex;              // index into the stocks vector
    std::vector<double> quantities; // quantities[i] is the count of products[i]
};

#endif // PATTERN_H
