#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

static float timer = 0;
const static float pi = 3.141592653589793;
static float ball_x_movement=-1;
static float ball_y_movement=-1;
static float ball_z_position = 0.1;
static float animation_timer = 0; /* Varijabla koja se inkrementira do odredjene vrednosti radi animacije objekata*/
static int ij1=0,ij2=0,ij3=0,ij4=0;
static int ongoing_animation = 0; /* Ako se animacija odvija jos uvek ili ne*/
enum player_direction{
 
    UP,DOWN,LEFT,RIGHT
    
};

static float phi , theta ;
static float delta_phi , delta_theta ;
static int BallPosX ,BallPosY;
static int board[21][12]={
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 2 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 , 0 , 0 ,-1 ,-1 ,-1 , -1}
        
    };

GLubyte rasters[4*24] = {
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
         
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
        
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
};

#define Animation_speed 0.025   /*intervali koliko se povecava varijabla animation_timer*/
#define Animation_threshold_movement 0.2 /* Ukupna kolicina "vremena" koliko ce se odvijati animacija za pomeranje*/
#define Animation_threshold_hit 0.1 /*Ukupna kolicina "vremena" koliko ce se odvijati animacija za udarac o protivnika*/
    
void floorMaterial(GLfloat *ambient,GLfloat *diffuse,GLfloat *specular)
{

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
}
    
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

void dance(int var)
{
    timer +=0.1;
    glutPostRedisplay();
    glutTimerFunc(100,dance,0);
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
    BallPosX = (-ball_x_movement)/0.2 + 1;
    BallPosY = (-ball_y_movement)/0.2 + 1;
    
    
    GLfloat light_ambient_spotlight[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat light_diffuse_spotlight[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat light_specular_spotlight[]= { 0.5, 0.5, 0.5, 1 };
  
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat light_diffuse[] = { 0.3, 0.3, 0.3, 1 };
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(700, 700);
    glutInitWindowPosition(500, 0);
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
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    
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
    case 'g':
        glutTimerFunc(100,dance,0);
        break;
        
    }
}

static void on_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float) width / height, 1, 40);
    
    printf("?");
    
}

static void on_display(void)
{
    
    GLfloat light_position_spotlight[] = {ball_x_movement,ball_y_movement,1.5,1};
    GLfloat light_position[] = {0,0,0,0};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 700, 0, 450, 0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);

    
    
    /*Bitmapa pokretnih linija*/
    glColor3f(0.0, 0.5, 0.8);
    int i1 = (int)(437.5-timer*70) + ij1*350;
    int i2 = (int)(525-timer*70) + ij2*350;
    int i3 = (int)(612.5-timer*70) + ij3*350;
    int i4 = (int)(700-timer*70) + ij4*350;
    if(i1 <= 350)
    {
        ij1++;
    }
    else if(i2 <= 350)
    {
        ij2++;
    } 
    else if(i3 <= 350)
    {
        ij3++;
    }
    else if(i4 <= 350)
    {
        ij4++;
    }
            
    glRasterPos2i( i1, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);

    glRasterPos2i( i2 , 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i( i3, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i( i4 , 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i((int)(0+timer*70) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);

    glRasterPos2i((int)(87.5+timer*70) % (350), 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i((int)(175+timer*70) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i((int)(262.5+timer*70) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 1, 40);
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

    glEnable(GL_LIGHTING);
    /*glScalef(3,3,3);*/


    GLfloat spot_direction[] = {0,0,-1};
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,24.0);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,40.0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position_spotlight);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);

    
    
    
    /*rename Colors*/
    
    GLfloat ambient_coeffsBall[] = {0.1, 0.1, 0.4 ,1};
    GLfloat diffuse_coeffsBall[] = {0.1, 0.1, 0.4 ,1};
    
    GLfloat ambient_coeffsFloor[] = {0.5, 0.164706, 0.164706, 1 };
    GLfloat diffuse_coeffsFloor[] = {0.5, 0.164706, 0.164706, 1 };
    
    GLfloat ambient_coeffsFloorBlue[] = {0.5, 0.164706, 0.8, 1 };
    GLfloat diffuse_coeffsFloorBlue[] = {0.5, 0.164706, 0.8, 1 };
    
    GLfloat specular_coeffs[] = {0.2, 0.2, 0.2, 1 };

    
    GLfloat ambient_coeffsBlack[] = { 0, 0, 0, 1 };
    GLfloat diffuse_coeffsBlack[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffs2[] = { 0, 0, 0, 1 };
    
    /*glPushMatrix();
        glTranslatef(-0.9,-0.9,-0.1);
        glScalef(10,10,1);
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsFloor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsFloor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess1);
        glutSolidCube(0.2);
    glPopMatrix();*/
    
    /*Kocke koje obelezavaju platforme na kojima ce se nalaziti igrac i protivnici*/
    int i,j;
    glPushMatrix();
        glTranslatef(0,0,-0.1);
        for(i=0;i<10;i++)
        {
            glPushMatrix();
            for(j=0;j<10;j++)
            {
               
                floorMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                glutWireCube(0.2);
                floorMaterial(ambient_coeffsFloor,diffuse_coeffsFloor,specular_coeffs);              
                glutSolidCube(0.2);
                glTranslatef(-0.2,0,0);
            }
            glPopMatrix();
            glTranslatef(0,-0.2,0);
        }
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(-2,0,0.1);
        for(i =0;i<10;i++)
        {
            if(i!= 5 && i!= 6)
            {

                floorMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                glutWireCube(0.2);
                floorMaterial(ambient_coeffsFloor,diffuse_coeffsFloor,specular_coeffs);              
                glutSolidCube(0.2);
                
            }
                glTranslatef(0,-0.2,0);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,-2,0.1);
        for(i =0;i<11;i++)
        {

            floorMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
            glutWireCube(0.2);
            floorMaterial(ambient_coeffsFloor,diffuse_coeffsFloor,specular_coeffs);             
            glutSolidCube(0.2);
            glTranslatef(-0.2,0,0);
        }
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-2,-0.8,-0.1);
        for(i=0;i<4;i++)
        {
            glPushMatrix();
            for(j=0;j<10;j++) 
            {
                    if(i != 0 && i!=3) /*putic*/
                    {
                        
                        floorMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                        glutWireCube(0.2);
                        floorMaterial(ambient_coeffsFloorBlue,diffuse_coeffsFloorBlue,specular_coeffs);
                        glutSolidCube(0.2);
                    }
                    else if(j!=0) /*zid putica*/
                    {
                        glPushMatrix();
                        glTranslatef(0,0,0.2);
                        floorMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                        glutWireCube(0.2);
                        floorMaterial(ambient_coeffsFloorBlue,diffuse_coeffsFloorBlue,specular_coeffs);              
                        glutSolidCube(0.2);
                            
                        glPopMatrix();
                    }
                    glTranslatef(-0.2,0,0);
                
            }
            glPopMatrix();
            glTranslatef(0,-0.2,0);
        }
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(ball_x_movement,ball_y_movement,ball_z_position);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBall);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs2);
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



