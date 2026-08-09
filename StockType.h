#ifndef STOCKTYPE_H
#define STOCKTYPE_H

struct StockType {
    int id;
    int length;
    int cost;
    int availabilty; // -1 for unlimited
    int sawWidth;
};

#endif // STOCKTYPE_H
