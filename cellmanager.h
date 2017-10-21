#ifndef CELLMANAGER_H
#define CELLMANAGER_H

#include "cell.h"
#include "geneticdata.h"
#include "stdio.h"
#include "math.h"

class CellManager : public GeneticLowlevelGrammar
{
public:

    vector<Cell*> nodes;

    CellManager();

    int TranslateHLCode(GeneticBase *gb, int idcell);

    void geneticDataFromRawCode(GeneticData *gd, vector<int8_t> rawcode);
    void makeLink(int idcell, int mld1, int mld2);
    int generateBody (GeneticData *cg);
};

#endif // CELLMANAGER_H
