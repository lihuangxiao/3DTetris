// #include <GL/glew.h>
#include <QtWidgets>
#include <QtOpenGL>
#include <QVector3D>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>

#include <unistd.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

Viewer::Viewer(const QGLFormat& format, QWidget *parent)
    : QGLWidget(format, parent)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mVertexBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else
    , mVertexBufferObject(QGLBuffer::VertexBuffer)
#endif
{   
    isfirst = true;
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
    mTimer->start(1000/30);

    mTimer3 = new QTimer(this);
    connect(mTimer3, SIGNAL(timeout()), this, SLOT(rotate()));
    mTimer4 = new QTimer(this);
    connect(mTimer4, SIGNAL(timeout()), this, SLOT(increase()));
    
    colourmode = 2;
}

Viewer::~Viewer() {

}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 600);
}

void Viewer::initializeGL() {


    // Enable Depth Testing only once, inside initializeGL()
//    glEnable(GL_DEPTH_TEST);

    // For back-face-culling
//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_CCW);






    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor(0.7, 0.7, 1.0, 0.0);


    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        std::cerr << "Cannot load fragment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
        return;
    }

    float cubeData[] = {
        //  X     Y     Z
         0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, -1.0f,
         0.0f, 1.0f, 0.0f,
         0.0f, 1.0f, -1.0f,
         1.0f, 1.0f, -1.0f,
         0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, -1.0f,
         1.0f, 0.0f, -1.0f,
         1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, -1.0f,
         0.0f, 0.0f, -1.0f,
         1.0f, 0.0f, -1.0f,
         0.0f, 1.0f, -1.0f,
         1.0f, 1.0f, -1.0f,
         1.0f, 0.0f, -1.0f,
         0.0f, 1.0f, -1.0f,
         0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, -1.0f,
         0.0f, 0.0f, -1.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, -1.0f,
         1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, -1.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, -1.0f,
         1.0f, 0.0f, -1.0f,
         0.0f, 0.0f, 0.0f,
    };



#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
#else
    /*
     * if qt version is less than 5.1, use the following commented code
     * instead of QOpenGLVertexVufferObject. Also use QGLBuffer instead of
     * QOpenGLBuffer.
     */
    uint vao;

    typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
    typedef void (APIENTRY *_glBindVertexArray) (GLuint);

    _glGenVertexArrays glGenVertexArrays;
    _glBindVertexArray glBindVertexArray;

    glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
    glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QGLBuffer::StaticDraw);


    //////////////////////////////try colour
  //  mVertexBufferObject2.create();
  //  mVertexBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
    ////////////////////////////////////////


#endif 

    if (!mVertexBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }
//    mVertexBufferObject.allocate(triangleData, 3 * 3 * sizeof(float));
    mVertexBufferObject.allocate(cubeData, 36 * 3 * sizeof(float));


    //////////////////////////////try colour
//    mVertexBufferObject2.allocate(cubeData, 36 * 3 * sizeof(floar));
////////////////////////////////////////////


    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    //mPerspMatrixLocation = mProgram.uniformLocation("cameraMatrix");
    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");


	// Get location value for "fragColor" GLuint
        // Set uniform using that location.
	GLint colorLocation = mProgram.uniformLocation("frag_color");
	if (colorLocation == -1) {
		//cerror << "...";
	}
	mProgram.setUniformValue(colorLocation, 0.1f, 0.1f, 0.1f); 


}

void Viewer::paintGL() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.bind();
#endif
    GLint colorLocation = mProgram.uniformLocation("frag_color");
//    GLint colorLocation2 = mProgram.uniformLocation("frag_color2");

    if (colourmode == 2) {
      if (!isfirst) {
        for (int i = 0; i < 300; i++) {
          mModelMatrices[i] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        }
        numCube = 52;
        // set the mModelMatrices
        for (int i = 0; i < 20; i++) {
          mModelMatrices[i].translate(-6,-10 + i,0);
          colour[i] = -1;
        }
        for (int i = 0; i < 20; i++) {
          mModelMatrices[i + 20].translate(5,-10 + i,0);
          colour[i + 20] = -1;
        }
        for (int i = 0; i < 10; i++) {
          mModelMatrices[i + 40].translate(i - 5,-10,0);
          colour[i + 40] = -1;
        }
        mModelMatrices[50].translate(-6,10,0);
        mModelMatrices[51].translate(5,10,0);
        colour[50] = -1;
        colour[51] = -1;
  
   
        //finished the well
        //game()
        for (int i = 23; i >= 0; i--) {
          for (int j = 9; j >= 0; j--) {
            if (game->get(i,j) != -1) {
              numCube++;
              // we have: i - y = 9; j + x = 4
              mModelMatrices[numCube-1].translate(4-j,i-9,0);
              colour[numCube-1] = game->get(i,j);
            }
          }
        }
      }

      for (int i = 0; i < numCube; i++) {
        mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * mModelMatrices[i]);
        switch(colour[i]) {
          case -1:
            mProgram.setUniformValue(colorLocation, 0.0f, 0.0f, 0.0f);
            break;
          case 0:
            mProgram.setUniformValue(colorLocation, 0.0f, 0.0f, 1.0f);
            break;
          case 1:
            mProgram.setUniformValue(colorLocation, 0.0f, 1.0f, 0.0f);
            break;
          case 2:
            mProgram.setUniformValue(colorLocation, 0.0f, 1.0f, 1.0f);
            break;
          case 3:
            mProgram.setUniformValue(colorLocation, 1.0f, 0.0f, 0.0f);
            break;
          case 4:
            mProgram.setUniformValue(colorLocation, 1.0f, 0.0f, 1.0f);
            break;
          case 5:
            mProgram.setUniformValue(colorLocation, 1.0f, 1.0f, 0.0f);
            break;
          case 6:
            mProgram.setUniformValue(colorLocation, 1.0f, 1.0f, 1.0f);
            break;
          case 7:
            mProgram.setUniformValue(colorLocation, 0.5f, 0.5f, 0.5f);
            break;
          default:
            break;
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    } else if (colourmode == 3) {
      if (!isfirst) {
        for (int i = 0; i < 300; i++) {
          mModelMatrices[i] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        }
        numCube = 52;
        // set the mModelMatrices
        for (int i = 0; i < 20; i++) {
          mModelMatrices[i].translate(-6,-10 + i,0);
        }
        for (int i = 0; i < 20; i++) {
          mModelMatrices[i + 20].translate(5,-10 + i,0);
        }
        for (int i = 0; i < 10; i++) {
          mModelMatrices[i + 40].translate(i - 5,-10,0);
        }
        mModelMatrices[50].translate(-6,10,0);
        mModelMatrices[51].translate(5,10,0);


        //finished the well
        //game()
        for (int i = 23; i >= 0; i--) {
          for (int j = 9; j >= 0; j--) {
            if (game->get(i,j) != -1) {
              numCube++;
              // we have: i - y = 9; j + x = 4
              mModelMatrices[numCube-1].translate(4-j,i-9,0);
            }
          }
        }
      }
      for (int i = 0; i < numCube; i++) {
        mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * mModelMatrices[i]);
        mProgram.setUniformValue(colorLocation, 0.8f, 0.8f, 0.8f);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        mProgram.setUniformValue(colorLocation, 0.81f, 0.81f, 0.8f);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        mProgram.setUniformValue(colorLocation, 0.82f, 0.82f, 0.8f);
        glDrawArrays(GL_TRIANGLES, 12, 6);
        mProgram.setUniformValue(colorLocation, 0.83f, 0.83f, 0.8f);
        glDrawArrays(GL_TRIANGLES, 18, 6);
        mProgram.setUniformValue(colorLocation, 0.84f, 0.84f, 0.8f);
        glDrawArrays(GL_TRIANGLES, 24, 6);
        mProgram.setUniformValue(colorLocation, 0.85f, 0.85f, 0.8f);
        glDrawArrays(GL_TRIANGLES, 30, 6);

      }
    } else {}
}

void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
    mouse = event->button();
    x = event->x();
    mTimer3->stop();
    // try holding
//    if (isselfrotating) {
//      isselfrotating = false;
//      holding = event->button();
//    }
    holding += event->button();
    persistence = holding;
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    //mouse = 0;
    x = 0;
    mTimer4->stop();
    (void)event;
    //std::cout<<num<<std::endl;
    if(((!shift) && (num < 2)) && ismoving) {
      persistence = holding;
      if (holding - event->button() == 0) {
        mTimer3->start(90);
      }
      ismoving = false;
      //num = 0;
    } else {
      mouse = 0;
      //num = 0;
      
    }
    //try holding
    holding -= event->button();
 
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    ismoving = true;
    num = 0;
    mTimer4->start(1);
    if (!shift) {
/**      if (mouse == 1) {
        rotateWorld((event->x() - x), 1.0, 0.0, 0.0);
        rotatevalue = event->x() - x;
        x = event->x();
      } else if (mouse == 4) {
        rotateWorld((event->x() - x), 0.0, 1.0, 0.0);
        rotatevalue = event->x() - x;
        x = event->x();
      } else if (mouse == 2){
        rotateWorld((event->x() - x), 0.0, 0.0, 1.0);
        rotatevalue = event->x() - x;
        x = event->x();
      } else {}*/
      rotatevalue = event->x() - x;
      //x = event->x();
      if (holding == 1) {
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotatex += rotatevalue;
      } else if (holding == 2) {
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatez += rotatevalue;
      } else if (holding == 3) {
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatex += rotatevalue;        
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotatez += rotatevalue;
      } else if (holding == 4) {
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotatey += rotatevalue;
      } else if (holding == 5) {
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotatex += rotatevalue;
        rotatey += rotatevalue;
      } else if (holding == 6) {
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatey += rotatevalue;
        rotatez += rotatevalue;
      } else if (holding == 7) {
        rotateWorld((rotatevalue), 1.0, 0.0, 0.0);
        rotateWorld((rotatevalue), 0.0, 1.0, 0.0);
        rotateWorld((rotatevalue), 0.0, 0.0, 1.0);
        rotatex += rotatevalue;
        rotatey += rotatevalue;
        rotatez += rotatevalue;
      } else {
      }
      x = event->x();
    } else {
      int change = event->x() - x;
      if ((change < -2) && (scale < 10)) {
        scaleWorld(0.8,0.8,0.8);
        scale++;
        x = event->x();
      } else if ((change > 2) && (scale > -10)) {
        scaleWorld(1.25,1.25,1.25);
        scale--;
        x = event->x();
      } else {}
    }
    //ismoving = false;
}

QMatrix4x4 Viewer::getCameraMatrix() {
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float angle, float x, float y, float z) {
    mTransformMatrix.rotate(angle, x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    mTransformMatrix.scale(x, y, z);
}


void Viewer::newgame() {
    // set number of cubes to be 52
    numCube = 52;
    // reset the mModelMatrtices
    for (int i = 0; i < 300; i++) {
      mModelMatrices[i] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    }
    // set the mModelMatrices
    for (int i = 0; i < 20; i++) {
      mModelMatrices[i].translate(-6,-10 + i,0);
    }
    for (int i = 0; i < 20; i++) {
      mModelMatrices[i + 20].translate(5,-10 + i,0);
    }
    for (int i = 0; i < 10; i++) {
      mModelMatrices[i + 40].translate(i - 5,-10,0);
    }
    mModelMatrices[50].translate(-6,10,0);
    mModelMatrices[51].translate(5,10,0);
    //finished the well
    //game()
    game = new Game(10,20);
    for (int i = 23; i >= 0; i--) {
      for (int j = 9; j >= 0; j--) {
        if (game->get(i,j) != -1) {
          numCube++;
          // we have: i - y = 9; j + x = 4
          mModelMatrices[numCube-1].translate(4-j,i-9,0);          
        }
      }
    }      
    update();

    if (isfirst){
      mTimer2 = new QTimer(this);
      connect(mTimer2, SIGNAL(timeout()), this, SLOT(onestep()));
      mTimer2->start(300);
      isfirst = false;
    }
}


void Viewer::onestep() {
       game->tick();
/*
       for (int i = 0; i < 300; i++) {
         mModelMatrices[i] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
       }
       numCube = 52;
      // set the mModelMatrices
      for (int i = 0; i < 20; i++) {
        mModelMatrices[i].translate(-6,-10 + i,0);
      }
      for (int i = 0; i < 20; i++) {
        mModelMatrices[i + 20].translate(5,-10 + i,0);
      }
      for (int i = 0; i < 10; i++) {
        mModelMatrices[i + 40].translate(i - 5,-10,0);
      }
      mModelMatrices[50].translate(-6,10,0);
      mModelMatrices[51].translate(5,10,0);
      //finished the well
      //game()
      for (int i = 23; i >= 0; i--) {
        for (int j = 9; j >= 0; j--) {
          if (game->get(i,j) != -1) {
            numCube++;
            // we have: i - y = 9; j + x = 4
            mModelMatrices[numCube-1].translate(4-j,i-9,0);
          }
        }
      }
*/
      update();
}

void Viewer::rotate() {
/*    if (mouse == 1) {
      rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
    } else if (mouse == 4) {
      rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
    } else if (mouse == 2){
      rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
    } else {}
*/
      if (persistence == 1) {
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotatex += rotatevalue;
      } else if (persistence == 2) {
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatez += rotatevalue;
      } else if (persistence == 3) {
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatex += rotatevalue;
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotatez += rotatevalue;
      } else if (persistence == 4) {
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotatey += rotatevalue;
      } else if (persistence == 5) {
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotatex += rotatevalue;
        rotatey += rotatevalue;
      } else if (persistence == 6) {
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatey += rotatevalue;
        rotatez += rotatevalue;
      } else if (persistence == 7) {
        rotateWorld((rotatevalue), 1.0, 0.0, 0.0);
        rotateWorld((rotatevalue), 0.0, 1.0, 0.0);
        rotateWorld((rotatevalue), 0.0, 0.0, 1.0);
        rotatex += rotatevalue;
        rotatey += rotatevalue;
        rotatez += rotatevalue;
      } else {
      }

}

void Viewer::increase() {
    num++;
}


void Viewer::doreset() {

  rotateWorld(rotatex, -1.0, 0.0, 0.0);
  rotateWorld(rotatey, 0.0, -1.0, 0.0);
  rotateWorld(rotatez, 0.0, 0.0, -1.0);
  rotatex = 0;
  rotatey = 0;
  rotatez = 0;
  if (scale > 0) {
    for (int i = scale; i >0; i--) {
      scaleWorld(1.25,1.25,1.25);
    }
  } else if (scale < 0) {
    for (int i = scale; i <0; i++) {
      scaleWorld(0.8,0.8,0.8);
    }
  } else {}
  scale = 0;


//   rotateWorld (100, 1.0, 0.0, 0.0);
//   rotateWorld (120, 0.0, 1.0, 0.0);
//   rotateWorld (-100, 1.0, 0.0, 0.0);
//   rotateWorld (-120, 0.0, 1.0, 0.0);


}

void Viewer::face() {
  colourmode = 2;
}

void Viewer::multicoloured() {
  colourmode = 3;
}










