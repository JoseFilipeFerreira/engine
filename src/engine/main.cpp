#include "engine/camera.hpp"
#include "engine/parser.hpp"
#include "utils/types.hpp"

#include <GL/gl.h>
#include <IL/il.h>
#include <iostream>
#include <sstream>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

// singletons
Camera camera;
Group group;
GroupBuffer group_buffer;
float TIME_SCALE = 1;
bool DEBUG = false;
bool PAUSE = false;

void changeSize(int w, int h) {
    if (h == 0) h = 1;
    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
}

void draw_axis() {
    const float size = 1000.0f;
    // axis
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-size, 0.0f, 0.0f);
    glVertex3f(size, 0.0f, 0.0f);
    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -size, 0.0f);
    glVertex3f(0.0f, size, 0.0f);
    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -size);
    glVertex3f(0.0f, 0.0f, size);
    glEnd();
}

auto frame_delta() -> float {
    static double elapsed = 0;
    auto new_elapsed = glutGet(GLUT_ELAPSED_TIME);
    auto frame_delta = new_elapsed - elapsed;
    elapsed = new_elapsed;
    return frame_delta;
}

void renderScene() {
    // calculate current elapsed
    static u64 elapsed = 0;
    auto delta = frame_delta();
    if (!PAUSE) elapsed += delta * TIME_SCALE;

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    camera.place_camera();

    if (DEBUG) {
        draw_axis();
        /*
        camera.with([&](auto const& pl, auto const& center) {
            glPushMatrix();
            glColor3f(0.0, 1.0, 0.0);
            glTranslatef(center.x(), center.y(), center.z());
            group_buffer.draw_model("models/sphere.3d");
            glPopMatrix();
        });
        */
    }

    group.draw_group(elapsed / 1000.0f, DEBUG, group_buffer);

    // calculate fps
    static u64 frame = 0;
    static float timebase = 0;
    static float fps = 0;
    frame++;
    u64 time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000.0f) {
        fps = frame * 1000.0f / (time - timebase);
        timebase = time;
        frame = 0;
    }

    // update window tittle
    std::stringstream title;
    title.precision(3);
    title << "CG-Engine ";
    title << camera;
    title << "| TIME SCALE: " << TIME_SCALE << "x ";
    title << "| FPS: " << fps << " ";
    if (PAUSE) title << "| PAUSED ";
    if (DEBUG) title << "| DEBUG ";

    glutSetWindowTitle(title.str().data());

    // End of frame
    glutSwapBuffers();
}

void react_key(unsigned char key, int x, int y) {
    camera.react_key(key, x, y);

    switch (key) {
        case 'g': // toggle debug mode
            DEBUG = !DEBUG;
            break;
        case 'p': // toggle pause
            PAUSE = !PAUSE;
            break;
        case '[': // slowdown time
            if (TIME_SCALE - 0.1 > 0.01) TIME_SCALE -= 0.1;
            break;
        case ']': // acelerate time
            TIME_SCALE += 0.1;
            break;
    }

    renderScene();
}

int main(int argc, char** argv) {
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
    glutKeyboardFunc(react_key);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    // glEnable(GL_LIGHTING);
    // glPolygonMode(GL_FRONT, GL_LINE);

    glewInit();

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(react_key);

    auto args = std::vector<const char*>(argv + 1, argv + argc);

    if (args.size() == 0) args.push_back("scenes/config.xml");

    std::vector<Group> parsed_groups;

    for (auto const& file : args) {
        try {
            parsed_groups.push_back(Parser(file, group_buffer));
        } catch (std::exception& e) {
            std::cout << "Error while parsing: " << file << "\n";
            std::cout << e.what() << '\n';
            return 1;
        }
    }

    group = Group(parsed_groups);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
