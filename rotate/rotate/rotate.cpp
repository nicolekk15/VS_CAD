#include <glut.h>
#include <math.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>

using namespace std;

#define PI 3.1415926
/* ÿ��ͼ�����Ķ����� */
#define MAX_VERTEX 10
/* ����ͼ�����ĸ��� */
#define MAX_PLOY 10
/* ���ڳ����һ�� */
int halfWidth, halfHeight;
/* ���ƶ���ε���ʼ��־��0�ǿ�ʼ���ƣ�1�ǽ������ƣ���ʼΪ-1 */
int drawStatus = -1;
int flag = 0;
char axis;

/* ����νṹ�� */
struct polygon {
	/* �������� */
	int x[MAX_VERTEX];
	int y[MAX_VERTEX];
	/* ����ڼ������� */
	int verNum;
	GLubyte color[3];
};
/* ������ɫ */
GLubyte nowColor[3] = { 0, 0, 0 };
GLubyte startBtn[3] = { 10, 10, 10 };
GLubyte endBtn[3] = { 20, 20, 20 };
GLubyte end1Btn[3] = { 30, 30, 30 };
/* ������������� */
polygon polygons[MAX_PLOY];
/* ��¼���˼�������� */
int con = 0;

/* ���ƶ���� */
void glPolygons()
{
	if (con >= 0) {
		for (int i = 0; i <= con; i++) {
			/* ȡ���������� */
			polygon poly = polygons[i];
			/* ������� */
			glBegin(GL_LINE_LOOP);
			int num = poly.verNum;
			//printf("num:%d\n", num);
			for (int j = 0; j < num; j++)
			{
				glColor3ub(poly.color[0], poly.color[1], poly.color[2]);
				//glVertex2d(poly.x[j] - halfWidth, halfHeight - poly.y[j]);
				glVertex3d(poly.x[j] - halfWidth, halfHeight - poly.y[j], 0);
				//printf("polyx:%d,polyy:%d", poly.x[j], poly.y[j]);
			}
			/* �������� */
			glEnd();
			/* ˢ�� */
			glFlush();;
		}
	}
}

void glPolygons_rotate()
{
	if (con >= 0) {
		for (int i = 0; i <= con; i++) {
			/* ȡ���������� */
			polygon poly = polygons[i];
			double delta_angle = PI / 180;
			/* ������� */
			//glBegin(GL_LINE_LOOP);
			int num = poly.verNum;
			//printf("num:%d\n", num);
			for (int j = 0; j < num; j++)
			{
				//��ʼ��������
				glBegin(GL_LINE_STRIP);
				//ÿ�λ����ӵĻ���
				//��Բ��
				double radius = abs(poly.y[j]);
				double vx = poly.x[j];
				double vy = 0;
				double vz = 0;
				if (axis == 'y')
				{
					radius = abs(poly.x[j]);
					vx = 0;
					vy = poly.y[j];
				}
				for (double i = 0; i <= 2 * PI; i += delta_angle)
				{
					if (axis == 'x')
					{
						glVertex3d(vx - halfWidth, halfHeight - vy + radius * cos(i), vz + radius * sin(i));
					}
					else
					{
						glVertex3d(vx - halfWidth + radius * cos(i), halfHeight - vy, vz + radius * sin(i));

					}
				}
				//�����滭
				glEnd();
			}
			for (double i = 0; i <= 2 * PI; i += delta_angle * 10)
			{
				glBegin(GL_LINE_LOOP);
				for (int j = 0; j < num; j++)
				{
					double tr = abs(poly.y[j]);
					if (axis == 'x')
					{
						glVertex3d(poly.x[j] - halfWidth, halfHeight + tr * cos(i), tr * sin(i));
					}
					else
					{
						tr = abs(poly.x[j]);
						glVertex3d(-halfWidth + tr * cos(i), -poly.y[j] + halfHeight, tr * sin(i));
					}
					//printf("polyx:%d,polyy:%d", poly.x[j], poly.y[j]);
				}
				glEnd();
			}
			/* �������� */
			//glEnd();

			/* ˢ�� */
			glFlush();;
		}
	}
}

/* ��������Բ�� */
void glColorCircle(int x, int y, int R, GLubyte color[3])
{
	/* ��ʼ�������� */
	glBegin(GL_POLYGON);
	/* ������ɫ */
	glColor3ub(color[0], color[1], color[2]);
	/* ÿ�λ����ӵĻ��� */
	double delta_angle = PI / 180;
	/* ��Բ�� */
	for (double i = 0; i <= 2 * PI; i += delta_angle)
	{
		/* ���Զ�λ�����Ǻ���ֵ */
		double	vx = x + R * cos(i);
		double	vy = y + R * sin(i);
		glVertex2d(vx, vy);
	}
	/* �����滭 */
	glEnd();
	glFlush();
}

//����������ͼ
void display(void)
{
	//GL_COLOR_BUFFER_BIT��ʾ�����ɫ
	glClear(GL_COLOR_BUFFER_BIT);
	glColorCircle(220, 280, 10, startBtn);
	glColorCircle(250, 280, 10, endBtn);
	glColorCircle(280, 280, 10, end1Btn);
	//��֤ǰ���OpenGL��������ִ�У��������������ڻ������еȴ� 
	/* ���ƶ���� */
	glPolygons();
	if (flag == 1) { glPolygons_rotate(); }
	glFlush();
}


//���ڴ�С�仯ʱ���õĺ���
void ChangeSize(GLsizei w, GLsizei h)
{
	//����߶�Ϊ0
	if (h == 0)
	{
		h = 1;
	}
	//�����ӿڴ�С�����һ��  
	glViewport(0, 0, w, h);
	int half = 300;
	/* �����ߵ�һ�� */
	halfHeight = 300;
	halfWidth = 300;
	//��������ϵͳ��ʹͶӰ�任��λ
	glMatrixMode(GL_PROJECTION);
	//����ǰ���û�����ϵ��ԭ���Ƶ�����Ļ����
	glLoadIdentity();
	//��������������  
	if (w < h)
	{
		//����߶ȴ��ڿ�ȣ��򽫸߶��ӽ�����ͼ����ʾ����
		glOrtho(-half, half, -half * h / w, half * h / w, -half, half);
	}
	else
	{
		//�����ȴ��ڸ߶ȣ��򽫿���ӽ�����ͼ����ʾ����
		glOrtho(-half * w / h, half * w / h, -half, half, -half, half);
	}

}
/* �ж�������ɫ�Ƿ���� */
bool sameColor(GLubyte color1[3], GLubyte color2[3])
{
	if (color1[0] - color2[0] < 5 && color1[1] - color2[1] < 5 && color1[2] - color2[1] < 5)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

/* ���� */
void glPoints(int x, int y) {
	glBegin(GL_POINTS);
	/* ��ֱ������Ϊ��ɫ */
	glColor3ub(0, 0, 0);
	glVertex2d(x - halfWidth, halfHeight - y);
	glEnd();
	glFlush();
}

/*������¼� */
void mouseClick(int btn, int state, int x, int y)
{
	/* ѡ��ķֽ緶Χ */
	int optionHeight = 40;
	int optionWidth = 250;
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		/* �������ɫѡ��е�� */
		if (y < optionHeight && x < optionWidth)
		{
			glReadPixels(x, 2 * halfHeight - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &nowColor);
			//printf("r:%d,g:%d,b:%d\n", nowColor[0], nowColor[1], nowColor[2]);
			/* ���û�ͼ��ɫ����ʾ��ǰȡɫ����ɫ */
			glColorCircle(-180, 280, 10, nowColor);
			/* �����������Ҳ�ѡ�ť */
		}
		else if (y < optionHeight && x > optionWidth) {
			//ȡ��ǰ�ĵ�
			GLubyte color[3];
			glReadPixels(x, 2 * halfHeight - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color);
			//printf("r:%d,g:%d,b:%d\n", color[0], color[1], color[2]);
			/* �������˿�ʼ���Ƶİ�ť */
			if (sameColor(color, startBtn)) {
				drawStatus = 0;
				/* ��ʼ��һ��ͼ�Σ������������ */
				polygons[con].verNum = 0;
				//printf("drawStatus:%d\n", drawStatus);
				/* �������˽������Ƶİ�ť */
			}
			else if (sameColor(color, endBtn)) {
				glutPostRedisplay();
				/* ����ͼ�θ�����һ */
				con++;
				//printf("drawStatus:%d\n", drawStatus);
			}
			/* �����������·��Ļ�ͼҳ�� */
			else if (sameColor(color, end1Btn)) {
				flag = 1;
				glutPostRedisplay();
				/* ����ͼ�θ�����һ */
				con++;
				//printf("drawStatus:%d\n", drawStatus);
			}
		}
		else {
			/* �����ǰ������ȡ��״̬ */
			if (drawStatus == 0)
			{
				/* ����ÿ���㣬Ȼ���ͼ�ζ��������һ */
				polygons[con].x[polygons[con].verNum] = x;
				polygons[con].y[polygons[con].verNum] = y;
				/* ���� */
				glPoints(x, y);
				/* ���õ�ǰ��ɫRGB�����ȡɫ�б䶯��������ȡɫΪ׼ */
				polygons[con].color[0] = nowColor[0];
				polygons[con].color[1] = nowColor[1];
				polygons[con].color[2] = nowColor[2];
				//printf("polyColor%d%d%d\n", polygons[con].color[0], polygons[con].color[1], polygons[con].color[2]);
				polygons[con].verNum++;
				//printf("con:%d,verNum:%d\n", con, polygons[con].verNum);
			}
		}
	}
}

void myKeyboard(unsigned char key, int x, int y)
{
	glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
	switch (key)
	{
	case 'a': case 'A': glTranslated(10, 0, 0); break;     //����ƽ��10
	case 'd': case 'D': glTranslated(-10, 0, 0); break;    //����ƽ��10
	case 'w': case 'W': glTranslated(0, 10, 0); break;     //����ƽ��10
	case 's': case 'S': glTranslated(0, -10, 0); break;    //����ƽ��10
	case 'q': case 'Q': glTranslated(0, 0, 10); break;     //����ƽ��10
	case 'e': case 'E': glTranslated(0, 0, -10); break;    //����ƽ��10

	case 'j': case 'J': glRotated(10, 0, 1, 0); break;      //��y����ת10��
	case 'l': case 'L': glRotated(10, 0, -1, 0); break;
	case 'i': case 'I': glRotated(10, 1, 0, 0); break;      //��x����ת10��
	case 'k': case 'K': glRotated(10, -1, 0, 0); break;
	case 'u': case 'U': glRotated(10, 0, 0, 1); break;      //��z����ת10��
	case 'o': case 'O': glRotated(10, 0, 0, -1); break;

	case 'z': case 'Z': glScalef(1.5, 1.5, 1.5); break;     //�Ŵ�
	case 'x': case 'X': glScalef(0.5, 0.5, 0.5); break;     //��С

	default: break;
	}

	glutPostRedisplay();      //���»���
}


//�������
int main(int argc, char* argv[])
{
	cout << "������x��y��";
	cin >> axis;

	glutInit(&argc, argv);						 //��GLUT���г�ʼ�������������е������в���
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //ָ��RGB��ɫģʽ�͵����崰��
	glutInitWindowPosition(100, 100);			 //���崰�ڵ�λ��
	glutInitWindowSize(600, 600);				 //���崰�ڵĴ�С
	glutCreateWindow("OpenGL_Rotate");			 //�������ڣ�ͬʱΪ֮����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		 //���ô��������ɫΪ��ɫ
	glutDisplayFunc(&display);					 //����Ϊһ����������ͼʱ��������ͻᱻ����
	glutReshapeFunc(ChangeSize);				 //����Ϊһ�������������ڴ�С�ı�ʱ�ᱻ����
	glutMouseFunc(mouseClick);					 //������¼�������������ɿ�ʱ���� 
	glutKeyboardFunc(myKeyboard);				 //�ú�����GLUT��ܿ�ʼ���У��������õĻص�������ʼ������ֱ���û���ֹ����Ϊֹ
	glutMainLoop();								 //���������Դ�����ͨ������Ϣ�ĺ�����֪����ϵͳ


	return 0;
}