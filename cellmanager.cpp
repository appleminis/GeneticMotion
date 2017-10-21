#include "cellmanager.h"

CellManager::CellManager()
{
    nodes.reserve(16);
    nodes.push_back(new Cell(0));

}

int CellManager::makeLink(int idcell, int mld1, int mld2)
{
    switch (mld1)
    {
        case 0:
        {
            nodes[idcell]->stator.push_back( nodes[mld2] );

        } break;

        case 1:
        {
            nodes[idcell]->rotor.push_back( nodes[mld2] );

        } break;

        /*case 2:
        {

        } break;

        case 3:
        {

        } break;*/

        default://error
            return 0;
        break;
    }
    return 1;
}

int CellManager::TranslateHLCode(GeneticBase *gb, int *idcell)
{
    switch (gb->data[0])
    {
        case 2://generate new cell linked with active cell
        {
            Cell *nc=new Cell(*idcell);
            nodes.push_back(nc);
            return makeLink(*idcell,nodes.size()-1,gb->data[2]);
        }
        break;

        case 3://generate new link from active cell to cell (uint)*(data+2)
        {

            return makeLink(*idcell,gb->data[1],gb->data[2]);
        }
        break;

        case 4://move to cell at uint8_t(data+1)
        {
            *idcell=(int)(*(gb->data+1));
            return 1;
        }
        break;

        default: //low level command or error
            return (gb->data[0]<2 && gb->data[0]>=0);
        break;
    }

}

int CellManager::generateBody(GeneticData *cg)
{
    int idcmd=0,mvcmd=0;
    int idcell=0;
    GeneticBase *gb=NULL;

    while (mvcmd<255)
    {
        gb = cg->data.front();
        mvcmd = TranslateLLCode(gb,idcmd);
        idcmd+=mvcmd;
        TranslateHLCode(gb, &idcell);
    }

    return 1;
}
