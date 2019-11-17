#include <stdlib.h>
#include <GL/glut.h>

class kocka
{
public:
	kocka(double width,double height,double depth);
	kocka();
	~kocka();
	void draw();
private:
	double width;
	double height;
	double depth;
};