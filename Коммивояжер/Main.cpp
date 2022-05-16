#include "Graph.h"


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");


	glutInit(&argc, argv);

	setGraph();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WinW, WinH);
	glutCreateWindow("Graph");
	glutDisplayFunc(display);
	glutReshapeFunc(change);
	glutMouseFunc(mouseClick);

	glutMainLoop();

	return 0;
}