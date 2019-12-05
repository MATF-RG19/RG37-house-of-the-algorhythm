#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

const static float pi = 3.141592653589793;
static float ball_x_movement=0;
static float ball_y_movement=0;

static float phi , theta ;
static float delta_phi , delta_theta ;

int main(int argc,char** argv)
{
    
    GLfloat light_ambient[] = { 1, 1, 1, 1 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };
    GLfloat light_specular[] = { 1, 1, 1, 1 };
    
    
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
    case 'w':
        ball_x_movement+=-0.2;
        glutPostRedisplay();
        break;
    case 's':
        ball_x_movement+=0.2;
        glutPostRedisplay();
        break;
    case 'a':
        ball_y_movement+=0.2;
        glutPostRedisplay();
        break;
    case 'd':
        ball_y_movement+=-0.2;
        glutPostRedisplay();
        break;
        
        
        
    case 'j':
        phi -= delta_phi;
        if (phi > 2 * pi) {
            phi -= 2 * pi;
        } else if (phi < 0) {
            phi += 2 * pi;
        }
        glutPostRedisplay();
        break;
    case 'i':
        theta += delta_theta;
        if (theta > pi / 2 - pi / 180) {
            theta = pi / 2 - pi / 180;
        }
        glutPostRedisplay();
        break;
    case 'l':
        phi += delta_phi;
        if (phi > 2 * pi) {
            phi -= 2 * pi;
        } else if (phi < 0) {
            phi += 2 * pi;
        }
        glutPostRedisplay();
        break;
    case 'k':
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
    gluPerspective(90, (float) width / height, 1, 20);
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
     /*   glVertex3f(0.5, -0.5, 0);
            glVertex3f(-0.5, -0.5, 0);
            glVertex3f(-0.5, 0.5, 0);
            glVertex3f(0.5, 0.5, 0);*/

	/*Kocke koje obelezavaju platforme na kojima ce se nalaziti igrac i protivnici*/
	
    glEnd();
    /*rename Colors*/
    
    GLfloat ambient_coeffsBall[] = {0.1, 0.1, 0.4 ,1};
    
    GLfloat ambient_coeffs1[] = { 0.5, 0.164706, 0.164706, 1 };
    GLfloat diffuse_coeffs1[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffs1[] = { 1, 1, 1, 1 };
    GLfloat shininess1 = 10;
    
    GLfloat ambient_coeffsBlack[] = { 0, 0, 0, 1 };
    GLfloat diffuse_coeffsBlack[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffs2[] = { 1, 1, 1, 1 };
    GLfloat shininess2 = 10;
    
    int i,j;
    glPushMatrix();
        glTranslatef(0,0,-0.1);
        for(i=0;i<10;i++)
        {
            glPushMatrix();
            for(j=0;j<10;j++)
            {
                /*toRename*/
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBlack);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBlack);
                glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs2);
                glMaterialf(GL_FRONT, GL_SHININESS, shininess2); 
                glutWireCube(0.2);
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs1);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs1);
                glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs1);
                glMaterialf(GL_FRONT, GL_SHININESS, shininess1); 
                glutSolidCube(0.20);
                glTranslatef(-0.2,0,0);
            }
            glPopMatrix();
            glTranslatef(0,-0.2,0);
        }
    glPopMatrix();
    
    
    glPushMatrix();
        glTranslatef(ball_y_movement,ball_x_movement,0.1);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBlack);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBlack);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs2);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess2); 
            glutWireSphere(0.1,10,10);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBlack);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs2);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess2); 
            glutSolidSphere(0.1,10,10);
    glPopMatrix();
    
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



