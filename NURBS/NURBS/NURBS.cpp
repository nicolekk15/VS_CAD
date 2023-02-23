#include <glut.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CALLBACK
#define CALLBACK
#endif

GLfloat ctlpoints[4][4][3];
int showPoints = 0;

GLUnurbsObj* theNurb;

//初始化曲面控制点，控制点阈值[-3,+3]
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
	// 开启自动生成法线向量
	glEnable(GL_AUTO_NORMAL);
	// 规范化法线向量，不规范会有问题的
	glEnable(GL_NORMALIZE);

	// 1.生成控制点和创建NURBS对象
	init_surface();
	theNurb = gluNewNurbsRenderer();
	// 2.设置NURBS渲染属性和回调函数
	// 参数可以是GLU_DOMAIN_DISTANCE,那么需要GLU_U_STEP或GLU_V_STEP来指定u,v方向的采样点数量默认都是100.

	gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
	// GLU_PATH_LENGTH时最大边分格化距离，边长度超过该距离就会分割出更多顶点和轮廓。
	// GLU_PARAMETRIC_ERROR被分格化的多边形和他们近似模拟的表面之间的最大距离，超过则分格化的多边形会被分割。
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	// 如果在视景体外部那么不启用分格化，提高性能
	gluNurbsProperty(theNurb, GLU_CULLING, GLU_TRUE);
	// 从OGL服务器获取投影矩阵，模型视图矩阵和视口，如果是GLU_FALSE那么需要gluLoadSampliingMatrices来提供这些矩阵。
	gluNurbsProperty(theNurb, GLU_AUTO_LOAD_MATRIX, GLU_TRUE);

	// 获取属性值用gluGetNurbsProperty
	GLfloat cullMethod = 0.0f;
	gluGetNurbsProperty(theNurb, GLU_CULLING, &cullMethod);
	// 设置错误回调
	gluNurbsCallback(theNurb, GLU_ERROR, (void(__stdcall*) (void))nurbsError);
}

void myDisplay(void)
{
	// 每个控制点(节点)uv的上下界，从[0,1]类似求值器的插值指定
	GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
	int i, j;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(330.0, 1., 0., 0.);
	glScalef(0.5, 0.5, 0.5);
	// 3.开始绘制
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb,
		8, knots, 8, knots,
		4 * 3, 3, &ctlpoints[0][0][0],
		4, 4, GL_MAP2_VERTEX_3);

	gluNurbsSurface(theNurb,
		8, knots, 8, knots,
		4 * 3, 3, &ctlpoints[0][0][0],
		4, 4, GL_MAP2_NORMAL);
	// 完成曲线或曲面的绘制
	gluEndSurface(theNurb);
	// 曲线的绘制用glBeginCurve, glNurbsCurve glEndCurve来指定，参数含义同曲面。
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