#include <QtWidgets>
#include <QGLFormat>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Tetrominoes on the Wall");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    // m_menubar = new QMenuBar;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    // scaleWorld is private, in order to do this we need to set it public...
    //m_viewer->scaleWorld(0.8, 0.8, 0.8);
    m_viewer->show();

    createActions();
    createMenu();
}

void AppWindow::keyPressEvent(QKeyEvent *event) {
    m_viewer->mTimer3->stop();
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::instance()->quit();
    } else if (event->key() == Qt::Key_Shift) {
        m_viewer->shift = true;
    } else if (event->key() == Qt::Key_Left) {
        m_viewer->game->moveRight();
        m_viewer->update();   
    } else if (event->key() == Qt::Key_Right) {
        m_viewer->game->moveLeft();
        m_viewer->update();
    } else if (event->key() == Qt::Key_Up) {
        m_viewer->game->rotateCW();
        m_viewer->update();
    } else if (event->key() == Qt::Key_Down) {
        m_viewer->game->rotateCCW();
        m_viewer->update();
    } else if (event->key() == Qt::Key_Space) {
        m_viewer->game->drop();
        m_viewer->update();
    }



      else {
        QWidget::keyPressEvent(event);
    }
}

void AppWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::instance()->quit();
    } else if (event->key() == Qt::Key_Shift) {
        m_viewer->shift = false;
    } else {
        QWidget::keyPressEvent(event);
    }
}




void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector 
    QAction* quitAct = new QAction(tr("&Quit"), this);
    //m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P); 
    quitAct->setShortcut(Qt::Key_Q);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

//////////////////////////////////////////////////////////////
    QAction* newgameAct = new QAction(tr("&New game"), this);
    newgameAct->setShortcut(Qt::Key_N);
    newgameAct->setStatusTip(tr("Start a new game"));
    connect(newgameAct, SIGNAL(triggered()), this, SLOT(app_newgame()));
    m_menu_actions.push_back(newgameAct);

    QAction* resetAct = new QAction(tr("&Reset"), this);
    resetAct->setShortcut(Qt::Key_R);
    resetAct->setStatusTip(tr("Reset the view of the game"));
    connect(resetAct, SIGNAL(triggered()), this, SLOT(app_reset()));
    m_menu_actions.push_back(resetAct);
    m_menu_actions.push_back(quitAct);

    QAction* wireframeAct = new QAction(tr("&Wire-frame"), this);
    wireframeAct->setShortcut(Qt::Key_W);
    wireframeAct->setStatusTip(tr("Draw the game in wire-frame mode"));
    connect(wireframeAct, SIGNAL(triggered()), this, SLOT(draw_wireframe()));

    QAction* faceAct = new QAction(tr("&FACE"), this);
    faceAct->setShortcut(Qt::Key_F);
    faceAct->setStatusTip(tr("fill in the faces in the game"));
    connect(faceAct, SIGNAL(triggered()), this, SLOT(draw_face()));

    QAction* multicolouredAct = new QAction(tr("&Multicoloured"), this);
    multicolouredAct->setShortcut(Qt::Key_M);
    multicolouredAct->setStatusTip(tr("draw each cube with different colour on each face"));
    connect(multicolouredAct, SIGNAL(triggered()), this, SLOT(draw_multicoloured()));

    m_menu_actions1.push_back(wireframeAct);
    m_menu_actions1.push_back(faceAct);
    m_menu_actions1.push_back(multicolouredAct);

    toolsGroup = new QActionGroup(this);
    toolsGroup->addAction(wireframeAct);
    toolsGroup->addAction(faceAct);
    toolsGroup->addAction(multicolouredAct);
    wireframeAct->setCheckable(true);
    faceAct->setCheckable(true);
    multicolouredAct->setCheckable(true);
    faceAct->setChecked(true);



    QAction* slowAct = new QAction(tr("&Slow"), this);
    slowAct->setShortcut(Qt::Key_1);
    slowAct->setStatusTip(tr("game speed: slow"));
    connect(slowAct, SIGNAL(triggered()), this, SLOT(speed_slow()));

    QAction* mediumAct = new QAction(tr("&Medium"), this);
    mediumAct->setShortcut(Qt::Key_2);
    mediumAct->setStatusTip(tr("game speed: medium"));
    connect(mediumAct, SIGNAL(triggered()), this, SLOT(speed_medium()));

    QAction* fastAct = new QAction(tr("&Fast"), this);
    fastAct->setShortcut(Qt::Key_3);
    fastAct->setStatusTip(tr("game speed: fast"));
    connect(fastAct, SIGNAL(triggered()), this, SLOT(speed_fast()));

    m_menu_actions2.push_back(slowAct);
    m_menu_actions2.push_back(mediumAct);
    m_menu_actions2.push_back(fastAct);


}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }

    m_menu_draw = menuBar()->addMenu(tr("&Draw Mode"));
    for (auto& action : m_menu_actions1) {
        m_menu_draw->addAction(action);
    }

    m_menu_speed = menuBar()->addMenu(tr("&Speed"));
    for (auto& action : m_menu_actions2) {
        m_menu_speed->addAction(action);
    }
}



void AppWindow::app_newgame() {
  m_viewer->newgame();
  //m_viewer->game = new Game(10,20);
  //m_viewer->game->reset();

}
void AppWindow::app_reset(){
/*
  rotateWorld(0 - rotatex, 1.0, 0.0, 0.0);
  rotateWolrd(0 - rotatey, 0.0, 1.0, 0.0);
  rotateWorld(0 - rotatez, 0.0, 0.0, 1.0);
  if (scale > 0) {
    for (int i = scale; i >0; i--) {
      scaleWorld(1.25,1.25,1.25);
    }
  } else if (scale < 0) {
    for (int i = scale; i <0; i++) {
      scaleWorld(0.8,0.8,0.8);
    }
  } else {}
*/
  m_viewer->doreset();
}

void AppWindow::draw_wireframe(){
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}
void AppWindow::draw_face(){
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  m_viewer->face();
}
void AppWindow::draw_multicoloured(){
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  m_viewer->multicoloured();
}

void AppWindow::speed_slow(){
  m_viewer->mTimer2->start(1000);
}
void AppWindow::speed_medium(){
  m_viewer->mTimer2->start(300);
}
void AppWindow::speed_fast(){
  m_viewer->mTimer2->start(100);
}






