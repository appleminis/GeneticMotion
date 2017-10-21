#include "cellmanager.h"

CellManager::CellManager() : GeneticLowlevelGrammar()
{
    initLLcmdsz ();
    cmdnm.push_back("cellgen  ");
    cmdsz.push_back(3);
    cmdnm.push_back("celllink ");
    cmdsz.push_back(2);
    cmdnm.push_back("cellmv   ");
    cmdsz.push_back(1);

    nodes.reserve(16);
    nodes.push_back(new Cell(0,0.1,0.1,24));

}

void CellManager::makeLink(int idcell, int mld1, int mld2)
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
            //return 0;
        break;
    }
    //return 1;
}

int CellManager::TranslateHLCode(GeneticBase *gb, int idcell)
{
    switch (gb->data[0])
    {
        case 2://generate new cell linked with active cell
        {
            float apx=nodes[idcell]->px;
            float apy=nodes[idcell]->py;
            float ad=nodes[idcell]->diameter;
            double ang = gb->data[2]/64.0f*M_PI;
            float dia = gb->data[3]+ad+12;
            Cell *nc=new Cell(nodes.size(),apx+cos(ang)*(dia),apy+sin(ang)*(dia),12);
            nodes.push_back(nc);
            makeLink(idcell,gb->data[1],nodes.size()-1);
            return 0;
        }
        break;

        case 3://generate new link from active cell to cell (uint)*(data+2)
        {

            makeLink(idcell,gb->data[1],gb->data[2]);
            return 0;
        }
        break;

        case 4://move to cell at idcell+uint8_t(data+1)
        {
            return (int)(*(gb->data+1));
        }
        break;

        default: //low level command or error
            return 0;
        break;
    }

}

void CellManager::geneticDataFromRawCode(GeneticData *gd, vector<int8_t> rawcode)
{
    printf ("\nasm development code\n\n");
    int i=0,j,cmd,cms=1,szc=rawcode.size();
    int8_t *db;
    while (i<szc && cms>0)
    {
        cmd=rawcode[i];
        cms=cmdsz[cmd]+1;


        //std::cout << cmdnm[cmd] << " ";
        printf("%s ",cmdnm[cmd]);

        db = new int8_t[cms];
        for (j=0; j<cms; j++)
        {
            db[j]=rawcode[i++];
            //std::cout << db[j] << ", ";
            if (j) printf("%i, ",(uint8_t)db[j]);

        }
        //std::cout << std::endl;
        printf("\n");
        gd->data.push_back(new GeneticBase(db,cms));

    }
}

int CellManager::generateBody(GeneticData *cg)
{
    printf ("\nexecute development code\n\n");

    int idcmd=0,mvcmd=0,mvcell;
    int idcell=0;
    GeneticBase *gb=NULL;


    deque<GeneticBase*>::iterator it = cg->data.begin();

    int ilim=0,limitinstruction=64;
    while (mvcmd<255 && ilim++<limitinstruction)
    {
        it+=mvcmd;
        gb = *it;
        mvcmd = TranslateLLCode(gb,idcmd, idcell);
        mvcell = TranslateHLCode(gb, idcell);
        idcmd+=mvcmd;
        idcell+=mvcell;

        printf ("%s mvcmd %i mvcell %i\n", cmdnm[gb->data[0]], mvcmd,mvcell);

    }

    return 1;
}
