#include <QAction>
#include <QMessageBox>
#include <QToolBar>
#include <QMenu>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QToolButton>
#include <QFontDialog>
#include <QSettings>
#include <QDataStream>

#include <QDebug>

#include "mainwindow.h"
#include "tabwidget.h"
#include "texteditor.h"
#include "findwidget.h"
#include "gotodialog.h"
#include "findandreplace.h"
#include "application.h"
#include "locationwidget.h"
#include "overwritewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_findWidget = 0;
    m_gotoDialog = 0;
    m_findAndReplace = 0;

    tabWidget = new TabWidget;
    setCentralWidget(tabWidget);

    createActions();
    createMenus();
    createToolBar();
    createStatusBar();

    connect(tabWidget, SIGNAL(currentChanged(int)), SLOT(updateActsSlot(int)));
    connect(tabWidget, SIGNAL(fileNameChanged(int)), SLOT(updateWindowTitle(int)));
    tabWidget->createNewTab();
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    if(tabWidget->closeAll()) {
        qWarning("I'm in closeEvent()");
        if(m_findWidget) {
            delete m_findWidget;
        }

        if(m_findAndReplace) {
            qWarning("delete m_findAndReplace");
            delete m_findAndReplace;
        }
        writeSettings();
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
    newAct->setStatusTip(tr("Create a new document"));
    connect(newAct, SIGNAL(triggered()), SLOT(newSlot()));

    newWindowAct = new QAction(tr("New Window"), this);
    newWindowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
    newWindowAct->setStatusTip(tr("Create a new document in a new window"));
    connect(newWindowAct, SIGNAL(triggered()), SLOT(newWindowSlot()));

    openAct = new QAction(tr("Open"), this);
    openAct->setIcon(QIcon(":/images/document-open.png"));
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file"));
    connect(openAct, SIGNAL(triggered()), SLOT(openSlot()));

    clearHistoryAct = new QAction(tr("Clear History"), this);
    clearHistoryAct->setIcon(QIcon(":/images/edit-clear.png"));
    clearHistoryAct->setStatusTip(tr("Clear the recently used files history"));
    connect(clearHistoryAct, SIGNAL(triggered()), SLOT(clearHistorySlot()));

    saveAct = new QAction(tr("Save"), this);
    saveAct->setIcon(QIcon(":/images/document-save.png"));
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current document"));

    saveAsAct = new QAction(tr("Save As..."), this);
    saveAsAct->setIcon(QIcon(":/images/document-save-as.png"));
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save current document as another file"));

    saveAllAct = new QAction(tr("Save All"), this);
    connect(saveAllAct, SIGNAL(triggered()), SLOT(saveAllSlot()));
    saveAllAct->setStatusTip(tr("Save all documents"));

    closeTabAct = new QAction(tr("Close Tab"), this);
    closeTabAct->setShortcut(QKeySequence::Close);
    closeTabAct->setStatusTip(tr("Close the current document"));
    connect(closeTabAct, SIGNAL(triggered()), tabWidget, SLOT(closeTabAt()));

    quitAct = new QAction(tr("Quit"), this);
    quitAct->setIcon(QIcon(":/images/application-exit.png"));
    quitAct->setShortcut(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the program"));
    connect(quitAct, SIGNAL(triggered()), SLOT(close()));

    undoAct = new QAction(tr("Undo"), this);
    undoAct->setIcon(QIcon(":/images/edit-undo.png"));
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last action"));
    
    redoAct = new QAction(tr("Redo"), this);
    redoAct->setIcon(QIcon(":/images/edit-redo.png"));
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last undone action"));

    cutAct = new QAction(tr("Cut"), this);
    cutAct->setIcon(QIcon(":/images/edit-cut.png"));
    cutAct->setShortcut(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the selection"));

    copyAct = new QAction(tr("Copy"), this);
    copyAct->setIcon(QIcon(":/images/edit-copy.png"));
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the selection"));

    pasteAct = new QAction(tr("Paste"), this);
    pasteAct->setIcon(QIcon(":/images/edit-paste.png"));
    pasteAct->setShortcut(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard"));

    deleteAct = new QAction(tr("Delete"), this);
    deleteAct->setIcon(QIcon(":/images/edit-delete.png"));
    deleteAct->setStatusTip(tr("Delete the current selection"));

    selectAllAct = new QAction(tr("Select All"), this);
    selectAllAct->setIcon(QIcon(":/images/edit-select-all.png"));
    selectAllAct->setShortcut(QKeySequence::SelectAll);
    selectAllAct->setStatusTip(tr("Select the text in the entire document"));

    findAct = new QAction(tr("Find"), this);
    findAct->setIcon(QIcon(":/images/edit-find.png"));
    findAct->setShortcut(QKeySequence::Find);
    findAct->setStatusTip(tr("Search for text"));
    connect(findAct, SIGNAL(triggered()), SLOT(findSlot()));

    findNextAct = new QAction(tr("Find Next"), this);
    findNextAct->setShortcut(QKeySequence(Qt::Key_F3));
    findNextAct->setStatusTip(tr("Search forwards for the same text"));

    findPrevAct = new QAction(tr("Find Previous"), this);
    findPrevAct->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F3));
    findPrevAct->setStatusTip(tr("Search backwards for the same text"));

    findAndReplaceAct = new QAction(tr("Find And Replace..."), this);
    findAndReplaceAct->setIcon(QIcon(":/images/edit-find-replace.png"));
    findAndReplaceAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    findAndReplaceAct->setStatusTip(tr("Search for and replace text"));
    connect(findAndReplaceAct, SIGNAL(triggered()), SLOT(findAndReplaceSlot()));

    selectFontAct = new QAction(tr("Select Font..."), this);
    selectFontAct->setStatusTip(tr("Change the editor font"));
    connect(selectFontAct, SIGNAL(triggered()), SLOT(selectFontSlot()));

    lineNumbersAct = new QAction(tr("Line Numbers"), this);
    lineNumbersAct->setCheckable(true);
    lineNumbersAct->setStatusTip(tr("Show line numbers"));
    connect(lineNumbersAct, SIGNAL(toggled(bool)), SLOT(lineNumbersSlot(bool)));

    toolBarAct = new QAction(tr("Toolbar"), this);
    toolBarAct->setCheckable(true);
//    toolBarAct->setChecked(true);
    toolBarAct->setStatusTip(tr("Change the visibility of the toolbar"));

    statusBarAct = new QAction(tr("Statusbar"), this);
    statusBarAct->setCheckable(true);
//    statusBarAct->setChecked(true);
    statusBarAct->setStatusTip(tr("Change the visibility of the statusbar"));

    fullScreenModeAct = new QAction(tr("Fullscreen"), this);
    fullScreenModeAct->setShortcut(QKeySequence(Qt::Key_F11));
    fullScreenModeAct->setCheckable(true);
    fullScreenModeAct->setStatusTip(tr("Enable and disable full screen mode"));
    connect(fullScreenModeAct, SIGNAL(triggered()), SLOT(fullScreenModeSlot()));

    toLowercaseAct = new QAction(tr("to Lowercase"), this);
    toLowercaseAct->setStatusTip(tr("Change the case of the selection to lowercase"));

    toUppercaseAct = new QAction(tr("to Uppercase"), this);
    toUppercaseAct->setStatusTip(tr("Change the case of the selection to uppercase"));

    toTitleCaseAct = new QAction(tr("to Title Case"), this);
    toTitleCaseAct->setStatusTip(tr("Change the case of the selection to title case"));

    toOppositeCaseAct = new QAction(tr("to Opposite Case"), this);
    toOppositeCaseAct->setStatusTip(tr("Change the case of the selection opposite case"));

    transposeAct = new QAction(tr("Transpose"), this);
    transposeAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    transposeAct->setStatusTip(tr("Reverse the order of something"));

    selectionToLineUpAct = new QAction(tr("Line Up"), this);
    selectionToLineUpAct->setStatusTip(tr("Move the line or selection one line up"));
    selectionToLineUpAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Up));

    selectionToLineDownAct = new QAction(tr("Line Down"), this);
    selectionToLineDownAct->setStatusTip(tr("Move the line or selection one line down"));
    selectionToLineDownAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Down));

    duplicateLineSelAct = new QAction(tr("Duplicate Line / Selection"), this);
    duplicateLineSelAct->setStatusTip(tr("Duplicate the current line or selection"));

    prevTabAct = new QAction(tr("Previous Tab"), this);
    prevTabAct->setIcon(QIcon(":/images/go-previous.png"));
    prevTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageDown));
    prevTabAct->setEnabled(false);
    prevTabAct->setStatusTip(tr("Select the previous tab"));
    connect(prevTabAct, SIGNAL(triggered()), tabWidget, SLOT(previousTab()));

    nextTabAct = new QAction(tr("Next Tab"), this);
    nextTabAct->setIcon(QIcon(":/images/go-next.png"));
    nextTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageUp));
    nextTabAct->setEnabled(false);
    nextTabAct->setStatusTip(tr("Select the next tab"));
    connect(nextTabAct, SIGNAL(triggered()), tabWidget, SLOT(nextTab()));

    gotoAct = new QAction(tr("Go to..."), this);
    gotoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    gotoAct->setStatusTip(tr("Go to a specific location in the document"));
    connect(gotoAct, SIGNAL(triggered()), SLOT(gotoSlot()));

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setIcon(QIcon(":/images/help-about.png"));
    aboutAct->setShortcut(QKeySequence::HelpContents);
    aboutAct->setStatusTip(tr("About this application"));
    connect(aboutAct, SIGNAL(triggered()), SLOT(aboutSlot()));
}

void MainWindow::createMenus()
{
    QMenuBar *mBar = menuBar();

    QMenu *m = mBar->addMenu(tr("&File"));
    m->addAction(newAct);
    m->addAction(newWindowAct);
    m->addSeparator();
    m->addAction(openAct);
    openRecentMenu = m->addMenu(tr("Open Recent"));
//    fillRecentFiles();
    m->addSeparator();
    m->addAction(saveAct);
    m->addAction(saveAsAct);
    m->addAction(saveAllAct);
    m->addSeparator();
    m->addAction(closeTabAct);
    m->addAction(quitAct);

    m = mBar->addMenu(tr("&Edit"));
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
//    m->addAction(findAndReplaceAct);

    m = mBar->addMenu(tr("&View"));
    m->addAction(selectFontAct);
    m->addSeparator();
    m->addAction(lineNumbersAct);
    m->addAction(toolBarAct);
    m->addAction(statusBarAct);
    m->addSeparator();
    m->addAction(fullScreenModeAct);

    m = mBar->addMenu(tr("&Text"));
    QMenu *subMenu = m->addMenu(tr("Convert"));
    subMenu->addAction(toLowercaseAct);
    subMenu->addAction(toUppercaseAct);
    subMenu->addAction(toTitleCaseAct);
    subMenu->addAction(toOppositeCaseAct);
    subMenu->addSeparator();
    subMenu->addAction(transposeAct);
    subMenu = m->addMenu(tr("Move Line / Selection"));
    subMenu->addAction(selectionToLineUpAct);
    subMenu->addAction(selectionToLineDownAct);
    m->addSeparator();
    m->addAction(duplicateLineSelAct);

    m = mBar->addMenu(tr("&Navigation"));
    m->addAction(prevTabAct);
    m->addAction(nextTabAct);
    m->addSeparator();
//    m->addSeparator();
    m->addAction(gotoAct);

    m = mBar->addMenu(tr("&Help"));
    m->addAction(aboutAct);
}

void MainWindow::createToolBar()
{
    m_toolBar = addToolBar(tr("Toolbar"));
    connect(toolBarAct, SIGNAL(toggled(bool)), m_toolBar, SLOT(setVisible(bool))); 

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
//    m_toolBar->addAction(findAndReplaceAct);
}

void MainWindow::createStatusBar()
{
    m_statusBar = statusBar();
    m_locationWidget = new LocationWidget;
    m_statusBar->addPermanentWidget(m_locationWidget);
    m_overwriteWidget = new OverwriteWidget;
    m_statusBar->addPermanentWidget(m_overwriteWidget);
    m_statusBar->showMessage(tr("Ready"), 2000);
    connect(statusBarAct, SIGNAL(toggled(bool)), m_statusBar, SLOT(setVisible(bool)));
}

void MainWindow::newSlot()
{
   tabWidget->createNewTab(); 
}

void MainWindow::newWindowSlot()
{
    myapp->createMainWindow();
}

void MainWindow::openSlot()
{
    QStringList list = QFileDialog::getOpenFileNames(this, tr("Select files"));

    openFiles(list);
}

void MainWindow::openFiles(const QStringList &files)
{
    QStringListIterator it(files);
    while(it.hasNext()) {
        tabWidget->createNewTab(it.next());
    }
    qDebug() << files;
    fillRecentFiles();
}

void MainWindow::openRecentSlot()
{
    QAction *act = qobject_cast<QAction*>(sender());
    QString fileName = act->data().toString();
    tabWidget->createNewTab(fileName);
}

void MainWindow::clearHistorySlot()
{
    AppSettings *s = myapp->appSettings();
    s->beginGroup("recentfiles");
    s->setValue("files", QStringList());
    s->endGroup();
    fillRecentFiles();
}

void MainWindow::saveAllSlot()
{
    for(int i=0; i<tabWidget->count(); ++i) {
        tabWidget->editor(i)->save();
    }
}

void MainWindow::findSlot()
{
    if(!m_findWidget) {
        m_findWidget = new FindWidget;

        connect(findNextAct, SIGNAL(triggered()), m_findWidget, SLOT(next()));
        connect(findPrevAct, SIGNAL(triggered()), m_findWidget, SLOT(previous()));

        m_findWidget->setMovable(false);
        m_findWidget->setFloatable(false);

        addToolBar(Qt::BottomToolBarArea, m_findWidget);
    }

    m_findWidget->setEditor(tabWidget->editor());
    m_findWidget->setFocus();
    m_findWidget->show();    
}

void MainWindow::findAndReplaceSlot()
{
    if(!m_findAndReplace) {
        m_findAndReplace = new FindAndReplace(this);
    }

    m_findAndReplace->setEditor(tabWidget->editor());
    m_findAndReplace->show();
}

void MainWindow::lineNumbersSlot(bool v)
{
    AppSettings *appset = myapp->appSettings();

    appset->beginGroup("texteditor");
    appset->setValue("linenumbers", v);
    appset->endGroup();

    myapp->updateSettingsRequest(TEXTEDITOR);
}

void MainWindow::gotoSlot()
{
    if(!m_gotoDialog) {
        m_gotoDialog = new GotoDialog(this);
    }

    m_gotoDialog->setEditor(tabWidget->editor());
    m_gotoDialog->exec();
}

void MainWindow::selectFontSlot()
{
    QFont font = tabWidget->editor()->document()->defaultFont();
    font = QFontDialog::getFont(0, font, this);
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    stream << font;

    // write to settings new font
    // and update each texteditor

    AppSettings *settings = myapp->appSettings();
    settings->beginGroup("texteditor");
    settings->setValue("font", ba);
    settings->endGroup();

    myapp->updateSettingsRequest(TEXTEDITOR);
}

void MainWindow::fullScreenModeSlot()
{
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::updateActsSlot(int i)
{
    qWarning("%d %d", tabWidget->count(), i);

    if(i == -1) {
        return;
    }

    if(tabWidget->count() == 1) {
        closeTabAct->setEnabled(false);
    } else {
        prevTabAct->setEnabled(true);
        nextTabAct->setEnabled(true);
        closeTabAct->setEnabled(true);
    }

    TextEditor *te = tabWidget->editor(i);
    m_locationWidget->setEditor(te);
    m_overwriteWidget->setEditor(te);

    saveAct->disconnect();
    saveAsAct->disconnect();
    undoAct->disconnect();
    redoAct->disconnect();
    cutAct->disconnect();
    copyAct->disconnect();
    pasteAct->disconnect();
    selectAllAct->disconnect();
    deleteAct->disconnect();
    toLowercaseAct->disconnect();
    toUppercaseAct->disconnect();
    toTitleCaseAct->disconnect();
    toOppositeCaseAct->disconnect();
    transposeAct->disconnect();
    selectionToLineUpAct->disconnect();
    selectionToLineDownAct->disconnect();
    duplicateLineSelAct->disconnect();

    undoAct->setEnabled(te->document()->isUndoAvailable());
    redoAct->setEnabled(te->document()->isRedoAvailable());
    cutAct->setEnabled(te->textCursor().hasSelection());
    copyAct->setEnabled(te->textCursor().hasSelection());
    deleteAct->setEnabled(te->textCursor().hasSelection());
    toLowercaseAct->setEnabled(te->textCursor().hasSelection());
    toUppercaseAct->setEnabled(te->textCursor().hasSelection());
    toTitleCaseAct->setEnabled(te->textCursor().hasSelection());
    toOppositeCaseAct->setEnabled(te->textCursor().hasSelection());

    connect(te->document(), SIGNAL(undoAvailable(bool)), undoAct, SLOT(setEnabled(bool)));
    connect(te->document(), SIGNAL(redoAvailable(bool)), redoAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), deleteAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), toLowercaseAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), toUppercaseAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), toTitleCaseAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(copyAvailable(bool)), toOppositeCaseAct, SLOT(setEnabled(bool)));
    connect(te, SIGNAL(cursorPositionChanged()), m_locationWidget, SLOT(updateLabel()));

    connect(saveAct, SIGNAL(triggered()), te, SLOT(save()));
    connect(saveAsAct, SIGNAL(triggered()), te, SLOT(saveAs()));
    connect(undoAct, SIGNAL(triggered()), te, SLOT(undo()));
    connect(redoAct, SIGNAL(triggered()), te, SLOT(redo()));
    connect(cutAct, SIGNAL(triggered()), te, SLOT(cut()));
    connect(copyAct, SIGNAL(triggered()), te, SLOT(copy()));
    connect(pasteAct, SIGNAL(triggered()), te, SLOT(paste()));
    connect(selectAllAct, SIGNAL(triggered()), te, SLOT(selectAll()));
    connect(deleteAct, SIGNAL(triggered()), te, SLOT(deleteSlot()));
    connect(toLowercaseAct, SIGNAL(triggered()), te, SLOT(toLowercase()));
    connect(toUppercaseAct, SIGNAL(triggered()), te, SLOT(toUppercase()));
    connect(toTitleCaseAct, SIGNAL(triggered()), te, SLOT(toTitleCase()));
    connect(toOppositeCaseAct, SIGNAL(triggered()), te, SLOT(toOppositeCase()));
    connect(transposeAct, SIGNAL(triggered()), te, SLOT(transpose()));
    connect(selectionToLineUpAct, SIGNAL(triggered()), te, SLOT(moveToLineUp()));
    connect(selectionToLineDownAct, SIGNAL(triggered()), te, SLOT(moveToLineDown()));
    connect(duplicateLineSelAct, SIGNAL(triggered()), te, SLOT(duplicateLineSelection()));

// ? do i need it?
    if(m_findWidget) {
        if(m_findWidget->isVisible()) {
            m_findWidget->setEditor(te);
        }
    }

    setWindowTitle(QString("%1 - TextEditor").arg(te->showName()));
}

void MainWindow::updateWindowTitle(int i)
{
    TextEditor *te = tabWidget->editor(i);
    setWindowTitle(QString("%1 - TextEditor").arg(te->showName()));
}

void MainWindow::aboutSlot()
{
    QMessageBox::about(this, tr("TextEditor"), 
            tr("<b>TextEditor 0.1.0</b><br>"
               "TextEditor is a simple text editor<br>"
               "<a href=\'https://github.com/Vourhey/TextEditor\'>https://github.com/Vourhey/TextEditor</a>"));
}

void MainWindow::readSettings(AppSettings *appSettings)
{
    appSettings->beginGroup("mainwindow");

    move(appSettings->value("position", QPoint(200, 200)).toPoint());
    resize(appSettings->value("size", QSize(450, 300)).toSize());

    bool toolBarVisible = appSettings->value("toolBarVisible", true).toBool();
    m_toolBar->setVisible(toolBarVisible);
    toolBarAct->setChecked(toolBarVisible);

    bool statusBarVisible = appSettings->value("statusBarVisible", true).toBool();
    m_statusBar->setVisible(statusBarVisible);
    statusBarAct->setChecked(statusBarVisible);

    fillRecentFiles();

    appSettings->endGroup();

    appSettings->beginGroup("texteditor");
    lineNumbersAct->setChecked(appSettings->value("linenumbers", false).toBool());
    appSettings->endGroup();
}

void MainWindow::writeSettings()
{
    AppSettings *appset = myapp->appSettings();

    appset->beginGroup("mainwindow");
    appset->setValue("position", pos());
    appset->setValue("size", size());
//    settings.setValue("isFullScreenMode", isFullScreen());
    appset->setValue("toolBarVisible", m_toolBar->isVisible());
    appset->setValue("statusBarVisible", m_statusBar->isVisible());
    appset->endGroup();
}

void MainWindow::fillRecentFiles()
{
    qWarning("MainWindow::fillRecentFiles()");

    AppSettings *appset = myapp->appSettings();
    appset->beginGroup("recentfiles");
    QStringList files = appset->value("files", QStringList()).toStringList();
    appset->endGroup();
    openRecentMenu->clear();

//    qDebug() << files;

    if(!files.isEmpty()) {
        foreach(QString s, files) {
            QAction *act = new QAction(QFileInfo(s).fileName(), this);
            act->setData(s);
            act->setStatusTip(tr("Open \'%1\'").arg(s));
            connect(act, SIGNAL(triggered()), SLOT(openRecentSlot()));
            openRecentMenu->addAction(act);
        }

        openRecentMenu->addSeparator();
        openRecentMenu->addAction(clearHistoryAct);
        openRecentMenu->setEnabled(true);
    } else {
        openRecentMenu->setEnabled(false);
    }
}

