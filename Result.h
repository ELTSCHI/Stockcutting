#ifndef RESULT_H
#define RESULT_H

#include <PatternUsage.h>


struct Result {
    std::vector<PatternUsage> usages;
    int cost;
    int waste;
};

#endif // RESULT_H
