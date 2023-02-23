#include <glut.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CALLBACK
#define CALLBACK
#endif

GLfloat ctlpoints[4][4][3];
int showPoints = 0;

GLUnurbsObj* theNurb;

//��ʼ��������Ƶ㣬���Ƶ���ֵ[-3,+3]
void init_surface(void)
{
	int u, v;
	for (u = 0; u < 4; u++) {
		for (v = 0; v < 4; v++) {
			ctlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5);
			ctlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5);

			if ((u == 1 || u == 2) && (v == 1 || v == 2))
				ctlpoints[u][v][2] = 3.0;
			else
				ctlpoints[u][v][2] = -3.0;
		}
	}

}


void CALLBACK nurbsError(GLenum errorCode)
{
	const GLubyte* estring;

	estring = gluErrorString(errorCode);
	fprintf(stderr, "Nurbs Error: %s\n", estring);
	exit(0);
}

/*  Initialize material property and depth buffer.
*/
void init(void)
{
	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	// �����Զ����ɷ�������
	glEnable(GL_AUTO_NORMAL);
	// �淶���������������淶���������
	glEnable(GL_NORMALIZE);

	// 1.���ɿ��Ƶ�ʹ���NURBS����
	init_surface();
	theNurb = gluNewNurbsRenderer();
	// 2.����NURBS��Ⱦ���Ժͻص�����
	// ����������GLU_DOMAIN_DISTANCE,��ô��ҪGLU_U_STEP��GLU_V_STEP��ָ��u,v����Ĳ���������Ĭ�϶���100.

	gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
	// GLU_PATH_LENGTHʱ���߷ָ񻯾��룬�߳��ȳ����þ���ͻ�ָ�����ඥ���������
	// GLU_PARAMETRIC_ERROR���ָ񻯵Ķ���κ����ǽ���ģ��ı���֮��������룬������ָ񻯵Ķ���λᱻ�ָ
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	// ������Ӿ����ⲿ��ô�����÷ָ񻯣��������
	gluNurbsProperty(theNurb, GLU_CULLING, GLU_TRUE);
	// ��OGL��������ȡͶӰ����ģ����ͼ������ӿڣ������GLU_FALSE��ô��ҪgluLoadSampliingMatrices���ṩ��Щ����
	gluNurbsProperty(theNurb, GLU_AUTO_LOAD_MATRIX, GLU_TRUE);

	// ��ȡ����ֵ��gluGetNurbsProperty
	GLfloat cullMethod = 0.0f;
	gluGetNurbsProperty(theNurb, GLU_CULLING, &cullMethod);
	// ���ô���ص�
	gluNurbsCallback(theNurb, GLU_ERROR, (void(__stdcall*) (void))nurbsError);
}

void myDisplay(void)
{
	// ÿ�����Ƶ�(�ڵ�)uv�����½磬��[0,1]������ֵ���Ĳ�ֵָ��
	GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
	int i, j;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(330.0, 1., 0., 0.);
	glScalef(0.5, 0.5, 0.5);
	// 3.��ʼ����
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb,
		8, knots, 8, knots,
		4 * 3, 3, &ctlpoints[0][0][0],
		4, 4, GL_MAP2_VERTEX_3);

	gluNurbsSurface(theNurb,
		8, knots, 8, knots,
		4 * 3, 3, &ctlpoints[0][0][0],
		4, 4, GL_MAP2_NORMAL);
	// ������߻�����Ļ���
	gluEndSurface(theNurb);
	// ���ߵĻ�����glBeginCurve, glNurbsCurve glEndCurve��ָ������������ͬ���档
	if (showPoints) {
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				glVertex3f(ctlpoints[i][j][0],
					ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
	glFlush();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 3.0, 8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
	case 'C':
		showPoints = !showPoints;
		glutPostRedisplay();
		break;
	case 27:
		gluDeleteNurbsRenderer(theNurb);
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMainLoop();
	return 0;
}