#ifndef CELLMANAGER_H
#define CELLMANAGER_H

#include "cell.h"
#include "geneticdata.h"

class CellManager : public GeneticLowlevelGrammar
{
public:

    vector<Cell*> nodes;

    CellManager();

    int TranslateHLCode(GeneticBase *gb, int *idcell);


    int makeLink(int idcell, int mld1, int mld2);
    int generateBody (GeneticData *cg);
};

#endif // CELLMANAGER_H
