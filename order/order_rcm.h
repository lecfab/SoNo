#ifndef ORDER_RCM_H
#define ORDER_RCM_H

// Reverse Cuthill–McKee algorithm
// https://en.wikipedia.org/wiki/Cuthill-McKee_algorithm

#include "../utils/tools.h"
struct Edgelist;

std::vector<ul> order_rcm(Edgelist &h);

#endif
