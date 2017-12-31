///TANK SHOOTER
///PREPARED BY:
///NAFISUL ISLAM NAZMI (15-29550-2)
///ESHREAT JAHAN  (14-27975-3)
///MD. GOLAM RAHAMAN (14-27496-3)
///ESMET ZERIN (15-29544-2)


#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>
#include <vector>
using namespace std;

int screen_width, screen_height;
bool welcome = 1, game = 0, over = 0, pause = 0;
int startx, starty;
int tankx = 30, tanky, tankl = 120;
vector<pair<int, int> > fire;
int enemytankx[10], enemytanky[10];
int tankw = 200, tankh = 150;
int score = 0;


int tankSpeed = 10;
int fireSpeed = 20;
int sleep = 1;
int tanks = 5;
int modulo = 600;

void myInit (void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(20.0);
    glLineWidth(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, screen_width, 0.0, screen_height);

}

void welcomeScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    string welcome_text = "Welcome to Tank Shooter game.\nInstructions:\n";
    welcome_text += "1. Use A, S, W, D to control your tank.\nA = left\nD = right\nW = up\nS = down\n";
    welcome_text += "2. Use SpaceBar / LMB to shoot.\n";
    welcome_text += "3. To avoid collision with enemy tanks, you can either destroy enemy tanks by shooting it or simply dodging it.\n";
    welcome_text += "4. Shooting the enemy tank will give you 20 points, whereas dodging it will give 10 points.\n";
    welcome_text += "5. Press Esc to quit the game.\n";
    welcome_text += "6. Press P to pause/resume.\n";
    memset(enemytankx, -1, sizeof(enemytankx));
    memset(enemytanky, -1, sizeof(enemytanky));
    fire.clear();
    score = 0;
    tankx = 30;
    tanky = screen_height / 2 - tankh / 2;


    int x = 50, y = screen_height - 50;
    int len = welcome_text.size();
    glColor3ub(0, 0, 0);
    glRasterPos2i(x, y);
    for(int i = 0; i < len; i++)
    {
        if(welcome_text[i] == '\n')
        {
            y -= 30;
            glRasterPos2i(x, y);
        }

        else
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, welcome_text[i]);
    }


    int w = 200, h = 100;
    startx = screen_width / 2 - w / 2;
    starty = screen_height / 4;

    glBegin(GL_QUADS);
    glVertex2i(startx, starty);
    glVertex2i(startx + w, starty);
    glVertex2i(startx + w, starty + h);
    glVertex2i(startx, starty + h);
    glEnd();

    string start = "Start Game";
    len = start.size();
    glColor3ub(255, 255, 255);
    glRasterPos2i(startx + 40, starty + 40);
    for(int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, start[i]);
    }
    glFlush();

}

void drawTank(int x, int y, int type)
{
    if(type == 1)
        glColor3ub(35, 108, 23);
    else
        glColor3ub(131, 33, 36);
    //glLineWidth(5);

    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + tankw, y);
    glVertex2i(x + tankw, y + tankh);
    glVertex2i(x, y + tankh);
    glEnd();

    int tempw = 50, temph = 40;
    if(type == 1)
        glColor3ub(0, 255, 0);
    else
        glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(x + tempw, y + temph);
    glVertex2i(x + tankw - tempw, y + temph);
    glVertex2i(x + tankw - tempw, y + tankh - temph);
    glVertex2i(x + tempw, y + tankh - temph);
    glEnd();

    //glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2i(x + tankw - tempw, y + tankh / 2);
    if(type == 1)
        glVertex2i(x + tankw - tempw + tankl, y + tankh / 2);
    else
        glVertex2i(x + tempw - tankl, y + tankh / 2);

    glEnd();
    glFlush();

}

void drawFire()
{
    glColor3ub(255, 255, 0);
    glBegin(GL_POINTS);
    for(int i = 0; i < fire.size(); i++)
        glVertex2i(fire[i].first, fire[i].second);

    glEnd();
    glFlush();
}

bool opponentCollision(int n)
{
    int x = enemytankx[n] - tankl, y = enemytanky[n], xl = enemytankx[n] + tankw, yl = enemytanky[n] + tankh;
    int tempx, tempy, tempxl, tempyl;
    for(int i = 0; i < tanks; i++)
    {
        if(i == n || enemytankx[i] == -1 || enemytanky[i] == -1)
            continue;
        tempx = enemytankx[i] - tankl;
        tempy = enemytanky[i];
        tempxl = enemytankx[i] + tankw;
        tempyl = enemytanky[i] + tankh;
        if(((x >= tempx && x <= tempxl) || (xl >= tempx && xl <= tempxl)) && ((y >= tempy && y <= tempyl) || (yl >= tempy && yl <= tempyl)))
        {
            return false;
        }

    }
    return true;
}

void generate_tank(int n)
{
    do
    {
        enemytankx[n] = screen_width + rand() % modulo;
        enemytanky[n] = rand() % (screen_height - tankh);
        //cout << n << ": " << enemytankx[n] << " " << enemytanky[n] << endl;
    } while(!opponentCollision(n));


}

void translate()
{
    for(int i = 0; i < tanks; i++)
    {
        enemytankx[i] -= tankSpeed;
        if(enemytankx[i] + tankw < 0)
        {
            enemytankx[i] = -1;
            enemytanky[i] = -1;
            score += 10;
        }

    }
    for(int i = 0; i < fire.size(); i++)
    {
        fire[i].first += fireSpeed;
        if(fire[i].first > screen_width)
        {
            fire.erase(fire.begin() + i);
            i--;
        }
    }

    glutPostRedisplay();
}

void fireCollision()
{
    bool temp;
    int fx, fy, tx, ty, txl, tyl;
    for(int i = 0; i < fire.size(); i++)
    {
        temp = 0;
        fx = fire[i].first;
        fy = fire[i].second;
        for(int j = 0; j < tanks; j++)
        {
//            tx = enemytankx[j] - tankl + 100;
            if(enemytankx[j] == -1 || enemytanky[j] == -1)
                continue;
            tx = enemytankx[j];
            txl = enemytankx[j] + tankw;
            ty = enemytanky[j];
            tyl = enemytanky[j] + tankh;
            if(fx >= tx && fx <= txl && fy >= ty && fy <= tyl)
            {
                enemytankx[j] = -1;
                enemytanky[j] = -1;
                fire.erase(fire.begin() + i);
                temp = 1;
                score += 20;
            }

        }
        if(temp)
            i--;
    }
}

void tankCollision()
{
    int tx, txl, ty, tyl;
//    int x = tankx, xl = tankx + tankw + tankl - 60;
    int x = tankx, xl = tankx + tankw;
    int y = tanky, yl = tanky + tankh;
    for(int i = 0; i < tanks; i++)
    {
//        tx = enemytankx[i] - tankl + 60;
        if(enemytankx[i] == -1 || enemytanky[i] == -1)
            continue;
        tx = enemytankx[i];
        txl = tx + tankw;
        ty = enemytanky[i];
        tyl = ty + tankh;
        if(((x >= tx && x <= txl) || (xl >= tx && xl <= txl)) && ((y >= ty && y <= tyl) || (yl >= ty && yl <= tyl)))
        {

            welcome = 0;
            game = 0;
            pause = 0;
            over = 1;
        }
    }

}

void drawRoad()
{
    glColor3ub(100, 100, 100);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(screen_width, 0);
    glVertex2i(screen_width, screen_height);
    glVertex2i(0, screen_height);
    glEnd();

    glLineWidth(20);
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2i(0, screen_height / 2);
    glVertex2i(screen_width, screen_height / 2);
    glVertex2i(0, screen_height / 4);
    glVertex2i(screen_width, screen_height / 4);
    glVertex2i(0, 3 * screen_height / 4);
    glVertex2i(screen_width, 3 * screen_height / 4);
    glEnd();
    glLineWidth(5);
    glFlush();
}

void drawScore()
{
    string message = "SCORE: ";
    int x = screen_width - 300;
    int y = screen_height - 100;
    glColor3ub(255, 255, 255);
    int len = message.size();
    glRasterPos2i(x, y);
    for(int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
    }

    char buffer[20];
    sprintf(buffer, "%d", score);
    len = strlen(buffer);
    for(int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
    }
    glFlush();
}

void gameScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawRoad();
    drawTank(tankx, tanky, 1);
    for(int i = 0; i < tanks; i++)
    {
        if(enemytankx[i] == -1 || enemytanky[i] == -1)
            generate_tank(i);

        drawTank(enemytankx[i], enemytanky[i], 2);
    }
    drawFire();
    drawScore();
    if(!pause)
    {
        fireCollision();
        tankCollision();
        Sleep(sleep);
        translate();
    }
}

void overScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    string message = "GAME OVER";
    int x = screen_width / 2 - 50, y = screen_height / 2 + 100;
    int len = message.size();
    glColor3ub(255, 0, 0);
    glRasterPos2i(x, y);
    for(int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
    }
    message = "YOUR SCORE: ";
    x -= 20;
    y -= 100;
    glColor3ub(0, 255, 0);
    len = message.size();
    glRasterPos2i(x, y);
    for(int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
    }

    char buffer[20];
    sprintf(buffer, "%d", score);
    len = strlen(buffer);
    for(int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
    }

    int w = 200, h = 100;
    startx = screen_width / 2 - w / 2 + 20;
    starty = screen_height / 4;

    glBegin(GL_QUADS);
    glVertex2i(startx, starty);
    glVertex2i(startx + w, starty);
    glVertex2i(startx + w, starty + h);
    glVertex2i(startx, starty + h);
    glEnd();

    message = "Go Back";
    len = message.size();
    glColor3ub(255, 255, 255);
    glRasterPos2i(startx + 55, starty + 40);
    for(int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
    }
    glFlush();
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if(welcome)
        welcomeScreen();
    else if(game)
        gameScreen();
    else if(over)
        overScreen();
}

void keyboard(unsigned char key, int x, int y)
{
    if(key == 27)
        exit(0);

    if(game)
    {
        if(key == 'a' && !pause)
        {
            tankx -= tankSpeed;
            if(tankx < 0)
                tankx = 0;
        }
        else if(key == 's'  && !pause)
        {
            tanky -= tankSpeed;
            if(tanky < 0)
                tanky = 0;
        }
        else if(key == 'd'  && !pause)
        {
            tankx += tankSpeed;
            if(tankx + tankw + tankl > screen_width)
                tankx = screen_width - tankw - tankl;
        }
        else if(key == 'w'  && !pause)
        {
            tanky += tankSpeed;
            if(tanky + tankh > screen_height)
                tanky = screen_height - tankh;
        }
        else if(key == ' ' && !pause)
        {
            fire.push_back(make_pair(tankx + tankw + tankl - 40, tanky + (tankh / 2)));
        }
        else if(key == 'p')
        {
            pause = !pause;
        }
        glutPostRedisplay();
    }

}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(game && !pause)
            fire.push_back(make_pair(tankx + tankw + tankl - 40, tanky + (tankh / 2)));

        else if(x >= startx && x <= startx + 200 && y <= screen_height - starty && y >= screen_height - starty - 100)
        {
            if(welcome)
            {
                welcome = 0;
                over = 0;
                game = 1;
                pause = 0;
            }
            else if(over)
            {
                welcome = 1;
                over = 0;
                game = 0;
                pause = 0;
            }
            glutPostRedisplay();
        }
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    screen_width = glutGet(GLUT_SCREEN_WIDTH);
    screen_height = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowSize (screen_width, screen_height);
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("Tank Shooter");
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutFullScreen();
    myInit();
    srand(clock());
    glutMainLoop();
    return 0;
}
