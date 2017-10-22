#ifndef CELLMANAGER_H
#define CELLMANAGER_H
#include "stdio.h"
#include "math.h"

#include "cell.h"
#include "geneticdata.h"

class CellManager : public GeneticLowlevelGrammar
{
public:

    vector<Cell*> nodes;

    CellManager();
    int TranslateHLCode(GeneticBase *gb, int idcell);
    void geneticDataFromRawCode(GeneticData *gd, vector<int8_t> rawcode);
    void makeLink(int idcell, int mld1, int mld2, int mld3, int mld4);
    int generateBody (GeneticData *cg);

    void forceCompute(Cell *tree, LinkCell *lc, float dt);
    int dynamicCompute (float dt, float f);
private:
    int dynamicComputeRec (Cell *tree, float dt);
};

#endif // CELLMANAGER_H
