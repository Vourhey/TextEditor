#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TabWidget;
class QAction;
class QMenu;
class QToolBar;
class QStatusBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *ev);

private slots:
    void newSlot();
    void openSlot();
    void saveSlot();
    void saveAsSlot();
    void saveAllSlot();
    void findSlot();
    void updateActsSlot(int i);
    void aboutSlot();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();

    TabWidget *tabWidget;

    QAction *newAct,
        *openAct,
        *saveAct,
        *saveAsAct,
        *saveAllAct,
        *closeTabAct,
        *quitAct;
    QAction *undoAct,
        *redoAct,
        *cutAct,
        *copyAct,
        *pasteAct,
        *deleteAct,
        *selectAllAct,
        *findAct,
        *findNextAct,
        *findPrevAct,
        *findAndReplaceAct;
    QAction *selectFontAct,
        *lineNumbersAct,
        *toolBarAct,
        *statusBarAct;
    QAction *prevTabAct,
        *nextTabAct,
        *gotoAct;
    QAction *aboutAct;

    QMenu *openRecentMenu;

    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
};

#endif // MAINWINDOW_H
