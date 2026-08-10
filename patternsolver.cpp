#include "patternsolver.h"
#include <iostream>

PatternSolver::PatternSolver(const std::vector<Product>& products, const std::vector<StockType>& stocks) {
    this->products = products;
    this->stocks = stocks;
    this->lp = glp_create_prob();

    // objective dir:
    glp_set_obj_dir(lp, GLP_MIN);

    // add constraints
    glp_add_rows(lp, products.size() + stocks.size());

    // demand constraint
    for(int i = 1; i < products.size() + 1; i++) {
        glp_set_row_bnds(lp, i, GLP_LO, products[i-1].demand, 0.0);
    }

    // availability constraint
    for (int i = 0; i < stocks.size(); i++) {
        int row = products.size() + i + 1;

        if (stocks[i].availabilty < 0) {
            glp_set_row_bnds(lp, row, GLP_FR, 0.0, 0.0);
        } else {
            glp_set_row_bnds(
                lp,
                row,
                GLP_UP,
                0.0,
                stocks[i].availabilty
                );
        }
    }
}

PatternSolver::~PatternSolver(){
    glp_delete_prob(lp);
}

void PatternSolver::addPattern(const Pattern& pattern) {
    patterns.push_back(pattern);

    // add variable with objective
    int new_col = glp_add_cols(lp, 1);
    glp_set_col_bnds(lp, new_col, GLP_LO, 0.0, 0.0);
    glp_set_col_kind(lp, new_col, GLP_IV);
    glp_set_obj_coef(lp, new_col, stocks[pattern.stockIndex].cost);

    // add constraint matrix column
    int ia[1+products.size()+stocks.size()]; // row index
    double ar[1+products.size() + stocks.size()]; // value

    // demand
    for (int i = 1; i < products.size() + 1; i++) {
        ia[i] = i;
        ar[i] = pattern.quantities[i-1];
    }

    // availability
    ia[products.size() + 1] = products.size() + pattern.stockIndex + 1;
    ar[products.size() + 1] = 1.0;


    glp_set_mat_col(lp, new_col, products.size(), ia, ar);
}

std::vector<double> PatternSolver::getPrimalValues() {

    std::vector<double> primalValues;
    primalValues.resize(patterns.size());

    for(int i = 1; i < patterns.size() + 1; i++) {
        primalValues[i-1] = glp_mip_col_val(lp, i);
    }

    return primalValues;

}

std::vector<double> PatternSolver::getDualPrices() {

    std::vector<double> dualPrices;
    dualPrices.resize(products.size());

    for(int i = 1; i < products.size() + 1; i++) {
        dualPrices[i-1] = glp_get_row_dual(lp, i);
    }

    return dualPrices;
}

bool PatternSolver::solveRelaxed() {
    // solve relaxed:
    glp_simplex(lp, nullptr);

    return true;
}

bool PatternSolver::solveLP() {
    glp_simplex(lp, nullptr);

    glp_iocp parm;
    glp_init_iocp(&parm);
    glp_intopt(lp, &parm);

    return false;
}


