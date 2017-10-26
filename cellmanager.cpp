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
            nodes[idcell]->links.push_back(new LinkCell(nodes[mld2],mld1,mld3*2,mld4) );
            nodes[mld2]->invLinks.push_back(LinkCellInv(nodes[idcell],nodes[idcell]->links.back()));

        } break;

        case 1:
        {
            nodes[idcell]->links.push_back(new LinkCell(nodes[mld2],mld1,mld3*2,mld4) );
            nodes[mld2]->invLinks.push_back(LinkCellInv(nodes[idcell],nodes[idcell]->links.back()));

        } break;

        case 2:
        {
            nodes[mld2]->links.push_back(new LinkCell(nodes[idcell],mld1,mld3*2,mld4) );
            nodes[idcell]->invLinks.push_back(LinkCellInv(nodes[mld2],nodes[mld2]->links.back()));
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
            ang+=0.025*0;
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

    static int idcmd=0,mvcmd=0,mvcell;
    static int idcell=0;
    GeneticBase *gb=NULL;

    static deque<GeneticBase*>::iterator it = cg->data.begin();

    static int ilim=0,limitinstruction=1024;
    while (mvcmd<255 && ilim++<limitinstruction)
    {
        it+=mvcmd;
        gb = *it;
        mvcmd = TranslateLLCode(gb,idcmd, idcell);
        mvcell = TranslateHLCode(gb, idcell);
        idcmd+=mvcmd;
        idcell+=mvcell;

        printf ("%i %s mvcmd %i mvcell %i\n", ilim, cmdnm[gb->data[0]], mvcmd,mvcell);
        if (mvcmd!=1) break;
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


        spring = (lc->cl - dsd)/2;
        if (lc->typ==2) spring/=16;

        //spring = spring<0 ? -spring*spring : spring*spring;
        //if (fabs(spring)>2) spring=spring<0 ? -2 : 2;

        tree->fx+=spring*dsx/dsd*dt/tree->mass;
        tree->fy+=spring*dsy/dsd*dt/tree->mass;

        lc->c->fx+=-spring*dsx/dsd*dt/lc->c->mass;
        lc->c->fy+=-spring*dsy/dsd*dt/lc->c->mass;

        if (lc->typ==0)
        {


            springtan = -(atan2(dsy,dsx)-lc->phi/80-(-tree->phi+0*lc->c->phi))/20;

            /*springtan = springtan<0 ? -springtan*springtan : springtan*springtan;
            if (fabs(springtan)>1) springtan=springtan<0 ? -1 : 1;*/

            tree->fx+=-springtan*dsy/dsd*dt/tree->mass;
            tree->fy+=springtan*dsx/dsd*dt/tree->mass;

            lc->c->fx+=springtan*dsy/dsd*dt/lc->c->mass;
            lc->c->fy+=-springtan*dsx/dsd*dt/lc->c->mass;
        }
    }
}

int CellManager::dynamicCompute(float dt, float f)
{
    //result of forces initialisation
    Cell *c,*d;
    LinkCell *lc;
    LinkCellInv *ilc;
    float dist,ux,uy;
    for (int i=0; i<nodes.size(); i++)
    {
        c=nodes[i];
        c->fx=0;
        c->fy=0;
        c->cpl=0;
        //c->fn=0;
    }

    //result of force recursive computation
    dynamicComputeRec(nodes[0],dt);

    //result of torque computation
    for (int i=0; i<nodes.size(); i++)
    {
        c=nodes[i];

        for (int j=0; j<c->links.size(); j++)
        {
            lc = c->links[j];
            if (lc->typ==0)
            {
            d=lc->c;

            ux=d->px-c->px;
            uy=d->py-c->py;

            c->cpl+=d->fx*uy-d->fy*ux;
            }
        }
        for (int j=0; j<c->invLinks.size(); j++)
        {
            ilc=&c->invLinks[j];
            lc=ilc->lc;
            if (lc->typ==0)
            {
            d=ilc->c;

            ux=d->px-c->px;
            uy=d->py-c->py;

            c->cpl+=d->fx*uy-d->fy*ux;
            }
        }
    }

    //update velocity and torque
    float fv;
    f*=dt;
    for (int i=0; i<nodes.size(); i++)
    {
        c=nodes[i];

        c->vx+=c->fx;
        c->vy+=c->fy;

        fv = sqrt(c->vx*c->vx+c->vy*c->vy)*f;
        c->vx-=c->vx*fv;c->vy-=c->vy*fv;
        c->px+=c->vx*dt;
        c->py+=c->vy*dt;

        c->dphi+=c->cpl/c->mass/10;
        c->phi+=d->dphi*dt;
        if (c->phi>M_PI)
            c->phi-=M_PI;
        if (c->phi<-M_PI)
            c->phi+=M_PI;
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
    for (j=0; j<tree->links.size(); j++)
    {
        lc = tree->links[j];
        forceCompute(tree, lc, dt);

        //if (tree->ok) //shit multiple pass.. use monte carlo mecanic... or heap of computecell order by magnitude of pertubation...
        if (lc->typ<2) //ok :) constraint skeleton to be a tree use grow and fork for develelopment
        {
            dynamicComputeRec(tree->links[j]->c,dt);
            //tree->ok=0;
        }
    }
}
