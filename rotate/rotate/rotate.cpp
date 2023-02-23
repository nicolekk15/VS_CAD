#include <glut.h>
#include <math.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>

using namespace std;

#define PI 3.1415926
/* 每个图形最多的顶点数 */
#define MAX_VERTEX 10
/* 画的图形最多的个数 */
#define MAX_PLOY 10
/* 窗口长宽的一半 */
int halfWidth, halfHeight;
/* 绘制多边形的起始标志，0是开始绘制，1是结束绘制，初始为-1 */
int drawStatus = -1;
int flag = 0;
char axis;

/* 多边形结构体 */
struct polygon {
	/* 顶点坐标 */
	int x[MAX_VERTEX];
	int y[MAX_VERTEX];
	/* 定义第几个顶点 */
	int verNum;
	GLubyte color[3];
};
/* 各种颜色 */
GLubyte nowColor[3] = { 0, 0, 0 };
GLubyte startBtn[3] = { 10, 10, 10 };
GLubyte endBtn[3] = { 20, 20, 20 };
GLubyte end1Btn[3] = { 30, 30, 30 };
/* 声明多边形数组 */
polygon polygons[MAX_PLOY];
/* 记录画了几个多边形 */
int con = 0;

/* 绘制多边形 */
void glPolygons()
{
	if (con >= 0) {
		for (int i = 0; i <= con; i++) {
			/* 取到这个多边形 */
			polygon poly = polygons[i];
			/* 画封闭线 */
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
			/* 结束画线 */
			glEnd();
			/* 刷新 */
			glFlush();;
		}
	}
}

void glPolygons_rotate()
{
	if (con >= 0) {
		for (int i = 0; i <= con; i++) {
			/* 取到这个多边形 */
			polygon poly = polygons[i];
			double delta_angle = PI / 180;
			/* 画封闭线 */
			//glBegin(GL_LINE_LOOP);
			int num = poly.verNum;
			//printf("num:%d\n", num);
			for (int j = 0; j < num; j++)
			{
				//开始绘制曲线
				glBegin(GL_LINE_STRIP);
				//每次画增加的弧度
				//画圆弧
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
				//结束绘画
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
			/* 结束画线 */
			//glEnd();

			/* 刷新 */
			glFlush();;
		}
	}
}

/* 绘制填充的圆形 */
void glColorCircle(int x, int y, int R, GLubyte color[3])
{
	/* 开始绘制曲线 */
	glBegin(GL_POLYGON);
	/* 设置颜色 */
	glColor3ub(color[0], color[1], color[2]);
	/* 每次画增加的弧度 */
	double delta_angle = PI / 180;
	/* 画圆弧 */
	for (double i = 0; i <= 2 * PI; i += delta_angle)
	{
		/* 绝对定位加三角函数值 */
		double	vx = x + R * cos(i);
		double	vy = y + R * sin(i);
		glVertex2d(vx, vy);
	}
	/* 结束绘画 */
	glEnd();
	glFlush();
}

//函数用来画图
void display(void)
{
	//GL_COLOR_BUFFER_BIT表示清除颜色
	glClear(GL_COLOR_BUFFER_BIT);
	glColorCircle(220, 280, 10, startBtn);
	glColorCircle(250, 280, 10, endBtn);
	glColorCircle(280, 280, 10, end1Btn);
	//保证前面的OpenGL命令立即执行，而不是让它们在缓冲区中等待 
	/* 绘制多边形 */
	glPolygons();
	if (flag == 1) { glPolygons_rotate(); }
	glFlush();
}


//窗口大小变化时调用的函数
void ChangeSize(GLsizei w, GLsizei h)
{
	//避免高度为0
	if (h == 0)
	{
		h = 1;
	}
	//定义视口大小，宽高一致  
	glViewport(0, 0, w, h);
	int half = 300;
	/* 定义宽高的一半 */
	halfHeight = 300;
	halfWidth = 300;
	//重置坐标系统，使投影变换复位
	glMatrixMode(GL_PROJECTION);
	//将当前的用户坐标系的原点移到了屏幕中心
	glLoadIdentity();
	//定义正交视域体  
	if (w < h)
	{
		//如果高度大于宽度，则将高度视角扩大，图形显示居中
		glOrtho(-half, half, -half * h / w, half * h / w, -half, half);
	}
	else
	{
		//如果宽度大于高度，则将宽度视角扩大，图形显示居中
		glOrtho(-half * w / h, half * w / h, -half, half, -half, half);
	}

}
/* 判断两个颜色是否相等 */
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

/* 画点 */
void glPoints(int x, int y) {
	glBegin(GL_POINTS);
	/* 点直接设置为黑色 */
	glColor3ub(0, 0, 0);
	glVertex2d(x - halfWidth, halfHeight - y);
	glEnd();
	glFlush();
}

/*鼠标点击事件 */
void mouseClick(int btn, int state, int x, int y)
{
	/* 选项卡的分界范围 */
	int optionHeight = 40;
	int optionWidth = 250;
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		/* 如果在颜色选项卡中点击 */
		if (y < optionHeight && x < optionWidth)
		{
			glReadPixels(x, 2 * halfHeight - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &nowColor);
			//printf("r:%d,g:%d,b:%d\n", nowColor[0], nowColor[1], nowColor[2]);
			/* 设置绘图颜色并显示当前取色板颜色 */
			glColorCircle(-180, 280, 10, nowColor);
			/* 如果点击的是右侧选项按钮 */
		}
		else if (y < optionHeight && x > optionWidth) {
			//取当前的点
			GLubyte color[3];
			glReadPixels(x, 2 * halfHeight - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color);
			//printf("r:%d,g:%d,b:%d\n", color[0], color[1], color[2]);
			/* 如果点击了开始绘制的按钮 */
			if (sameColor(color, startBtn)) {
				drawStatus = 0;
				/* 开始画一个图形，顶点个数置零 */
				polygons[con].verNum = 0;
				//printf("drawStatus:%d\n", drawStatus);
				/* 如果点击了结束绘制的按钮 */
			}
			else if (sameColor(color, endBtn)) {
				glutPostRedisplay();
				/* 画的图形个数加一 */
				con++;
				//printf("drawStatus:%d\n", drawStatus);
			}
			/* 如果点击的是下方的绘图页面 */
			else if (sameColor(color, end1Btn)) {
				flag = 1;
				glutPostRedisplay();
				/* 画的图形个数加一 */
				con++;
				//printf("drawStatus:%d\n", drawStatus);
			}
		}
		else {
			/* 如果当前是正在取点状态 */
			if (drawStatus == 0)
			{
				/* 保存每个点，然后该图形顶点个数加一 */
				polygons[con].x[polygons[con].verNum] = x;
				polygons[con].y[polygons[con].verNum] = y;
				/* 画点 */
				glPoints(x, y);
				/* 设置当前颜色RGB，如果取色有变动，以最后的取色为准 */
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
	case 'a': case 'A': glTranslated(10, 0, 0); break;     //向左平移10
	case 'd': case 'D': glTranslated(-10, 0, 0); break;    //向右平移10
	case 'w': case 'W': glTranslated(0, 10, 0); break;     //向上平移10
	case 's': case 'S': glTranslated(0, -10, 0); break;    //向下平移10
	case 'q': case 'Q': glTranslated(0, 0, 10); break;     //向外平移10
	case 'e': case 'E': glTranslated(0, 0, -10); break;    //向里平移10

	case 'j': case 'J': glRotated(10, 0, 1, 0); break;      //绕y轴旋转10度
	case 'l': case 'L': glRotated(10, 0, -1, 0); break;
	case 'i': case 'I': glRotated(10, 1, 0, 0); break;      //绕x轴旋转10度
	case 'k': case 'K': glRotated(10, -1, 0, 0); break;
	case 'u': case 'U': glRotated(10, 0, 0, 1); break;      //绕z轴旋转10度
	case 'o': case 'O': glRotated(10, 0, 0, -1); break;

	case 'z': case 'Z': glScalef(1.5, 1.5, 1.5); break;     //放大
	case 'x': case 'X': glScalef(0.5, 0.5, 0.5); break;     //缩小

	default: break;
	}

	glutPostRedisplay();      //重新绘制
}


//程序入口
int main(int argc, char* argv[])
{
	cout << "请输入x或y：";
	cin >> axis;

	glutInit(&argc, argv);						 //对GLUT进行初始化，并处理所有的命令行参数
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //指定RGB颜色模式和单缓冲窗口
	glutInitWindowPosition(100, 100);			 //定义窗口的位置
	glutInitWindowSize(600, 600);				 //定义窗口的大小
	glutCreateWindow("OpenGL_Rotate");			 //创建窗口，同时为之命名
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		 //设置窗口清除颜色为白色
	glutDisplayFunc(&display);					 //参数为一个函数，绘图时这个函数就会被调用
	glutReshapeFunc(ChangeSize);				 //参数为一个函数，当窗口大小改变时会被调用
	glutMouseFunc(mouseClick);					 //鼠标点击事件，鼠标点击或者松开时调用 
	glutKeyboardFunc(myKeyboard);				 //该函数让GLUT框架开始运行，所有设置的回调函数开始工作，直到用户终止程序为止
	glutMainLoop();								 //将被调用以处理普通按键消息的函数告知窗口系统


	return 0;
}