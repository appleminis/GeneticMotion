#ifndef CELL_H
#define CELL_H

#include <vector>
using namespace std;

class Cell;

class LinkCell
{
public:
    int typ;//0 rigid 1 free 2 actif
    Cell *c;
    LinkCell(Cell *_c, int _typ) :c(_c),typ(_typ) {}
};

class Cell
{
public:

    vector<LinkCell*> stator;
    vector<LinkCell*> rotor;

    int id;
    float px,py,diameter;

    Cell(int _id,float _px,float _py,float _diameter) : id(_id),px(_px),py(_py),diameter(_diameter) {;}
};

#endif // CELL_H
