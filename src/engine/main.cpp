#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "model.hpp"

#include "../deps/rapidxml.hpp"
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <math.h>
#include<string>
#include<ostream>
#include<iostream>

using namespace std;
using namespace rapidxml;

vector<string> read_models() {
    ifstream file;
    file.open("config.xml");
    string txt, buf;
    while(getline(file, buf))
        txt.append(buf);
    xml_document<> doc;
    char* txtm = (char*) malloc(txt.length() + 1);
    strcpy(txtm, txt.c_str());
    doc.parse<0>(txtm);
    vector<string> res;
    for(xml_node<> *n = doc.first_node()->first_node(); n; n = n->next_sibling()) 
        res.push_back(n->first_attribute()->value());
    return res;
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
        glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void draw_axis(){
    // axis
    glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f( 100.0f, 0.0f, 0.0f);
        // Y Axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        // Z Axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f,  100.0f);
    glEnd();
}

float angle = 0;

void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(10.0,10.0,10.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

    glRotated(angle, 0, 1, 0);

    angle += 1;

    draw_axis();
    Models(read_models()).draw_models();

    // End of frame
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv) {

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");
		
    // Required callback registry 
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // put here the registration of the keyboard callbacks
//    glutKeyboardFunc(reactKey);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	
    // enter GLUT's main cycle
    glutMainLoop();
	
    return 1;
}
