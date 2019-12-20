#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

static float timer = 0;   /*varijabla koja oznacava broj crtanja ekrana i sluzi za iscrtavanje plavih linija*/
const static float pi = 3.141592653589793;
static float ball_x_movement=-1;
static float ball_y_movement=-1;
static float ball_z_position = 0.1;
static float animation_timer = 0; /* Varijabla koja se inkrementira radi animacije objekata*/
static int ij1=0,ij2=0,ij3=0,ij4=0;
static int ongoing_animation = 0; /* Ako se animacija odvija jos uvek ili ne*/
static float pushButtonThreshold;

static int windowWidth = 700;
static int windowHeight = 700;
enum player_direction{
 
    UP,DOWN,LEFT,RIGHT
    
};
static int stopAnimation = 0;
static int scoreNum = 0;
#define bpm 0.07/*beats per minute, tj brzina igranja koju zelimo da postignemo*/
static float idleTimer = 0;


static float phi , theta ;
static float delta_phi , delta_theta ;
static int BallPosX ,BallPosY;
static int board[21][12]={
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 2 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
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

GLubyte rasters[20*24] = {
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

    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 
                                                    
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
                                         
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
                                         
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000,
    
};

typedef struct enemy{
    int health;
    int posX;
    int posY;
}Enemy;

Enemy blobs[3];

#define Animation_speed 0.025   /*intervali koliko se povecava varijabla animation_timer*/
#define Animation_threshold_movement 0.2 /* Ukupna kolicina "vremena" koliko ce se odvijati animacija za pomeranje*/
#define Animation_threshold_hit 0.1 /*Ukupna kolicina "vremena" koliko ce se odvijati animacija za udarac o protivnika*/
#define timerIncrementValue 1 /*za koliko se inkrementira timer u svakom pozivu funkcije*/
    

void changeMaterial(GLfloat *ambient,GLfloat *diffuse,GLfloat *specular)
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
    timer += timerIncrementValue;
    idleTimer += timerIncrementValue;
    if(idleTimer >= timerIncrementValue*(1/bpm)*3)
        scoreNum = 0;
    
    glutPostRedisplay();
    
    if(stopAnimation == 0)
        glutTimerFunc(50,dance,0);
}
    
void reduceHealthOfEnemy(int enemyPosX,int enemyPosY)
{
    int i;
    for(i=0;i<3;i++)
    {
        if(blobs[i].posX == enemyPosX && blobs[i].posY == enemyPosY)
        {
            blobs[i].health--;
            if(blobs[i].health == 0)
                board[enemyPosX][enemyPosY] = 0;
        }
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
    /*glutPostRedisplay();*/
    
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
   /* glutPostRedisplay();*/
    
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

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(500, 0);
    glutCreateWindow(argv[0]);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_spotlight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_spotlight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular_spotlight);
  
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  
    pushButtonThreshold = timerIncrementValue*(1/bpm)-timerIncrementValue*(1/bpm)/3;
    
    phi = pi/4;
    theta = pi / 4;
    delta_phi = delta_theta = pi / 90;
    
    /*deklaracija Neprijatelja*/
    srand(time(NULL));
    printf("%d",rand()%21);
    int i;
    for(i = 0;i<3;i++)
    {
        blobs[i].posX = rand()%21;
        blobs[i].posY = rand()%12;
        blobs[i].health = 2;
        
        if(board[blobs[i].posX][blobs[i].posY] == 0)
            board[blobs[i].posX][blobs[i].posY] = 2;
        else
            i--;
    }

 
    
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
            animation_timer = 0;        /*Kazemo da je pocetak animacije (unutar animate_movement funkcije raste*/
            ongoing_animation = 1;      /*Ako udjemo u if ne zelimo da udjemo opet dok se ne zavrsi animacija*/
            idleTimer = 0;              /*Resetujemo idleTimer na 0 kada se igrac pomeri*/
            glutTimerFunc(0,animate_movement,UP);
            
            board[BallPosX][BallPosY] = 0;
            BallPosY++;
            board[BallPosX][BallPosY] = 1;
            
           
            if(fmod(timer,(1/bpm)) >= pushButtonThreshold )
                scoreNum +=1;
            else
                scoreNum = 0;
        }
        else if(board[BallPosX][BallPosY+1] == 2 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            idleTimer = 0; 
            reduceHealthOfEnemy(BallPosX,BallPosY+1);
            
            glutTimerFunc(0,animate_hit,UP);
        }
            
        break;
    case 's':
        if(board[BallPosX][BallPosY-1] == 0 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            idleTimer = 0;
            glutTimerFunc(0,animate_movement,DOWN);
                
            board[BallPosX][BallPosY] = 0;
            BallPosY--;
            board[BallPosX][BallPosY] = 1;
            
            if(fmod(timer,(1/bpm)) >= pushButtonThreshold )
                scoreNum +=1;
            else
                scoreNum = 0;
            
        }
        else if(board[BallPosX][BallPosY-1] == 2 && ongoing_animation == 0)
        {
            animation_timer = 0;
            idleTimer = 0; 
            ongoing_animation = 1;
            reduceHealthOfEnemy(BallPosX,BallPosY-1);
            glutTimerFunc(0,animate_hit,DOWN);
            
        }
        break;
    case 'a':
        if(board[BallPosX-1][BallPosY] == 0 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            idleTimer = 0;
            glutTimerFunc(0,animate_movement,LEFT);
 
            
            board[BallPosX][BallPosY] = 0;
            BallPosX--;
            board[BallPosX][BallPosY] = 1;
            
            if(fmod(timer,(1/bpm)) >= pushButtonThreshold )
                scoreNum +=1;
            else
                scoreNum = 0;
            
        }
        else if(board[BallPosX-1][BallPosY] == 2 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            idleTimer = 0;
            reduceHealthOfEnemy(BallPosX-1,BallPosY);
            glutTimerFunc(0,animate_hit,LEFT);
            
        }
        break;
    case 'd':
        if(board[BallPosX+1][BallPosY] == 0 && ongoing_animation == 0)
        {
            
            animation_timer = 0;        
            ongoing_animation = 1;      
            idleTimer = 0;              
            glutTimerFunc(0,animate_movement,RIGHT);
 
            board[BallPosX][BallPosY] = 0;
            BallPosX++;
            board[BallPosX][BallPosY] = 1;
            
            if(fmod(timer,(1/bpm)) >= pushButtonThreshold )
                scoreNum +=1;
            else
                scoreNum = 0;
            
        }
        else if(board[BallPosX+1][BallPosY] == 2 && ongoing_animation == 0)
        {
            animation_timer = 0;
            ongoing_animation = 1;
            idleTimer = 0; 
            reduceHealthOfEnemy(BallPosX+1,BallPosY);
            glutTimerFunc(0,animate_hit,RIGHT);
            
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
        stopAnimation = 0;
        glutTimerFunc(100,dance,0);
        break;
        
    case 'h':
        stopAnimation = 1;
        break;
        
    }
}

static void on_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, 0, 1.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float) width / height, 1, 40);
    
    
}


static void on_display(void)
{   
   /* printf("%f %f\n",timer,fmod(timer,1/bpm));*/
    
    
    GLfloat light_position_spotlight[] = {ball_x_movement,ball_y_movement,1.5,1};
    GLfloat light_position[] = {0,0,0,0};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, 0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);

    glColor3f(1,1,1);
    glRasterPos2i(windowWidth/2-8*3, 20);
    
    char score[25] = {'S','c','o','r','e',' ','m','u','l','t','i','p','l','i','e','r',' ','\0'};
    sprintf(score+strlen(score),"%d",scoreNum);
    int z;
    int scoreLen = strlen(score);
    for(z=0;z<scoreLen;z++)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15,score[z]);

    glColor3f(0,0,0);
    glRasterPos2i(windowWidth/2-8*3, 40);
    glBitmap(8*8,12*4,0.0, 0.0,1.0, 0.0,rasters+24*4);
    float movementPerSecond = timer*87.5*bpm;
    
    /*Bitmapa pokretnih linija*/
    glColor3f(0.0, 0.5, 0.8);
    int i1 = (int)(437.5-movementPerSecond) + ij1*350;
    int i2 = (int)(525-movementPerSecond) + ij2*350;
    int i3 = (int)(612.5-movementPerSecond) + ij3*350; 
    int i4 = (int)(700-movementPerSecond) + ij4*350;
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
    
    glRasterPos2i((int)(0+movementPerSecond) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);

    glRasterPos2i((int)(87.5+movementPerSecond) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i((int)(175+movementPerSecond) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i((int)(262.5+movementPerSecond) % 350, 40);
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
    GLfloat ambient_coeffsEnemyBlobGreen[] = {0.1, 0.7, 0.4 ,0.4};
    GLfloat diffuse_coeffsEnemyBlobGreen[] = {0.1, 0.7, 0.4 ,0.4};
    
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
               
                changeMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                glutWireCube(0.2);
                changeMaterial(ambient_coeffsFloor,diffuse_coeffsFloor,specular_coeffs);              
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

                changeMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                glutWireCube(0.2);
                changeMaterial(ambient_coeffsFloor,diffuse_coeffsFloor,specular_coeffs);              
                glutSolidCube(0.2);
                
            }
                glTranslatef(0,-0.2,0);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,-2,0.1);
        for(i =0;i<11;i++)
        {

            changeMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
            glutWireCube(0.2);
            changeMaterial(ambient_coeffsFloor,diffuse_coeffsFloor,specular_coeffs);             
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
                        
                        changeMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                        glutWireCube(0.2);
                        changeMaterial(ambient_coeffsFloorBlue,diffuse_coeffsFloorBlue,specular_coeffs);
                        glutSolidCube(0.2);
                    }
                    else if(j!=0) /*zid putica*/
                    {
                        glPushMatrix();
                        glTranslatef(0,0,0.2);
                        changeMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                        glutWireCube(0.2);
                        changeMaterial(ambient_coeffsFloorBlue,diffuse_coeffsFloorBlue,specular_coeffs);              
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
    
    
    /*Neprijatelji*/


    int nE;
    for(nE=0;nE<3;nE++)
    {
        glPushMatrix();
        if(blobs[nE].health != 0)
        {
            glTranslatef(-0.2*(blobs[nE].posX -1),-0.2*(blobs[nE].posY - 1),0.1);
            changeMaterial(ambient_coeffsEnemyBlobGreen,diffuse_coeffsEnemyBlobGreen,specular_coeffs);
            glutSolidCube(0.15);
        }
        glPopMatrix();
    }
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



