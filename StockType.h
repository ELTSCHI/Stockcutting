#ifndef STOCKTYPE_H
#define STOCKTYPE_H

struct StockType {
    int id;
    int length;
    int cost;
    int availabilty; // -1 for unlimited; has to be -1 at the moment
};

#endif // STOCKTYPE_H
