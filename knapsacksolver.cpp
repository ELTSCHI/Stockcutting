#include "knapsacksolver.h"
#include <glpk.h>
#include <iostream>

KnapsackSolver::KnapsackSolver() {}

Pattern KnapsackSolver::solve(const std::vector<Product>& products, const StockType& stock, int stockIndex, const std::vector<double>& dualPrices) {
    glp_prob* lp = glp_create_prob();

    // objective dir:
    glp_set_obj_dir(lp, GLP_MAX);

    // add constraints
    glp_add_rows(lp, 1);
    glp_set_row_bnds(lp, 1, GLP_UP, 0.0, stock.length + stock.sawWidth);

    // add variables
    glp_add_cols(lp, products.size());
    for(int i=1; i < products.size() + 1; i++) {
        glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0);
        glp_set_col_kind(lp, i, GLP_IV);
    }

    // objective
    for(int i=1; i < products.size() + 1; i++) {
        glp_set_obj_coef(lp, i, dualPrices[i-1]);
    }

    // constraint matrix:
    int ia[1+products.size()];
    int ja[1+products.size()];
    double ar[1+products.size()];

    for(int i=1; i < products.size() + 1; i++) {
        ia[i] = 1;
        ja[i] = i;
        ar[i] = products[i-1].length + stock.sawWidth;
    }
    glp_load_matrix(lp, products.size(), ia, ja, ar);

    // solve ganzzahlig, simplex first:
    glp_simplex(lp, nullptr);
    glp_iocp parm;
    glp_init_iocp(&parm);
    glp_intopt(lp, &parm);


    // return solution
    std::vector<double> quantities;
    quantities.resize(products.size());

    for(int i = 1; i < products.size() + 1; i++) {
        quantities[i-1] = glp_mip_col_val(lp, i);
    }

    Pattern pattern;
    pattern.stockIndex = stockIndex;
    pattern.quantities = std::move(quantities);

    glp_delete_prob(lp);

    return pattern;
}

double KnapsackSolver::reducedCost(const Pattern& bestPattern, const StockType& stock, const std::vector<double>& dualPrices) {
    double value = 0.0;
    for(int i=0; i < dualPrices.size(); i++) {
        value += dualPrices[i] * bestPattern.quantities[i];
    }
    return stock.cost-value;
}
