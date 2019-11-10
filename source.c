#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

const static float pi = 3.141592653589793;

static float phi , theta ;
static float delta_phi , delta_theta ;

int main(int argc,char** argv)
{
    
    
    GLfloat light_ambient[] = { 1, 1, 1, 1 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };
    GLfloat light_specular[] = { 1, 1, 1, 1 };
    GLfloat ambient_coeffs[] = { 0.647059, 0.164706, 0.164706, 1 };
    GLfloat diffuse_coeffs[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 30;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(700, 700);
    glutInitWindowPosition(500, 200);
    glutCreateWindow(argv[0]);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    
    phi = pi/4;
    theta = pi / 4;
    delta_phi = delta_theta = pi / 90;
    
    glClearColor(0.2, 0.2, 0.2, 0);
    glEnable(GL_DEPTH_TEST);
    

    glutMainLoop();

    return 0;
    
    
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
        
    case 'a':
        phi -= delta_phi;
        if (phi > 2 * pi) {
            phi -= 2 * pi;
        } else if (phi < 0) {
            phi += 2 * pi;
        }
        glutPostRedisplay();
        break;
    case 'w':
        theta += delta_theta;
        if (theta > pi / 2 - pi / 180) {
            theta = pi / 2 - pi / 180;
        }
        glutPostRedisplay();
        break;
    case 'd':
        phi += delta_phi;
        if (phi > 2 * pi) {
            phi -= 2 * pi;
        } else if (phi < 0) {
            phi += 2 * pi;
        }
        glutPostRedisplay();
        break;
    case 's':
        theta -= delta_theta;
        if (theta < -(pi / 2 - pi / 180)) {
            theta = -(pi / 2 - pi / 180);
        }
        glutPostRedisplay();
        break;
    }
}

static void on_reshape(int width, int height)
{
     glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 10);
}

static void on_display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat light_position[] = { 5 * cos(theta) * cos(phi), 5 * cos(theta) * sin(phi), 5 * sin(theta), 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    printf("5*cos(theta)*cos(phi) = %f\n", 5 * cos(theta) * cos(phi));
    printf("5 * cos(theta) * sin(phi) = %f\n",5 * cos(theta) * sin(phi));
    printf("5 * sin(theta) = %f\n", 5 * sin(theta));
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5 * cos(theta) * cos(phi), 
              5 * cos(theta) * sin(phi),
              5 * sin(theta), 
              0, 0, 0, 
              0, 0, 1);

    /*int i;*/
    glScalef(3,3,3);
    
    glBegin(GL_POLYGON);
    /*for(i=0;i<30;i++){
        glVertex3f(0.5+i, -0.5, 0);
        glVertex3f(-0.5+i, -0.5, 0);
        glVertex3f(-0.5+i, 0.5, 0);
        glVertex3f(0.5+i, 0.5, 0);
    }*/
        glVertex3f(0.5, -0.5, 0);
            glVertex3f(-0.5, -0.5, 0);
            glVertex3f(-0.5, 0.5, 0);
            glVertex3f(0.5, 0.5, 0);
    glEnd();

    glPushMatrix();
        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,5,0);
        glEnd();
        

        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(5,0,0);
        glEnd();
        

        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,0,5);
        glEnd();
    glPopMatrix();

    
    glutSwapBuffers();
}



