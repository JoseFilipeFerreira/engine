#include "engine/camera.hpp"
#include "engine/parser.hpp"
#include "utils/types.hpp"

#include <GL/gl.h>
#include <iostream>
#include <sstream>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

// singletons
Camera camera;
Group group;
bool DEBUG = false;

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0) h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    camera.place_camera(DEBUG);

    group.draw_group();

    if (DEBUG) {
        static u64 frame = 0;
        static double timebase = 0;
        static double fps = 60;
        frame++;
        u64 time = glutGet(GLUT_ELAPSED_TIME);
        if (time - timebase > 1000) {
            fps = frame * 1000.0 / (time - timebase);
            timebase = time;
            frame = 0;
        }

        std::stringstream title;
        title.precision(2);
        title << "CG-Engine"
              << " | FPS: " << fps;

        glutSetWindowTitle(title.str().data());
    }
    else{
        glutSetWindowTitle("CG-Engine");
    }

    // End of frame
    glutSwapBuffers();
}

void react_key(unsigned char key, int x, int y) {
    camera.react_key(key, x, y);

    switch (key) {
        case 'g': // toggle debug mode
            DEBUG = ! DEBUG;
            break;
    }

    renderScene();
}

int main(int argc, char** argv) {
    auto sceneName = "scenes/config.xml";
    if (argc > 1) sceneName = argv[1];
    group = Group(sceneName);

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG-Engine");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
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
