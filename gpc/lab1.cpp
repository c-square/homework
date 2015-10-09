
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/freeglut.h>

// dimensiunea ferestrei in pixeli
#define dim 600

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
   double xmax, ymax, xmin, ymin;
   double a = 1, b = 2;
   double pi = 4 * atan(1.0);
   double ratia = 0.05;
   double t;

   // calculul valorilor maxime/minime ptr. x si y
   // aceste valori vor fi folosite ulterior la scalare
   xmax = a - b - 1;
   xmin = a + b + 1;
   ymax = ymin = 0;
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = a + b * cos(t);
      xmax = (xmax < x1) ? x1 : xmax;
      xmin = (xmin > x1) ? x1 : xmin;

      x2 = a - b * cos(t);
      xmax = (xmax < x2) ? x2 : xmax;
      xmin = (xmin > x2) ? x2 : xmin;

      y1 = a * tan(t) + b * sin(t);
      ymax = (ymax < y1) ? y1 : ymax;
      ymin = (ymin > y1) ? y1 : ymin;

      y2 = a * tan(t) - b * sin(t);
      ymax = (ymax < y2) ? y2 : ymax;
      ymin = (ymin > y2) ? y2 : ymin;
   }

   xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
   ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP);
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();

   glBegin(GL_LINE_STRIP);
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x2,y2);
   }
   glEnd();
}

// graficul functiei
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$,
void Display2() {
   double pi = 4 * atan(1.0);
   double xmax = 8 * pi;
   double ymax = exp(1.1);
   double ratia = 0.05;

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP);
   for (double x = 0; x < xmax; x += ratia) {
      double x1, y1;
      x1 = x / xmax;
      y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();
}

void Display3() {
	/*
	Graficul functiei
	1, pt x = 0
	d(x) / x pt x > 0
	*/

	double x = 0;
	double t = 0;

	double fx, dx;
	double integer_part = 0, fraction_part = 0;
	double ratio = 0.05;

	glColor3f(1,0.1,0.1); // rosu
	glBegin(GL_LINE_STRIP);

	for (x = 0; x < 100; x += ratio) {
		if (x == 0)
			fx = 1;
		else {
			fraction_part = modf(x, &integer_part);
			if (fraction_part >= 0.5) {
				dx = fabs(x - ceil(x));
			}
			else {
				dx = fabs(x - floor(x));
			}
		fx = dx / x;
		}

		glVertex2f(t / 500, fx);
		t += 1;
	}
   glEnd();
}

void Display4() {

	//Melcul lui Pascal
	double a = 0.3;
	double b = 0.2;
	double x,y;

	double t = 0.0;
	double tmin = -3.14;
	double tmax = 3.14;
	double ratia = 0.05;

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1,0.1,0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double t = tmin; t < tmax; t += ratia) {
		x = 2 * (a * cos(t) + b) * cos(t);
		y = 2 * (a * cos(t) + b) * sin(t);
		glVertex2f(x,y);
	}
	glEnd();

}

void Display5()
{
	//Trisectoarea lui Longchamps
	double a = 0.2;
	double t, x, y;
	double pi = 3.14;
	double tmin = -pi/2;
	double tmax = pi/2;
    double last_x =  0.0;
    double last_y = 0.0;
	double ratia = 0.002;

    for (t = tmin; t < tmax; t += ratia) {
		if (t == pi/6 || t == pi/(-6)) continue;
		else {
			x = a / (4 * cos(t) * cos(t) - 3);
			y = a * tan(t) / (4 * cos(t) * cos(t) - 3);
			if(x<0 && y > 0)    {

                if (y > 0.3) {

                    if (last_x == last_y){
                        last_x = x;
                        last_y = y;
                    }
                    else{
                        glColor3f(1,0.1,0.1); // rosu
                        glBegin(GL_TRIANGLES);
                            glVertex2f(last_x, last_y);
                            glVertex2f(x,y);
                            glVertex2f(-1,1);
                        glEnd();
                        last_x = 0;
                        last_y = 0;
                    }
                }
			}
		}
	}

	glColor3f(0.2,0.15,0.88); // albastru
	glBegin(GL_LINE_STRIP);
	for (t = tmin; t < tmax; t += ratia) {
		if (t == pi/6 || t == pi/(-6)) continue;
		else {
			x = a / (4 * cos(t) * cos(t) - 3);
			y = a * tan(t) / (4 * cos(t) * cos(t) - 3);
			if(x<0 && y > 0)    {
                glVertex2f(x,y);
			}
		}
	}
	glEnd();

}

void Display6()
{
	//Cicloida
	double pi = 3.14;
	double a = 0.1;
	double b = 0.2;
	double t, x, y;
	double tmin = -10;
	double tmax = 10;
	double ratio = 0.05;

	glColor3f(1,0.1,0.1); // rosu

	glBegin(GL_LINE_STRIP);

	for (t = tmin; t < tmax; t += ratio) {
		x = a * t - b * sin(t);
		y = a - b * cos(t);
		glVertex2f(x,y);
	}

	glEnd();

}

void Display7()
{
	//Epicicloida
	double r = 0.3;
	double R = 0.1;
	double t, x, y;
	double tmin = 0;
	double tmax = 2 * 3.14;
	double ratio = 0.05;

	glColor3f(1,0.1,0.1); // rosu

	glBegin(GL_LINE_STRIP);

	for (t = tmin; t <= tmax; t += ratio) {
		x = (R + r) * cos(r/R * t) - r * cos(t + r/R * t);
		y = (R + r) * sin(r/R * t) - r * sin(t + r/R *t);
		glVertex2f(x, y);
	}

	glEnd();
}

void Display8() {
	//hipocicloida
	double R = 0.1;
	double r = 0.3;
	double t, x, y;
	double tmin = 0;
	double tmax = 2 * 3.14;
	double ratio = 0.05;

	glColor3f(1,0.1,0.1); // rosu
	glBegin(GL_LINE_STRIP);

	for(t = tmin; t <= tmax; t += ratio) {
		x = (R - r) * cos(r/R * t) - r * cos(t - r/R * t);
		y = (R - r) * sin(r/R * t) - r * sin(t - r/R * t);
		glVertex2f(x,y);
	}
	glEnd();

}

void Display9(){
	// Lemniscata lui Bernoulli
	double a = 0.4;
	double r;
	double t, x, y;
	double pi = 3.14;
	double tmin = pi/-4;
	double tmax = pi/4;
	double ratio = 0.05;
    double copy_x, copy_y;
	glColor3f(1,0.1,0.1); // rosu
	glBegin(GL_LINE_LOOP);
        for (t = tmin; t < tmax; t += ratio) {
            r = a * sqrt(2 * cos(2*t));
            x = r * cos(t);
            y = r * sin(t);
            glVertex2f(x, y);
        }

        for (t = tmax; t > tmin; t -= ratio) {
            r = -a * sqrt(2 * cos(2*t));
            x = r * cos(t);
            y = r * sin(t);
            glVertex2f(x, y);
        }

        //glVertex2f(copy_x, copy_y);
	glEnd();
}

void Display0() {
	//Spirala logaritmica
	double a = 0.02;
	double r;
	double t, x, y;
	double tmin = 0;
	double tmax = 10;
	double ratio = 0.05;
	double e = 2.7;

	glColor3f(1,0.1,0.1); // rosu
	glBegin(GL_LINE_STRIP);

	for(t = tmin; t < tmax; t += ratio) {
		r = a * pow(e, 1+t);
		x = r * cos(t);
		y = r * sin(t);
		glVertex2f(x, y);
	}
	glEnd();
}

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

//   glPointSize(4);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
   glClear(GL_COLOR_BUFFER_BIT);

   switch(prevKey) {
   case '1':
      Display1();
      break;
   case '2':
		Display2();
		break;
   case '3':
		Display3();
		break;
   case '4':
		Display4();
		break;
   case '5':
		Display5();
		break;
	case '6':
		Display6();
		break;
	case '7':
		Display7();
		break;
	case '8':
		Display8();
		break;
	case '9':
		Display9();
		break;
	case '0':
		Display0();
		break;
   default:
      break;
   }

   glFlush();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
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

   glutInitWindowSize(dim, dim);

   glutInitWindowPosition(100, 100);

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);

   glutKeyboardFunc(KeyboardFunc);

   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);

   glutMainLoop();

   return 0;
}
