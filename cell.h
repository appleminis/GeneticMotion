#ifndef CELL_H
#define CELL_H


#include "linkcell.h"

class Cell
{
public:

    vector<Cell*> stator;
    vector<Cell*> rotor;

    int id;
    float px,py,diameter;

    Cell(int _id,float _px,float _py,float _diameter) : id(_id),px(_px),py(_py),diameter(_diameter) {;}
};

#endif // CELL_H
