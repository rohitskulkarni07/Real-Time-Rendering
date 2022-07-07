#include <GL/freeglut.h>

bool bFullScreen = false;


int main(int argc,char** argv) {

	//code

	//function Declearation

	void initialize(void);
	void display(void);
	void resize(int, int);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);


	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLUT : RTR2020-004 ROHIT SUDHIR KULKARNI");
	
	initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();
}

void initialize(void) {

	//code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}


void display(void) {
	//code

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	glFlush();

}
void resize(int width, int height) {
	//code

	if (height <= 0) {
		height = 1;
	}
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}
void keyboard(unsigned char key, int x, int y) {

	//code

	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;

	case 'F':
	case 'f':
		
		if (bFullScreen == false) {
			glutFullScreen();
			bFullScreen = true;
		} else {
			glutLeaveFullScreen();
			bFullScreen = false;
		}
		break;

	default:
		break;
	}

}
void mouse(int button, int status, int x, int y) {

	switch (button) {

	case GLUT_LEFT_BUTTON:
		break;

	case GLUT_RIGHT_BUTTON:
		glutLeaveMainLoop();
		break;

	default:
		break;
	}
}
void uninitialize(void) {
	//code
}

