#ifndef CELL_H
#define CELL_H


#include "linkcell.h"

class Cell
{
public:

    vector<LinkCell> stator;
    vector<LinkCell> rotor;



    Cell();
};

#endif // CELL_H
