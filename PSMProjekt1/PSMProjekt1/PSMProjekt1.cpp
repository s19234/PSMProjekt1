#include <iostream>
#include <cmath>
#include <stdlib.h>

#include <Windows.h>
#include "glut.h"
#include <cmath>
#include <ctime>
#include "struktury.h"

#define YS 1.0f
#define PI 3.1415

double rot = 0.0f;
int solver = 0;

void derivatives(Wektor* in, Wektor* out, Punkt* p)
{
	out[0] = in[1];
	out[1] = p->f * (1.0 / p->m);
}

void solveEuler(Punkt* p, float dt)
{
	p->v = p->v + p->f * (1.0 / p->m) * dt;
	p->r = p->r + p->v * dt;
}

void solveMidPoint(Punkt* p, float dt)
{

}

void solveRK4(Punkt* p, float dt)
{

}

struct SferaN
{
	Wektor r1;
	float r;
	float t;
	float color[3];
	SferaN* right;
};

Wektor g(0, -1.0, 0);
Punkt* root = NULL;
SferaN* sroot = NULL;
int loop = 0;

SferaN* SphereAlloc(float R, Wektor r1, float t, float c[3])
{
	SferaN* tmp = new SferaN();
	if (!tmp)
		return NULL;

	tmp->right = NULL;
	tmp->r = R;
	tmp->r1 = r1;
	tmp->t = t;
	tmp->color[0] = c[0];
	tmp->color[1] = c[1];
	tmp->color[2] = c[2];

	return tmp;
}

void SphereTest(SferaN* s, Punkt* p)
{
	float d;
	Wektor n;
	float z;
	d = (s->r1 - p->r).length();
	if (d - s->r < 0)
	{
		n = (s->r1 - p->r);
		n.normalize();
		z = d - s->r;
		p->r = p->r + n * z;

		Wektor vs, vn;
		vn = n * (n * p->v);
		vs = p->v - vn;
		p->v = (vs - vn * s->t);
	}
}

void AddSphere(SferaN* ro, float R, Wektor r1, float t, float c[3])
{
	SferaN* tmp;
	for (tmp = ro; tmp->right != NULL; tmp = tmp->right)
	{
		tmp->right = SphereAlloc(R, r1, t, c);
	}
}

Punkt* PointAlloc(float m, int flaga, Wektor r, Wektor v)
{
	Punkt* tmp = new Punkt();
	if (!tmp)
		return NULL;

	tmp->m = m;
	tmp->flag = flaga;
	tmp->r = r;
	tmp->v = v;
	tmp->right = NULL;
	return tmp;
}

void AddPoint(Punkt* ro, float m, int flag, Wektor r, Wektor v)
{
	Punkt* tmp;
	for (tmp = ro; tmp->right != NULL; tmp = tmp->right)
	{
		tmp->right = PointAlloc(m, flag, r, v);
	}
}

Wektor W_Euler(Wektor f, float h)
{
	return (f * h);
}

void calcDeriv(Punkt* p)
{

}

void Solver(Punkt* ro, float dt)
{
	Punkt* tmp;
	for (tmp = ro; tmp != NULL; tmp->right)
	{
		switch (solver)
		{
		case 0:
			solveEuler(tmp, dt);
			break;
		case 1:
			solveMidPoint(tmp, dt);
			break;
		case 2:
			solveRK4(tmp, dt);
			break;
		default:
			solveEuler(tmp, dt);
		}
	}
}

void Sily(Punkt* ro)
{
	Punkt* tmp;
	for (tmp = ro; tmp != NULL; tmp = tmp->right)
	{
		tmp->f = g * tmp->m;
	}
}

void AnimateScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Punkt* tmp;
	Solver(root, 0.005);
	Sily(root);
	rot += 0.01;
	
	glPointSize(5);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	for (tmp = root; tmp != NULL; tmp = tmp->right)
	{
		glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(tmp->r.getX() / 1.0, tmp->r.getX(), tmp->r.getZ() / 1.0);
		glutSolidSphere(0.011, 5, 5);
		glPopMatrix();
		SferaN* stmp;

		for (stmp = sroot; stmp != NULL; stmp = stmp->right)
		{
			SphereTest(stmp, tmp);
		}

		glBegin(GL_LINES);
		glColor3f(1, 0, 0);

		double x1, x2, y1, y2, z1, z2;
		x1 = tmp->r.getX();
		y1 = tmp->r.getY();
		x2 = tmp->v.getX();
		y2 = tmp->v.getY();
		z1 = tmp->r.getZ();
		z2 = tmp->v.getZ();

		glVertex3f(x1, y1, z1);
		glVertex3f(x1 + (x2) * 0.05, y1 + (y2) * 0.05, z1 + (z2) * 0.05);
		glEnd();
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	SferaN* stmp;
	for (stmp = sroot; stmp != NULL; stmp = stmp->right)
	{
		glPushMatrix();
		glTranslatef(stmp->r1.getX(), stmp->r1.getY(), stmp->r1.getZ());
		glColor3fv(stmp->color);

		glutSolidSphere(stmp->r, 50, 50);
		glPopMatrix();
	}

	glFlush();
	glutSwapBuffers();
	loop++;

	if (loop > 2000)
	{
		double vx, vy, vz;
		double zz = 0.01;

		for (tmp = root; tmp != NULL; tmp = tmp->right)
		{
			vx = 0.5 - std::rand() / (float)RAND_MAX;
			vy = 1.0 - std::rand() / (float)RAND_MAX;
			vz = 0.5 - std::rand() / (float)RAND_MAX;
			vz = 0.0f;
			vy *= YS;
			vy += zz;
			zz += 0.001;
			tmp->r.setX(0);
			tmp->r.setY(0);
			tmp->v = Wektor(vx, vy, vz);
		}

		loop = 0;
	}
}

void InitGraphics()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 90.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_EQUAL);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
}

void myReshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double pl = 1.0f;

	if(w <= h)
		glOrtho(-pl, pl, -pl * (GLfloat)h / (GLfloat)w, pl * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-pl * (GLfloat)w / (GLfloat)h, pl * (GLfloat)w / (GLfloat)h, -pl, pl, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'e':
		solver = 0;
		std::cout << "Solver --> Euler\n";
		break;
	case 'm':
		solver = 1;
		std::cout << "Solver --> MidPoint\n";
		break;
	case 'r':
		solver = 2;
		std::cout << "Solver --> RK4\n";
		break;
	}
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	double vx, vy, vz;
	int i;
	std::srand(std::time(NULL));
	double zz = 0.01;

	for (i = 1; i < 620; i++)
	{
		vx = 0.5 - std::rand() / (float)RAND_MAX;
		vy = 1.0 - std::rand() / (float)RAND_MAX;
		vz = 0.5 - std::rand() / (float)RAND_MAX;
		vz = 0.0f;

		vy *= YS;
		vy += zz;
		zz += 0.001;

		if (!root)
			root = PointAlloc(1, 0, Wektor(0, 0, 0), Wektor(vx, vy, vz));
		else
			AddPoint(root, 1, 0, Wektor(0, 0, 0), Wektor(vx, vy, vz));
	}
	zz = -0.2;
	for (i = 1; i < 140; i++)
	{
		AddPoint(root, 10, 0, Wektor(zz, 1, 0), Wektor(0, 0, 0));
		zz += 0.01;
	}

	float c1[] = { 0, 0, 1 };
	float c2[] = { 0, 1, 0 };
	float c3[] = { 1, 1, 0 };
	float c4[] = { 1, 0, 1 };
	float c5[] = { 0.6, 0.5, 1 };
	float c6[] = { 0.6, 0.5, 0.3 };

	sroot = SphereAlloc(0.5, Wektor(0, -0.5, 0), 0.9, c1);
	AddSphere(sroot, 0.1, Wektor(-0.5, 0.5, 0), 0.8, c2);
	AddSphere(sroot, 0.1, Wektor(0.5, 0.5, 0), 0.3, c3);
	AddSphere(sroot, 0.1, Wektor(1, 0.0), 0.3, c4);
	AddSphere(sroot, 0.15, Wektor(0, 0.7, 0), 0.9, c5);
	AddSphere(sroot, 0.2, Wektor(-1, -0.2, 0), 1.0, c6);

	Sily(root);

	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("GLUT example");

	InitGraphics();

	glutDisplayFunc(AnimateScene);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(myReshape);
	glutMainLoop();
}