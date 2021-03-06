#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class TabWidget;
class FindWidget;
class GotoDialog;
class FindAndReplace;
class AppSettings;
class LocationWidget;
class OverwriteWidget;

class QAction;
class QMenu;
class QToolBar;
class QStatusBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    void openFiles(const QStringList &files);
    void readSettings(AppSettings *appSettings);

protected:
    void closeEvent(QCloseEvent *ev);

private slots:
    void newSlot();
    void newWindowSlot();
    void openSlot();
    void openRecentSlot();
    void clearHistorySlot();
    void saveAllSlot();
    void findSlot();
    void findAndReplaceSlot();
    void lineNumbersSlot(bool v);
    void gotoSlot();
    void selectFontSlot();
    void fullScreenModeSlot();
    void updateActsSlot(int i);
    void updateWindowTitle(int i);
    void aboutSlot();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();

    void writeSettings();
    void fillRecentFiles();

    TabWidget *tabWidget;
    FindWidget *m_findWidget;
    GotoDialog *m_gotoDialog;
    FindAndReplace *m_findAndReplace;
    LocationWidget *m_locationWidget;
    OverwriteWidget *m_overwriteWidget;

    QAction *newAct,
        *newWindowAct,
        *openAct,
        *clearHistoryAct,
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
        *statusBarAct,
        *fullScreenModeAct;
    QAction *toLowercaseAct,
        *toUppercaseAct,
        *toTitleCaseAct,
        *toOppositeCaseAct,
        *transposeAct,
        *selectionToLineUpAct,
        *selectionToLineDownAct,
        *duplicateLineSelAct;
    QAction *prevTabAct,
        *nextTabAct,
        *gotoAct;
    QAction *aboutAct;

    QMenu *openRecentMenu;

    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
};

#endif // MAINWINDOW_H
