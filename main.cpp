#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <cmath>

#include <glut.h>

#define BRUTE_FORCE 0
#define BRESENHAM 1
#define ELLIPSES 2
#define CIRCLES 3
#define OPENGL 4

void (*drawFunction)() = NULL;
int mainMenu, linesMenu;

void swap(GLfloat& a, GLfloat& b){
    GLfloat c = a;
    a = b;
    b = c;
}

void swap(GLint& a, GLint& b){
    GLint c = a;
    a = b;
    b = c;
}

void glutPrint(int x, int y, char *string)
{
    glRasterPos2i(x, y);
    while(string){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
    }
}

void glLine(GLint x1, GLint y1, GLint x2, GLint y2){
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
}

void glLines(){
	glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
	glColor3f(1.0, 0, 0);
    GLint dx = 0, dy = 0;
    for (GLint i = 0; i < 11; i++){
        glLine(-400, 300, -400 + dx, 0);
        glLine(-400, 300, 0, 0 + dy);
        glLine(400, 300, 0 + dx, 0);
        glLine(400, 300, 0, 0 +dy);
        glLine(-400, -300, -400 + dx, 0);
        glLine(-400, -300, 0, -300 + dy);
        glLine(400, -300, 0 + dx, 0);
        glLine(400, -300, 0, -300 + dy);
        dx += 40; dy += 30;
	}
	glEnd();
    glutSwapBuffers();
	glGetError();
}

void bruteForceLine(GLint x1, GLint y1, GLint x2, GLint y2){
    if (x2 == x1){
        GLint y = y1;
        int numPts = y2 - y1 + 1;
        for(int i = 0; i < numPts; i++){
            glVertex2i(round(x1), y++);
        }
        return;
    }else if (y1 == y2){
        GLint x = x1;
        int numPts = x2 - x1 + 1;
        for (int i = 0; i < numPts; i++){
            glVertex2i(x++, round(y1));
        }
        return;
    }
    if (fabs(y2 - y1) > fabs(x2 - x1)){ //if line has slope > 1
        if (y1 > y2){
            swap(x1, x2);
            swap(y1, y2);
        }
        GLfloat inv_m = (float) (x2-x1)/(y2-y1);
        int numPts = y2-y1+1;
        GLint y = y1, x;
        for (int i = 0; i < numPts; i++){ 
            x = inv_m * (y-y1) + x1; 
            glVertex2i(round(x), y);
            y = y+1;
        }
    }else{
        if (x1 > x2){ //if line has change
            swap(x1, x2);
            swap(y1, y2);
        } 
        GLfloat m = (float) (y2 - y1) / (x2 - x1);
        int numPts = x2 - x1 + 1;
        GLint x = x1, y;
        for (int i = 0; i < numPts; i++){
            y = m * (x - x1) + y1;
            glVertex2i(x, round(y));
            x = x + 1;
        }
    }
}

void bruteForce(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0, 0);
    clock_t start = clock();
    for (int j = 0; j < 1000; j++){
        GLint dx = 0, dy = 0;
        for (GLint i = 0; i < 11; i++){
            bruteForceLine(-400, 300, -400 + dx, 0);
            bruteForceLine(-400, 300, 0, 0 + dy);
            bruteForceLine(400, 300, 0 + dx, 0);
            bruteForceLine(400, 300, 0, 0 +dy);
            bruteForceLine(-400, -300, -400 + dx, 0);
            bruteForceLine(-400, -300, 0, -300 + dy);
            bruteForceLine(400, -300, 0 + dx, 0);
            bruteForceLine(400, -300, 0, -300 + dy);
            dx += 40; dy += 30;
        }
    }
    clock_t end = clock();
    int timeTaken = (float) (end - start) / CLOCKS_PER_SEC * 1000;
    printf("%i\n", timeTaken);
    glEnd();
    glutSwapBuffers();
    glGetError();
}

void bresenhamLine(GLint x1, GLint y1, GLint x2, GLint y2){
    bool slopeAboveOne = abs(x2 - x1) < abs(y2 - y1);
    if (slopeAboveOne){
        swap(x1, y1);
        swap(x2, y2);
    }
    if (x2 < x1){
        swap(x1, x2);
        swap(y1, y2);
    }
    GLint dx = x2 - x1, dy = abs(y2 - y1), y = y1, err = dx / 2;
    for (int i = x1; i <= x2; i++){
        if (slopeAboveOne){
            glVertex2i(y, i);
        } else {
            glVertex2i(i, y);
        }
        err -= dy;
        if (0 > err){
            if (y2 > y1){
                y += 1;
            } else {
                y -= 1;
            }
            err += dx;
        }
    } 
}

void bresenham(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0, 0);
    
    clock_t start = clock();
    for (int j = 0; j < 1000; j++){
        GLint dx = 0, dy = 0;
        for (GLint i = 0; i < 11; i++){
            bresenhamLine(-400, 300, -400 + dx, 0);
            bresenhamLine(-400, 300, 0, 0 + dy);
            bresenhamLine(400, 300, 0 + dx, 0);
            bresenhamLine(400, 300, 0, 0 +dy);
            bresenhamLine(-400, -300, -400 + dx, 0);
            bresenhamLine(-400, -300, 0, -300 + dy);
            bresenhamLine(400, -300, 0 + dx, 0);
            bresenhamLine(400, -300, 0, -300 + dy);
            dx += 40; dy += 30;
        }
    }
    clock_t end = clock();
    int timeTaken = (float) (end - start) / CLOCKS_PER_SEC * 1000;
    printf("%i\n", timeTaken);
    glEnd();
    glutSwapBuffers();
    glGetError();
}
//not yet implemented
void ellipses(){

}
//not yet implemented
void circles(){

}

void menuHandler(int returnCode){
	glClear(GL_COLOR_BUFFER_BIT);
    switch (returnCode){
		case OPENGL:
			drawFunction = &glLines;
			break;
		case BRUTE_FORCE:
			drawFunction = &bruteForce;
			break;
		case BRESENHAM:
			drawFunction = &bresenham;
			break;
		case ELLIPSES:
			drawFunction = &ellipses;
			break;
		case CIRCLES:
			drawFunction = &circles;
			break;
	}
    glutDisplayFunc(drawFunction);
	(*drawFunction)();
};

void init(void){
	linesMenu = glutCreateMenu(menuHandler);
	glutAddMenuEntry("OpenGL GL_LINES", OPENGL);
	glutAddMenuEntry("Brute Force", BRUTE_FORCE);
	glutAddMenuEntry("Bresenham", BRESENHAM);
	mainMenu = glutCreateMenu(menuHandler);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddSubMenu("Line Drawing Algorithms: ", linesMenu);
	glutAddMenuEntry("Ellipses", ELLIPSES);
	glutAddMenuEntry("Circles", CIRCLES);
	glClearColor(1.0,1.0,1.0,0.0);  // white background
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-400.0, 400.0, -300.0, 300.0); // orthogonal 2D view, set clipping window
}

int main(int argc, char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(50,100);  //physical position of window on desktop
	glutInitWindowSize(400,300);     //physical width and height of window
	glutCreateWindow("Right click to pick drawing function!");
	init();
	drawFunction = &bruteForce;
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
	glutDisplayFunc(drawFunction);
	glutMainLoop();
}