#ifndef CELLDRAWER_H
#define CELLDRAWER_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

#include "cellmanager.h"


class CellDrawer
{
public:

    void drawCell(Cell *cell)
    {
        glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_LINE_LOOP);
        for (int i=0; i<50; i++)
        {
            glVertex3f(cell->px+sin(i*M_PI*2/50.)*cell->diameter,cell->py+cos(i*M_PI*2/50.)*cell->diameter,0);
        }
        glEnd();

        glBegin(GL_LINE_LOOP);
        for (int i=0; i<50; i++)
        {
            glVertex3f(cell->px+sin(i*M_PI*2/50.)*cell->diameter/2,cell->py+cos(i*M_PI*2/50.)*cell->diameter/2,0);
        }
        glEnd();

    }

    void drawQuiver (Cell *c, Cell *d, float c1, float c2, float c3)
    {
        float px=d->px - c->px;
        float py=d->py - c->py;
        float pd=sqrt(px*px+py*py);
        px/=pd/c1;
        py/=pd/c1;

        glBegin(GL_LINES);

        glVertex3f(c->px,c->py,0.0f);
        glVertex3f(d->px-px*(2),d->py-py*(2),0.0f);



        glVertex3f(d->px-px,d->py-py,0.0f);        glVertex3f(d->px+py*c3-px*c2,d->py-px*c3-py*c2,0.0f);

        glVertex3f(d->px-px,d->py-py,0.0f);        glVertex3f(d->px-py*c3-px*c2,d->py+px*c3-py*c2,0.0f);


        glVertex3f(d->px+py*c3-px*c2,d->py-px*c3-py*c2,0.0f);
        glVertex3f(d->px-py*c3-px*c2,d->py+px*c3-py*c2,0.0f);

        glEnd();


    }

    void draw(CellManager *cm)
    {
        int i,j;
        float px,py,pd;
        Cell *c;
        for (i=0; i<cm->nodes.size(); i++)
        {
            c = cm->nodes[i];
            drawCell(c);

            glColor3f(1.0f,1.0f,1.0f);
            glBegin(GL_LINES);
            for (j=0; j<c->stator.size(); j++)
            {
                drawQuiver(c,c->stator[j]->c,4,2,2);

            }
            glEnd();

            for (j=0; j<c->rotor.size(); j++)
            {
                if (c->rotor[j]->typ==1)
                    glColor3f(0.2f,0.7f,0.7f);
                else
                    glColor3f(0.8f,0.2f,0.2f);


                drawQuiver(c,c->rotor[j]->c,12,2,1);

            }
        }
    }

    CellDrawer();
};

#endif // CELLDRAWER_H
