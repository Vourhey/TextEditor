#include <QtGui>
#include <QtWidgets>

#include "mainwindow.h"
#include "tabwidget.h"
#include "texteditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabWidget = new TabWidget;
    setCentralWidget(tabWidget);

    tabWidget->createNewTab();

    createActions();
    createMenus();

    connect(tabWidget, SIGNAL(currentChanged(int)), SLOT(updateActsSlot(int)));
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    if(tabWidget->closeAll()) {
        ev->accept();
    } else {
        ev->ignore();
    }
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("New"), this);
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, SIGNAL(triggered()), SLOT(newSlot()));

    openAct = new QAction(tr("Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), SLOT(openSlot()));

    saveAct = new QAction(tr("Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    connect(saveAct, SIGNAL(triggered()), SLOT(saveSlot()));

    saveAsAct = new QAction(tr("Save As..."), this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAct, SIGNAL(triggered()), SLOT(saveAsSlot()));

    saveAllAct = new QAction(tr("Save All"), this);
    connect(saveAllAct, SIGNAL(triggered()), SLOT(saveAllSlot()));

    closeTabAct = new QAction(tr("Close Tab"), this);
    closeTabAct->setShortcut(QKeySequence::Close);

    quitAct = new QAction(tr("Quit"), this);
    quitAct->setShortcut(QKeySequence::Quit);
    connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    undoAct = new QAction(tr("Undo"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    
    redoAct = new QAction(tr("Redo"), this);
    redoAct->setShortcut(QKeySequence::Redo);

    cutAct = new QAction(tr("Cut"), this);
    cutAct->setShortcut(QKeySequence::Cut);

    copyAct = new QAction(tr("Copy"), this);
    copyAct->setShortcut(QKeySequence::Copy);

    pasteAct = new QAction(tr("Paste"), this);
    pasteAct->setShortcut(QKeySequence::Paste);

    deleteAct = new QAction(tr("Delete"), this);

    selectAllAct = new QAction(tr("Select All"), this);
    selectAllAct->setShortcut(QKeySequence::SelectAll);

    findAct = new QAction(tr("Find"), this);
    findAct->setShortcut(QKeySequence::Find);

    findNextAct = new QAction(tr("Find Next"), this);
    findNextAct->setShortcut(QKeySequence(Qt::Key_F3));

    findPrevAct = new QAction(tr("Find Previous"), this);
    findPrevAct->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F3));

    findAndReplaceAct = new QAction(tr("Find And Replace..."), this);
    findAndReplaceAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    selectFontAct = new QAction(tr("Select Font..."), this);

    lineNumbersAct = new QAction(tr("Line Numbers"), this);
    lineNumbersAct->setCheckable(true);

    toolBarAct = new QAction(tr("Toolbar"), this);
    toolBarAct->setCheckable(true);

    statusBarAct = new QAction(tr("Statusbar"), this);
    statusBarAct->setCheckable(true);

    prevTabAct = new QAction(tr("Previous Tab"), this);
    prevTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageDown));

    nextTabAct = new QAction(tr("Next Tab"), this);
    nextTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageUp));

    gotoAct = new QAction(tr("Go to..."), this);
    gotoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setShortcut(QKeySequence::HelpContents);
    connect(aboutAct, SIGNAL(triggered()), SLOT(aboutSlot()));
}

void MainWindow::createMenus()
{
    QMenuBar *mBar = menuBar();

    QMenu *m = mBar->addMenu(tr("File"));
    m->addAction(newAct);
    m->addSeparator();
    m->addAction(openAct);
    openRecentMenu = m->addMenu(tr("Open Recent"));
    m->addSeparator();
    m->addAction(saveAct);
    m->addAction(saveAsAct);
    m->addAction(saveAllAct);
    m->addSeparator();
    m->addAction(closeTabAct);
    m->addAction(quitAct);

    m = mBar->addMenu(tr("Edit"));
    m->addAction(undoAct);
    m->addAction(redoAct);
    m->addSeparator();
    m->addAction(cutAct);
    m->addAction(copyAct);
    m->addAction(pasteAct);
    m->addAction(deleteAct);
    m->addSeparator();
    m->addAction(selectAllAct);
    m->addSeparator();
    m->addAction(findAct);
    m->addAction(findNextAct);
    m->addAction(findPrevAct);
    m->addAction(findAndReplaceAct);

    m = mBar->addMenu(tr("View"));
    m->addAction(selectFontAct);
    m->addSeparator();
    m->addAction(lineNumbersAct);
    m->addAction(toolBarAct);
    m->addAction(statusBarAct);

    m = mBar->addMenu(tr("Navigation"));
    m->addAction(prevTabAct);
    m->addAction(nextTabAct);
    m->addSeparator();
    m->addSeparator();
    m->addAction(gotoAct);

    m = mBar->addMenu(tr("Help"));
    m->addAction(aboutAct);
}

void MainWindow::newSlot()
{
   tabWidget->createNewTab(); 
}

void MainWindow::openSlot()
{
    QStringList list = QFileDialog::getOpenFileNames(this, tr("Select files"));
    
    QStringListIterator it(list);
    while(it.hasNext()) {
        tabWidget->createNewTab(it.next());
    }
}

void MainWindow::saveSlot()
{
    tabWidget->editor()->save();
}

void MainWindow::saveAsSlot()
{
    tabWidget->editor()->saveAs();
}

void MainWindow::saveAllSlot()
{
    for(int i=0; i<tabWidget->count(); ++i) {
        tabWidget->editor(i)->save();
    }
}

void MainWindow::findSlot()
{
}

void MainWindow::updateActsSlot(int i)
{
    TextEditor *te = tabWidget->editor(i);

    undoAct->setEnabled(te->document()->isUndoAvailable());
    redoAct->setEnabled(te->document()->isRedoAvailable());

    /* todo... */

    undoAct->disconnect();
    redoAct->disconnect();
    cutAct->disconnect();
    copyAct->disconnect();
    pasteAct->disconnect();
    deleteAct->disconnect();

    /* todo */
    /* нужно отсоединять предыдущий редактор от всех этих сигналов/слотов */
    connect(te->document(), SIGNAL(undoAvailable(bool)), undoAct, SLOT(setEnabled(bool)));
    connect(te->document(), SIGNAL(redoAvailable(bool)), redoAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
    /* todo */

    connect(undoAct, SIGNAL(triggered()), te, SLOT(undo()));
    connect(redoAct, SIGNAL(triggered()), te, SLOT(redo()));
    connect(cutAct, SIGNAL(triggered()), te, SLOT(cut()));
    connect(copyAct, SIGNAL(triggered()), te, SLOT(copy()));
    connect(pasteAct, SIGNAL(triggered()), te, SLOT(paste()));
    connect(deleteAct, SIGNAL(triggered()), te, SLOT(deleteSlot()));
}

void MainWindow::aboutSlot()
{
    QMessageBox::about(this, tr("TextEditor"), tr("About this programm"));
}

