#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

// dimensiunea ferestrei in pixeli
#define initial_dim				500
#define WRITE_PIXEL_NR_POINTS	50
#define WRITE_PIXEL_CLOCKWISE	false

#define GRID_SQUARE_WIDTH 15
#define WRITE_PIXEL_SIZE	4  //Marimea unui pixel
#define GRID_LINE_WIDTH		1  //Grosimea liniilor din grila
#define WITNESS_LINE_WIDTH	2  //Grosimea liniilor martor din grila
							   //la desenarea liniilor


struct POINT
{
	double x, y;
};
typedef void(*PlottingFunction)(vector<POINT> *, int, int);

/*
===========================
		GLOBALS
===========================
*/

unsigned int dimW = initial_dim;
unsigned int dimH = initial_dim;

unsigned char prevKey;


/*
===========================
		HELPERS
===========================
*/

/*
Functii de transformare din Sistemul de coordonate de tip grila
ce acopera toti pixelii canvas-ului in sistemul de coordonate clasic
(cel de la -1 la 1) si invers
*/
double CoordXGridToNormal(int x)
{
	return (double)((double)x * 2.0 / dimW) - 1.0;
}

double CoordYGridToNormal(int y)
{
	return (double)(((double)y * 2.0 / dimH) - 1.0) * -1;
}

int CoordXNormalToGrid(double x)
{
	return (x+1) * dimW / 2;
}

int CoordYNormalToGrid(double y)
{
	return ((y+1) * dimH / 2) * -1;
}

float round(float f, float prec)
{
	return (float)(floor(f*(1.0f / prec) + 0.5) / (1.0f / prec));
}



//SPIRALA LOGARITMICA 
void functionToPlot(vector<POINT> * returns, int maxW, int maxH)
{
	double tmax = 100;
	double ratia = 0.05;

	POINT * p;

	for (double t = 0; t < tmax; t += ratia)
	{
		double y1, x1, r;

		r = 0.02 * powf(M_E, 1 + t);
		x1 = r * cos(t);
		y1 = r * sin(t) * -1;

		p = new POINT();
		p->x = (x1 + 1) * maxW / 2;
		p->y = (y1 + 1) * maxH / 2;
		returns->insert(returns->end(), *p);
	}
}


/*
===========================
		GRILA CLASS
===========================
*/


class Grila
{
public:
	Grila(int linii, int coloane, int width, int height, int startX, int startY);
	void draw();
	void plot(PlottingFunction f);
	void writePixel(int x, int y, float raza);
	void writeGridPixel(int x, int y, float raza);
	void afisareDreapta(POINT s, POINT f, int size);
	void drawLineLayer(int sx, int sy, int size, bool vertical);

	int toBaseGridX(int);
	int toBaseGridY(int);
	
private:
	int ratieW, ratieH;
	int linii, coloane;
	int maxW, maxH;
	int startX, startY;
	int finishX, finishY;
};


Grila::Grila(int linii, int coloane, int maxW, int maxH, int startX, int startY)
{
	this->linii = linii;
	this->coloane = coloane;

	this->maxW = maxW;
	this->maxH = maxH;

	this->ratieH = maxH / linii;
	this->ratieW = maxW / coloane;

	this->startX = startX;
	this->startY = startY;

	if (this->maxW % this->ratieW != 0)
	{
		this->maxW -= (this->maxW % this->ratieW);
	}

	if (this->maxH % this->ratieH != 0)
	{
		this->maxH -= (this->maxH % this->ratieH);
	}

	this->finishX = this->startX + this->maxW;
	this->finishY = this->startY + this->maxH;
}

void Grila::draw()
/*
Deseneaza grila
*/
{
	glLineWidth(GRID_LINE_WIDTH);
	for (int coord = this->startY; coord <= this->finishY; coord += this->ratieH)
	{
		glBegin(GL_LINE_STRIP);
		/* Horizontal line */
		glVertex2f(CoordXGridToNormal(this->startX), CoordYGridToNormal(coord));
		glVertex2f(CoordXGridToNormal(this->finishX), CoordYGridToNormal(coord));
		glEnd();
	}

	for (int coord = this->startX; coord <= this->finishX; coord += this->ratieW)
	{
		glBegin(GL_LINE_STRIP);
		/* Vertical line */
		glVertex2f(CoordXGridToNormal(coord), CoordYGridToNormal(this->startY));
		glVertex2f(CoordXGridToNormal(coord), CoordYGridToNormal(this->finishY));
		glEnd();
	}
	glLineWidth(1);
}

void Grila::writePixel(int x, int y, float raza)
/*
Deseneaza un cerc la anumite coordonate in sistemul de coordonate
de tip grila pe pixeli
*/
{
	x += this->startX;
	y += this->startY;

	if (!(x >= this->startX
		&& x <= this->finishX
		&& y >= this->startY
		&& y <= this->finishY))
		return;

	float stX = x;
	float stY = y;
	float angle, inx, iny;
	int i;

	i = 0;
	if (WRITE_PIXEL_CLOCKWISE)
		i = WRITE_PIXEL_NR_POINTS;

	glBegin(GL_POLYGON);
	while (1)
	{
		angle = 2.0f * M_PI * float(i) / float(WRITE_PIXEL_NR_POINTS); //(2 * PI * al x-lea punct)
		inx = round(cos(angle) * raza + stX, 0.001);
		iny = round(sin(angle) * raza + stY, 0.001);
		glVertex2f(CoordXGridToNormal(inx), CoordYGridToNormal(iny));

		if (WRITE_PIXEL_CLOCKWISE)
		{
			i--;
			if (i == 0)
				break;
		}
		else
		{
			i++;
			if (i == WRITE_PIXEL_NR_POINTS)
				break;
		}
	}
	glEnd();

}

void Grila::writeGridPixel(int x, int y, float raza)
/*
Deseneaza un "pixel" de o anumita raza in grila
*/
{
	this->writePixel(x * this->ratieW, y * this->ratieH, raza);
}

int Grila::toBaseGridX(int x)
/*
Transforma din coordonate in grila
in coordonate in grila pixelilor pe axa X
*/
{
	return this->startX + x * this->ratieW;
}

int Grila::toBaseGridY(int y)
/*
	Transforma din coordonate in grila
	in coordonate in grila pixelilor pe axa Y
*/
{
	return this->startY + y * this->ratieH;
}

void Grila::plot(PlottingFunction f)
/*
	Deseneaza graficul unei functii in grila, relativ la 
	marimea acesteia
*/
{
	vector<POINT> points;
	f(&points, this->maxW, this->maxH);

	glBegin(GL_LINE_STRIP);
	/* Vertical line */
	for (unsigned int i = 0; i < points.size(); i++)
	{
		if (points[i].x >= this->startX 
			&& points[i].x <= this->finishX 
			&& points[i].y >= this->startY
			&& points[i].y <= this->finishY)
			glVertex2f(CoordXGridToNormal(points[i].x + this->startX), CoordYGridToNormal(points[i].y + this->startY));
	}
	glEnd();
}

void Grila::afisareDreapta(POINT s, POINT f, int size)
/*
	Aceasta functie primeste ca parametri
	2 puncte in grila desenata, nu in sistemul de coordonate
	tip grid (care acceseaza pixel by pixel) facut cu ajutorul
	functiilor CoordXGridToNormal etc.

	Deseneaza o linie de o anumita grosime (specificata in parametrul size)
*/
{
	int dx = f.x - s.x;
	int dy = f.y - s.y;
	int d, dE, dNE;
	int sx, sy, fx, fy;
	double m1x, m1y, m2x, m2y;
	bool c1, c2;

	//Cu asta reduc la 4 cazuri
	if (dx >= 0)
	{
		sx = s.x;
		sy = s.y;
		fx = f.x;
		fy = f.y;
	}
	else
	{
		sx = f.x;
		sy = f.y;
		fx = s.x;
		fy = s.y;
	}

	// DESENAREA MARTORULUI - Incadrarea lui in grila
	m1x = (double)sx;
	m1y = (double)sy;
	m2x = (double)fx;
	m2y = (double)fy;
	c1 = false;
	if (m1x >= 0 && m1x <= this->coloane && m1y >= 0 && m1y <= this->linii) c1 = true;
	c2 = false;
	if (m2x >= 0 && m2x <= this->coloane && m2y >= 0 && m2y <= this->linii) c2 = true;

	if (!c1 || !c2)
	{
		if (!c1)
		{
			if (m1x < 0)
			{
				m1y = (double)((0 - m1x) / (m2x - m1x) * (m2y - m1y) + m1y);
				m1x = 0;
			}
			
			if (m1x > this->coloane)
			{
				m1y = (double)((this->coloane - m1x) / (m2x - m1x) * (m2y - m1y) + m1y);
				m1x = this->coloane;
			}
			
			if (m1y < 0)
			{
				m1x = (double)((0 - m1y) / (m2y - m1y) * (m2x - m1x) + m1x);
				m1y = 0;
			}
			
			if (m1y > this->linii)
			{
				m1x = (double)((this->linii - m1y) / (m2y - m1y) * (m2x - m1x) + m1x);
				m1y = this->linii;
			}
		}

		if (!c2)
		{
			if (m2x < 0)
			{
				m2y = (double)((0 - m1x) / (m2x - m1x) * (m2y - m1y) + m1y);
				m2x = 0;
			}
			
			if (m2x > this->coloane)
			{
				m2y = (double)((this->coloane - m1x) / (m2x - m1x) * (m2y - m1y) + m1y);
				m2x = this->coloane;
			}
			
			if (m2y < 0)
			{
				m2x = (double)((0 - m1y) / (m2y - m1y) * (m2x - m1x) + m1x);
				m2y = 0;
			}
			
			if (m2y > this->linii)
			{
				m2x = (double)((this->linii - m1y) / (m2y - m1y) * (m2x - m1x) + m1x);
				m2y = this->linii;
			}
		}

		if (m1x >= 0 && m1x <= this->coloane && m1y >= 0 && m1y <= this->linii) c1 = true;
		if (m2x >= 0 && m2x <= this->coloane && m2y >= 0 && m2y <= this->linii) c2 = true;
	}
	

	// Desenez linia de control
	if (c1 && c2)
	{
		glLineWidth(WITNESS_LINE_WIDTH);
		glBegin(GL_LINE_STRIP);
		glVertex2f(CoordXGridToNormal(this->toBaseGridX(m1x)), CoordYGridToNormal(this->toBaseGridY(m1y)));
		glVertex2f(CoordXGridToNormal(this->toBaseGridX(m2x)), CoordYGridToNormal(this->toBaseGridY(m2y)));
		glEnd();
		glLineWidth(1);
	}

	
	//LINIE VERTICALA
	if (dx == 0)
	{
		printf("dx=0");
		sx = s.x;
		if (s.y > f.y)
		{
			sy = f.y;
			fy = s.y;
		}
		else
		{
			sy = s.y;
			fy = f.y;
		}
		this->drawLineLayer(sx, sy, size, false);
		while (sy <= fy - 1)
		{
			sy++;
			this->drawLineLayer(sx, sy, size, false);
		}
	}
	else
	{
		dx = fx - sx;
		dy = fy - sy;

		if (dy < 0)
		{
			if (dx >= -dy)
			{
				d = 2 * dy + dx;
				dE = 2 * dy;
				dNE = 2 * (dy + dx);

				this->drawLineLayer(sx, sy, size, true);
				while (sx <= fx - 1)
				{
					if (d <= 0)
					{
						d += dNE;
						sx++;
						sy--;
					}
					else
					{
						d += dE;
						sx++;
					}
					this->drawLineLayer(sx, sy, size, true);
				}
			}
			else
			{
				d = 2 * dx + dy;
				dE = 2 * dx;
				dNE = 2 * (dx + dy);

				this->drawLineLayer(sx, sy, size, false);
				while (sy >= fy + 1)
				{
					if (d > 0)
					{
						d += dNE;
						sx++;
						sy--;
					}
					else
					{
						d += dE;
						sy--;
					}
					this->drawLineLayer(sx, sy, size, false);
				}
			}
		}
		else
		{
			if (dx >= dy)
			{
				d = 2 * dy - dx;
				dE = 2 * dy;
				dNE = 2 * (dy - dx);

				this->drawLineLayer(sx, sy, size, true);
				while (sx <= fx)
				{
					if (d <= 0)
					{
						d += dE;
						sx++;
					}
					else
					{
						d += dNE;
						sx++;
						sy++;
					}
					this->drawLineLayer(sx, sy, size, true);
				}
			}
			else
			{
				d = 2 * dx - dy;
				dE = 2 * dx;
				dNE = 2 * (dx - dy);

				this->drawLineLayer(sx, sy, size, false);
				while (sy <= fy-1)
				{
					if (d <= 0)
					{
						d += dE;
						sy++;
					}
					else
					{
						d += dNE;
						sx++;
						sy++;
					}
					this->drawLineLayer(sx, sy, size, false);
				}
			}
		}
	}

}

void Grila::drawLineLayer(int sx, int sy, int size, bool vertical)
/*
	Deseaneaza un layer din linie (adica o linie sau o coloana de o
	anumita grosime)
*/
{
	int i;
	if (vertical)
	{
		for (i = 0; i < size/2; i++)
		{
			this->writeGridPixel(sx, sy-(size/2 - i), WRITE_PIXEL_SIZE);
		}
		for (i = 0; i <= (size-1)/2; i++)
		{
			this->writeGridPixel(sx, sy + i, WRITE_PIXEL_SIZE);
		}
	}
	else
	{
		for (i = 0; i < size / 2; i++)
		{
			this->writeGridPixel(sx - (size / 2 - i), sy, WRITE_PIXEL_SIZE);
		}
		for (i = 0; i <= (size - 1) / 2; i++)
		{
			this->writeGridPixel(sx + i, sy, WRITE_PIXEL_SIZE);
		}
	}
}



/*
===========================
		MAIN LOGIC
===========================
*/

// Draw Grill
void DrawGrill() 
{
	int max = dimW;
	if (max > dimH)
		max = dimH;
	POINT s1, f1, s2, f2;

	printf("Grila linii: %d     Grila coloane: %d\n", max / GRID_SQUARE_WIDTH, max / GRID_SQUARE_WIDTH);
	Grila grila(33, 33, 400, 400, 20, 20);

	//Desenarea grilei
	glColor3f(1, 0.1, 0.1); // rosu
	grila.draw();
	
	//Plotarea functiei - TEST
	glColor3f(0.1, 0.1, 1); 
	if (prevKey == 'p')
	{
		grila.plot(functionToPlot);
	}
	else
	if (prevKey == 'd')
	{	
		/*
		s1.x = 6;
		s1.y = 10;
		f1.x = 7;
		f1.y = 2;
		grila.afisareDreapta(s1, f1, 1);

		s1.x = 5;
		s1.y = 10;
		f1.x = 0;
		f1.y = 0;
		grila.afisareDreapta(s1, f1, 1);
		*/
		
		/*
		s1.x = 0;
		s1.y = 33;
		f1.x = 33;
		f1.y = 0;
		grila.afisareDreapta(s1, f1, 1);

		s1.x = 10;
		s1.y = 0;
		f1.x = 20;
		f1.y = 0;
		grila.afisareDreapta(s1, f1, 3);

		s1.x = 34;
		s1.y = 10;
		f1.x = 34;
		f1.y = 20;
		grila.afisareDreapta(s1, f1, 3);


		s1.x = 25;
		s1.y = 10;
		f1.x = 25;
		f1.y = 20;
		grila.afisareDreapta(s1, f1, 3);


		s1.x = 10;
		s1.y = 5;
		f1.x = 20;
		f1.y = 5;
		grila.afisareDreapta(s1, f1, 3);

		
		s1.x = 30;
		s1.y = 35;
		f1.x = 35;
		f1.y = 30;
		grila.afisareDreapta(s1, f1, 1);
		*/

		s1.x = 5;
		s1.y = 10;
		f1.x = 50;
		f1.y = 50;
		grila.afisareDreapta(s1, f1, 1);
	}
}

void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glLineWidth(1);
	glPointSize(4);
	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	
	DrawGrill();
	
	glFlush();
}

void Reshape(int w, int h) {
	dimW = w;
	dimH = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glutPostRedisplay();
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(dimW, dimH);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}