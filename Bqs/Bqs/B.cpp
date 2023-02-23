#include <windows.h>
#include <glut.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
GLfloat x_coord[100], y_coord[100];

int nPoints = 0;

int j = 0;

inline GLfloat x_convert(int x)
{
    return -5.0 + x / 249.0 * 10;
}

inline GLfloat y_convert(int y)
{
    return 5.0 - y / 249.0 * 10;
}

void init() {
    glClearColor(1, 1, 1, 0);

}
void myReshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45.0,(GLfloat)w/(GLfloat)h,1.0,50.0);
    if (w <= h)
        glOrtho(-5.0, 5.0, -5.0 * (GLfloat)h / (GLfloat)w, 5.0 * (GLfloat)h / (GLfloat)w, -5.0, 5.0);
    else
        glOrtho(-5.0 * (GLfloat)h / (GLfloat)w, 5.0 * (GLfloat)h / (GLfloat)w, -5.0, 5.0, -5.0, 5.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void B2(int n)  //B样条3次曲线
{
    float t, dt, t2, t3, f1, f2, f3, f4;
    dt = 1.0 / n;      // t runs from 0 to 1.
    glBegin(GL_LINE_STRIP);

    for (int j = 0; j < (nPoints - 2); j++)
        for (t = 0.0; t <= 1.0; t += dt)
        {
            f1 = (1.0 / 6) * ((-1) * t * t * t + 3 * t * t - 3 * t + 1);
            f2 = (1.0 / 6) * (3 * t * t * t - 6 * t * t + 4);
            f3 = (1.0 / 6) * ((-3) * t * t * t + 3 * t * t + 3 * t + 1);
            f4 = (1.0 / 6) * (t * t * t);
            glVertex2f(f1 * x_coord[j] + f2 * x_coord[j + 1]
                + f3 * x_coord[j + 2] + f4 * x_coord[j + 3],
                f1 * y_coord[j] + f2 * y_coord[j + 1]
                + f3 * y_coord[j + 2] + f4 * y_coord[j + 3]);
        }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINE_STRIP);
    glColor3f(1, 0, 0);
    for (int i = 0; i < nPoints; i++) {
        glVertex3f(x_coord[i], y_coord[i], 0.0);
    }
    glEnd();

    glColor3f(0, 0, 1);
    if (nPoints >= 4) {
        B2(20);
    }
    glFlush();

}

void handle_mouseclick(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        if (nPoints >= 4)  j++;

        //printf("%d (%d, %d) ==> (%f, %f)\n", nPoints, x, y, x_convert(x), y_convert(y));

        x_coord[nPoints] = x_convert(x);
        y_coord[nPoints] = y_convert(y);

        nPoints++;

        glutPostRedisplay();
    }
}

void mousemotion(int x, int y) {
    float min = 99999999;
    int index;
    x = x_convert(x);
    y = y_convert(y);
    int i;
    for (i = 0; i < nPoints; i++)
        if (min > (x - x_coord[i]) * (x - x_coord[i]) + (y - y_coord[i]) * (y - y_coord[i])) {
            min = (x - x_coord[i]) * (x - x_coord[i]) + (y - y_coord[i]) * (y - y_coord[i]);
            index = i;
        }
    x_coord[index] = x;
    y_coord[index] = y;

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("B样条曲线");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutMouseFunc(handle_mouseclick);
    glutMotionFunc(mousemotion);

    glutMainLoop();
    return 0;
}