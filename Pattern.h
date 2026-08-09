#ifndef PATTERN_H
#define PATTERN_H

#include "Product.h"

#include <vector>
struct Pattern {
    int stockType;
    std::vector<double> quantities;
};

#endif // PATTERN_H
