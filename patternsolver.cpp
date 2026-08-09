#include "patternsolver.h"
#include <iostream>

// TODO: constraint for stock availability

PatternSolver::PatternSolver(const std::vector<Product>& products, const std::vector<StockType>& stocks) {
    this->products = products;
    this->stocks = stocks;
    this->lp = glp_create_prob();

    // objective dir:
    glp_set_obj_dir(lp, GLP_MIN);

    // add constraints
    glp_add_rows(lp, products.size());
    for(int i = 1; i < products.size() + 1; i++) {
        glp_set_row_bnds(lp, i, GLP_LO, products[i-1].demand, 0.0);
    }

    // add variables
    /*
    glp_add_cols(lp, patterns.size());
    for(int i=1; i < patterns.size() + 1; i++) {
        glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0);
    }
    */

    // objective
    /*
    for(int i=1; i < patterns.size() + 1; i++) {
        glp_set_obj_coef(lp, i, 1.0);
    }
    */

    // constraint matrix
    /*
    int ia[1+products.size()*patterns.size()]; // row index
    int ja[1+products.size()*patterns.size()]; // column index
    double ar[1+products.size()*patterns.size()]; // value

    int k = 1;
    for(int i = 1; i < products.size() + 1; i++) {
        for(int j = 1; j < patterns.size() + 1; j++) {
            ia[k] = i;
            ja[k] = j;
            ar[k] = patterns[j-1].quantities[i-1];
            k++;
        }
    }

    glp_load_matrix(lp, products.size()*patterns.size(), ia, ja, ar);
    */
}

PatternSolver::~PatternSolver(){
    glp_delete_prob(lp);
}

void PatternSolver::addPattern(const Pattern& pattern) {

    std::cout << "Columns before: "
              << glp_get_num_cols(lp) << '\n';
    patterns.push_back(pattern);

    // add variable with objective
    int new_col = glp_add_cols(lp, 1);
    glp_set_col_bnds(lp, new_col, GLP_LO, 0.0, 0.0);
    glp_set_obj_coef(lp, new_col, 1.0);

    // add constraint matrix column
    int ia[1+products.size()]; // row index
    double ar[1+products.size()]; // value

    for (int i = 1; i < products.size() + 1; i++) {
        ia[i] = i;
        ar[i] = pattern.quantities[i-1];
    }

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
    glp_iocp parm;
    glp_init_iocp(&parm);
    glp_intopt(lp, &parm);

    return false;
}


