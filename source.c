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

typedef struct player{
    float worldPosX;
    float worldPosY;
    float worldPosZ;
    int playerHealth;
}Player;

Player Ball;

static float animation_timer = 0; /* Varijabla koja se inkrementira radi animacije objekata*/
static float animation_timer_for_getting_hit = 0; /*Varijabla koja se inkrementira radi animacije primanja udarca od neprijatelja*/
static int ij1=0,ij2=0,ij3=0,ij4=0; /*varijable koje sluze radi racunanja pozicije plavih linija*/
static int ongoing_animation = 0; /* Animacija se odvija ako je varijabla 1, u suprotnom 0*/
static float pushButtonThreshold; /* Varijabla koja oznacava dokle sme da ide animation_timer */

static int windowWidth = 700;
static int windowHeight = 700;
enum player_direction{
 
    UP,DOWN,LEFT,RIGHT
    
};
static int scoreNum = 0;         /*Varijabla koja sluzi da povecava multiplierScore. Povecava se svaki put kada se igrac pomeri u pravom ritmu*/
static int multiplierScore = 0;  /*Trenutni multiplier. Mnozi se sa 4 i daje poene igracu kad god ubije monstruma*/
static int scoreGained = 0; /*Koliko igrac ima poena*/
#define bpm 0.07/*beats per minute, tj brzina igranja koju zelimo da postignemo*/
static float idleTimer = 0; /*tajmer koji sluzi da proveri koliko dugo igrac ne radi nikakvu akciju*/

static float phi , theta ; /*ugao pod kojim se nalazi kamera*/
static int BallPosX ,BallPosY; /*pozicija lopte po x i y koordinatama na matrici*/
/*matrica koja sluzi za racunanje svih interakcija medju objektima*/
static int board[22][22]={
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1},
        {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 , -1},
                                                                                              
    };
/*plave linije i crna kutija*/
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
    float posZ;
    int deleted;
}Enemy;


#define NumberOfEnemies 10
static int NumberOfEnemiesLeft;

void animate_movement(int direction);
void dance(int var);
void printMatrix();
void changeMaaterial(GLfloat *ambient,GLfloat *diffuse,GLfloat *specular);
int nexttileIs(int posX,int posY);
void playerLoseLife();
void kikPlayer(int way,int direction);
void jumpInPlaceAnimation(int blobNumber);
void enemyMove(Enemy *blobs,const int blobNumber,const int direction);
void reduceHealthOfEnemy(int enemyPosX,int enemyPosY);
void animate_hit(int player_direction);
void animate_getting_hit(int player_direction);

Enemy blobs[NumberOfEnemies];

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
    for(i = 0;i<22;i++)
    {
        for(j=0;j<22;j++)
            printf("%d ",board[i][j]);
        printf("\n");
    }
    printf("\n");
}
/*Najbitnija funkcija koja se  tokom citavog rada igrice poziva.*/
void dance(int var)
{
    timer += timerIncrementValue;    
    idleTimer += timerIncrementValue;
    if(idleTimer >= timerIncrementValue*(1/bpm)*3)
        scoreNum = 0;
    
    glutPostRedisplay();
    
    glutTimerFunc(50,dance,0);
}

int nextTileIs(int posX,int posY)
{
    if(board[posX][posY] == 0)
        return 0;
    else if(board[posX][posY] == 2 || (board[posX][posY] == -1))
        return -1;
    else if(board[posX][posY] == 1)
        return 1;
    
    return -2;
}

void playerLoseLife()
{
    Ball.playerHealth--;
}

void kickPlayer(int way,int direction)
{

    ongoing_animation = 1;
    animation_timer_for_getting_hit = 0;

    
    if(way == 0) /*gore dole*/
    {
        if(direction == -1) 
        {
            glutTimerFunc(50,animate_getting_hit,DOWN);
            
            board[BallPosX][BallPosY] = 0;
            BallPosY-=2;
            board[BallPosX][BallPosY] = 1;  
        }
        else  
        {
            glutTimerFunc(50,animate_getting_hit,UP);
            
            board[BallPosX][BallPosY] = 0;
            BallPosY+=2;
            board[BallPosX][BallPosY] = 1;
        }
    }
    else /*levo desno*/
    {
        
        if(direction == -1)
        {
            glutTimerFunc(50,animate_getting_hit,LEFT);
            
            board[BallPosX][BallPosY] = 0;
            BallPosX-=2;
            board[BallPosX][BallPosY] = 1;
        }
        else 
        {
            glutTimerFunc(50,animate_getting_hit,RIGHT);
            
            board[BallPosX][BallPosY] = 0;
            BallPosX+=2;
            board[BallPosX][BallPosY] = 1;
        }
        
    }
    
    
}
/*Animacija slime monsturma kako skacu u mestu*/
void jumpInPlaceAnimation(int blobNumber)
{
    blobs[blobNumber].posZ = 0.15 - cos((fmod(timer,1/bpm))*bpm*4*pi)/10;
    
    if(fmod(timer,1/bpm) <= ((1/bpm)-1))
    glutTimerFunc(50,jumpInPlaceAnimation,blobNumber);
}
/*Animacija i ponasanje monstruma*/
void enemyMove(Enemy *blobs,const int blobNumber,const int direction)
{
    int randomDirection = rand() % 3 - 1;

    if(randomDirection != 0)
    {
        board[blobs[blobNumber].posX][blobs[blobNumber].posY] = 0;
        
        if(direction == 1)  /*gore dole*/
        {
            if(nextTileIs(blobs[blobNumber].posX+randomDirection,blobs[blobNumber].posY) == 0)
            {
                
                blobs[blobNumber].posX+=randomDirection;
                
            }
            else if(nextTileIs(blobs[blobNumber].posX+randomDirection,blobs[blobNumber].posY) == 1)
            {
                kickPlayer(1,randomDirection);
                playerLoseLife();
                blobs[blobNumber].posX+=randomDirection;
            }
        }
        else if(direction == 0) /* levo desno*/
        {
            if(nextTileIs(blobs[blobNumber].posX,blobs[blobNumber].posY+randomDirection) == 0)
            {
                blobs[blobNumber].posY+=randomDirection;
            }
            else if(nextTileIs(blobs[blobNumber].posX,blobs[blobNumber].posY+randomDirection) == 1)
            {
                kickPlayer(0,randomDirection);
                playerLoseLife();
                blobs[blobNumber].posY+=randomDirection;
            }
            
        }
        board[blobs[blobNumber].posX][blobs[blobNumber].posY] = 2;
    }
    else /*u slucaju da je randomDirection == 0 onda ce slime da skakuce*/
    {
        
        glutTimerFunc(50,jumpInPlaceAnimation,blobNumber);
    
    }

}
    /*Ako igrac povredi slime-a on izgubi helte*/
void reduceHealthOfEnemy(int enemyPosX,int enemyPosY)
{
    int i;
    for(i=0;i<NumberOfEnemies;i++)
    {
        if(blobs[i].posX == enemyPosX && blobs[i].posY == enemyPosY)
        {
            blobs[i].health--;
            if(blobs[i].health == 0)
                board[enemyPosX][enemyPosY] = 0;
        }
    }

}
/*Ako slime skoci na igraca povredi ga i odbaci za 2 kocke u pravcu kretanja*/
void animate_getting_hit(int player_direction)
{
    switch(player_direction)
    {
        case UP:
            if(animation_timer_for_getting_hit <= Animation_threshold_movement)
            {
                Ball.worldPosY+=-Animation_speed*2;
                animation_timer_for_getting_hit+=Animation_speed;
            }
            else 
            {
                
                ongoing_animation = 0;
                return;
            }
        break;
        case DOWN:
            if(animation_timer_for_getting_hit <= Animation_threshold_movement)
            {
                Ball.worldPosY+=Animation_speed*2;
                animation_timer_for_getting_hit+=Animation_speed;
            }
            else 
            {
                
                ongoing_animation = 0;
                return;
            }
        break;
        case LEFT:
            if(animation_timer_for_getting_hit <= Animation_threshold_movement)
            {
                Ball.worldPosX+=Animation_speed*2;
                animation_timer_for_getting_hit+=Animation_speed;
            }
            else 
            {
                
                ongoing_animation = 0;
                return;
            }
            
        break;
        case RIGHT:
            if(animation_timer_for_getting_hit <= Animation_threshold_movement)
            {
                Ball.worldPosX+=-Animation_speed*2;
                animation_timer_for_getting_hit+=Animation_speed;
            }
            else 
            {
                
                ongoing_animation = 0;
                return;
            }           
        break;
    }
    
    
    Ball.worldPosZ=0.1 + sin(5*animation_timer_for_getting_hit*pi)/5; /*poskok igraca*/
    
    glutTimerFunc(50,animate_getting_hit,player_direction);
    
}

    /*animiranje igraca u zavisnosti u kom smeru skace*/
void animate_movement(int player_direction){
 
    switch(player_direction)
    {
        case UP:
            if(animation_timer <= Animation_threshold_movement)
            {
                Ball.worldPosY+=-Animation_speed;
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
                Ball.worldPosY+=Animation_speed;
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
                Ball.worldPosX+=Animation_speed;
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
                Ball.worldPosX+=-Animation_speed;
                animation_timer+=Animation_speed;
            }
            else 
            {
                
                ongoing_animation = 0;
                return;
            }           
        break;
    }
    
    
    Ball.worldPosZ=0.1 + sin(5*animation_timer*pi)/10; /*poskok igraca*/
    
    glutTimerFunc(50,animate_movement,player_direction);
}
/*Animacija kada igrac povredi slime-a*/
void animate_hit(int player_direction){
    
        switch(player_direction)
        {
        case UP:
            if(animation_timer <= Animation_threshold_movement)
            {
                if(animation_timer < Animation_threshold_movement/2)
                {
                    Ball.worldPosY+=-Animation_speed;
                }
                else
                {
                    Ball.worldPosY+=Animation_speed;
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
                    Ball.worldPosY+=Animation_speed;
                }
                else
                {
                    Ball.worldPosY+=-Animation_speed;
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
                    Ball.worldPosX+=Animation_speed;
                }
                else
                {
                    Ball.worldPosX+=-Animation_speed;
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
                    Ball.worldPosX+=-Animation_speed;
                }
                else
                {
                    Ball.worldPosX+=Animation_speed;
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
    
    
    Ball.worldPosZ=0.1 + sin(5*animation_timer*pi)/10; /*poskok igraca*/
    
    glutTimerFunc(50,animate_hit,player_direction);
    
}
/*postavljanje svih varijabli na bazicno stanje*/
void initialize()
{
    /*inicijalizacija igraca*/
    Ball.worldPosX = -1;      /*x pozicija lopte u prostoru igre*/
    Ball.worldPosY = -1;       /*y pozicija lopte u prostoru igre*/
    Ball.worldPosZ = 0.1;     /*Broj zivotnih poena koju ima igrac*/
    Ball.playerHealth = 3;      /*z pozicija lopte u prostoru igre*/
    
    NumberOfEnemiesLeft = NumberOfEnemies;
    
    /*racunanje pozicije lopte u matrici*/
    BallPosX = (-Ball.worldPosX)/0.2 + 1; 
    BallPosY = (-Ball.worldPosY)/0.2 + 1;
    
    multiplierScore = 0;
    scoreGained = 0;
    
    int i,j;
    /*brisanje svega sa matrice*/
    for(i=0;i<22;i++)
    {
        for(j=0;j<22;j++)
        {
            if(i!=0 && j!=0 && i!= 21 && j!= 21)
                board[i][j] = 0;
        }
    }
    /*inicijalizacija neprijatelja*/
    srand(time(NULL));
    for(i = 0;i<NumberOfEnemies;i++)
    {
        blobs[i].posX = rand()%20;
        blobs[i].posY = rand()%20;
        blobs[i].posZ = 0.1;
        blobs[i].health = 2;
        blobs[i].deleted = 0;
        
        if(board[blobs[i].posX][blobs[i].posY] == 0)
            board[blobs[i].posX][blobs[i].posY] = 2;
        else
            i--;
    }  
    
}

int main(int argc,char** argv)
{
    
    initialize();
    
    /*paramteri koji sluze za spotlight svetlo*/
    GLfloat light_ambient_spotlight[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat light_diffuse_spotlight[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat light_specular_spotlight[]= { 0.5, 0.5, 0.5, 1 };
  
    /*paramteri za obicno svetlo*/
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
  
    /*vrednost koja diktira koliko lufta igrac ima da klikne dugme za pomeranje da bi dobio poen*/
    pushButtonThreshold = timerIncrementValue*(1/bpm)-timerIncrementValue*(1/bpm)/3;
    
    phi = pi/4;
    theta = pi / 4;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glClearColor(0.2, 0.2, 0.2, 0);
    glEnable(GL_DEPTH_TEST);
    
    
    glutTimerFunc(100,dance,0);

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
        if(ongoing_animation == 0 && Ball.playerHealth > 0)
        {
            idleTimer = 0;              /*Resetujemo idleTimer na 0 kada se igrac pomeri*/
            if(fmod(timer,(1/bpm)) >= pushButtonThreshold )
            {
                
                scoreNum +=1;
                /*multiplierScore se povecava u zavinosti koliko puta za redom smo uspeli da skocimo u ritmu tj koliko puta se povecao scoreNum*/
                if(scoreNum >= 15)
                    multiplierScore = 4;
                else if(scoreNum >= 10)
                    multiplierScore = 3;
                else if(scoreNum >= 5)
                    multiplierScore = 2;
                else if(scoreNum >= 2)
                    multiplierScore = 1;
            }
            else
            {   
                multiplierScore = 0;
                scoreNum = 0;
            }
                
            if(board[BallPosX][BallPosY+1] == 0 ) /*u slucaju da je sledece polje prazno smes da skocis na njega*/
            {
                animation_timer = 0;        /*Kazemo da je pocetak animacije (unutar animate_movement funkcije raste*/
                ongoing_animation = 1;      /*Ako udjemo u if ne zelimo da udjemo opet dok se ne zavrsi animacija*/
                glutTimerFunc(0,animate_movement,UP);
                
                board[BallPosX][BallPosY] = 0;
                BallPosY++;
                board[BallPosX][BallPosY] = 1;           
            }
            else if(board[BallPosX][BallPosY+1] == 2 ) /*Ako je na sledecem polju neprijatelj, povredi ga*/
            {
                animation_timer = 0;
                ongoing_animation = 1;
                reduceHealthOfEnemy(BallPosX,BallPosY+1);
                
                glutTimerFunc(0,animate_hit,UP);
            }
        }
        break;
    case 's':
        if(ongoing_animation == 0 && Ball.playerHealth > 0)
        {
            idleTimer = 0;
            if(fmod(timer,(1/bpm)) >= pushButtonThreshold )
            {
                scoreNum +=1;
                if(scoreNum >= 15)
                    multiplierScore = 4;
                else if(scoreNum >= 10)
                    multiplierScore = 3;
                else if(scoreNum >= 5)
                    multiplierScore = 2;
                else if(scoreNum >= 2)
                    multiplierScore = 1;
            }
            else
            {   
                multiplierScore = 0;
                scoreNum = 0;
            }
            
            if(board[BallPosX][BallPosY-1] == 0 )
            {
                animation_timer = 0;
                ongoing_animation = 1;
                
                glutTimerFunc(0,animate_movement,DOWN);
                    
                board[BallPosX][BallPosY] = 0;
                BallPosY--;
                board[BallPosX][BallPosY] = 1;        
            }
            else if(board[BallPosX][BallPosY-1] == 2 )
            {
                animation_timer = 0;
                ongoing_animation = 1;
                reduceHealthOfEnemy(BallPosX,BallPosY-1);
                glutTimerFunc(0,animate_hit,DOWN);
                
            }
        }
        break;
    case 'a':
        if(ongoing_animation == 0 && Ball.playerHealth > 0)
        {
            idleTimer = 0;
            if(fmod(timer,(1/bpm)) >= pushButtonThreshold )
            {
                scoreNum +=1;
                if(scoreNum >= 15)
                    multiplierScore = 4;
                else if(scoreNum >= 10)
                    multiplierScore = 3;
                else if(scoreNum >= 5)
                    multiplierScore = 2;
                else if(scoreNum >= 2)
                    multiplierScore = 1;
            }
            else 
            {   
            multiplierScore = 0;
            scoreNum = 0;
            }
            
            if(board[BallPosX-1][BallPosY] == 0)
            {
                animation_timer = 0;
                ongoing_animation = 1;
                
                glutTimerFunc(0,animate_movement,LEFT);
    
                board[BallPosX][BallPosY] = 0;
                BallPosX--;
                board[BallPosX][BallPosY] = 1;
            }
            else if(board[BallPosX-1][BallPosY] == 2 )
            {
                animation_timer = 0;
                ongoing_animation = 1;
                reduceHealthOfEnemy(BallPosX-1,BallPosY);
                glutTimerFunc(0,animate_hit,LEFT);
                
            }
        }
        break;
    case 'd':
        if(ongoing_animation == 0 && Ball.playerHealth > 0)
        {
            idleTimer = 0; 
            if(fmod(timer,(1/bpm)) >= pushButtonThreshold )
            {
                scoreNum +=1;
                if(scoreNum >= 15)
                    multiplierScore = 4;
                else if(scoreNum >= 10)
                    multiplierScore = 3;
                else if(scoreNum >= 5)
                    multiplierScore = 2;
                else if(scoreNum >= 2)
                    multiplierScore = 1;
            }
            else
            {   
                multiplierScore = 0;
                scoreNum = 0;
            }
            
            if(board[BallPosX+1][BallPosY] == 0 )
            {            
                animation_timer = 0;        
                ongoing_animation = 1;                   
                glutTimerFunc(0,animate_movement,RIGHT);
    
                board[BallPosX][BallPosY] = 0;
                BallPosX++;
                board[BallPosX][BallPosY] = 1;
                                                    
            }
            else if(board[BallPosX+1][BallPosY] == 2 )
            {
                animation_timer = 0;
                ongoing_animation = 1;
                reduceHealthOfEnemy(BallPosX+1,BallPosY);
                glutTimerFunc(0,animate_hit,RIGHT);
                
            }
        }
        break;
        
    case 'r':
    case 'R':
        initialize();
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
    GLfloat light_position_spotlight[] = {Ball.worldPosX,Ball.worldPosY,1.5,1};
    GLfloat light_position[] = {0,0,0,0};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, 0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);

    glColor3f(1,1,1);
    glRasterPos2i(windowWidth/2-8*10, 650);
    
    /*Ispisivanje multiplier-a preko bitmapa*/
    char score[25] = {'S','c','o','r','e',' ','m','u','l','t','i','p','l','i','e','r',':',' ','\0'};
    sprintf(score+strlen(score),"%d",multiplierScore);
    int z;
    int scoreLen = strlen(score);
    for(z=0;z<scoreLen;z++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,score[z]);    /*funkcija koja ispisuje string kao bitmapu*/
    
    /*Ispisivanje zivota preko bitmapa*/
    glRasterPos2i(0, 650);
    char lives[25] = {'L','i','v','e','s',':',' ','\0'};
    sprintf(lives+strlen(lives),"%d",Ball.playerHealth);
    int livesLen = strlen(lives);
    for(z=0;z<livesLen;z++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,lives[z]);  
    
    /*Ispisivanje score preko bitmapa*/
    glRasterPos2i(windowWidth-100, 650);
    char endScore[12] = {'S','c','o','r','e',':',' ','\0'};
    sprintf(endScore+strlen(endScore),"%d",scoreGained);
    int endScoreLen = strlen(endScore);
    for(z=0;z<endScoreLen;z++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,endScore[z]);   

    /*Iscrtavanje crnog kvadrata*/
    glColor3f(0,0,0);
    glRasterPos2i(windowWidth/2-8*3, 40);    /*Nalazi se na sredini ekrana uvek*/
    glBitmap(8*8,12*4,0.0, 0.0,1.0, 0.0,rasters+24*4);
    
    if(Ball.playerHealth <= 0) /*Ako igrac nema vise helta, ispisi game over*/
    {
        glColor3f(0, 0, 0); 
        glRasterPos2i( windowWidth/2-50,windowHeight/2 );
        char gameOver[10] = {'G','a','m','e',' ','o','v','e','r','\0'};
        int gameOverLen = strlen(gameOver);
        for(z=0;z<gameOverLen;z++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15,gameOver[z]);
    }
    
    if(NumberOfEnemiesLeft == 0) /*Ako nema vise neprijatelja, ispisi you win!*/
    {
        glColor3f(0, 0, 0); 
        glRasterPos2i( windowWidth/2-50,windowHeight/2 );
        char gameOver[10] = {'Y','o','u',' ','w','i','n','!','!','\0'};
        int gameOverLen = strlen(gameOver);
        for(z=0;z<gameOverLen;z++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15,gameOver[z]);
        
    }
    
        
    float movementPerSecond = timer*87.5*bpm;    /*Svaki frejm sve linije se pomere za 87.5*bpm*/
    /*Bitmapa pokretnih linija*/                  /*Posto mi je originalna velicina ekrana 700 x 700 onda mi se sredina nalazi na 350, sto znaci da ako se timer inkrementira za po 1 svaki put on ce u roku od 4 frejma doci do 350 po ovoj formuli. Da bismo usporili linije uvodimo promenljivu bpm koja usporava movementPerSecond.*/
    glColor3f(0.0, 0.5, 0.8);                      
    /*Svi ovi intovi (i1,i2,i3,i4) predstavljaju pojedine linije koje se nalaze sa desne strane kocke. Posto moram da ih vratim na pocetno mesto kad stignu u sredinu moram ponaosob da pratim dokle su stigli i ako su stigli do 350 saberem svaku x komponentu sa 350 puta koliko puta su dosli do 350*/
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
    /*Iscrtavanje linija koje idu sa desna na levo*/
    glRasterPos2i( i1, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);

    glRasterPos2i( i2 , 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i( i3, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i( i4 , 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    /*Iscrtavanje linija koje idu sa leva na desno*/
    glRasterPos2i((int)(0+movementPerSecond) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);

    glRasterPos2i((int)(87.5+movementPerSecond) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i((int)(175+movementPerSecond) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    glRasterPos2i((int)(262.5+movementPerSecond) % 350, 40);
    glBitmap(16,48,0.0, 0.0,0.0, 0.0,rasters);
    
    /*3D perspektiva*/

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 1, 40);

    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( 2*cos(theta) * cos(phi)+Ball.worldPosX, 
               2*cos(theta) * sin(phi)+Ball.worldPosY,
               2.5*sin(theta), 
              Ball.worldPosX, Ball.worldPosY, 0.1, 
              0, 0, 1);

    /*Osvetljenje*/
    glEnable(GL_LIGHTING);


    GLfloat spot_direction[] = {0,0,-1};
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,24.0);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,40.0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position_spotlight);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);

    /*zeleni slime monstrum*/
    GLfloat ambient_coeffsEnemyBlobGreen[] = {0.1, 0.7, 0.4 ,0.4};
    GLfloat diffuse_coeffsEnemyBlobGreen[] = {0.1, 0.7, 0.4 ,0.4};
    /*plavi slime monstrum*/
    GLfloat ambient_coeffsEnemyBlobBlue[] = {0.1, 0.7, 0.8 ,0.4};
    GLfloat diffuse_coeffsEnemyBlobBlue[] = {0.1, 0.7, 0.8 ,0.4};
    /*Igrac*/
    GLfloat ambient_coeffsBall[] = {0.1, 0.1, 0.4 ,1};
    GLfloat diffuse_coeffsBall[] = {0.1, 0.1, 0.4 ,1};
    /*Pod*/
    GLfloat ambient_coeffsFloor[] = {0.5, 0.164706, 0.164706, 1 };
    GLfloat diffuse_coeffsFloor[] = {0.5, 0.164706, 0.164706, 1 };
   
    GLfloat specular_coeffs[] = {0.2, 0.2, 0.2, 1 };

    
    GLfloat ambient_coeffsBlack[] = { 0, 0, 0, 1 };
    GLfloat diffuse_coeffsBlack[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffs2[] = { 0, 0, 0, 1 };
    
    
    /*Kocke koje obelezavaju platforme na kojima ce se nalaziti igrac i protivnici*/
    int i,j;
    glPushMatrix();
        
        for(i=0;i<22;i++)
        {
            glPushMatrix();
            for(j=0;j<22;j++)
            {
                if(board[i][j] == -1) /*u slucaju da se u matrici nalazi -1, iscrtaj zid*/
                {
                    
                    glPushMatrix();
                    glTranslatef(-0.2*i+0.2,-0.2*j+0.2,0.1);
                    changeMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                    glutWireCube(0.2);
                    changeMaterial(ambient_coeffsFloor,diffuse_coeffsFloor,specular_coeffs);              
                    glutSolidCube(0.2);
                    glPopMatrix();
                    
                }
                else/*ako je bilo sta drugo (verovatno 0), iscrtaj pod*/
                {
                    glPushMatrix();
                    glTranslatef(-0.2*i+0.2,-0.2*j+0.2,-0.1);
                    changeMaterial(ambient_coeffsBlack,diffuse_coeffsBlack,specular_coeffs);
                    glutWireCube(0.2);
                    changeMaterial(ambient_coeffsFloor,diffuse_coeffsFloor,specular_coeffs);              
                    glutSolidCube(0.2);
                    glPopMatrix();
                }
            }
            glPopMatrix();
        }
    glPopMatrix();
    

    /*Iscrtavanje lopte*/
    if(Ball.playerHealth > 0) /*ako igrac ima 0 helta izgubio je, pa se lopta vise ne iscrtava*/
    {
        glPushMatrix();
            glTranslatef(Ball.worldPosX,Ball.worldPosY,Ball.worldPosZ);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBall);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs2);
            glutSolidSphere(0.1,10,10);
        glPopMatrix();
    }
    else
    {
        board[BallPosX][BallPosY] = 0;
    }

    
    /*Neprijatelji*/

    int isEnemyMoving; /*Ako je broj veci od 70 neprijatelj se pomera, tj ima 30% sanse da se neprijatelj pomeri*/

    int nE;
    for(nE=0;nE<NumberOfEnemies;nE++)
    {
        isEnemyMoving = rand() %100;
        
        glPushMatrix();
        if(blobs[nE].health != 0)
        {
            /*fmod(timer,1/bpm) >= (1/bpm) - 1 oznacava trenutak kada se neprijatelji pomeraju*/
            if(isEnemyMoving >= 70 && fmod(timer,1/bpm) >= (1/bpm) - 1) 
                enemyMove(blobs,nE,nE%2);
            /*Iscrtavanje neprijatelja*/
            glTranslatef(-0.2*(blobs[nE].posX - 1),-0.2*(blobs[nE].posY - 1),blobs[nE].posZ);
            
            /*u zavisnosti da li su parni ili neparni indeksi niza monstruma iscrtavaju se plavi ili zeleni*/
            if(nE%2 == 0) 
                changeMaterial(ambient_coeffsEnemyBlobGreen,diffuse_coeffsEnemyBlobGreen,specular_coeffs);
            else
                changeMaterial(ambient_coeffsEnemyBlobBlue,diffuse_coeffsEnemyBlobBlue,specular_coeffs);
            glutSolidCube(0.15);
        } 
        else if(blobs[nE].deleted != 1) /*kada blob umre zelimo da igrac dobije poene, pa da izbrisemo blob-a skroz*/
        {
            NumberOfEnemiesLeft--;
            blobs[nE].deleted = 1;
            scoreGained = scoreGained + multiplierScore * 4;
        }
        glPopMatrix();
        
    }
    
    glutSwapBuffers();
}
