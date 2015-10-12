#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

// dimensiunea ferestrei in pixeli
#define dim 300
#define PI 3.14159265

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

bool FLAG_CORRECT_TRIANGLE = false;
unsigned char prevKey;

enum axis {axisX, axisY, axisZ};
enum EObiect { cubw, cubs, sferaw, sferas, tr };
EObiect ob = tr;

int mouseX = 10, mouseY = 30;
float translatex = 0.0;
float translatey = 0.0;
float translatez = 0.0;

/*
	HELPERS
*/

struct point3d
{
	float x, y, z;
};

struct tr_coords
{
	point3d p1, p2, p3;
};
tr_coords myTriangle;


void InitTRCoords()
{
	/*
	myTriangle.p1.x = 1;
	myTriangle.p1.y = 2;
	myTriangle.p1.z = 2;

	myTriangle.p2.x = 2;
	myTriangle.p2.y = 2;
	myTriangle.p2.z = 5;

	myTriangle.p3.x = 4;
	myTriangle.p3.y = 5;
	myTriangle.p3.z = 5;
	*/
	myTriangle.p1.x = 13;
	myTriangle.p1.y = 0;
	myTriangle.p1.z = 0;

	myTriangle.p2.x = 16;
	myTriangle.p2.y = -3;
	myTriangle.p2.z = 6;

	myTriangle.p3.x = 10;
	myTriangle.p3.y = 0;
	myTriangle.p3.z = 3;

	/*
	myTriangle.p1.x = 1;
	myTriangle.p1.y = 4;
	myTriangle.p1.z = 1;

	myTriangle.p2.x = 4;
	myTriangle.p2.y = 1;
	myTriangle.p2.z = 1;

	myTriangle.p3.x = 1;
	myTriangle.p3.y = 1;
	myTriangle.p3.z = 4;
	*/
}

double getDistance(point3d a, point3d b)
{
	return sqrt(
		  (a.x - b.x)*(a.x - b.x) 
		+ (a.y - b.y)*(a.y - b.y) 
		+ (a.z - b.z)*(a.z - b.z));
}

point3d getRotationUpdatesMatrix(GLfloat matrix[16], point3d initial)
{
	point3d result;
	result.x = initial.x * matrix[0] + initial.y * matrix[4] + initial.z * matrix[8] + matrix[12];
	result.y = initial.x * matrix[1] + initial.y * matrix[5] + initial.z * matrix[9] + matrix[13];
	result.z = initial.x * matrix[2] + initial.y * matrix[6] + initial.z * matrix[10] + matrix[14];

	return result;
}

point3d getRotationUpdates(double angle, point3d initial, axis ax)
{
	point3d result;
	if (ax == axisX)
	{
		result.x = initial.x * 1 + initial.y * 0 + initial.z * 0;
		result.y = initial.x * 0 + initial.y * cos(angle) - initial.z * sin(angle);
		result.z = initial.x * 0 + initial.y * sin(angle) + initial.z * cos(angle);
	}
	else
	if (ax == axisY)
	{
		result.x = initial.x * cos(angle) + initial.y * 0 + initial.z * sin(angle);
		result.y = initial.x * 0 + initial.y * 1 + initial.z * 0;
		result.z = -1 * initial.x * sin(angle) + initial.y * 0 + initial.z * cos(angle);
	}
	else
	{
		result.x = initial.x * cos(angle) - initial.y * sin(angle) + initial.z * 0;
		result.y = initial.x * sin(angle) + initial.y * cos(angle) + initial.z * 0;
		result.z = initial.x * 0 + initial.y * 0 + initial.z * 1;
	}

	return result;
}

/*
REST OF THE CODE
*/


void DisplayAxe(int divide = 1) {
	int X, Y, Z;
	X = Y = 200;
	Z = 200;

	glLineWidth(2);

	// axa Ox - verde
	glColor3f(0, 1/divide, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(X, 0, 0);
	glEnd();

	// axa Oy - albastru
	glColor3f(0, 0, 1/divide);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, Y, 0);
	glEnd();

	// axa Oz - rosu
	glColor3f(1/divide, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, Z);
	glEnd();

	glLineWidth(1);
}

// cub solid
void Display2() {
	glColor3f(1, 0, 0);
	glutSolidCube(1);
}

// sfera wireframe
void Display3() {
	glColor3f(0, 0, 1);
	glutWireSphere(1, 10, 10);
}

// sfera solida
void Display4() {
	glColor3f(0, 0, 1);
	glutSolidSphere(1, 10, 10);
}

void drawLine(float x, float y, float z, float x2, float y2, float z2)
{
	glBegin(GL_LINES);
	glVertex3f(x, y, z);
	glVertex3f(x2, y2, z2);
	glEnd();
}

void drawCube()
{
	int top = 1;
	int bottom = 0;
	glBegin(GL_QUADS);        // Draw The Cube Using quads
		glColor3f(1.0f, 0.5f, 0.0f);    // Color Orange
			glVertex3f(top, top, bottom);
			glVertex3f(bottom, top, bottom);
			glVertex3f(bottom, top, top);  
			glVertex3f(top, top, top);    
		
			glVertex3f(top, bottom, top);    
			glVertex3f(bottom, bottom, top);    
			glVertex3f(bottom, bottom, bottom);   
			glVertex3f(top, bottom, bottom);    
		//glColor3f(1.0f, 0.0f, 0.0f);    // Color Red    
			glVertex3f(top, top, top);
			glVertex3f(bottom, top, top);
			glVertex3f(bottom, bottom, top);   
			glVertex3f(top, bottom, top);
		//glColor3f(1.0f, 1.0f, 0.0f);    // Color Yellow
			glVertex3f(top, bottom, bottom); 
			glVertex3f(bottom, bottom, bottom);    
			glVertex3f(bottom, top, bottom);    
			glVertex3f(top, top, bottom); 
		//glColor3f(0.0f, 0.0f, 1.0f);    // Color Blue
			glVertex3f(bottom, top, top);
			glVertex3f(bottom, top, bottom);  
			glVertex3f(bottom, bottom, bottom);    
			glVertex3f(bottom, bottom, top);    
		//glColor3f(1.0f, 0.0f, 1.0f);    // Color Violet
			glVertex3f(top, top, bottom);    
			glVertex3f(top, top, top);    
			glVertex3f(top, bottom, top); 
			glVertex3f(top, bottom, bottom);    
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	drawLine(bottom, bottom, bottom, bottom, bottom, top);
	drawLine(bottom, bottom, top, bottom, top, top);
	drawLine(bottom, top, top, bottom, top, bottom);
	drawLine(bottom, top, bottom, bottom, bottom, bottom);

	drawLine(bottom, top, top, top, top, top);
	drawLine(top, top, top, top, top, bottom);
	drawLine(top, top, bottom, bottom, top, bottom);

	drawLine(bottom, bottom, top, top, bottom, top);
	drawLine(top, bottom, top, top, bottom, bottom);
	drawLine(top, bottom, bottom, bottom, bottom, bottom);

	drawLine(top, top, top, top, bottom, top);
	drawLine(top, top, bottom, top, bottom, bottom);
}

// cub wireframe
void Display1() {
	glPushMatrix();
	glColor3f(1, 0, 0);
	
	if (FLAG_CORRECT_TRIANGLE)
	{
		double angle = 45;

		angle = 180;
		glRotated(angle, 1, 1, 1);

		DisplayAxe(5);
	}

	drawCube();
	glPopMatrix();
}

// sfera solida
void Display5() {
	glPushMatrix();
	glColor3f(0, 0, 1);

	if (FLAG_CORRECT_TRIANGLE)
	{
		//Get closest 2 points
		point3d sorted[3], origin1, origin2, aux, aux2, min, max, temp;
		point3d coord_axisX, coord_axisY, coord_axisZ;
		bool ok = true;
		float angle1 = 0, angle2 = 0, angle3 = 0;
		int multiplyBy = 1;
		float distance = 0;
		coord_axisX.x = coord_axisX.y = coord_axisX.z = 0;
		coord_axisY.x = coord_axisY.y = coord_axisY.z = 0;
		coord_axisZ.x = coord_axisZ.y = coord_axisZ.z = 0;
		coord_axisX.x = coord_axisY.y = coord_axisZ.z = 1;
		origin1.x = origin1.y = origin1.z = 0;
		origin2.x = origin2.y = origin2.z = 0;

		sorted[0] = myTriangle.p1;
		sorted[1] = myTriangle.p2;
		sorted[2] = myTriangle.p3;
		
		//bubblesort the points
		while (ok)
		{
			ok = false;
			for (int i = 0; i < 2; i++)
			{
				origin1.z = sorted[i].z;
				origin2.z = sorted[i + 1].z;
				if (getDistance(origin1, sorted[i]) > getDistance(origin2, sorted[i + 1]))
				{
					ok = true;
					aux = sorted[i];
					sorted[i] = sorted[i + 1];
					sorted[i + 1] = aux;
				}
			}
		}

		
		// PRIMA ROTATIE DUPA X
		multiplyBy = 1;
		if (sorted[0].y > sorted[1].y)
		{
			max = sorted[0];
			min = sorted[1];
		}
		else
		{
			max = sorted[1];
			min = sorted[0];
		}
		if (max.z < min.z)
		{
			multiplyBy = -1;
		}

		aux.x = aux2.x = 0;
		aux.y = max.y;
		aux2.y = min.y;
		aux.z = max.z;
		aux2.z = min.z;
		distance = getDistance(aux, aux2);
		if (distance == 0)
		{
			angle1 = 0;
		}
		else
		{
			angle1 = asin((max.y - min.y) / distance) * 180 / PI;
			angle1 = asin((max.y - min.y) / distance);
		}
		angle1 *= multiplyBy;
		glRotated(angle1 * 180 / PI, coord_axisX.x, coord_axisX.y, coord_axisX.z);

		sorted[0] = getRotationUpdates(angle1, sorted[0], axisX);
		sorted[1] = getRotationUpdates(angle1, sorted[1], axisX);
		sorted[2] = getRotationUpdates(angle1, sorted[2], axisX);
		coord_axisX = getRotationUpdates(-angle1, coord_axisX, axisX);
		coord_axisY = getRotationUpdates(-angle1, coord_axisY, axisX);
		coord_axisZ = getRotationUpdates(-angle1, coord_axisZ, axisX);
		
		
		// A DOUA ROTATIE DUPA Y
		multiplyBy = 1;
		if (sorted[0].x > sorted[1].x)
		{
			max = sorted[0];
			min = sorted[1];
		}
		else
		{
			max = sorted[1];
			min = sorted[0];
		}
		if (max.z > min.z)
		{
			multiplyBy = -1;
		}

		aux.y = aux2.y = 0;
		aux.x = sorted[0].x;
		aux2.x = sorted[1].x;
		aux.z = sorted[0].z;
		aux2.z = sorted[1].z;
		distance = getDistance(aux, aux2);
		if (distance == 0)
		{
			angle2 = 0;
		}
		else
		{
			angle2 = asin((max.x - min.x) / distance);
		}
		angle2 *= multiplyBy;
		glRotatef(angle2 * 180 / PI, coord_axisY.x, coord_axisY.y, coord_axisY.z);

		sorted[0] = getRotationUpdates(angle2, sorted[0], axisY);
		sorted[1] = getRotationUpdates(angle2, sorted[1], axisY);
		sorted[2] = getRotationUpdates(angle2, sorted[2], axisY);
		coord_axisX = getRotationUpdates(-angle2, coord_axisX, axisY);
		coord_axisY = getRotationUpdates(-angle2, coord_axisY, axisY);
		coord_axisZ = getRotationUpdates(-angle2, coord_axisZ, axisY);


		// ROTIREA DUPA Z
		multiplyBy = 1;
		if (sorted[2].x > sorted[0].x)
		{
			max = sorted[0];
			min = sorted[2];
		}
		else
		{
			max = sorted[2];
			min = sorted[0];
		}
		if (max.y < min.y)
		{
			multiplyBy = -1;
		}

		aux.z = aux2.z = 0;
		aux.x = sorted[0].x;
		aux2.x = sorted[2].x;
		aux.y = sorted[0].y;
		aux2.y = sorted[2].y;
		distance = getDistance(aux, aux2);
		if (distance == 0)
		{
			angle3 = 0;
		}
		else
		{
			angle3 = asin((max.x - min.x) / distance);
		}
		angle3 *= multiplyBy;
		glRotatef(angle3 * 180 / PI, coord_axisZ.x, coord_axisZ.y, coord_axisZ.z);

		sorted[0] = getRotationUpdates(angle3, sorted[0], axisZ);
		sorted[1] = getRotationUpdates(angle3, sorted[1], axisZ);
		sorted[2] = getRotationUpdates(angle3, sorted[2], axisZ);
		coord_axisX = getRotationUpdates(-angle3, coord_axisX, axisZ);
		coord_axisY = getRotationUpdates(-angle3, coord_axisY, axisZ);
		coord_axisZ = getRotationUpdates(-angle3, coord_axisZ, axisZ);

		//Translatia in 0
		temp = sorted[0];
		temp = getRotationUpdates(-angle3, temp, axisZ);
		temp = getRotationUpdates(-angle2, temp, axisY);
		temp = getRotationUpdates(-angle1, temp, axisX);
		
		glTranslated(-temp.x, -temp.y, -temp.z);
		
		/*
		sorted[1].x -= sorted[0].x;
		sorted[1].y -= sorted[0].y;
		sorted[1].z -= sorted[0].z;

		sorted[2].x -= sorted[0].x;
		sorted[2].y -= sorted[0].y;
		sorted[2].z -= sorted[0].z;

		sorted[0].x = sorted[0].y = sorted[0].z = 0;
		*/
		DisplayAxe(5);
	}

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(myTriangle.p1.x, myTriangle.p1.y, myTriangle.p1.z);
	glVertex3f(myTriangle.p2.x, myTriangle.p2.y, myTriangle.p2.z);
	glVertex3f(myTriangle.p3.x, myTriangle.p3.y, myTriangle.p3.z);
	glEnd();

	glPopMatrix();
	/*drawLine(1, 1, 0, 1, -1, 0);
	drawLine(2, 1, 0, 2, -1, 0);
	drawLine(3, 1, 0, 3, -1, 0);

	drawLine(0, 1, 1, 0, -1, 1);
	drawLine(0, 1, 2, 0, -1, 2);
	drawLine(0, 1, 3, 0, -1, 3);

	drawLine(1, 1, 0, -1, 1, 0);
	drawLine(1, 2, 0, -1, 2, 0);
	drawLine(1, 3, 0, -1, 3, 0);*/
}


void DisplayObiect()
{
	switch (ob)
	{
	case cubw:
		Display1();
		break;
	case cubs:
		Display2();
		break;
	case sferaw:
		Display3();
		break;
	case sferas:
		Display4();
		break;
	case tr:
		Display5();
		break;
	default:
		break;
	}
}

// rotatia cu un unghi de 10 grade in raport cu axa x
void DisplayX() {
	glMatrixMode(GL_MODELVIEW);
	glRotated(10, 1, 0, 0);
}

// rotatia cu un unghi de 10 grade in raport cu axa y
void DisplayY() {
	glMatrixMode(GL_MODELVIEW);
	glRotated(10, 0, 1, 0);
}

// rotatia cu un unghi de 10 grade in raport cu axa z
void DisplayZ() {
	glMatrixMode(GL_MODELVIEW);
	glRotated(10, 0, 0, 1);
}

// Translatia cu 0.2, 0.2, 0.2
void DisplayT() {
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.2, 0.2, 0.2);
}

// Scalarea cu 1.2, 1.2, 1.2
void DisplayS() {
	glMatrixMode(GL_MODELVIEW);
	glScalef(1.2, 1.2, 1.2);
}

// Scalarea cu 1.2, 1.2, 1.2
void DisplayMinusS() {
	glMatrixMode(GL_MODELVIEW);
	glScalef(0.8, 0.8, 0.8);
}

void Init(void) {
	InitTRCoords();

	glClearColor(1, 1, 1, 1);
	glLineWidth(2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, 30, -30);    // Setarea planului de decupare anterior - spatiu de coordonate de la -10,-10 la 10,10 pe XY

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(mouseX, 1, 0, 0);				   // Rotirea perspectivei initiale astfel incat sa nu vedem totul doar in planul oXY
	glRotated(-mouseY, 0, 1, 0);
}

void Display(void) {

	switch (prevKey)
	{
	case 'a':
		DisplayAxe();
		break;
	case 'c':
		FLAG_CORRECT_TRIANGLE = true;
		break;
	case 'i':
		translatex += 0.2;
		break;
	case 'o':
		translatey += 0.2;
		break;
	case 'p':
		translatez += 0.2;
		break;
	case '0':
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(mouseX, 1, 0, 0);
		glRotated(-mouseY, 0, 1, 0);
		break;
	case '1':
		//Display1();
		ob = cubw;
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(35, 1, 0, 0);
		glRotated(-45, 0, 1, 0);

		DisplayAxe();
		DisplayObiect();
		break;
	case '2':
		Display2();
		ob = cubs;
		break;
	case '3':
		Display3();
		ob = sferaw;
		break;
	case '4':
		Display4();
		ob = sferas;
		break;
	case '9':
		ob = tr;
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayAxe();
		DisplayObiect();
		break;
	case 'x':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayX();
		DisplayAxe();
		DisplayObiect();
		break;
	case 'y':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayY();
		DisplayAxe();
		DisplayObiect();
		break;
	case 'z':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayZ();
		DisplayAxe();
		DisplayObiect();
		break;
	case 't':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayT();
		DisplayAxe();
		DisplayObiect();
		break;
	case 's':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayS();
		DisplayAxe();
		DisplayObiect();
		break;
	case '-':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayMinusS();
		DisplayAxe();
		DisplayObiect();
		break;
	default:
		break;
	}
	glutSwapBuffers();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
	mouseX = x;
	mouseY = y;
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}