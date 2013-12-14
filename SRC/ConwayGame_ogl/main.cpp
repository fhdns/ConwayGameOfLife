#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "glut.h"
#include "Game.h"

#define M_PI                3.14159265359
// --------------------------------------
#define WINDOW_SIZE_WIDTH   800
#define WINDOW_SIZE_HEIGHT  600

#define DISPLAY_MODE        GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH
#define WINDOW_POS_X        100
#define WINDOW_POS_Y        100
#define WINDOW_TITLE        "Torus OpenGL"
// --------------------------------------
//Torus
GLfloat xRotated = -10.0f, yRotated = 30.0f, zRotated = 40.0f;
const GLdouble innerRaidus  = 0.5f;
const GLdouble outterRaidus = 0.8f;
const GLdouble rotatePerOneTime = 0.5f;
// --------------------------------------
// Colors                   R, G, B, Alpha
#define CLEAR_COLOR         0.0f, 0.0f, 0.0f, 0.0f
#define CELL_ALIVE_COLOR    0.0f, 255.0f, 0.0f
#define CELL_DEATH_COLOR    0.0f, 0.0f, 255.0f
#define CELL_DEATH_COLOR2   255.0f, 0.0f, 255.0f
// --------------------------------------
// Global
Game    ConwayGame;
#define GAME_WIDTH          40
#define GAME_HEIGHT         40
// --------------------------------------

void DrawTorus(GLfloat r, GLfloat R, GLint nsides, GLint rings, GLenum type = GL_QUADS)
{
  int i, j;
  GLfloat theta, phi, theta1, phi1;
  GLfloat p0[03], p1[3], p2[3], p3[3];
  GLfloat n0[3], n1[3], n2[3], n3[3];

  for (i = 0; i < rings; i++)
  {
    theta = (GLfloat) i *2.0 * M_PI / rings;
    theta1 = (GLfloat) (i + 1) * 2.0 * M_PI / rings;
    for (j = 0; j < nsides; j++)
    {
      phi = (GLfloat) j *2.0 * M_PI / nsides;
      phi1 = (GLfloat) (j + 1) * 2.0 * M_PI / nsides;

      p0[0] = cos(theta) * (R + r * cos(phi));
      p0[1] = -sin(theta) * (R + r * cos(phi));
      p0[2] = r * sin(phi);

      p1[0] = cos(theta1) * (R + r * cos(phi));
      p1[1] = -sin(theta1) * (R + r * cos(phi));
      p1[2] = r * sin(phi);

      p2[0] = cos(theta1) * (R + r * cos(phi1));
      p2[1] = -sin(theta1) * (R + r * cos(phi1));
      p2[2] = r * sin(phi1);

      p3[0] = cos(theta) * (R + r * cos(phi1));
      p3[1] = -sin(theta) * (R + r * cos(phi1));
      p3[2] = r * sin(phi1);

      n0[0] = cos(theta) * (cos(phi));
      n0[1] = -sin(theta) * (cos(phi));
      n0[2] = sin(phi);

      n1[0] = cos(theta1) * (cos(phi));
      n1[1] = -sin(theta1) * (cos(phi));
      n1[2] = sin(phi);

      n2[0] = cos(theta1) * (cos(phi1));
      n2[1] = -sin(theta1) * (cos(phi1));
      n2[2] = sin(phi1);

      n3[0] = cos(theta) * (cos(phi1));
      n3[1] = -sin(theta) * (cos(phi1));
      n3[2] = sin(phi1);

      // Drawing
      glBegin(type);
      
      if (ConwayGame.GetCellState(i, j))
        glColor3f(CELL_ALIVE_COLOR);
      else if ((j + i)&1)
        glColor3f(CELL_DEATH_COLOR);
      else
        glColor3f(CELL_DEATH_COLOR2);

      glNormal3fv(n3);
      glVertex3fv(p3);
      glNormal3fv(n2);
      glVertex3fv(p2);
      glNormal3fv(n1);
      glVertex3fv(p1);
      glNormal3fv(n0);
      glVertex3fv(p0);      
      glEnd();
    }
  }
}

void display(void)
{
  // clear the drawing buffer.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glTranslated(0.0f, 0.0f, -4.0f);

  glRotated(xRotated, 1.0f, 0.0f, 0.0f);
  glRotated(yRotated, 0.0f, 1.0f, 0.0f);
  glRotated(zRotated, 0.0f, 0.0f, 1.0f);

  DrawTorus(innerRaidus, outterRaidus, ConwayGame.GetWidth(),  ConwayGame.GetHeight());

  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int x, int y)
{
  if (y == 0 || x == 0) return;  //Nothing is visible then, so return

  glLoadIdentity();

  // Angle of view:40 degrees
  // Near clipping plane distance: 0.5
  // Far clipping plane distance: 20.0
  gluPerspective(40.0f, (GLdouble)x / (GLdouble)y, 0.5f, 20.0f);

  // Use the whole window for rendering
  glViewport(0, 0, x, y);
}

void idle(void)
{
  yRotated += rotatePerOneTime;
  if ((int)yRotated %3)
    ConwayGame.Step();
  display();
}

int main(int argc, char **argv)
{
  // Initialize GLUT
  glutInit(&argc, argv);

  glutInitDisplayMode(DISPLAY_MODE);

  // window size
  glutInitWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);

  // create the window 
  glutCreateWindow(WINDOW_TITLE);

  glClearColor(CLEAR_COLOR);

  glEnable(GL_DEPTH_TEST);

  // Assign the function used in events
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  // Game Init
  ConwayGame.Init(GAME_WIDTH, GAME_HEIGHT);
  ConwayGame.SetTestValues();

  // Let start glut loop
  glutMainLoop();
  return 0;
}
