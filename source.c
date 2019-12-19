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
static float ball_z_position = 0.1;
static float animation_timer = 0; /* Varijabla koja se inkrementira do odredjene vrednosti radi animacije objekata*/
static int ongoing_animation = 0; /* Ako se animacija odvija jos uvek ili ne*/
enum player_direction{
 
    UP,DOWN,LEFT,RIGHT
    
};

static float phi , theta ;
static float delta_phi , delta_theta ;
static int BallPosX = 1,BallPosY = 1;
static int board[12][12]={
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , -1},
        {-1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 2 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , -1}
        
    };
#define Animation_speed 0.025   /*intervali koliko se povecava varijabla animation_timer*/
#define Animation_threshold_movement 0.2 /* Ukupna kolicina "vremena" koliko ce se odvijati animacija za pomeranje*/
#define Animation_threshold_hit 0.1 /*Ukupna kolicina "vremena" koliko ce se odvijati animacija za udarac o protivnika*/
    
void printMatrix()
{
    int i,j;
    for(i = 0;i<12;i++)
    {
        for(j=0;j<12;j++)
            printf("%d ",board[i][j]);
        printf("\n");
    }
}
    
void animate_movement(int player_direction){
 
    switch(player_direction)
    {
        case UP:
            if(animation_timer <= Animation_threshold_movement)
            {
                ball_y_movement+=-Animation_speed;
                animation_timer+=Animation_speed;
            }
            else 
            {
                ongoing_animation = 0;
                return;
            }
        break;
        case DOWN:
            if(animation_timer <= Animation_threshold_movement)
            {
                ball_y_movement+=Animation_speed;
                animation_timer+=Animation_speed;
            }
            else 
            {
                ongoing_animation = 0;
                return;
            }
        break;
        case LEFT:
            if(animation_timer <= Animation_threshold_movement)
            {
                ball_x_movement+=Animation_speed;
                animation_timer+=Animation_speed;
            }
            else 
            {
                ongoing_animation = 0;
                return;
            }
            
        break;
        case RIGHT:
            if(animation_timer <= Animation_threshold_movement)
            {
                ball_x_movement+=-Animation_speed;
                animation_timer+=Animation_speed;
            }
            else 
            {
                ongoing_animation = 0;
                return;
            }           
        break;
    }
    
    
    ball_z_position=0.1 + sin(5*animation_timer*pi)/10; /*poskok igraca*/
    glutPostRedisplay();
    
    glutTimerFunc(50,animate_movement,player_direction);
}

void animate_hit(int player_direction){
    
        switch(player_direction)
        {
        case UP:
            if(animation_timer <= Animation_threshold_movement)
            {
                if(animation_timer < Animation_threshold_movement/2)
                {
                    ball_y_movement+=-Animation_speed;
                }
                else
                {
                    ball_y_movement+=Animation_speed;
                }
                animation_timer+=Animation_speed;
            }
            else 
            {
                ongoing_animation = 0;
                return;
            }
        break;
        case DOWN:
            if(animation_timer <= Animation_threshold_movement)
            {
                if(animation_timer < Animation_threshold_movement/2)
                {
                    ball_y_movement+=Animation_speed;
                }
                else
                {
                    ball_y_movement+=-Animation_speed;
                }
                animation_timer+=Animation_speed;
            }
            else 
            {
                ongoing_animation = 0;
                return;
            }
        break;
        case LEFT:
            if(animation_timer <= Animation_threshold_movement)
            {
                if(animation_timer < Animation_threshold_movement/2)
                {
                    ball_x_movement+=Animation_speed;
                }
                else
                {
                    ball_x_movement+=-Animation_speed;
                }
                animation_timer+=Animation_speed;
            }
            else 
            {
                ongoing_animation = 0;
                return;
            }
            
        break;
        case RIGHT:
            if(animation_timer <= Animation_threshold_movement)
            {
                if(animation_timer < Animation_threshold_movement/2)
                {
                    ball_x_movement+=-Animation_speed;
                }
                else
                {
                    ball_x_movement+=Animation_speed;
                }
                animation_timer+=Animation_speed;
            }
            else 
            {
                ongoing_animation = 0;
                return;
            }           
        break;
    }
    
    
    ball_z_position=0.1 + sin(5*animation_timer*pi)/10; /*poskok igraca*/
    glutPostRedisplay();
    
    glutTimerFunc(50,animate_hit,player_direction);
    
}

int main(int argc,char** argv)
{
    
    
    
    GLfloat light_ambient_spotlight[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat light_diffuse_spotlight[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat light_specular_spotlight[] = { 0.5, 0.5, 0.5, 1 };
  
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat light_diffuse[] = { 0.3, 0.3, 0.3, 1 };
    
    
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
    glEnable(GL_LIGHT1);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_spotlight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_spotlight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular_spotlight);
  
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  

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
        if(board[BallPosX][BallPosY+1] == 0 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            glutTimerFunc(0,animate_movement,UP);
            
            board[BallPosX][BallPosY] = 0;
            BallPosY++;
            board[BallPosX][BallPosY] = 1;
            
            printMatrix();
            
            
        }
        else if(board[BallPosX][BallPosY+1] == 2 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            glutTimerFunc(0,animate_hit,UP);
            printMatrix();
            
        }
            
        break;
    case 's':
        if(board[BallPosX][BallPosY-1] == 0 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            glutTimerFunc(0,animate_movement,DOWN);
                
            board[BallPosX][BallPosY] = 0;
            BallPosY--;
            board[BallPosX][BallPosY] = 1;
            
        }
        else if(board[BallPosX][BallPosY-1] == 2 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            glutTimerFunc(0,animate_hit,DOWN);
            printMatrix();
            
        }
        break;
    case 'a':
        if(board[BallPosX-1][BallPosY] == 0 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            glutTimerFunc(0,animate_movement,LEFT);
 
            
            board[BallPosX][BallPosY] = 0;
            BallPosX--;
            board[BallPosX][BallPosY] = 1;
            
        }
        else if(board[BallPosX-1][BallPosY] == 2 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            glutTimerFunc(0,animate_hit,LEFT);
            printMatrix();
            
        }
        break;
    case 'd':
        if(board[BallPosX+1][BallPosY] == 0 && ongoing_animation == 0)
        {
            
            animation_timer = 0;
            ongoing_animation = 1;
            glutTimerFunc(0,animate_movement,RIGHT);
 
            board[BallPosX][BallPosY] = 0;
            BallPosX++;
            board[BallPosX][BallPosY] = 1;
            
        }
        else if(board[BallPosX+1][BallPosY] == 2 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            glutTimerFunc(0,animate_hit,RIGHT);
            printMatrix();
            
        }
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
    gluPerspective(90, (float) width / height, 1, 40);
}

static void on_display(void)
{
    GLfloat light_position_spotlight[] = {ball_x_movement,ball_y_movement,1,1};
    GLfloat light_position[] = {0,0,0,0};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /*
    printf("5*cos(theta)*cos(phi)+ball_x_movement = %f\n", 5 * cos(theta) * cos(phi)+ball_x_movement);
    printf("5 * cos(theta) * sin(phi)+ball_y_movement = %f\n",5 * cos(theta) * sin(phi)+ball_y_movement);
    printf("5 * sin(theta) = %f\n", 5 * sin(theta));
    printf("ball_x_movement: %0.2f\n",ball_x_movement);
    printf("ball_y_movement: %0.2f\n",ball_y_movement);
    */
     
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( 2*cos(theta) * cos(phi)+ball_x_movement, 
               2*cos(theta) * sin(phi)+ball_y_movement,
               2.5*sin(theta), 
              ball_x_movement, ball_y_movement, 0.1, 
              0, 0, 1);

    /*glScalef(3,3,3);*/


    GLfloat spot_direction[] = {0,0,-1};
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,24.0);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,40.0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position_spotlight);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
   /* 
    glPushMatrix();
        glTranslatef(ball_x_movement,ball_y_movement,0.8);
        glutSolidCube(0.1);
    glPopMatrix();*/
	/*Kocke koje obelezavaju platforme na kojima ce se nalaziti igrac i protivnici*/
	
    
    /*rename Colors*/
    
    GLfloat ambient_coeffsBall[] = {0.1, 0.1, 0.4 ,1};
    GLfloat diffuse_coeffsBall[] = {0.1, 0.1, 0.4 ,1};
    
    GLfloat ambient_coeffsFloor[] = {0.5, 0.164706, 0.164706, 1 };
    GLfloat diffuse_coeffsFloor[] = {0.5, 0.164706, 0.164706, 1 };
    GLfloat specular_coeffs[] = {1, 1, 1, 1 };

    GLfloat shininess1 = 30;
    
    GLfloat ambient_coeffsBlack[] = { 0, 0, 0, 1 };
    GLfloat diffuse_coeffsBlack[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffs2[] = { 0, 0, 0, 1 };
    
    /*glPushMatrix();*/
        /*glTranslatef(-0.9,-0.9,-0.1);*/
      /*  glTranslatef(0,0,-0.1);
        glScalef(20,20,1);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsFloor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsFloor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess1);
        glutSolidCube(0.2);
    glPopMatrix();*/
    int i,j;
    glPushMatrix();
        glTranslatef(0,0,-0.1);
        for(i=0;i<10;i++)
        {
            glPushMatrix();
            for(j=0;j<10;j++)
            {
               /* toRename*/
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBlack);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBlack);
                glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
                glutWireCube(0.2);
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsFloor);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsFloor);
                glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
                glMaterialf(GL_FRONT, GL_SHININESS, shininess1);               
                glutSolidCube(0.2);
                glTranslatef(-0.2,0,0);
            }
            glPopMatrix();
            glTranslatef(0,-0.2,0);
        }
    glPopMatrix();
    
    /*sfere*/
    glPushMatrix();
        glTranslatef(ball_x_movement,ball_y_movement,ball_z_position);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBlack);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBlack);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
            glutWireSphere(0.1,10,10);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBall);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs2);
     /*   glMaterialf(GL_FRONT, GL_SHININESS,shininess1);*/
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



