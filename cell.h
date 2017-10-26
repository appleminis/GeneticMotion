#ifndef CELL_H
#define CELL_H

#include <vector>
using namespace std;

class Cell; //Cell is a linked bearing

class LinkCell
{
public:
    int typ;        //0 rigid 1 free 2 actif
    float cl;       //size link consign
    float phi,dphi; //dynamic : angle, d/dt angle
    float l,dl;     //dynamic : sizelink, d/dt sizelink

    Cell *c;        //Cell *dst;
    LinkCell(Cell *_c, int _typ, float _cl, float _phi) :c(_c),typ(_typ),cl(_cl),phi(_phi) {l=cl;dl=dphi=0;}
};

class LinkCellInv
{
public :
    Cell *c;
    LinkCell *lc;

    LinkCellInv(Cell *_c, LinkCell *_lc) : c(_c),lc(_lc) {}
};


class Cell
{
public:

    int id;

    vector<LinkCell*> links;
    vector<LinkCellInv> invLinks;

    float px,py,vx,vy,phi,dphi,diameter,mass; //dynamic parameters
    float fx,fy,fn,cpl;    //result of the forces

    Cell(int _id,float _px,float _py,float _diameter) : id(_id),px(_px),py(_py),diameter(_diameter) {vx=vy=phi=dphi=0;mass=diameter*diameter;}
};

#endif // CELL_H
