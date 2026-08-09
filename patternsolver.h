#ifndef PATTERNSOLVER_H
#define PATTERNSOLVER_H

#include "Product.h"
#include "StockType.h"
#include "Pattern.h"

#include <vector>


class PatternSolver
{
private:
    std::vector<Product> products;
    std::vector<StockType> stocks;
    std::vector<Pattern> patterns;

public:
    PatternSolver(const std::vector<Product>& products, const std::vector<StockType>& stocks);
    ~PatternSolver();

    void addPattern(const Pattern & pattern);

    bool solveRelaxed();

    std::vector<double> getPrimalValues();
    std::vector<double> getDualPrices();

    bool solveLP();

};

#endif // PATTERNSOLVER_H
