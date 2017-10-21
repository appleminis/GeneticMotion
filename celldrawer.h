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

    void draw(CellManager *cm)
    {
        int i,j;
        Cell *c;
        for (i=0; i<cm->nodes.size(); i++)
        {
            c = cm->nodes[i];
            drawCell(c);

            glColor3f(1.0f,1.0f,1.0f);
            glBegin(GL_LINES);
            for (j=0; j<c->stator.size(); j++)
            {
                glVertex3f(c->px,c->py,0.0f);
                glVertex3f(c->stator[j]->px,c->stator[j]->py,0.0f);
            }
            glEnd();

            glColor3f(1.0f,0.0f,0.0f);
            glBegin(GL_LINES);
            for (j=0; j<c->rotor.size(); j++)
            {
                glVertex3f(c->px,c->py,0.0f);
                glVertex3f(c->rotor[j]->px,c->rotor[j]->py,0.0f);
            }
            glEnd();
        }



        /*int i;
        for (i=0; i<cell->stator.size(); i++)
        {
            draw(cm->cell->stator[i]);
        }

        for (i=0; i<cell->rotor.size(); i++)
        {
            draw(cell->rotor[i]);
        }*/
    }

    CellDrawer();
};

#endif // CELLDRAWER_H
