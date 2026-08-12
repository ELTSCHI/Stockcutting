#ifndef PATTERNSOLVER_H
#define PATTERNSOLVER_H

#include "Product.h"
#include "StockType.h"
#include "Pattern.h"

#include <glpk.h>
#include <vector>


class PatternSolver
{
private:
    std::vector<Product> products;
    std::vector<StockType> stocks;
    std::vector<Pattern> patterns;
    glp_prob* lp;

public:
    PatternSolver(const std::vector<Product>& products, const std::vector<StockType>& stocks);
    ~PatternSolver();

    void addPattern(const Pattern & pattern);

    std::vector<Pattern>& getPatterns() {
        return patterns;
    }

    bool solveRelaxed();

    std::vector<double> getPrimalValues();
    std::vector<double> getDualPrices();

    bool solveLP();

};

#endif // PATTERNSOLVER_H
