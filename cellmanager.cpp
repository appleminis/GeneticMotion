#include "cellmanager.h"

CellManager::CellManager() : GeneticLowlevelGrammar()
{
    initLLcmds ();
    cmdnm.push_back("cellgen  ");
    cmdsz.push_back(1);
    cmdnm.push_back("celllink ");
    cmdsz.push_back(4);
    cmdnm.push_back("cellmv   ");
    cmdsz.push_back(1);

    nodes.reserve(16);
    nodes.push_back(new Cell(0,0,0,24));

}

void CellManager::makeLink(int idcell, int mld1, int mld2, int mld3, int mld4)
{

    if (mld2<0) return;
    if (mld2>=nodes.size()) return;


    switch (mld1)
    {
        case 0:
        {
            nodes[idcell]->stator.push_back(new LinkCell(nodes[mld2],mld1,mld3*2,mld4) );

        } break;

        case 1:
        {
            nodes[idcell]->rotor.push_back(new LinkCell(nodes[mld2],mld1,mld3*2,mld4) );

        } break;

        case 2:
        {
            nodes[idcell]->rotor.push_back(new LinkCell(nodes[mld2],mld1,mld3*2,mld4) );

        } break;

        /*case 3:
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
    static double ang = 0;


    switch (gb->data[0])
    {
        case 2://generate new cell
        {
            float apx=nodes[idcell]->px;
            float apy=nodes[idcell]->py;
            float ad=nodes[idcell]->diameter;
            float dia = (gb->data[1]+ad)*1.5;
            ang+=0.1;
            Cell *nc=new Cell(nodes.size(),apx+cos(ang)*dia,apy+sin(ang)*dia,gb->data[1]);
            nodes.push_back(nc);
            return 0;
        }
        break;

        case 3://generate new link from active cell to cell (uint)*(data+2)
        {
            makeLink(idcell,gb->data[1],idcell+gb->data[2],gb->data[3],gb->data[4]);

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
    printf ("\nraw development src\n\n");
    for (auto i = rawcode.begin(); i != rawcode.end(); ++i)
        printf("%i ",*i);
    printf("\n\n");

    int i=0,j,cmd,cms=1,szc=rawcode.size();
    int8_t *db;
    while (i<szc && cms>0)
    {
        cmd=rawcode[i];
        cms=cmdsz[cmd]+1;
        db = new int8_t[cms];
        printf("%s ",cmdnm[cmd]);
        for (j=0; j<cms; j++)
        {
            db[j]=rawcode[i++];
            if (j) printf("%i, ",(uint8_t)db[j]);

        }
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

    int ilim=0,limitinstruction=256;
    while (mvcmd<255 && ilim++<limitinstruction)
    {
        it+=mvcmd;
        gb = *it;
        mvcmd = TranslateLLCode(gb,idcmd, idcell);
        mvcell = TranslateHLCode(gb, idcell);
        idcmd+=mvcmd;
        idcell+=mvcell;

        printf ("%i %s mvcmd %i mvcell %i\n", ilim, cmdnm[gb->data[0]], mvcmd,mvcell);
        //if (mvcmd!=1)for (int i=0; i<100; i++) dynamicCompute(0.01,0.0001);

    }

    return 1;
}

void CellManager::forceCompute(Cell *tree, LinkCell *lc, float dt)
{
    int j;
    float cl,l,dl,psx,psy,pdx,pdy,dsd2,dsd,dsx,dsy;
    float spring,springtan;

    psx=tree->px;
    psy=tree->py;
    if (lc->typ<3)
    {

        pdx=lc->c->px;          pdy=lc->c->py;
        dsx=psx-pdx;            dsy=psy-pdy;

        dsd2=dsx*dsx+dsy*dsy;   dsd=sqrt(dsd2);


        spring = (lc->cl - dsd)/4;
        if (lc->typ==2) spring/=4;

        //spring = spring<0 ? -spring*spring : spring*spring;

        if (fabs(spring)>2) spring=spring<0 ? -2 : 2;

        tree->vx+=spring*dsx/dsd*dt/tree->mass;
        tree->vy+=spring*dsy/dsd*dt/tree->mass;

        lc->c->vx+=-spring*dsx/dsd*dt/lc->c->mass;
        lc->c->vy+=-spring*dsy/dsd*dt/lc->c->mass;

        if (lc->typ<2)
        {
            springtan = -(atan2(dsy,dsx)*180/M_PI-lc->phi)/8;
            if (lc->typ==1) springtan/=64;

            springtan = springtan<0 ? -springtan*springtan : springtan*springtan;

            if (fabs(springtan)>1) springtan=springtan<0 ? -1 : 1;


            tree->vx+=-springtan*dsy/dsd*dt/tree->mass;
            tree->vy+=springtan*dsx/dsd*dt/tree->mass;

            lc->c->vx+=springtan*dsy/dsd*dt/lc->c->mass;
            lc->c->vy+=-springtan*dsx/dsd*dt/lc->c->mass;
        }
    }
}

int CellManager::dynamicCompute(float dt, float f)
{
    dynamicComputeRec(nodes[0],dt);
    float fv;
    f*=dt;
    Cell *c;
    for (int i=0; i<nodes.size(); i++)
    {
        c=nodes[i];
        fv = sqrt(c->vx*c->vx+c->vy*c->vy)*f;
        c->vx-=c->vx*fv;c->vy-=c->vy*fv;

        c->px+=c->vx*dt;
        c->py+=c->vy*dt;
    }
}



int CellManager::dynamicComputeRec(Cell *tree, float dt)
{
    int j;
    float cl,l,dl,psx,psy,pdx,pdy,dsd2,dsd,dsx,dsy;
    float spring,springtan;
    LinkCell *lc;
    psx=tree->px;
    psy=tree->py;
    for (j=0; j<tree->stator.size(); j++)
    {
        lc = tree->stator[j];
        forceCompute(tree, lc, dt);
        dynamicComputeRec(tree->stator[j]->c,dt);
    }

    for (j=0; j<tree->rotor.size(); j++)
    {
        lc = tree->rotor[j];
        forceCompute(tree, lc, dt);
        dynamicComputeRec(tree->rotor[j]->c,dt);
    }
}
