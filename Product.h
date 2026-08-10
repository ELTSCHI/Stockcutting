#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

// Product is identified by its position in the products vector
struct Product {
    int length;
    int demand;
    std::string name;
};

#endif // PRODUCT_H
