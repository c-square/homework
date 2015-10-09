#include <iostream>
#include <fstream>
#include "shape.h"
#include "triunghi.h"
#include "dreptunghi.h"
#include "cerc.h"
#include "grup.h"
#include "Patrat.h"
#include "shapefactory.h"


using namespace std;
typedef int TIP;

vector<Shape<TIP> *> shapes;


void createSampleShapeVector()
{
	cout << "Shapes created:" << endl;

	Triunghi<TIP> *t1 = new Triunghi<TIP>(Punct<TIP>(10, 10), Punct<TIP>(20, 20), Punct<TIP>(10, 20));
	Triunghi<TIP> *t2 = new Triunghi<TIP>(Punct<TIP>(15, 15), Punct<TIP>(35, 35), Punct<TIP>(35, 15));
	Shape<TIP> &s1(*t1);
	Shape<TIP> &s2(*t2);
	cout << s1 << endl;
	t1->moveBy(6, 7);
	cout << s1 << endl;
	s1.moveBy(1, 0);
	cout << s1 << endl;
	cout << s2 << endl;
	cout << endl;


	Dreptunghi<TIP> *d1 = new Dreptunghi<TIP>(Punct<TIP>(15, 15), Punct<TIP>(35, 35));
	Shape<TIP> &s3(*d1);
	cout << s3 << endl;
	d1->moveBy(5, 5);
	cout << s3 << endl;
	s3.moveBy(-10, -10);
	cout << *d1 << endl;
	cout << endl;

	Cerc<TIP> *c1 = new Cerc<TIP>(Punct<TIP>(5, 5), 10);
	Shape<TIP> &s4(*c1);
	cout << s4 << endl;
	c1->moveBy(5, 5);
	cout << s4 << endl;
	s4.moveBy(-10, -10);
	cout << *c1 << endl;
	cout << endl;

	Grup<TIP> *g1 = new Grup<TIP>();
	Shape<TIP> &s5(*g1);
	g1->add(&s1);
	g1->add(&s2);
	g1->add(&s3);
	g1->add(&s4);
	cout << s5 << endl;
	s1.moveBy(100, 100);
	cout << s5 << endl;
	cout << endl;

	Grup<TIP> *g2 = new Grup<TIP>();
	g2->add(g1);
	g2->add(new Cerc<TIP>(Punct<TIP>(3, 4), 5));
	cout << *g2 << endl;
	shapes.push_back(t1);
	shapes.push_back(t2);
	shapes.push_back(c1);
	shapes.push_back(d1);
	shapes.push_back(g1);
	shapes.push_back(g2);
}


int main()
{
	int i;
	createSampleShapeVector();

	
	cout << endl << endl << "Writing shapes to file" << endl;
	ofstream out("shapes.txt");
	out << shapes.size() << endl;

	for (int i = 0; i < (int)shapes.size(); ++i) {
	out << *shapes[i] << endl;
	}

	out.close();
	cout << endl << endl << "Reading shapes from file" << endl;
	
	vector<Shape<TIP> *> shapesRead;
	ifstream in("shapes.txt");
	int n;
	in >> n;

	for (int i = 0; i < n; ++i) {
	Shape<TIP> *s = ShapeFactory<TIP>::createShape(in);
	shapesRead.push_back(s);
	}


	in.close();
	cout << endl << endl << "Shapes read from file:" << endl;

	for (int i = 0; i < (int)shapesRead.size(); ++i) {
	cout << *shapesRead[i] << endl;
	}
	cout << endl;

	Patrat<double> *p = new Patrat<double>(Punct<double>(12.3, 12.5), Punct<double>(12.3, 45.3));
	cout<<*p;

	cin>>i;
	return 0;
	
}