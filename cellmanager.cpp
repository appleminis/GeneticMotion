#include "cellmanager.h"

CellManager::CellManager()
{

}

int CellManager::TranslateHLCode(GeneticBase *gb, int pos)
{
    return TranslateLLCode(gb,pos);
}

int CellManager::generateBody(GeneticData cg)
{
    return 1;
}
