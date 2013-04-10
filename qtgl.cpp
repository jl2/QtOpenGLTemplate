/*
  qtgl.cpp
  
  Copyright (c) 2013, Jeremiah LaRocco jeremiah.larocco@gmail.com

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <QMainWindow>
#include "qtgl.h"

#ifdef __APPLE_CC__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <cmath>

QtGl::QtGl(QWidget *parent) : QGLWidget(parent) {
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(1000/60);
    eps=0.001;
}
void QtGl::timeout() {
    rotAngle += 1.0;
    if (rotAngle >=360.0)
        rotAngle=0.0;
    updateGL();
}

void QtGl::initializeGL() {
    qglClearColor(Qt::black);
    glShadeModel(GL_SMOOTH);
    rotAngle = 0.0;

    mat_specular[0]=1.0;
    mat_specular[1]=1.0;
    mat_specular[2]=1.0;
    mat_specular[3]=1.0;

    mat_diffuse[0]=0.0;
    mat_diffuse[1]=1.0;
    mat_diffuse[2]=0.0;
    mat_diffuse[3]=1.0;
  
    mat_shininess[0]=50.0;

    light_position[0]=20.0;
    light_position[1]=20.0;
    light_position[2]=0.0;
    light_position[3]=1.0;

    light_color[0]=1.0;
    light_color[1]=1.0;
    light_color[2]=1.0;
    light_color[3]=1.0;

    lmodel_ambient[0]=0.2;
    lmodel_ambient[1]=0.2;
    lmodel_ambient[2]=0.2;
    lmodel_ambient[3]=1.0;
  
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
  
    listName = glGenLists(1);

    glNewList(listName, GL_COMPILE);

    // Not the best way to draw a cube, but it works :-/
    glBegin(GL_QUADS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Bottom
    glVertex3f(-10,-10,-10); glNormal3f(0,0,-1);
    glVertex3f(-10,10,-10); glNormal3f(0,0,-1);
    glVertex3f(10,10,-10); glNormal3f(0,0,-1);
    glVertex3f(10,-10,-10); glNormal3f(0,0,-1);

    // Top
    glVertex3f(-10,-10,10); glNormal3f(0,0,1);
    glVertex3f(-10,10,10); glNormal3f(0,0,1);
    glVertex3f(10,10,10); glNormal3f(0,0,1);
    glVertex3f(10,-10,10); glNormal3f(0,0,1);

    // Back
    glVertex3f(-10,-10,-10); glNormal3f(-1,0,0);
    glVertex3f(-10,10,-10); glNormal3f(-1,0,0);
    glVertex3f(-10,10,10); glNormal3f(-1,0,0);
    glVertex3f(-10,-10,10); glNormal3f(-1,0,0);

    // Front
    glVertex3f(10,-10,-10); glNormal3f(1,0,0);
    glVertex3f(10,10,-10); glNormal3f(1,0,0);
    glVertex3f(10,10,10); glNormal3f(1,0,0);
    glVertex3f(10,-10,10); glNormal3f(1,0,0);

    // Left
    glVertex3f(-10,-10,-10); glNormal3f(0,-1,0);
    glVertex3f(10,-10,-10); glNormal3f(0,-1,0);
    glVertex3f(10,-10,10); glNormal3f(0,-1,0);
    glVertex3f(-10,-10,10); glNormal3f(0,-1,0);

    // Right
    glVertex3f(-10,10,-10); glNormal3f(0,1,0);
    glVertex3f(10,10,-10); glNormal3f(0,1,0);
    glVertex3f(10,10,10); glNormal3f(0,1,0);
    glVertex3f(-10,10,10); glNormal3f(0,1,0);
    
    glEnd();

    glEndList();
}

void QtGl::resizeGL(int width, int height) {
    glViewport(0,0, (GLsizei) width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1.0, 1.0, 140);
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void QtGl::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0,35.0,35.0,
              0.0,0.0,0.0,
              0.0,0.0,1.0);

    glPushMatrix();
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
    glPopMatrix();
    glRotatef(rotAngle, 0,0,1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listName);

    glFlush();
}
