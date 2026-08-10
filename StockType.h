#ifndef STOCKTYPE_H
#define STOCKTYPE_H

#include <string>

// StockType is identified by its position in the stocks vector
struct StockType {
    int length;
    int cost;
    int availabilty; // -1 for unlimited
    int sawWidth;
    std::string name;
};

#endif // STOCKTYPE_H
