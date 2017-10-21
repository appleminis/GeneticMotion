#ifndef CELL_H
#define CELL_H


#include "linkcell.h"

class Cell
{
public:

    vector<Cell*> stator;
    vector<Cell*> rotor;

    int id;

    Cell(int _id) : id(_id) {;}
};

#endif // CELL_H
