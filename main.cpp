#include <iostream>
#include <cmath>

#include "cellmanager.h"
#include "celldrawer.h"


// example dvlpt genetic code
vector<int8_t> rv={ 2,8,  2,8,  3,0,1,35,-35,  3,0,2,35,35,  2,10,  3,1,3,40,0, 2,12,  4,1,  3,2,-4,55,0,  4,1,  3,2,-4,55,0, 4,1,  3,0,1,12,0,  4,1,  1,36,-14,  0};

CellManager cellmanager;
GeneticData *geneticdata;
CellDrawer celldrawer;

void renderScene() // this function is called when you need to redraw the scene
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear the scene
    glMatrixMode(GL_MODELVIEW); // enter in model view matrix
    glLoadIdentity(); // load the identity matrix, to reset transformations
    gluLookAt(0,0,-500, 0,0,0, 0,1,0);
    for (int i=0; i<20; i++) cellmanager.dynamicCompute(0.1,0.00125);
    celldrawer.draw(&cellmanager,1);
    glutSwapBuffers();
}

// this function is called when the window is resized
void reshapeScene(int w, int h)
{
    glMatrixMode(GL_PROJECTION); // enter in projection matrix mode
    glLoadIdentity(); // load the identity, to reset transformations
    glViewport(0, 0, w, h); // set the viewport (window) size

    gluPerspective(90, w*1.0f/h, 0.1f, 10000);
    glMatrixMode(GL_MODELVIEW); // revert to model view
}

 void selectObject(int x, int y)
 {
    GLuint buff[64] = {0}; // the buffer
    GLint hits, view[4];

    glSelectBuffer(64, buff); // This choose the buffer where store the values for the selection data
    glGetIntegerv(GL_VIEWPORT, view); // This retrieve info about the viewport
    glRenderMode(GL_SELECT); // Switching in selecton mode
    glInitNames(); // This stack contains all the info about the objects
    glPushName(0); // Now fill the stack with one element (or glLoadName will generate an error)

    // Now modify the vieving volume, restricting selection area around the cursor
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    gluPickMatrix(x, y, 1.0, 1.0, view); // restrict the draw to an area around the cursor

    // [IMPORTANT] apply the same perspective in the reshapeFunc
    gluPerspective(20, view[2]*1.0f/view[3], 0.1f, 100);
    glMatrixMode(GL_MODELVIEW); // Draw the objects onto the screen

    glutSwapBuffers(); // draw only the names in the stack, and fill the array
    renderScene(); // render

    glMatrixMode(GL_PROJECTION); // Do you remeber? We do pushMatrix in PROJECTION mode
    glPopMatrix();

    hits = glRenderMode(GL_RENDER); // get number of objects drawed in that area and return to render mode

    // NOW
    // hits: number of hits
    // buff[ (hits*x)+3] the ID
    std::cout<<"Hits: "<<hits<<"\n"<<"ID: "<<buff[3]<<"\n";

    glMatrixMode(GL_MODELVIEW); // and revert again
 }

 void mouseHandler(int button, int state, int x, int y) // mouse handler callbacks
 {
    if 	((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
    {
        selectObject(x, y);
    }
 }

void init() // called to glEnable features or to init display lists
{
    geneticdata = new GeneticData();
    // asm parser
    cellmanager.geneticDataFromRawCode(geneticdata,rv);

    // asm interpreter
    cellmanager.generateBody(geneticdata);

    glEnable(GL_DEPTH_TEST); // to enable when you draw in 3D
}

void vIdle()
{
    glutPostRedisplay(); // force le réaffichage de la scène
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv); // init OpenGL
    glutIdleFunc(vIdle);		// Activation du callback

    glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH ); // set the display mode
    glutInitWindowSize(1000, 600); // set the window size
    glutCreateWindow("Base | OpenGL"); // set the window title

    glutDisplayFunc(renderScene); // set the callback to render
    glutReshapeFunc(reshapeScene); // set the callback to reshape

    glutMouseFunc(mouseHandler);

    init(); // init

    glutMainLoop(); // enter in a loop
}
