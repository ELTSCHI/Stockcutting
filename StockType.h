#ifndef STOCKTYPE_H
#define STOCKTYPE_H

// StockType is identified by its position in the stocks vector
struct StockType {
    int length;
    int cost;
    int availabilty; // -1 for unlimited
    int sawWidth;
};

#endif // STOCKTYPE_H
