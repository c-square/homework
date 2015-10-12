#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

// dimensiunea ferestrei in pixeli
#define initial_dim				500
#define WRITE_PIXEL_NR_POINTS	50
#define WRITE_PIXEL_CLOCKWISE	false
#define WRITE_PIXEL_HIDE_EXCESS false

#define GRID_SQUARE_WIDTH 15
#define WRITE_PIXEL_SIZE	4  //Marimea unui pixel
#define GRID_LINE_WIDTH		1  //Grosimea liniilor din grila
#define WITNESS_LINE_WIDTH	2  //Grosimea liniilor martor din grila
//la desenarea liniilor

//HELPERS
#define min(a,b) ((a)>(b)?(b):(a))
#define max(a,b) ((a)>(b)?(a):(b))


struct POINT
{
	double x, y;
};
struct MUCHIE
{
	POINT vi, vf;
};
struct INTERSECTIE
{
	int ymax; 
	double xmin;
	double ratia;
};
struct SCANARE
{
	int ym;
	vector<INTERSECTIE *> i;
};

typedef void(*PlottingFunction)(vector<POINT> *, int, int);

vector<SCANARE> scan;
vector<SCANARE> ssm;
vector<MUCHIE> poly;
int polyn;
int nrScans;

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
	return (x + 1) * dimW / 2;
}

int CoordYNormalToGrid(double y)
{
	return ((y + 1) * dimH / 2) * -1;
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
===========================a
*/


class Grila
{
public:
	Grila(int linii, int coloane, int width, int height, int startX, int startY);
	void draw();
	void plot(PlottingFunction f);
	void writePixel(int x, int y, float raza, bool clockwise = WRITE_PIXEL_CLOCKWISE, bool hide = WRITE_PIXEL_HIDE_EXCESS);
	void writeGridPixel(int x, int y, float raza, bool clockwise = WRITE_PIXEL_CLOCKWISE, bool hide = WRITE_PIXEL_HIDE_EXCESS);
	void afisareDreapta(POINT s, POINT f, int size, bool martor = true, bool dots = true);
	void drawLineLayer(int sx, int sy, int size, bool vertical);

	int toBaseGridX(int);
	int toBaseGridY(int);

	void drawCircleWitness(int x, int y, float raza, bool clockwise = WRITE_PIXEL_CLOCKWISE, bool hide = WRITE_PIXEL_HIDE_EXCESS);
	void afisareCerc(int, int, int, int);

	void drawElipseWitness(int x, int y, int razaX, int razaY, bool hide = WRITE_PIXEL_HIDE_EXCESS);
	void umplereElipsa(int x, int y, int razaX, int razaY);

	void drawPoly(vector<MUCHIE> * p);
	void fillPoly(vector<MUCHIE> * p);

	int ratieW, ratieH;

private:
	int linii, coloane;
	int maxW, maxH;
	int startX, startY;
	int finishX, finishY;

	void initET(vector<MUCHIE> * p);
	void calculSSM(vector<MUCHIE> * p);
	void colorare();
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

void Grila::writePixel(int x, int y, float raza, bool clockwise, bool hide)
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
	if (clockwise)
		i = WRITE_PIXEL_NR_POINTS;

	glBegin(GL_POLYGON);
	while (1)
	{
		angle = 2.0f * M_PI * float(i) / float(WRITE_PIXEL_NR_POINTS); //(2 * PI * al x-lea punct)
		inx = round(cos(angle) * raza + stX, 0.001);
		iny = round(sin(angle) * raza + stY, 0.001);

		
		if (hide)
		{
			if ((inx >= this->startX
			&& inx <= this->finishX
			&& iny >= this->startY
			&& iny <= this->finishY))
			{
				glVertex2f(CoordXGridToNormal(inx), CoordYGridToNormal(iny));
			}
			else
			{	
				glEnd();
				glBegin(GL_POLYGON);
			}
		}
		else
			glVertex2f(CoordXGridToNormal(inx), CoordYGridToNormal(iny));

		if (clockwise)
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

void Grila::writeGridPixel(int x, int y, float raza, bool clockwise, bool hide)
/*
Deseneaza un "pixel" de o anumita raza in grila
*/
{
	this->writePixel(x * this->ratieW, y * this->ratieH, raza, clockwise, hide);
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

void Grila::afisareDreapta(POINT s, POINT f, int size, bool martor, bool dots)
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
	if (c1 && c2 && martor)
	{
		glLineWidth(WITNESS_LINE_WIDTH);
		glBegin(GL_LINE_STRIP);
		glVertex2f(CoordXGridToNormal(this->toBaseGridX(m1x)), CoordYGridToNormal(this->toBaseGridY(m1y)));
		glVertex2f(CoordXGridToNormal(this->toBaseGridX(m2x)), CoordYGridToNormal(this->toBaseGridY(m2y)));
		glEnd();
		glLineWidth(1);
	}

	if (dots)
	{
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
					while (sy <= fy - 1)
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
		for (i = 0; i < size / 2; i++)
		{
			this->writeGridPixel(sx, sy - (size / 2 - i), WRITE_PIXEL_SIZE);
		}
		for (i = 0; i <= (size - 1) / 2; i++)
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
TEMA 4
CERC
*/
//PUNCTUL 1
void Grila::drawCircleWitness(int x, int y, float raza, bool clockwise, bool hide)
{
	x = x * this->ratieW + this->startX;
	y = y * this->ratieH + this->startY;

	float stX = x;
	float stY = y;
	float angle, inx, iny;
	int i;

	i = 0;
	if (clockwise)
		i = WRITE_PIXEL_NR_POINTS;

	glBegin(GL_LINE_STRIP);
	while (1)
	{
		angle = 2.0f * M_PI * float(i) / float(WRITE_PIXEL_NR_POINTS); //(2 * PI * al x-lea punct)
		inx = round(cos(angle) * raza + stX, 0.001);
		iny = round(sin(angle) * raza + stY, 0.001);


		if (hide)
		{
			if ((inx >= this->startX - 1
				&& inx <= this->finishX + 1
				&& iny >= this->startY - 1
				&& iny <= this->finishY + 1))
			{
				glVertex2f(CoordXGridToNormal(inx), CoordYGridToNormal(iny));
			}
			else
			{
				glEnd();
				glBegin(GL_LINE_STRIP);
			}
		}
		else
			glVertex2f(CoordXGridToNormal(inx), CoordYGridToNormal(iny));

		if (clockwise)
		{
			i--;
			if (i == -1)
				break;
		}
		else
		{
			i++;
			if (i == WRITE_PIXEL_NR_POINTS+1)
				break;
		}
	}
	glEnd();
}

void Grila::afisareCerc(int origX, int origY, int raza, int size)
{
	int x = raza;
	int y = 0;
	int d = 1 - raza;
	int dE = 3, dSE = -2 * raza + 5;

	this->drawLineLayer(origX + x, origY + y, size, false);
	while (x > -y)
	{
		if (d < 0)
		{
			d += dE;
			dE += 2;
			dSE += 2;
		}
		else
		{
			d += dSE;
			dE += 2;
			dSE += 4;
			x--;
		}
		y--;
		this->drawLineLayer(origX + x, origY + y, size, false);
	}
}

//PUNCTUL 2
void Grila::drawElipseWitness(int x, int y, int razaX, int razaY, bool hide)
{
	x = x * this->ratieW + this->startX;
	y = y * this->ratieH + this->startY;

	float stX = x;
	float stY = y;
	float angle, inx, iny;
	int i;

	i = 0;

	glBegin(GL_LINE_STRIP);
	while (1)
	{
		angle = 2.0f * M_PI * float(i) / float(WRITE_PIXEL_NR_POINTS); //(2 * PI * al x-lea punct)
		inx = round(cos(angle) * razaX + stX, 0.001);
		iny = round(sin(angle) * razaY + stY, 0.001);

		if (hide)
		{
			if ((inx >= this->startX - 1
				&& inx <= this->finishX + 1
				&& iny >= this->startY - 1
				&& iny <= this->finishY + 1))
			{
				glVertex2f(CoordXGridToNormal(inx), CoordYGridToNormal(iny));
			}
			else
			{
				glEnd();
				glBegin(GL_LINE_STRIP);
			}
		}
		else
			glVertex2f(CoordXGridToNormal(inx), CoordYGridToNormal(iny));


		i++;
		if (i == WRITE_PIXEL_NR_POINTS + 1)
			break;
	}
	glEnd();
}

void Grila::umplereElipsa(int stx, int sty, int razaX, int razaY)
{
	POINT p1, p2;
	int x = 0;
	int indice = 0;
	int y = razaY;
	double d1 = razaY * razaY - razaX * razaX * razaY + razaX * razaX / 4.0;
	double d2;

	if (razaX > razaY)
		indice = 1;

	p1.x = stx + x;
	p1.y = sty + y;
	p2.x = stx + x;
	p2.y = sty;
	this->afisareDreapta(p2, p1, 1, false);
	while (razaX * razaX * (y - 0.5) > razaY * razaY * (x + 1 + indice))
	{
		if (d1 < 0)
		{
			printf("last here1\n");
			d1 += razaY * razaY * (2 * x + 3);
			x++;
		}
		else
		{
			printf("last here2: %d\n", d1);
			d1 += razaY * razaY * (2 * x + 3) + razaX * razaX * (-2 * y + 2);
			x++;
			y--;
		}

		p1.x = stx + x * -1;
		p1.y = sty + y;
		p2.x = stx + x * -1;
		p2.y = sty;
		if (razaY * razaY * x * x + razaX * razaX * y * y - razaY * razaY * razaX * razaX > 0) // in afara elipsei
			p1.y--;
		this->afisareDreapta(p2, p1, 1, false);
	}

	
	d2 = razaY * razaY * (x + 0.5) * (x + 0.5) + razaX * razaX * (y - 1) * (y - 1) - razaX * razaX * razaY * razaY;
	while (y > 0)
	{
		if (d2 < 0)
		{
			d2 += razaY * razaY * (2 * x + 2) + razaX * razaX * (-2 * y + 3);
			x++;
			y--;
		}
		else
		{
			d2 += razaX * razaX * (-2 * y + 3);
			y--;
		}

		p1.x = stx + x * -1;
		p1.y = sty + y;
		p2.x = stx + x * -1;
		p2.y = sty;
		if (razaY * razaY * x * x + razaX * razaX * y * y - razaY * razaY * razaX * razaX > 0) // in afara elipsei
			p1.x--;
		this->afisareDreapta(p2, p1, 1, false);
	}
}


//PUNCTUL 3
void Grila::drawPoly(vector<MUCHIE> * p)
{
	int i;
	for (i = 0; i < polyn; i++)
	{
		this->afisareDreapta(poly[i].vi, poly[i].vf, 1, true, false);
	}
}
void Grila::fillPoly(vector<MUCHIE> * p)
{
	scan.clear();
	ssm.clear();
	initET(p);
	calculSSM(p);
	colorare();
}

void Grila::initET(vector<MUCHIE> * p)
{
	int xm, ym, XM, YM;
	int i, j;
	bool change;

	nrScans = 0;

	INTERSECTIE * intersectie;
	MUCHIE * m;
	SCANARE * s = 0;
	for (i = 0; i < polyn; i++)
	{
		s = 0;
		m = &(*p)[i];
		if (m->vi.y != m->vf.y)
		{
			ym = min(m->vi.y, m->vf.y);
			YM = max(m->vi.y, m->vf.y);
			xm = (ym == m->vi.y) ? m->vi.x : m->vf.x;
			XM = (YM == m->vi.y) ? m->vi.x : m->vf.x;

			change = false;
			for (j = 0; j < nrScans; j++)
			{
				if (scan[j].ym == ym)
				{
					s = &(scan[j]);
					break;
				}
			}
			if (s == 0)
			{
				change = true;
				s = new SCANARE();
				nrScans++;
			}
				
			intersectie = new INTERSECTIE();
			intersectie->ymax = YM;
			intersectie->xmin = xm;
			intersectie->ratia = (xm - XM) / (ym - YM);

			s->ym = ym;
			s->i.insert(s->i.end(), intersectie);

			if (change)
			{
				scan.insert(scan.end(), *s);
			}
		}
	}

	for (i = 0; i < nrScans; i++)
	{
		s = &scan[i];
		change = true;
		while (change)
		{
			change = false;
			for (j = 0; j < (s->i.size()) - 1; j++)
			{
				if (s->i[j]->xmin > s->i[j + 1]->xmin)
				{
					change = true;
					INTERSECTIE * tmp = s->i[j];
					s->i[j] = s->i[j + 1];
					s->i[j + 1] = tmp;
				}
			}
		}
	}
}


void Grila::calculSSM(vector<MUCHIE> * p)
{
	INTERSECTIE * intersectie;
	vector<INTERSECTIE *> aet;
	SCANARE * s;
	int y, k;
	int i, j, l;

	if (nrScans == 0)
		return;

	for (i = 0; i < scan.size(); i++)
	{
		if (scan[i].i.size() == 0)
			break;
	
		for (j = 0; j < scan[i].i.size(); j++)
		{
			aet.insert(aet.end(), scan[i].i[j]);
		}
		for (j = 0; j < aet.size();)
		{
			if (aet[j]->ymax == scan[i].ym)
				aet.erase(aet.begin() + j);
			else
				j++;
		}

		k = aet.size();
		while (k >= 2)
		{
			for (j = 0; j < k-1; j++)
			{
				if (aet[j]->xmin > aet[j + 1]->xmin)
				{
					intersectie = aet[j];
					aet[j] = aet[j + 1];
					aet[j + 1] = intersectie;
				}
			}
			k--;
		}

		s = new SCANARE();
		s->ym = scan[i].ym;
		s->i = aet;
		ssm.insert(ssm.end(), *s);

		for (j = 0; j < aet.size(); j++)
		{
			if (aet[j]->ratia != 0)
				aet[j]->xmin += aet[j]->ratia;
		}

		if (aet.empty()) break;
	}
		
}

void Grila::colorare()
{

}
/*
===========================
MAIN LOGIC
===========================
*/

void ReadPoly()
{
	MUCHIE m;
	POINT p;
	int n, i;
	ifstream fin("poly.txt");

	poly.clear();
	
	fin >> polyn;
	fin >> p.x >> p.y;
	for (i = 1; i < polyn; i++)
	{
		m.vi.x = p.x;
		m.vi.y = p.y;
		fin >> p.x >> p.y;
		m.vf.x = p.x;
		m.vf.y = p.y;
		poly.insert(poly.end(), m);
	}
	m.vi.x = p.x;
	m.vi.y = p.y;

	m.vf.x = poly[0].vi.x;
	m.vf.y = poly[0].vi.y;

	poly.insert(poly.end(), m);

	fin.close();
}

// Draw Grill
void DrawGrill()
{
	int max = dimW;
	int radX, radY, rad;
	if (max > dimH)
		max = dimH;
	POINT s1, f1, s2, f2;

	printf("Grila linii: %d     Grila coloane: %d\n", max / GRID_SQUARE_WIDTH, max / GRID_SQUARE_WIDTH);
	Grila grila(40, 40, 500, 500, 0, 0);

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
		if (prevKey == '1')
		{
			s1.x = 20;
			s1.y = 20;
			grila.drawCircleWitness(s1.x, s1.y, 10 * grila.ratieW, true, false);
			grila.afisareCerc(s1.x, s1.y, 10, 3);
		}
	else
		if (prevKey == '2')
		{
			s1.x = 20;
			s1.y = 20;
			radX = 10;
			radY = 20;
			grila.drawElipseWitness(s1.x, s1.y, radX*grila.ratieW, radY*grila.ratieH, false);
			grila.umplereElipsa(s1.x, s1.y, radX, radY);
		}
	else
		if (prevKey == '3')
		{
			ReadPoly();
			grila.drawPoly(&poly);
			grila.fillPoly(&poly);
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