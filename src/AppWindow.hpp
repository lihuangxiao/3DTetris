#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();
    QActionGroup* toolsGroup;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


private slots:
    void app_newgame();
    void app_reset();

    void draw_wireframe();
    void draw_face();
    void draw_multicoloured();

    void speed_slow();
    void speed_medium();
    void speed_fast();


private:
    void createActions();
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_draw;
    QMenu* m_menu_speed;

    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_menu_actions1;
    std::vector<QAction*> m_menu_actions2;

    Viewer* m_viewer;
};

#endif
