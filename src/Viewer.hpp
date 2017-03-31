#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>

#include "game.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif



class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    bool shift;
    int mouse;
    int x;
    int scale;

    int numCube;

    void newgame();
    bool isfirst;

    QTimer* mTimer2;
    
    //implementing gravity
    QTimer* mTimer3;// for gravity, each tick rotates once
    int rotatevalue;
    bool ismoving;
    QTimer* mTimer4;// for detecting gravity, each tick increase num by one
    int num;

    //implementing holding multiple mouse buttons, rotating;
    int holding;
    int persistence;

    //implementing reset
    int rotatex;
    int rotatey;
    int rotatez;
    void doreset();

    Game* game;
    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.

    // implement colour
    int colour[300];
    int colourmode;
    void face();
    void multicoloured();

  
   // void scaleWorld(float x, float y, float z);

private slots:
    void onestep();
    void rotate();
    void increase();

protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );




private:

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float angle, float x, float y, float z);
    void scaleWorld(float x, float y, float z);


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mVertexBufferObject;
    //QGLBuffer mVertexBufferObject2; // for face color

#endif

    int mVertexLocation;
    int mMvpMatrixLocation;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mModelMatrices[300];
    QMatrix4x4 mTransformMatrix;
    
    QTimer* mTimer;
//    QTimer* mTimer2;
    QGLShaderProgram mProgram;
};

#endif
