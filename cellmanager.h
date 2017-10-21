#ifndef CELLMANAGER_H
#define CELLMANAGER_H

#include "cell.h"
#include "geneticdata.h"

class CellManager : public GeneticLowlevelGrammar
{
public:

    vector<Cell> nodes;

    CellManager();

    int TranslateHLCode(GeneticBase *gb, int pos);


    int generateBody (GeneticData cg);
};

#endif // CELLMANAGER_H
