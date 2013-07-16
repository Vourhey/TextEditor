#include <QtGui>
#include <QtWidgets>

#include "mainwindow.h"
#include "tabwidget.h"
#include "texteditor.h"
#include "findwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_findBar = 0;
    m_findWidget = 0;

    tabWidget = new TabWidget;
    setCentralWidget(tabWidget);

    createActions();
    createMenus();
    createToolBar();
    createStatusBar();

    connect(tabWidget, SIGNAL(currentChanged(int)), SLOT(updateActsSlot(int)));
    tabWidget->createNewTab();
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
    newAct->setIcon(QIcon(":/images/document-new.png"));
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, SIGNAL(triggered()), SLOT(newSlot()));

    openAct = new QAction(tr("Open"), this);
    openAct->setIcon(QIcon(":/images/document-open.png"));
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), SLOT(openSlot()));

    saveAct = new QAction(tr("Save"), this);
    saveAct->setIcon(QIcon(":/images/document-save.png"));
    saveAct->setShortcut(QKeySequence::Save);

    saveAsAct = new QAction(tr("Save As..."), this);
    saveAsAct->setIcon(QIcon(":/images/document-save-as.png"));
    saveAsAct->setShortcut(QKeySequence::SaveAs);

    saveAllAct = new QAction(tr("Save All"), this);
    connect(saveAllAct, SIGNAL(triggered()), SLOT(saveAllSlot()));

    closeTabAct = new QAction(tr("Close Tab"), this);
    closeTabAct->setShortcut(QKeySequence::Close);
    connect(closeTabAct, SIGNAL(triggered()), tabWidget, SLOT(closeTabAt()));

    quitAct = new QAction(tr("Quit"), this);
    quitAct->setIcon(QIcon(":/images/application-exit.png"));
    quitAct->setShortcut(QKeySequence::Quit);
    connect(quitAct, SIGNAL(triggered()), SLOT(close()));

    undoAct = new QAction(tr("Undo"), this);
    undoAct->setIcon(QIcon(":/images/edit-undo.png"));
    undoAct->setShortcut(QKeySequence::Undo);
    
    redoAct = new QAction(tr("Redo"), this);
    redoAct->setIcon(QIcon(":/images/edit-redo.png"));
    redoAct->setShortcut(QKeySequence::Redo);

    cutAct = new QAction(tr("Cut"), this);
    cutAct->setIcon(QIcon(":/images/edit-cut.png"));
    cutAct->setShortcut(QKeySequence::Cut);

    copyAct = new QAction(tr("Copy"), this);
    copyAct->setIcon(QIcon(":/images/edit-copy.png"));
    copyAct->setShortcut(QKeySequence::Copy);

    pasteAct = new QAction(tr("Paste"), this);
    pasteAct->setIcon(QIcon(":/images/edit-paste.png"));
    pasteAct->setShortcut(QKeySequence::Paste);

    deleteAct = new QAction(tr("Delete"), this);
    deleteAct->setIcon(QIcon(":/images/edit-delete.png"));

    selectAllAct = new QAction(tr("Select All"), this);
    selectAllAct->setIcon(QIcon(":/images/edit-select-all.png"));
    selectAllAct->setShortcut(QKeySequence::SelectAll);

    findAct = new QAction(tr("Find"), this);
    findAct->setIcon(QIcon(":/images/edit-find.png"));
    findAct->setShortcut(QKeySequence::Find);
    connect(findAct, SIGNAL(triggered()), SLOT(findSlot()));

    findNextAct = new QAction(tr("Find Next"), this);
    findNextAct->setShortcut(QKeySequence(Qt::Key_F3));

    findPrevAct = new QAction(tr("Find Previous"), this);
    findPrevAct->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F3));

    findAndReplaceAct = new QAction(tr("Find And Replace..."), this);
    findAndReplaceAct->setIcon(QIcon(":/images/edit-find-replace.png"));
    findAndReplaceAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    selectFontAct = new QAction(tr("Select Font..."), this);

    lineNumbersAct = new QAction(tr("Line Numbers"), this);
    lineNumbersAct->setCheckable(true);

    toolBarAct = new QAction(tr("Toolbar"), this);
    toolBarAct->setCheckable(true);

    statusBarAct = new QAction(tr("Statusbar"), this);
    statusBarAct->setCheckable(true);

    prevTabAct = new QAction(tr("Previous Tab"), this);
    prevTabAct->setIcon(QIcon(":/images/go-previous.png"));
    prevTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageDown));

    nextTabAct = new QAction(tr("Next Tab"), this);
    nextTabAct->setIcon(QIcon(":/images/go-next.png"));
    nextTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageUp));

    gotoAct = new QAction(tr("Go to..."), this);
    gotoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setIcon(QIcon(":/images/help-about.png"));
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
    openRecentMenu->setEnabled(false);
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

void MainWindow::createToolBar()
{
    m_toolBar = addToolBar(tr("Toolbar"));

    m_toolBar->addAction(newAct);
    m_toolBar->addAction(openAct);
    m_toolBar->addAction(saveAct);
    m_toolBar->addSeparator();
    m_toolBar->addAction(undoAct);
    m_toolBar->addAction(redoAct);
    m_toolBar->addSeparator();
    m_toolBar->addAction(cutAct);
    m_toolBar->addAction(copyAct);
    m_toolBar->addAction(pasteAct);
    m_toolBar->addSeparator();
    m_toolBar->addAction(findAct);
    m_toolBar->addAction(findAndReplaceAct);
}

void MainWindow::createStatusBar()
{
    m_statusBar = statusBar();
    m_statusBar->showMessage(tr("Ready"), 2000);
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

void MainWindow::saveAllSlot()
{
    for(int i=0; i<tabWidget->count(); ++i) {
        tabWidget->editor(i)->save();
    }
}

void MainWindow::findSlot()
{
    if(!m_findBar) {
        m_findBar = new QToolBar(this);
        m_findWidget = new FindWidget(m_findBar);

        connect(findNextAct, SIGNAL(triggered()), m_findWidget, SLOT(next()));
        connect(findPrevAct, SIGNAL(triggered()), m_findWidget, SLOT(previous()));

        m_findBar->setMovable(false);
        m_findBar->setFloatable(false);
        m_findBar->addWidget(m_findWidget);
        addToolBar(Qt::BottomToolBarArea, m_findBar);
    }

    m_findWidget->setEditor(tabWidget->editor());
    m_findWidget->setFocus();
    m_findBar->show();    
}

void MainWindow::updateActsSlot(int i)
{
    qWarning("%d %d", tabWidget->count(), i);
    if(i == -1) {
        saveAct->setEnabled(false);
        saveAsAct->setEnabled(false);
        saveAllAct->setEnabled(false);
        closeTabAct->setEnabled(false);
        undoAct->setEnabled(false);
        redoAct->setEnabled(false);
        cutAct->setEnabled(false);
        copyAct->setEnabled(false);
        pasteAct->setEnabled(false);
        deleteAct->setEnabled(false);
        selectAllAct->setEnabled(false);
        findAct->setEnabled(false);
        findNextAct->setEnabled(false);
        findPrevAct->setEnabled(false);
        findAndReplaceAct->setEnabled(false);
        prevTabAct->setEnabled(false);
        nextTabAct->setEnabled(false);
        gotoAct->setEnabled(false);

        if(m_findBar) {
            m_findBar->hide();
        }
        setWindowTitle("TextEditor");
        return;
    }

    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    saveAllAct->setEnabled(true);
    closeTabAct->setEnabled(true);
    pasteAct->setEnabled(true);
    selectAllAct->setEnabled(true);
    findAct->setEnabled(true);
    findNextAct->setEnabled(true);
    findPrevAct->setEnabled(true);
    findAndReplaceAct->setEnabled(true);
    gotoAct->setEnabled(true);

    TextEditor *te = tabWidget->editor(i);

    saveAct->disconnect();
    connect(saveAct, SIGNAL(triggered()), te, SLOT(save()));
    saveAsAct->disconnect();
    connect(saveAsAct, SIGNAL(triggered()), te, SLOT(saveAs()));

    undoAct->setEnabled(te->document()->isUndoAvailable());
    redoAct->setEnabled(te->document()->isRedoAvailable());
    cutAct->setEnabled(te->textCursor().hasSelection());
    copyAct->setEnabled(te->textCursor().hasSelection());
    deleteAct->setEnabled(te->textCursor().hasSelection());

    /* todo... */

    undoAct->disconnect();
    redoAct->disconnect();
    cutAct->disconnect();
    copyAct->disconnect();
    pasteAct->disconnect();
    selectAllAct->disconnect();
    deleteAct->disconnect();

    /* todo */
    /* нужно отсоединять предыдущий редактор от всех этих сигналов/слотов */
    connect(te->document(), SIGNAL(undoAvailable(bool)), undoAct, SLOT(setEnabled(bool)));
    connect(te->document(), SIGNAL(redoAvailable(bool)), redoAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), deleteAct, SLOT(setEnabled(bool)));
    /* todo */

    connect(undoAct, SIGNAL(triggered()), te, SLOT(undo()));
    connect(redoAct, SIGNAL(triggered()), te, SLOT(redo()));
    connect(cutAct, SIGNAL(triggered()), te, SLOT(cut()));
    connect(copyAct, SIGNAL(triggered()), te, SLOT(copy()));
    connect(pasteAct, SIGNAL(triggered()), te, SLOT(paste()));
    connect(selectAllAct, SIGNAL(triggered()), te, SLOT(selectAll()));
    connect(deleteAct, SIGNAL(triggered()), te, SLOT(deleteSlot()));

    if(m_findBar) {
        if(m_findBar->isVisible()) {
            m_findWidget->setEditor(te);
        }
    }

    setWindowTitle(QString("%1 - TextEditor").arg(te->showName()));
}

void MainWindow::aboutSlot()
{
    QMessageBox::about(this, tr("TextEditor"), tr("About this programm"));
}

