#include "engine/model.hpp"
#include "engine/camera.hpp"
#include "deps/rapidxml.hpp"

#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <math.h>
#include<string>
#include<ostream>
#include<iostream>

// singletons
Camera camera;
Models models;

std::vector<std::string> read_models(std::string fileName) {
    std::ifstream file;
    file.open(fileName);
    std::string txt, buf;
    while(getline(file, buf))
        txt.append(buf);
    rapidxml::xml_document<> doc;
    char* txtm = (char*) malloc(txt.length() + 1);
    strcpy(txtm, txt.c_str());
    doc.parse<0>(txtm);
    std::vector<std::string> res;
    for(rapidxml::xml_node<> *n = doc.first_node()->first_node(); n; n = n->next_sibling()) 
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

void renderScene() {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    camera.place_camera();

    draw_axis();
    models.draw_models();

    // End of frame
    glutSwapBuffers();
}

void react_key(unsigned char key, int x, int y){
    camera.react_key(key, x, y);
    renderScene();
}

int main(int argc, char** argv) {
    std::string sceneName = "scenes/config.xml";
    if(argc > 1)
        sceneName = std::string(argv[1]);
    models = Models(read_models(sceneName));

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
    glutKeyboardFunc(react_key);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	
    // enter GLUT's main cycle
    glutMainLoop();
	
    return 1;
}
